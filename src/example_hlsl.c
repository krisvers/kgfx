#include <kgfx/kgfx.h>
#include <kgfx_gh/kgfx_gh.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <linmath.h>

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

		cbuffer ubo_t : register(b0) {
			float4x4 mvp;
		};

		Texture2D texture : register(t0);
		SamplerState sampler : register(s0);

		pinput_t vmain(vinput_t input) {
			pinput_t output;
			output.position = mul(float4(input.position, 1.0f), mvp);
			return output;
		}

		float4 pmain(pinput_t input) : SV_TARGET {
			return texture.Sample(sampler, input.position.xy);
		}
	*/
	const char* shader_hlsl = "struct vinput_t { float3 position : POSITION; }; struct pinput_t { float4 position : SV_POSITION; }; cbuffer ubo_t : register(b0) { float4x4 mvp; }; pinput_t vmain(vinput_t input) { pinput_t output; output.position = mul(float4(input.position, 1.0f), mvp); return output; } float4 pmain(pinput_t input) : SV_TARGET { return float4(1, 0, 0, 1); }";

	KGFXshaderdesc shaderDesc = {
		.entryName = "vmain",
		.type = KGFX_SHADERTYPE_VERTEX,
		.medium = KGFX_MEDIUM_HLSL,
		.pData = (const void*) shader_hlsl,
		.size = (u32) strlen(shader_hlsl),
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
	shaderDesc.size = (u32) strlen(shader_hlsl);

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

	KGFXdescriptorsetdesc descriptorSetDescs[2] = {
		{
			.bindpoint = KGFX_BINDPOINT_VERTEX,
			.binding = 0,
			.usage = KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER,
			.size = 64,
		},
		{
			.bindpoint = KGFX_BINDPOINT_FRAGMENT,
			.binding = 0,
			.usage = KGFX_DESCRIPTOR_USAGE_TEXTURE,
			.size = 0,
		},
	};

	/* pipeline description */
	KGFXpipelinedesc pipelineDesc = {
		.pShaders = shaders,
		.shaderCount = 2,
		.layout = {
			.pBindings = &binding,
			.bindingCount = 1,
			.pDescriptorSets = descriptorSetDescs,
			.descriptorSetCount = 1,
		},
		.cullMode = KGFX_CULLMODE_NONE,
		.frontFace = KGFX_FRONTFACE_CCW,
		.fillMode = KGFX_FILLMODE_SOLID,
		.topology = KGFX_TOPOLOGY_TRIANGLES,
	};

	/* create pipeline based on description */
	KGFXpipeline pipeline = kgfxCreatePipeline(ctx, pipelineDesc);
	if (pipeline == KGFX_HANDLE_NULL) {
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
	if (buffer == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateBuffer failed\n");
		return 1;
	}

	u32 indices[3] = {
		0, 1, 2
	};

	/* GPU sided index buffer with data from [indices] */
	bufferDesc.usage = KGFX_BUFFER_USAGE_INDEX_BUFFER;
	bufferDesc.size = sizeof(indices);
	bufferDesc.pData = indices;

	/* create the buffer */
	KGFXbuffer indexBuffer = kgfxCreateBuffer(ctx, bufferDesc);
	if (indexBuffer == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateBuffer failed\n");
		return 1;
	}

	/* create a uniform buffer */
	mat4x4 mvp;
	mat4x4_identity(mvp);

	/* CPU sided uniform buffer with data from [mvp] */
	bufferDesc.location = KGFX_BUFFER_LOCATION_CPU;
	bufferDesc.usage = KGFX_BUFFER_USAGE_UNIFORM_BUFFER;
	bufferDesc.size = sizeof(mvp);
	bufferDesc.pData = mvp;

	KGFXbuffer uniformBuffer = kgfxCreateBuffer(ctx, bufferDesc);
	if (uniformBuffer == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateBuffer failed\n");
		return 1;
	}

	int width, height, channels;
	unsigned char* pixels = stbi_load("assets/texture.png", &width, &height, &channels, STBI_rgb_alpha);
	if (pixels == NULL) {
		fprintf(stderr, "stbi_load failed\n");
		return 1;
	}

	KGFXtexturedesc textureDesc = {
		.width = width,
		.height = height,
		.depth = 0,
		.format = KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM,
	};

	KGFXtexture texture = kgfxCreateTexture(ctx, textureDesc);
	if (texture == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateTexture failed\n");
		return 1;
	}

	bufferDesc.location = KGFX_BUFFER_LOCATION_CPU;
	bufferDesc.usage = KGFX_BUFFER_USAGE_TEXTURE_SRC;
	bufferDesc.size = width * height * STBI_rgb_alpha;
	bufferDesc.pData = pixels;
	KGFXbuffer stagingBuffer = kgfxCreateBuffer(ctx, bufferDesc);
	if (stagingBuffer == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateBuffer failed\n");
		return 1;
	}

	stbi_image_free(pixels);

	if (kgfxCopyBufferToTexture(ctx, texture, stagingBuffer, 0) != KGFX_SUCCESS) {
		fprintf(stderr, "kgfxCopyBufferToTexture failed\n");
		return 1;
	}

	kgfxDestroyBuffer(ctx, stagingBuffer);

	KGFXsamplerdesc samplerDesc = {
		.minFilter = KGFX_SAMPLER_FILTER_LINEAR,
		.magFilter = KGFX_SAMPLER_FILTER_LINEAR,
		.mipmapMode = KGFX_SAMPLER_FILTER_LINEAR,
		.addressModeU = KGFX_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = KGFX_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = KGFX_SAMPLER_ADDRESS_MODE_REPEAT,
	};

	KGFXsampler sampler = kgfxCreateSampler(ctx, samplerDesc);
	if (sampler == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateSampler failed\n");
		return 1;
	}

	/* create a command list */
	KGFXcommandlist commandList = kgfxCreateCommandList(ctx);
	if (commandList == KGFX_HANDLE_NULL) {
		fprintf(stderr, "kgfxCreateCommandListList failed\n");
		return 1;
	}

	void* mapped = kgfxBufferMap(ctx, uniformBuffer);
	if (mapped == NULL) {
		fprintf(stderr, "kgfxMapBuffer failed\n");
		return 1;
	}

	while (!glfwWindowShouldClose(window)) {
		mat4x4_identity(mvp);
		mat4x4_rotate_Z(mvp, mvp, (float) glfwGetTime());
		memcpy(mapped, mvp, sizeof(mvp));

		kgfxCommandReset(ctx, commandList);
		kgfxCommandBindVertexBuffer(ctx, commandList, buffer, 0);
		kgfxCommandBindIndexBuffer(ctx, commandList, indexBuffer, 0);
		kgfxCommandBindPipeline(ctx, commandList, pipeline);
		kgfxCommandBindDescriptorSetBuffer(ctx, commandList, uniformBuffer, 0);
		//kgfxCommandBindDescriptorSetTexture(ctx, commandList, texture, sampler, 0);
		kgfxCommandDrawIndexed(ctx, commandList, 3, 1, 0, 0, 0);
		kgfxCommandPresent(ctx, commandList);
		kgfxCommandListSubmit(ctx, commandList);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	kgfxBufferUnmap(ctx, uniformBuffer);

	/* destroy the command list */
	kgfxDestroyCommandList(ctx, commandList);
	/* destroy the sampler */
	kgfxDestroySampler(ctx, sampler);
	/* destroy the texture */
	kgfxDestroyTexture(ctx, texture);
	/* destroy the uniform buffer */
	kgfxDestroyBuffer(ctx, uniformBuffer);
	/* destroy the index buffer */
	kgfxDestroyBuffer(ctx, indexBuffer);
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
