#include <kgfx/kgfx.h>
#include <kgfx_gh/kgfx_gh.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>

int example_hlsl() {
	if (!glfwInit()) {
		fprintf(stderr, "GLFW init failed\n");
		return 1;
	}

	/* make sure glfw does not make an opengl context */
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "example_triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "GLFW window creation failed\n");
		glfwTerminate();
		return 1;
	}
	
	/* set platform specific window values using GLFWwindow* */
	KGFXwindow kgfxWindow = kgfxWindowFromGLFW(window);

	/* create the kgfx context which manages a vulkan, d3d12, or metal instance depending on which API you choose */
	KGFXcontext ctx;
	if (kgfxCreateContext(KGFX_ANY_VERSION, kgfxWindow, &ctx) != KGFX_SUCCESS) {
		fprintf(stderr, "kgfxCreateContext failed\n");
		return 1;
	}

	/*
		struct vinput_t {
			float3 position : POSITION;
		};

		struct pinput_t {
			float4 position : SV_POSITION;
		};

		pinput_t vmain(vinput_t input) {
			pinput_t output;
			output.position = float4(input.position, 1.0f);
			return output;
		}

		float4 pmain(pinput_t input) : SV_TARGET {
			return float4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	*/
	const char* shader_hlsl = "struct vinput_t { float3 position : POSITION; }; struct pinput_t { float4 position : SV_POSITION; }; pinput_t vmain(vinput_t input) { pinput_t output; output.position = float4(input.position, 1.0f); return output; } float4 pmain(pinput_t input) : SV_TARGET { return float4(1.0f, 0.0f, 0.0f, 1.0f); }";

	KGFXshaderdesc shaderDesc = {
		.entryName = "vmain",
		.type = KGFX_SHADERTYPE_VERTEX,
		.medium = KGFX_MEDIUM_HLSL,
		.pData = (const void*) shader_hlsl,
		.size = strlen(shader_hlsl),
	};

	/* compile the shaders and setup for pipeline use */
	KGFXshader shaders[2];
	shaders[0] = kgfxCreateShader(ctx, shaderDesc);
	if (shaders[0] == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateShader vertex shader failed\n");
		return 1;
	}

	shaderDesc.entryName = "pmain";
	shaderDesc.type = KGFX_SHADERTYPE_FRAGMENT;
	shaderDesc.pData = (const void*) shader_hlsl;
	shaderDesc.size = strlen(shader_hlsl);

	shaders[1] = kgfxCreateShader(ctx, shaderDesc);
	if (shaders[1] == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateShader fragment shader failed\n");
		return 1;
	}

	/* describe vertex attribute of vec2/float2 for the position at location 0 */
	KGFXpipelineattribute attribute = {
		.semanticName = "POSITION",
		.location = 0,
		.type = KGFX_DATATYPE_FLOAT2,
	};

	/* describe a binding */
	KGFXpipelinebinding binding = {
		.inputRate = KGFX_VERTEX_INPUT_RATE_VERTEX,
		.pAttributes = &attribute,
		.attributeCount = 1,
		.bindpoint = KGFX_BINDPOINT_VERTEX,
		.binding = 0,
	};

	/* pipeline description */
	KGFXpipelinedesc pipelineDesc = {
		.pShaders = shaders,
		.shaderCount = 2,
		.layout = {
			.pBindings = &binding,
			.bindingCount = 1,
			.pDescriptorSets = NULL,
			.descriptorSetCount = 0,
		},
		.cullMode = KGFX_CULLMODE_NONE,
		.frontFace = KGFX_FRONTFACE_CCW,
		.fillMode = KGFX_FILLMODE_SOLID,
		.topology = KGFX_TOPOLOGY_TRIANGLES,
	};

	/* create pipeline based on description */
	KGFXpipeline pipeline = kgfxCreatePipeline(ctx, pipelineDesc);
	if (pipeline == NULL) {
		fprintf(stderr, "kgfxCreatePipeline failed\n");
		return 1;
	}

	/* vertex positions for triangle */
	f32 vertices[6] = {
		 0.0f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	/* GPU sided vertex buffer with data from [vertices] */
	KGFXbufferdesc bufferDesc = {
		.location = KGFX_BUFFER_LOCATION_GPU,
		.usage = KGFX_BUFFER_USAGE_VERTEX_BUFFER,
		.size = sizeof(vertices),
		.pData = vertices,
	};

	/* create the buffer */
	KGFXbuffer buffer = kgfxCreateBuffer(ctx, bufferDesc);
	if (buffer == NULL) {
		fprintf(stderr, "kgfxCreateBuffer failed\n");
		return 1;
	}

	/* describe where the mesh is bound */
	KGFXmeshbuffer meshBuffer = {
		.buffer = buffer,
		.bindpoint = KGFX_MESH_BUFFER_BINDPOINT_VERTEX,
		.offset = 0,
	};

	/* describe the buffers used in the mesh */
	KGFXmeshdesc meshDesc = {
		.pBuffers = &meshBuffer,
		.bufferCount = 1,
	};

	/*
		create the mesh
		this does not copy the data, but instead uses the meshes themselves as the buffer, any change to the buffer affects the mesh
	*/
	KGFXmesh mesh = kgfxCreateMesh(ctx, meshDesc);
	if (mesh == NULL) {
		fprintf(stderr, "kgfxCreateMesh failed\n");
		return 1;
	}

	/* push the mesh onto the pipeline's render list */
	KGFXpipelinemesh pipelineMesh = kgfxPipelineAddMesh(ctx, pipeline, mesh, 0);
	if (pipelineMesh == NULL) {
		fprintf(stderr, "kgfxPipelineAddMesh failed\n");
		return 1;
	}

	while (!glfwWindowShouldClose(window)) {
		/* render all valid meshes on [pipeline]'s render list */
		kgfxRender(ctx, pipeline);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* remove the mesh from the pipeline's render list */
	kgfxPipelineRemoveMesh(ctx, pipeline, pipelineMesh);
	/* destroy the mesh */
	kgfxDestroyMesh(ctx, mesh);
	/* destroy the buffer */
	kgfxDestroyBuffer(ctx, buffer);
	/* destroy the pipeline */
	kgfxDestroyPipeline(ctx, pipeline);
	/* destroy shaders */
	kgfxDestroyShader(ctx, shaders[0]);
	kgfxDestroyShader(ctx, shaders[1]);
	/* destroy context */
	kgfxDestroyContext(ctx);
	return 0;
}
