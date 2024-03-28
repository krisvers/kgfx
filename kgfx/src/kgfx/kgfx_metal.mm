#include <kgfx/kgfx.h>

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <string>
#include <iostream>
#include <sstream>

#define KGFX_IMPL_VERSION KGFX_MAKE_VERSION(0, 1, 0)
#define KGFX_IMPL_BACKEND KGFX_BACKEND_METAL

#ifdef KGFX_DEBUG
#define DEBUG_OUT(msg) debugFunc(msg)
#define DEBUG_OUTF(fmt, ...) debugFunc(fmt, __VA_ARGS__)
#else
#define DEBUG_OUT(msg)
#define DEBUG_OUTF(fmt, ...)
#endif

static void debugFuncConcat(std::stringstream& stream, std::string& format) {
	stream << format;
}

template<typename T, typename... Types>
static void debugFuncConcat(std::stringstream& stream, std::string& format, T& arg, Types&... args) {
	usize index = format.find("{}");
	if (index == std::string::npos) {
		stream << format;
		return;
	}

	stream << std::string(format.begin(), format.begin() + index);
	stream << arg;
	if (index + 2 < format.size()) {
		format = std::string(format.begin() + index + 2, format.end());
		debugFuncConcat(stream, format, args...);
	}
}

static void debugFunc(const char* format) {
	std::cout << "KGFX debug: " << format << std::endl;
}

template<typename... Types>
static void debugFunc(const char* format, Types... types) {
	std::string fmt = format;
	std::stringstream stream;

	debugFuncConcat(stream, fmt, types...);

	std::cout << "KGFX debug: " << stream.str() << std::endl;
}

struct MetalRenderer {
	NSWindow* nsWindow;

	int init(NSWindow* w);
};

struct KGFXcontext_t {
	MetalRenderer renderer;
};

KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context) {
	@autoreleasepool {
		if (context == nullptr) {
			return KGFX_NULL_OUTPUT_ARGUMENT;
		}

		if (KGFX_MAJOR(version) != KGFX_ANY_MAJOR && KGFX_MAJOR(version) != KGFX_MAJOR(KGFX_IMPL_VERSION)) {
			return KGFX_VERSION_MAJOR_MISMATCH;
		}

		if (KGFX_MINOR(version) != KGFX_ANY_MINOR && KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VERSION)) {
			return KGFX_VERSION_NOT_SUPPORTED;
		}

		KGFXcontext ctx = new KGFXcontext_t();
		// convert void* window.window to NSWindow*
		NSWindow* w = (__bridge NSWindow*) window.window;
		if (ctx->renderer.init(w) != 0) {
			return KGFX_GENERIC_ERROR;
		}

		return KGFX_SUCCESS;
	}
}

void kgfxDestroyContext(KGFXcontext ctx) {

}

KGFXshader kgfxCreateShader(KGFXcontext ctx, KGFXshaderdesc shaderDesc) {
	return nullptr;
}

void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {

}

KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	return nullptr;
}

void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline) {

}

KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc) {
	return nullptr;
}

KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data) {
	return KGFX_GENERIC_ERROR;
}

void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer) {
	return nullptr;
}

void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer) {

}

KGFXresult kgfxBufferCopy(KGFXcontext ctx, KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	return KGFX_GENERIC_ERROR;
}

u32 kgfxBufferSize(KGFXcontext ctx, KGFXbuffer buffer) {
	return 0;
}

void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer) {

}

KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc) {
	return nullptr;
}

KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	return KGFX_GENERIC_ERROR;
}

void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture) {

}

KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc) {
	return nullptr;
}

void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler) {

}

KGFXcommandlist kgfxCreateCommandList(KGFXcontext ctx) {
	return nullptr;
}

void kgfxCommandReset(KGFXcontext ctx, KGFXcommandlist commandList) {

}

void kgfxCommandBindPipeline(KGFXcontext ctx, KGFXcommandlist commandList, KGFXpipeline pipeline) {

}

void kgfxCommandBindVertexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {

}

void kgfxCommandBindIndexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {

}

void kgfxCommandBindDescriptorSetBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {

}

void kgfxCommandBindDescriptorSetTexture(KGFXcontext ctx, KGFXcommandlist commandList, KGFXtexture texture, KGFXsampler sampler, u32 binding) {

}

void kgfxCommandDraw(KGFXcontext ctx, KGFXcommandlist commandList, u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {

}

void kgfxCommandDrawIndexed(KGFXcontext ctx, KGFXcommandlist commandList, u32 indexCount, u32 instanceCount, u32 firstIndex, s32 vertexOffset, u32 firstInstance) {

}

void kgfxCommandPresent(KGFXcontext ctx, KGFXcommandlist commandList) {

}

void kgfxCommandListSubmit(KGFXcontext ctx, KGFXcommandlist commandList) {

}

void kgfxDestroyCommandList(KGFXcontext ctx, KGFXcommandlist commandList) {

}

u32 kgfxGetImplementationVersion(void) {
	return KGFX_IMPL_VERSION;
}

u32 kgfxGetBackends(KGFXbackend* pBackends, u32 count) {
	if (pBackends == nullptr) {
		return 1;
	}

	if (count < 1) {
		return count;
	}

	pBackends[0] = KGFX_IMPL_BACKEND;
	return 1;
}
