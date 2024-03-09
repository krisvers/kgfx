#include <kgfx/kgfx.h>
#include <glad/glad.h>
#include <iostream>

#define KGFX_IMPL_VERSION KGFX_MAKE_VERSION(0, 1, 0)

static s8 dummy = 0;

struct KGFXshader_t {
	GLuint handle;
};

struct KGFXpipeline_t {
	GLuint program;

};

KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context) {
	if (context == NULL) {
		std::cout << "Invalid out pointer to context\n";
		return KGFX_INVALID_ARGUMENT;
	}

	*((s8**) context) = &dummy;
	return KGFX_SUCCESS;
}

void kgfxDestroyContext(KGFXcontext ctx) {
	return;
}

KGFXshader kgfxCreateShader(KGFXcontext ctx, KGFXshaderdesc shaderDesc) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_HANDLE_NULL;
	}

	if (shaderDesc.type > KGFX_SHADERTYPE_MAX || shaderDesc.type < KGFX_SHADERTYPE_MIN) {
		std::cout << "Invalid shader type\n";
		return KGFX_HANDLE_NULL;
	}

	if (shaderDesc.type == KGFX_SHADERTYPE_COMPUTE) {
		std::cout << "Implementation does not support compute shaders\n";
		return KGFX_HANDLE_NULL;
	}

	if (shaderDesc.medium != KGFX_MEDIUM_GLSL) {
		std::cout << "Implementation does not support non-GLSL shaders\n";
		return KGFX_HANDLE_NULL;
	}

	GLuint handle = glCreateShader(shaderTypeToGLEnum(shaderDesc.type));
	if (handle == 0) {
		return KGFX_HANDLE_NULL;
	}

	KGFXshader kgfxShader = new KGFXshader_t();
	kgfxShader->handle = handle;
	return kgfxShader;
}

void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return;
	}

	if (shader == KGFX_HANDLE_NULL) {
		std::cout << "Invalid shader\n";
		return;
	}

	if (shader->handle != 0) {
		glDeleteShader(shader->handle);
	}

	delete shader;
}

KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_HANDLE_NULL;
	}

	return (KGFXpipeline) &dummy;
}

void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline) {
	return;
}

KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_HANDLE_NULL;
	}

	u32* buffer = new u32[bufferDesc.size];
	if (buffer == NULL) {
		return KGFX_HANDLE_NULL;
	}

	*buffer = bufferDesc.size;
	return (KGFXbuffer) buffer;
}

KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_INVALID_CONTEXT;
	}

	if (buffer == NULL) {
		return KGFX_INVALID_ARGUMENT;
	}

	if (size > *((u32*) buffer)) {
		return KGFX_INVALID_ARGUMENT;
	}

	return KGFX_SUCCESS;
}

void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return NULL;
	}

	if (buffer == NULL) {
		return NULL;
	}

	return (u32*) ((usize) buffer + sizeof(u32));
}

void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer) {
	return;
}

u32 kgfxBufferSize(KGFXcontext ctx, KGFXbuffer buffer) {
	return *((u32*) buffer);
}

KGFXresult kgfxBufferCopy(KGFXcontext ctx, KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_INVALID_CONTEXT;
	}

	if (dstBuffer == NULL) {
		return KGFX_INVALID_ARGUMENT;
	}

	if (srcBuffer == NULL) {
		return KGFX_INVALID_ARGUMENT;
	}

	return KGFX_SUCCESS;
}

void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return;
	}

	if (buffer == NULL) {
		return;
	}

	delete[] ((u32*) buffer);
	return;
}

KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_HANDLE_NULL;
	}

	return (KGFXtexture) &dummy;
}

KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_INVALID_CONTEXT;
	}

	if (dstTexture == NULL) {
		return KGFX_INVALID_ARGUMENT;
	}

	if (srcBuffer == NULL) {
		return KGFX_INVALID_ARGUMENT;
	}

	return KGFX_SUCCESS;
}

void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture) {
	return;
}

KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_HANDLE_NULL;
	}

	return (KGFXsampler) &dummy;
}

void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler) {
	return;
}

KGFXcommandlist kgfxCreateCommandList(KGFXcontext ctx) {
	if (ctx == NULL) {
		std::cout << "Invalid context\n";
		return KGFX_HANDLE_NULL;
	}

	return (KGFXcommandlist) &dummy;
}

void kgfxCommandReset(KGFXcontext ctx, KGFXcommandlist commandList) {
	return;
}

void kgfxCommandBindPipeline(KGFXcontext ctx, KGFXcommandlist commandList, KGFXpipeline pipeline) {
	return;
}

void kgfxCommandBindVertexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	return;
}

void kgfxCommandBindIndexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	return;
}

void kgfxCommandBindDescriptorSetBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	return;
}

void kgfxCommandBindDescriptorSetTexture(KGFXcontext ctx, KGFXcommandlist commandList, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	return;
}

void kgfxCommandDraw(KGFXcontext ctx, KGFXcommandlist commandList, u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
	return;
}

void kgfxCommandDrawIndexed(KGFXcontext ctx, KGFXcommandlist commandList, u32 indexCount, u32 instanceCount, u32 firstIndex, s32 vertexOffset, u32 firstInstance) {
	return;
}

void kgfxCommandPresent(KGFXcontext ctx, KGFXcommandlist commandList) {
	return;
}

void kgfxCommandListSubmit(KGFXcontext ctx, KGFXcommandlist commandList) {
	return;
}

void kgfxDestroyCommandList(KGFXcontext ctx, KGFXcommandlist commandList) {
	return;
}

u32 kgfxGetImplementationVersion(void) {
	return KGFX_IMPL_VERSION;
}

KGFXbackend kgfxGetBackend(void) {
	return KGFX_BACKEND_OPENGL;
}
