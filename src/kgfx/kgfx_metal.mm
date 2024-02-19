#include "kgfx.h"
#include <stdlib.h>
#include <stdio.h>

#import <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/QuartzCore.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(1, 0, 0)

struct Metal {
	NSWindow* window;
	NSView* view;
	CAMetalLayer* layer;
	KGFXcontext context;

	id<MTLDevice> device;
	id<MTLCommandQueue> commandQueue

	int init();
	void destroy();
};

typedef struct KGFXcontext_t {
	u32 version;

	Metal metal;
	KGFXwindow window;
} KGFXcontext_t;

struct KGFXpipeline_t {
	int temp;
};

struct KGFXrenderpass_t {
	int temp;
};

struct KGFXshader_t {
	int temp;
};

struct KGFXbuffer_t {
	int temp;
};

struct KGFXmesh_t {
	int temp;
};

struct KGFXpipelinemesh_t {
	int temp;
};

KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context) {
	if (context == NULL) {
		return KGFX_NULL_OUTPUT_ARGUMENT;
	}

	if ((KGFX_MAJOR(version) > KGFX_MAJOR(KGFX_IMPL_VER) && KGFX_MAJOR(version) != KGFX_ANY_MAJOR) || (KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VER) && KGFX_MINOR(version) != KGFX_ANY_MINOR) || (KGFX_PATCH(version) > KGFX_PATCH(KGFX_IMPL_VER) && KGFX_PATCH(version) != KGFX_ANY_PATCH)) {
		return KGFX_VERSION_NOT_SUPPORTED;
	}

	KGFXcontext ctx = new KGFXcontext_t;
	ctx->version = (version == KGFX_ANY_VERSION) ? KGFX_IMPL_VER : version;
	ctx->metal.window = reinterpret_cast<NSWindow*>(window.window);
	ctx->metal.view = reinterpret_cast<NSView*>(window.contentView);
	ctx->metal.layer = reinterpret_cast<CAMetalLayer*>(window.layer);
	ctx->metal.context = ctx;
	if (ctx->metal.init() != 0) {
		return KGFX_GENERIC_ERROR;
	}

	*context = ctx;
	return KGFX_SUCCESS;
}

void kgfxDestroyContext(KGFXcontext context) {
	if (context == NULL) {
		return;
	}

	context->metal.destroy();
	free(context);
}

u32 kgfxGetImplementationVersion() {
	return KGFX_IMPL_VER;
}

KGFXshader kgfxCreateShader(KGFXcontext ctx, const void* data, u32 size, KGFXshadertype type, KGFXshadermedium medium) {
	return new KGFXshader_t;
}

void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {
	delete shader;
}

KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	return new KGFXpipeline_t;
}

KGFXpipelinemesh kgfxPipelineAddMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXmesh mesh, u32 binding) {
	return new KGFXpipelinemesh_t;
}

void kgfxPipelineRemoveMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinemesh pipelineMesh) {
	delete pipelineMesh;
}

void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline) {
	delete pipeline;
}

KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc) {
	return new KGFXbuffer_t;
}

KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data) {
	return KGFX_SUCCESS;
}

void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer) {
	delete buffer;
}

KGFXmesh kgfxCreateMesh(KGFXcontext ctx, KGFXmeshdesc meshDesc) {
	return new KGFXmesh_t;
}

void kgfxDestroyMesh(KGFXcontext ctx, KGFXmesh mesh) {
	delete mesh;
}

void kgfxRender(KGFXcontext ctx, KGFXpipeline pipeline) {

}

int Metal::init() {
	device = MTLCreateSystemDefaultDevice();
	[layer setDevice:device];
	[layer setPixelFormat:MTLPixelFormatBGRA8Unorm];
	[layer setFrame:[view bounds]];
	[[view layer] addSublayer:layer];

	return 0;
}

void Metal::destroy() {
	
}
