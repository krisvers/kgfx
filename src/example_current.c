#include <kgfx/kgfx.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <linmath.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846264338327950288

#ifdef KGFX_MACOS
#include <unistd.h>
#endif

#include <kgfx_gh/kgfx_gh.h>

typedef struct {
	vec3 position;
	vec3 rotation;
	float fov;
	float aspect;
	float near;
	float far;
} camera_t;

typedef struct {
	vec3 pos;
	vec2 uv;
} vertex_t;

static u8 keys[GLFW_KEY_LAST + 1] = { 0 };
static u8 prev_keys[GLFW_KEY_LAST + 1] = { 0 };

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key < 0) {
		return;
	}

	if (action == GLFW_PRESS) {
		keys[key] = 1;
	} else if (action == GLFW_RELEASE) {
		keys[key] = 0;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

static u8 key(int key) {
	return keys[key];
}

static u8 key_down(int key) {
	return keys[key] && !prev_keys[key];
}

static u8 key_up(int key) {
	return !keys[key] && prev_keys[key];
}

int example_current() {
	/*if (!glfwInit()) {
		return 1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "example_current", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return 1;
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, glfw_key_callback);

	KGFXcontext ctx;
	if (kgfxCreateContext(KGFX_ANY_VERSION, kgfxWindowFromGLFW(window), &ctx) != KGFX_SUCCESS) {
		return 1;
	}

	const struct aiScene* scene = aiImportFile("assets/mesh.obj", aiProcess_Triangulate);
	if (scene == NULL) {
		printf("Failed to import GLTF scene\n");
		return 1;
	}

	void* vsrc = NULL;
	usize vsize = 0;
	{
		FILE* fp = fopen("assets/vertex.spv", "rb");
		if (fp == NULL) {
			return 1;
		}

		fseek(fp, 0, SEEK_END);
		vsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		vsrc = malloc(vsize + 1);
		if (vsrc == NULL) {
			fclose(fp);
			return 1;
		}

		if (fread(vsrc, 1, vsize, fp) != vsize) {
			fclose(fp);
			free(vsrc);
			return 1;
		}

		fclose(fp);
	}

	KGFXshaderdesc shaderDesc = {
		.entryName = NULL,
		.type = KGFX_SHADERTYPE_VERTEX,
		.medium = KGFX_MEDIUM_SPIRV,
		.pData = (const void*) vsrc,
		.size = (u32) vsize,
	};
	
	KGFXshader vshader = kgfxCreateShader(ctx, shaderDesc);
	free(vsrc);
	if (vshader == KGFX_HANDLE_NULL) {
		return 1;
	}

	void* fsrc = NULL;
	usize fsize = 0;
	{
		FILE* fp = fopen("assets/fragment.spv", "rb");
		if (fp == NULL) {
			return 1;
		}

		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		fsrc = malloc(fsize + 1);
		if (fsrc == NULL) {
			fclose(fp);
			return 1;
		}

		if (fread(fsrc, 1, fsize, fp) != fsize) {
			fclose(fp);
			free(fsrc);
			return 1;
		}

		fclose(fp);
	}

	shaderDesc.type = KGFX_SHADERTYPE_FRAGMENT;
	shaderDesc.pData = (const void*) fsrc;
	shaderDesc.size = (u32) fsize;
	
	KGFXshader fshader = kgfxCreateShader(ctx, shaderDesc);
	free(fsrc);
	if (fshader == KGFX_HANDLE_NULL) {
		return 1;
	}

	KGFXpipelinedesc pipelineDesc;
	KGFXshader shaders[2] = { vshader, fshader };
	pipelineDesc.pShaders = shaders;
	pipelineDesc.shaderCount = 2;

	KGFXpipelineattribute attributes[2] = {
		{ KGFX_DATATYPE_FLOAT3, 0 },
		{ KGFX_DATATYPE_FLOAT2, 1 },
	};

	KGFXpipelinebinding binding;
	binding.inputRate = KGFX_VERTEX_INPUT_RATE_VERTEX;
	binding.attributeCount = 2;
	binding.pAttributes = attributes;
	binding.bindpoint = KGFX_BINDPOINT_VERTEX;
	binding.binding = 0;

	KGFXdatatype descriptorSetType = KGFX_DATATYPE_MAT4;

	KGFXdescriptorsetdesc descriptorSetDescs[2];
	descriptorSetDescs[0].bindpoint = KGFX_BINDPOINT_VERTEX;
	descriptorSetDescs[0].binding = 0;
	descriptorSetDescs[0].usage = KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER;
	descriptorSetDescs[1].bindpoint = KGFX_BINDPOINT_FRAGMENT;
	descriptorSetDescs[1].binding = 1;
	descriptorSetDescs[1].usage = KGFX_DESCRIPTOR_USAGE_TEXTURE;

	pipelineDesc.layout.pBindings = &binding;
	pipelineDesc.layout.bindingCount = 1;
	pipelineDesc.layout.pDescriptorSets = descriptorSetDescs;
	pipelineDesc.layout.descriptorSetCount = 2;
	pipelineDesc.cullMode = KGFX_CULLMODE_BACK;
	pipelineDesc.frontFace = KGFX_FRONTFACE_CCW;

	KGFXpipeline pipeline = kgfxCreatePipeline(ctx, pipelineDesc);
	if (pipeline == KGFX_HANDLE_NULL) {
		return 1;
	}
	kgfxDestroyShader(ctx, vshader);
	kgfxDestroyShader(ctx, fshader);

	const struct aiMesh* gltf_mesh = scene->mMeshes[0];

	KGFXbufferdesc bufferDesc;
	bufferDesc.location = KGFX_BUFFER_LOCATION_GPU;
	bufferDesc.usage = KGFX_BUFFER_USAGE_VERTEX_BUFFER;
	bufferDesc.size = sizeof(vertex_t) * gltf_mesh->mNumVertices;

	vertex_t* vertices = malloc(sizeof(vertex_t) * gltf_mesh->mNumVertices);
	for (unsigned int i = 0; i < gltf_mesh->mNumVertices; ++i) {
		vertices[i].pos[0] = gltf_mesh->mVertices[i].x;
		vertices[i].pos[1] = gltf_mesh->mVertices[i].y;
		vertices[i].pos[2] = gltf_mesh->mVertices[i].z;
		vertices[i].uv[0] = gltf_mesh->mTextureCoords[0][i].x;
		vertices[i].uv[1] = gltf_mesh->mTextureCoords[0][i].y;
	}
	bufferDesc.pData = vertices;

	KGFXbuffer buffer = kgfxCreateBuffer(ctx, bufferDesc);
	if (buffer == KGFX_HANDLE_NULL) {
		return 1;
	}

	KGFXbufferdesc indexBufferDesc;
	indexBufferDesc.location = KGFX_BUFFER_LOCATION_GPU;
	indexBufferDesc.usage = KGFX_BUFFER_USAGE_INDEX_BUFFER;
	indexBufferDesc.size = sizeof(u32) * gltf_mesh->mNumFaces * 3;
	u32* indices = malloc(sizeof(u32) * gltf_mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < gltf_mesh->mNumFaces; ++i) {
		indices[i * 3 + 0] = gltf_mesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = gltf_mesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = gltf_mesh->mFaces[i].mIndices[2];
	}
	indexBufferDesc.pData = indices;

	KGFXbuffer indexBuffer = kgfxCreateBuffer(ctx, indexBufferDesc);
	if (indexBuffer == KGFX_HANDLE_NULL) {
		return 1;
	}

	free(vertices);
	free(indices);
	aiReleaseImport(scene);

	KGFXmeshbuffer meshBuffer;
	meshBuffer.buffer = buffer;
	meshBuffer.bindpoint = KGFX_MESH_BUFFER_BINDPOINT_VERTEX;
	meshBuffer.offset = 0;

	KGFXmeshbuffer indexMeshBuffer;
	indexMeshBuffer.buffer = indexBuffer;
	indexMeshBuffer.bindpoint = KGFX_MESH_BUFFER_BINDPOINT_INDEX;
	indexMeshBuffer.offset = 0;

	KGFXmeshbuffer meshBuffers[] = { meshBuffer, indexMeshBuffer };

	KGFXmeshdesc meshDesc;
	meshDesc.pBuffers = meshBuffers;
	meshDesc.bufferCount = 2;
	KGFXmesh mesh = kgfxCreateMesh(ctx, meshDesc);
	if (mesh == KGFX_HANDLE_NULL) {
		return 1;
	}

	KGFXpipelinemesh pipelinemesh = kgfxPipelineAddMesh(ctx, pipeline, mesh, 0);
	if (pipelinemesh == KGFX_HANDLE_NULL) {
		return 1;
	}

	mat4x4 matrixData;
	mat4x4_identity(matrixData);

	KGFXbuffer uBuffer = kgfxCreateBuffer(ctx, (KGFXbufferdesc) {
		.location = KGFX_BUFFER_LOCATION_CPU,
		.usage = KGFX_BUFFER_USAGE_UNIFORM_BUFFER,
		.size = sizeof(f32) * 16,
		.pData = NULL,
	});

	KGFXuniformbuffer uniformBuffer = kgfxPipelineBindDescriptorSetBuffer(ctx, pipeline, uBuffer, 0, 0);
	if (uniformBuffer == KGFX_HANDLE_NULL) {
		printf("failed to bind uniform buffer\n");
		return 1;
	}

	int textureWidth, textureHeight;
	stbi_set_flip_vertically_on_load(1);
	u8* textureData = stbi_load("assets/texture.png", &textureWidth, &textureHeight, NULL, 4);
	if (textureData == NULL) {
		printf("failed to load texture\n");
		return 1;
	}

	KGFXtexturedesc textureDesc;
	textureDesc.format = KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM;
	textureDesc.width = textureWidth;
	textureDesc.height = textureHeight;
	textureDesc.depth = 0;

	KGFXtexture texture = kgfxCreateTexture(ctx, textureDesc);
	if (texture == KGFX_HANDLE_NULL) {
		printf("failed to create texture\n");
		return 1;
	}

	KGFXbufferdesc textureBufferDesc;
	textureBufferDesc.location = KGFX_BUFFER_LOCATION_GPU;
	textureBufferDesc.usage = KGFX_BUFFER_USAGE_TEXTURE_SRC;
	textureBufferDesc.size = textureWidth * textureHeight * 4;
	textureBufferDesc.pData = textureData;

	KGFXbuffer textureBuffer = kgfxCreateBuffer(ctx, textureBufferDesc);
	if (textureBuffer == KGFX_HANDLE_NULL) {
		printf("failed to create texture buffer\n");
		return 1;
	}

	stbi_image_free(textureData);
	if (kgfxCopyBufferToTexture(ctx, texture, textureBuffer, 0) != KGFX_SUCCESS) {
		printf("failed to copy buffer to texture\n");
		return 1;
	}

	KGFXsamplerdesc samplerDesc;
	samplerDesc.magFilter = KGFX_SAMPLER_FILTER_NEAREST;
	samplerDesc.minFilter = KGFX_SAMPLER_FILTER_NEAREST;
	samplerDesc.mipmapMode = KGFX_SAMPLER_FILTER_NEAREST;
	samplerDesc.addressModeU = KGFX_SAMPLER_ADDRESS_MODE_CLAMP;
	samplerDesc.addressModeV = KGFX_SAMPLER_ADDRESS_MODE_CLAMP;
	samplerDesc.addressModeW = KGFX_SAMPLER_ADDRESS_MODE_CLAMP;

	KGFXsampler sampler = kgfxCreateSampler(ctx, samplerDesc);
	if (sampler == KGFX_HANDLE_NULL) {
		printf("failed to create sampler\n");
		return 1;
	}

	KGFXpipelinetexture pipelineTexture = kgfxPipelineBindDescriptorSetTexture(ctx, pipeline, texture, sampler, 1);
	if (pipelineTexture == KGFX_HANDLE_NULL) {
		printf("failed to bind texture\n");
		return 1;
	}

	void* mapped = kgfxBufferMap(ctx, uBuffer);
	if (mapped == NULL) {
		printf("failed to map kgfx buffer\n");
		return 1;
	}

	camera_t camera = {
		.position = { 0, 0, 5 },
		.fov = 60.0f,
		.aspect = 800.0f / 600.0f,
		.near = 0.1f,
		.far = 100.0f,
	};
	mat4x4 m, v, p;
	mat4x4_identity(m);

	vec3 forward;
	vec3 up = { 0, 1, 0 };
	vec3 right;

	float delta = 0;
	float startTime = 0;
	const float moveSpeed = 5.0f;
	while (!glfwWindowShouldClose(window)) {
		if (key(GLFW_KEY_ESCAPE)) {
			break;
		}

		startTime = glfwGetTime();

		up[1] = 1;
		forward[0] = sinf(camera.rotation[1] * (M_PI / 180.0));
		forward[1] = -sinf(camera.rotation[0] * (M_PI / 180.0));
		forward[2] = -cosf(camera.rotation[1] * (M_PI / 180.0));
		vec3_mul_cross(right, forward, up);

		vec3_scale(forward, forward, delta * moveSpeed);
		vec3_scale(up, up, delta * moveSpeed);
		vec3_scale(right, right, delta * moveSpeed);

		if (key(GLFW_KEY_W)) {
			vec3_add(camera.position, camera.position, forward);
		}
		if (key(GLFW_KEY_S)) {
			vec3_sub(camera.position, camera.position, forward);
		}
		if (key(GLFW_KEY_A)) {
			vec3_sub(camera.position, camera.position, right);
		}
		if (key(GLFW_KEY_D)) {
			vec3_add(camera.position, camera.position, right);
		}
		if (key(GLFW_KEY_E)) {
			vec3_add(camera.position, camera.position, up);
		}
		if (key(GLFW_KEY_Q)) {
			vec3_sub(camera.position, camera.position, up);
		}

		if (key(GLFW_KEY_LEFT)) {
			camera.rotation[1] -= 1.0f;
		}
		if (key(GLFW_KEY_RIGHT)) {
			camera.rotation[1] += 1.0f;
		}
		if (key(GLFW_KEY_UP)) {
			camera.rotation[0] -= 1.0f;
		}
		if (key(GLFW_KEY_DOWN)) {
			camera.rotation[0] += 1.0f;
		}

		mat4x4_translate(v, -camera.position[0], camera.position[1], -camera.position[2]);
		mat4x4_perspective(p, camera.fov * (M_PI / 180.0), camera.aspect, camera.near, camera.far);
		mat4x4_rotate_X(p, p, -camera.rotation[0] * (M_PI / 180.0));
		mat4x4_rotate_Y(p, p, camera.rotation[1] * (M_PI / 180.0));
		mat4x4_rotate_Z(p, p, -camera.rotation[2] * (M_PI / 180.0));

		v[1][1] = -1;
		mat4x4_mul(matrixData, p, v);
		memcpy(mapped, matrixData, sizeof(f32) * 16);
		kgfxRender(ctx, pipeline);
		glfwSwapBuffers(window);

		memcpy(prev_keys, keys, sizeof(keys));
		glfwPollEvents();

		delta = glfwGetTime() - startTime;
	}
	kgfxBufferUnmap(ctx, uBuffer);

	printf("objects for krunching and destruktion\n");
	kgfxPipelineUnbindDescriptorSetTexture(ctx, pipeline, pipelineTexture);
	kgfxDestroySampler(ctx, sampler);
	kgfxDestroyBuffer(ctx, textureBuffer);
	kgfxDestroyTexture(ctx, texture);
	kgfxPipelineRemoveMesh(ctx, pipeline, pipelinemesh);
	kgfxPipelineUnbindDescriptorSetBuffer(ctx, pipeline, uniformBuffer);
	kgfxDestroyBuffer(ctx, uBuffer);
	kgfxDestroyMesh(ctx, mesh);
	kgfxDestroyBuffer(ctx, indexBuffer);
	kgfxDestroyBuffer(ctx, buffer);
	kgfxDestroyPipeline(ctx, pipeline);
	kgfxDestroyContext(ctx);
	*/
	return 0;
}
