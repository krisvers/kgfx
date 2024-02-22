#include "kgfx/kgfx.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <linmath.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef KGFX_MACOS
#include <unistd.h>
#endif

#include "kgfx_gh.h"

int main(int argc, char** argv) {
	if (!glfwInit()) {
		return 1;
	}

	#ifdef KGFX_MACOS
	if (argc >= 2) {
		chdir(argv[1]);
	}
	#endif

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "kgfx", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return 1;
	}
	glfwSwapInterval(1);

	KGFXcontext ctx;
	if (kgfxCreateContext(KGFX_ANY_VERSION, kgfxWindowFromGLFW(window), & ctx) != KGFX_SUCCESS) {
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
	
	KGFXshader vshader = kgfxCreateShader(ctx, (const void*) vsrc, (u32) vsize, KGFX_SHADERTYPE_VERTEX, KGFX_MEDIUM_SPIRV);
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
	
	KGFXshader fshader = kgfxCreateShader(ctx, (const void*) fsrc, (u32) fsize, KGFX_SHADERTYPE_FRAGMENT, KGFX_MEDIUM_SPIRV);
	free(fsrc);
	if (fshader == KGFX_HANDLE_NULL) {
		return 1;
	}

	KGFXpipelinedesc pipelineDesc;
	pipelineDesc.pShaders = (KGFXshader[]) { vshader, fshader };
	pipelineDesc.shaderCount = 2;

	KGFXpipelineattribute attributes[2] = {
		{ KGFX_DATATYPE_FLOAT2, 0 },
		{ KGFX_DATATYPE_FLOAT3, 1 },
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

	KGFXpipeline pipeline = kgfxCreatePipeline(ctx, pipelineDesc);
	if (pipeline == KGFX_HANDLE_NULL) {
		return 1;
	}
	kgfxDestroyShader(ctx, vshader);
	kgfxDestroyShader(ctx, fshader);

	f32 vertices[] = {
		 0.5f,  0.5f,	1, 0, 0,
		-0.5f,  0.5f,	0, 0, 0,
		 0.0f, -0.5f,	0.5f, 1, 0,
	};

	KGFXbufferdesc bufferDesc;
	bufferDesc.location = KGFX_BUFFER_LOCATION_GPU;
	bufferDesc.usage = KGFX_BUFFER_USAGE_VERTEX_BUFFER;
	bufferDesc.size = sizeof(vertices);
	bufferDesc.pData = vertices;

	KGFXbuffer buffer = kgfxCreateBuffer(ctx, bufferDesc);
	if (buffer == KGFX_HANDLE_NULL) {
		return 1;
	}

	u32 indices[] = {
		0, 1, 2,
	};

	KGFXbufferdesc indexBufferDesc;
	indexBufferDesc.location = KGFX_BUFFER_LOCATION_GPU;
	indexBufferDesc.usage = KGFX_BUFFER_USAGE_INDEX_BUFFER;
	indexBufferDesc.size = sizeof(indices);
	indexBufferDesc.pData = indices;

	KGFXbuffer indexBuffer = kgfxCreateBuffer(ctx, indexBufferDesc);
	if (indexBuffer == KGFX_HANDLE_NULL) {
		return 1;
	}

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
	mat4x4_translate(matrixData, 0.5f, 0.5f, 0.0f);

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

	void* mapped = kgfxBufferMap(ctx, uBuffer);
	if (mapped == NULL) {
		printf("failed to map kgfx buffer\n");
		return 1;
	}
	memcpy(mapped, matrixData, sizeof(f32) * 16);
	kgfxBufferUnmap(ctx, uBuffer);

	int textureWidth, textureHeight;
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

	while (!glfwWindowShouldClose(window)) {
		kgfxRender(ctx, pipeline);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
	return 0;
}
