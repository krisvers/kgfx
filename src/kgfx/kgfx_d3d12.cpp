#include <kgfx/kgfx.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <vector>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(0, 1, 0)
#define KGFX_VK_TARGET_FRAMES 2

#ifdef KGFX_DEBUG
#define DEBUG_OUT(msg) debugFunc(msg)
#define DEBUG_OUTF(fmt, ...) debugFunc(fmt, __VA_ARGS__)
#else
#define DEBUG_OUT(msg)
#define DEBUG_OUTF(fmt, ...)
#endif

struct D3D12 {
	ID3D12Debug* debug;
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;
	ID3D12Device* device;

	ID3D12CommandAllocator* commandAllocator;
	ID3D12CommandQueue* commandQueue;

	IDXGISwapChain4* swapchain;

	HRESULT init();
	void destroy();

	void render(KGFXpipeline pipeline);

	HRESULT createDevice();
	HRESULT createSwapchain();
	HRESULT createCommandUtilities();
	HRESULT createSyncUtilities();

	RECT getWindowExtent();
	HRESULT recreateSwapchain();

	KGFXbuffer createBuffer(KGFXbufferdesc bufferDesc);
	KGFXresult uploadBuffer(KGFXbuffer buffer, u32 size, void* data);
	void* mapBuffer(KGFXbuffer buffer);
	void unmapBuffer(KGFXbuffer buffer);
	KGFXresult copyBuffer(KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset);
	KGFXresult copyBufferToTexture(KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset);

	KGFXmesh createMesh(KGFXmeshdesc meshDesc);
	KGFXtexture createTexture(KGFXtexturedesc textureDesc);
	KGFXsampler createSampler(KGFXsamplerdesc samplerDesc);

	KGFXshader createShader(const void* data, u32 size, KGFXshadertype type, KGFXshadermedium medium);
	KGFXpipeline createPipeline(KGFXpipelinedesc pipelineDesc);

	KGFXresult pipelineUpdateDescriptorSets(KGFXpipeline pipeline);

	void pipelineUnbindDescriptorSet(KGFXpipeline pipeline, struct KGFXdescriptorset_t& set);

	KGFXpipelinemesh pipelineAddMesh(KGFXpipeline pipeline, KGFXmesh mesh, u32 binding);
	void pipelineRemoveMesh(KGFXpipeline pipeline, KGFXpipelinemesh mesh);
	KGFXpipelinetexture pipelineBindDescriptorSetTexture(KGFXpipeline pipeline, KGFXtexture texture, KGFXsampler sampler, u32 binding);
	void pipelineUnbindDescriptorSetTexture(KGFXpipeline pipeline, KGFXpipelinetexture pipelineTexture);

	KGFXuniformbuffer pipelineBindDescriptorSetBuffer(KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding, u32 offset);
	void pipelineUnbindDescriptorSetBuffer(KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer);

	void destroyShader(KGFXshader shader);
	void destroyPipeline(KGFXpipeline pipeline);
	void destroyBuffer(KGFXbuffer buffer);
	void destroyMesh(KGFXmesh mesh);
	void destroyTexture(KGFXtexture texture);
	void destroySampler(KGFXsampler sampler);
};

struct KGFXcontext_t {
	u32 version;

	KGFXwindow window;
	D3D12 d3d12;
};

struct KGFXshader_t {
	s8 temp;
};

struct KGFXpipeline_t {
	s8 temp;
};

struct KGFXrenderpass_t {
	s8 temp;
};

struct KGFXbuffer_t {
	s8 temp;
};

struct KGFXmesh_t {
	s8 temp;
};

struct KGFXpipelinemesh_t {
	s8 temp;
};

struct KGFXuniformbuffer_t {
	s8 temp;
};

struct KGFXtexture_t {
	s8 temp;
};

struct KGFXsampler_t {
	s8 temp;
};

struct KGFXpipelinetexture_t {
	u32 id;
};

KGFX_API KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context) {
	if (window.hwnd == INVALID_HANDLE_VALUE) {
		return KGFX_INVALID_ARGUMENT;
	}

	if (context == nullptr) {
		return KGFX_NULL_OUTPUT_ARGUMENT;
	}

	if (KGFX_MAJOR(version) != KGFX_MAJOR(KGFX_IMPL_VER)) {
		return KGFX_VERSION_MAJOR_MISMATCH;
	}

	if (KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VER)) {
		return KGFX_VERSION_NOT_SUPPORTED;
	}

	KGFXcontext ctx = new KGFXcontext_t{};
	ctx->version = version;
	ctx->window = window;

	*context = ctx;
	if (FAILED(ctx->d3d12.init())) {
		return KGFX_GENERIC_ERROR;
	}

	return KGFX_SUCCESS;
}

/* destroys a kgfx context */
KGFX_API void kgfxDestroyContext(KGFXcontext ctx) {
	if (ctx == nullptr) {
		return;
	}

	ctx->d3d12.destroy();
	delete ctx;
}

KGFX_API KGFXshader kgfxCreateShader(KGFXcontext ctx, const void* data, u32 size, KGFXshadertype type, KGFXshadermedium medium) {
	return new KGFXshader_t{};
}

KGFX_API void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {
	delete shader;
}

KGFX_API KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	return new KGFXpipeline_t{};
}

KGFX_API KGFXpipelinemesh kgfxPipelineAddMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXmesh mesh, u32 binding) {
	return new KGFXpipelinemesh_t{};
}

KGFX_API void kgfxPipelineRemoveMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinemesh pipelineMesh) {
	delete pipelineMesh;
}

KGFX_API KGFXuniformbuffer kgfxPipelineBindDescriptorSetBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding, u32 offset) {
	return new KGFXuniformbuffer_t{};
}

KGFX_API void kgfxPipelineUnbindDescriptorSetBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer) {
	delete uniformBuffer;
}

KGFX_API KGFXpipelinetexture kgfxPipelineBindDescriptorSetTexture(KGFXcontext ctx, KGFXpipeline pipeline, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	return new KGFXpipelinetexture_t{};
}

KGFX_API void kgfxPipelineUnbindDescriptorSetTexture(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinetexture pipelineTexture) {
	delete pipelineTexture;
}

KGFX_API void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline) {
	delete pipeline;
}

KGFX_API KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc) {
	return new KGFXbuffer_t{};
}

KGFX_API KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data) {
	return KGFX_SUCCESS;
}

KGFX_API void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer) {
	return new u8[256];
}

KGFX_API void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer) {
	delete[] buffer;
}

KGFX_API KGFXresult kgfxBufferCopy(KGFXcontext ctx, KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	return KGFX_SUCCESS;
}

KGFX_API void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer) {
	delete buffer;
}

KGFX_API KGFXmesh kgfxCreateMesh(KGFXcontext ctx, KGFXmeshdesc meshDesc) {
	return new KGFXmesh_t{};
}

KGFX_API void kgfxDestroyMesh(KGFXcontext ctx, KGFXmesh mesh) {
	delete mesh;
}

KGFX_API KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc) {
	return new KGFXtexture_t{};
}

KGFX_API KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	return KGFX_SUCCESS;
}

KGFX_API void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture) {
	delete texture;
}

KGFX_API KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc) {
	return new KGFXsampler_t{};
}

KGFX_API void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler) {
	delete sampler;
}

/* returns implementation version */
KGFX_API u32 kgfxGetImplementationVersion() {
	return KGFX_IMPL_VER;
}

KGFX_API void kgfxRender(KGFXcontext ctx, KGFXpipeline pipeline) {
	ctx->d3d12.render(pipeline);
}