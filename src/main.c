#include "kgfx/kgfx.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

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

	pipelineDesc.layout.pBindings = &binding;
	pipelineDesc.layout.bindingCount = 1;
	pipelineDesc.layout.pDescriptorSets = NULL;
	pipelineDesc.layout.descriptorSetCount = 0;

	KGFXpipeline pipeline = kgfxCreatePipeline(ctx, pipelineDesc);
	if (pipeline == KGFX_HANDLE_NULL) {
		return 1;
	}
	kgfxDestroyShader(ctx, vshader);
	kgfxDestroyShader(ctx, fshader);

	f32 vertices[] = {
		 0.5f,  0.5f,	1, 1, 0,
		-0.5f,  0.5f,	1, 0, 1,
		 0.0f, -0.5f,	0, 1, 1,
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

	KGFXmeshbuffer meshBuffer;
	meshBuffer.buffer = buffer;
	meshBuffer.bindpoint = KGFX_MESH_BUFFER_BINDPOINT_VERTEX;
	meshBuffer.offset = 0;

	KGFXmeshdesc meshDesc;
	meshDesc.pBuffers = &meshBuffer;
	meshDesc.bufferCount = 1;
	KGFXmesh mesh = kgfxCreateMesh(ctx, meshDesc);
	if (mesh == KGFX_HANDLE_NULL) {
		return 1;
	}

	KGFXpipelinemesh pipelinemesh = kgfxPipelineAddMesh(ctx, pipeline, mesh, 0);
	if (pipelinemesh == KGFX_HANDLE_NULL) {
		return 1;
	}

	while (!glfwWindowShouldClose(window)) {
		kgfxRender(ctx, pipeline);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	printf("objects of crunching and destruction\n");
	kgfxDestroyBuffer(ctx, buffer);
	kgfxDestroyPipeline(ctx, pipeline);
	kgfxDestroyContext(ctx);
	return 0;
}
