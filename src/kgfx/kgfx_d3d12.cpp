#include <kgfx/kgfx.h>
#define WIN32_LEAN_OR_MEAN
#define NOMINMAX
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(0, 1, 0)
#define KGFX_D3D_TARGET_FRAMES 2

#ifdef KGFX_DEBUG
#define DEBUG_OUT(msg) debugFunc(msg)
#define DEBUG_OUTF(fmt, ...) debugFunc(fmt, __VA_ARGS__)
#else
#define DEBUG_OUT(msg)
#define DEBUG_OUTF(fmt, ...)
#endif

struct D3D12 {
	KGFXcontext ctx;

	#ifdef KGFX_DEBUG
	ID3D12Debug* debug;
	#endif
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;
	ID3D12Device* device;

	ID3D12Resource* renderTargets[KGFX_D3D_TARGET_FRAMES];
	ID3D12DescriptorHeap* rtvDescHeap;
	ID3D12PipelineState* pipeline;

	ID3D12RootSignature* rootSignature;
	ID3D12CommandAllocator* commandAllocator;
	ID3D12CommandQueue* commandQueue;

	IDXGISwapChain4* swapchain;

	HRESULT init();
	void destroy();

	void render(KGFXpipeline pipeline);

	HRESULT selectAdapter();
	HRESULT createDevice();
	HRESULT createCommandUtilities();
	HRESULT createSwapchain();
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
	ctx->d3d12.ctx = ctx;

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

HRESULT D3D12::init() {
	HRESULT hr = S_OK;
	#ifdef KGFX_DEBUG
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to get debug interface");
		return hr;
	} else {
		debug->EnableDebugLayer();
	}
	#endif

	{
		u32 flags = 0;
		#ifdef KGFX_DEBUG
		flags |= DXGI_CREATE_FACTORY_DEBUG;
		#endif

		hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&factory));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create DXGI factory");
			return hr;
		}
	}

	hr = selectAdapter();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to select adapter");
		return hr;
	}

	hr = createDevice();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create device");
		return hr;
	}

	hr = createCommandUtilities();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command utilities");
		return hr;
	}

	hr = createSwapchain();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create swapchain");
		return hr;
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = KGFX_D3D_TARGET_FRAMES;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;

		hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&rtvDescHeap));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create RTV descriptor heap");
			return hr;
		}

		const u32 size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

		for (u32 i = 0; i < KGFX_D3D_TARGET_FRAMES; ++i) {
			hr = swapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
			if (FAILED(hr)) {
				DEBUG_OUT("Failed to get swapchain buffer");
				return hr;
			}

			device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);
			rtvHandle.ptr += size;
		}
	}

	{
		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
		rootSignatureDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.Desc_1_0.NumParameters = 0;
		rootSignatureDesc.Desc_1_0.pParameters = nullptr;
		rootSignatureDesc.Desc_1_0.NumStaticSamplers = 0;
		rootSignatureDesc.Desc_1_0.pStaticSamplers = nullptr;

		ID3DBlob* signature;
		hr = D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signature, nullptr);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to serialize root signature");
			return hr;
		}

		hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create root signature");
			return hr;
		}

		signature->Release();
	}

	{
		const char* src = R"(
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
		)";

		u32 compileFlags = 0;
		#ifdef KGFX_DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		#endif

		ID3DBlob* error;

		ID3DBlob* vertexShader;
		hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr, "vmain", "vs_5_0", compileFlags, 0, &vertexShader, &error);
		if (FAILED(hr)) {
			char* buf = new char[error->GetBufferSize()];
			memcpy(buf, error->GetBufferPointer(), error->GetBufferSize());
			DEBUG_OUTF("Failed to compile vertex shader: {}", buf);
			delete[] buf;
			return hr;
		}

		ID3DBlob* pixelShader;
		hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr, "pmain", "ps_5_0", compileFlags, 0, &pixelShader, &error);
		if (FAILED(hr)) {
			char* buf = new char[error->GetBufferSize()];
			memcpy(buf, error->GetBufferPointer(), error->GetBufferSize());
			DEBUG_OUTF("Failed to compile pixel shader: {}", buf);
			delete[] buf;
			return hr;
		}

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc.FrontCounterClockwise = TRUE;
		rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.ForcedSampleCount = 0;
		rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		D3D12_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.StencilEnable = FALSE;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.pRootSignature = rootSignature;
		psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
		psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
		psoDesc.StreamOutput = { nullptr, 0, nullptr, 0, 0 };
		psoDesc.BlendState = blendDesc;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.RasterizerState = rasterizerDesc;
		psoDesc.DepthStencilState = depthStencilDesc;
		psoDesc.InputLayout = { inputElementDescs, 1 };
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;

		hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create pipeline state");
			return hr;
		}

		vertexShader->Release();
		pixelShader->Release();
		if (error != nullptr) {
			error->Release();
		}
	}

	DEBUG_OUT("KGFX and D3D12 initialized properly");
	return S_OK;
}

void D3D12::destroy() {

}

void D3D12::render(KGFXpipeline pipeline) {

}

RECT D3D12::getWindowExtent() {
	RECT extent;
	GetClientRect(reinterpret_cast<HWND>(ctx->window.hwnd), &extent);
	return extent;
}

HRESULT D3D12::selectAdapter() {
	u32 best = 0;
	u32 bestIndex = 0;

	DXGI_ADAPTER_DESC1 desc;
	for (u32 i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		adapter->GetDesc1(&desc);

		u32 score = 0;
		if (!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) {
			score += 5000;
		}

		score += desc.DedicatedVideoMemory / 1024;

		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
			if (score > best) {
				best = score;
				bestIndex = i;
			}
		}
	}

	if (best == 0) {
		DEBUG_OUT("Failed to find a suitable adapter");
		return E_FAIL;
	}

	if (FAILED(factory->EnumAdapters1(bestIndex, &adapter))) {
		DEBUG_OUT("Failed to select adapter");
		return E_FAIL;
	}
	adapter->GetDesc1(&desc);
	DEBUG_OUTF("Device Adapter \"{}\" scored {}", desc.Description, best);
	return S_OK;
}

HRESULT D3D12::createDevice() {
	HRESULT hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create device");
		return hr;
	}

	return S_OK;
}

HRESULT D3D12::createCommandUtilities() {
	HRESULT hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command allocator");
		return hr;
	}

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command queue");
		return hr;
	}

	return S_OK;
}

HRESULT D3D12::createSwapchain() {
	RECT extent = getWindowExtent();

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = extent.right - extent.left;
	swapchainDesc.Height = extent.bottom - extent.top;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = FALSE;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = KGFX_D3D_TARGET_FRAMES;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = 0;

	HRESULT hr = factory->CreateSwapChainForHwnd(commandQueue, reinterpret_cast<HWND>(ctx->window.hwnd), &swapchainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapchain));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create swapchain");
		return hr;
	}

	factory->MakeWindowAssociation(reinterpret_cast<HWND>(ctx->window.hwnd), DXGI_MWA_NO_ALT_ENTER);
	return S_OK;
}