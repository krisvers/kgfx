#include "kgfx.h"
#include <stdlib.h>
#include <stdio.h>

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/QuartzCore.h>

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
	id<MTLCommandQueue> commandQueue;
	id<MTLLibrary> library;
	id<MTLRenderPipelineState> pipelineState;
	id<CAMetalDrawable> drawable;

	int init();
	void destroy();
	void getDrawable();
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

struct KGFXtexture_t {
	int temp;
};

struct KGFXsampler_t {
	int temp;
};

struct KGFXuniformbuffer_t {
	int temp;
};

struct KGFXpipelinetexture_t {
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

KGFXuniformbuffer kgfxPipelineBindDescriptorSetBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding, u32 offset) {
	return new KGFXuniformbuffer_t;
}

void kgfxPipelineUnbindDescriptorSetBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer) {
	delete uniformBuffer;
}

KGFXpipelinetexture kgfxPipelineBindDescriptorSetTexture(KGFXcontext ctx, KGFXpipeline pipeline, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	return new KGFXpipelinetexture_t;
}

void kgfxPipelineUnbindDescriptorSetTexture(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinetexture pipelineTexture) {
	delete pipelineTexture;
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

void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer) {
	return nullptr;
}

void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer) {

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

KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc) {
	return new KGFXtexture_t;
}

KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	return KGFX_SUCCESS;
}

void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture) {
	delete texture;
}

KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc) {
	return new KGFXsampler_t;
}

void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler) {
	delete sampler;
}

void kgfxRender(KGFXcontext ctx, KGFXpipeline pipeline) {

}

int Metal::init() {
	device = MTLCreateSystemDefaultDevice();
	layer.device = device;
	layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
	layer.frame = [view bounds];
	[[view layer] addSublayer:layer];
	commandQueue = [device newCommandQueue];

	MTLCompileOptions* compileOptions = [MTLCompileOptions new];

	NSError* error = NULL;
	library = [device newLibraryWithSource:@R"(
		#include <metal_stdlib>
		typedef struct {
			float2 pos;
		} vinput_t;

		typedef struct {
			float4 position [[position]];
		} voutput_t;

		vertex voutput_t vertex_function(device vinput_t* vertices [[buffer(0)]], uint vid [[vertex_id]]) {
			voutput_t out;
			out.position = float4(vertices[vid].pos, 0.0, 1.0);
			return out;
		}
		
		fragment half4 fragment_function(voutput_t in [[stage_in]]) {
			return half4(1.0, 0.0, 1.0, 1.0);
		}
	)" options:compileOptions error:&error];
	if (library == nil) {
		NSLog(@"Failed to compile shader library, error %@", error);
		return 1;
	}

	f32 vertices[6] = {
		-0.5f, 0.5f,
		0.0f, -0.5f,
		0.5f, 0.5f,
	};
	id<MTLBuffer> vertexBuffer = [device newBufferWithBytes:vertices length:sizeof(vertices) options:MTLResourceOptionCPUCacheModeDefault];

	id<MTLFunction> vertexProgram = [library newFunctionWithName:@"vertex_function"];
	id<MTLFunction> fragmentProgram = [library newFunctionWithName:@"fragment_function"];

	MTLRenderPipelineDescriptor* pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
	[pipelineStateDescriptor setVertexFunction:vertexProgram];
	[pipelineStateDescriptor setFragmentFunction:fragmentProgram];
	pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

	MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];
	vertexDescriptor.attributes[0].format = MTLVertexFormatFloat2;
	vertexDescriptor.attributes[0].offset = 0;
	vertexDescriptor.attributes[0].bufferIndex = 0;

	pipelineStateDescriptor.vertexDescriptor = vertexDescriptor;

    pipelineState = [device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
    if (pipelineState == nil) {
        NSLog(@"Failed to create pipeline state, error %@", error);
		return 1;
    }

	return 0;
}

void Metal::destroy() {
	
}


void Metal::getDrawable() {
	id<CAMetalDrawable> d = nil;
	while (d == nil) {
		d = [layer nextDrawable];
	}

	drawable = d;
}
