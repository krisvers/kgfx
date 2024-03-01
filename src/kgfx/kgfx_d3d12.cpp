#include <kgfx/kgfx.h>
#define WIN32_LEAN_OR_MEAN
#define NOMINMAX
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#ifdef KGFX_DEBUG
#include <dxgidebug.h>
#endif
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
#define RELEASE(obj) if (obj != nullptr) { reinterpret_cast<IUnknown*>(obj)->Release(); obj = nullptr; }
#else
#define DEBUG_OUT(msg)
#define DEBUG_OUTF(fmt, ...)
#define RELEASE(obj) if (obj != nullptr) { reinterpret_cast<IUnknown*>(obj)->Release(); }
#endif

struct D3D12 {
	KGFXcontext ctx;

	#ifdef KGFX_DEBUG
	ID3D12Debug* debug;
	IDXGIDebug1* dxgiDebug;
	#endif
	IDXGIFactory4* factory;
	IDXGIAdapter1* adapter;
	ID3D12Device* device;
	ID3D12CommandQueue* commandQueue;

	HANDLE inFlightFenceEvent;
	u64 inFlightFenceValue;
	ID3D12Fence* inFlightFence;

	ID3D12Resource* renderTargets[KGFX_D3D_TARGET_FRAMES];
	ID3D12DescriptorHeap* rtvDescHeap;
	
	ID3D12RootSignature* rootSignature;
	ID3D12CommandAllocator* commandAllocator;

	ID3D12Resource* vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	u32 frameIndex;
	IDXGISwapChain4* swapchain;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissor;

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

	KGFXshader createShader(KGFXshaderdesc shaderDesc);
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
	ID3DBlob* blob;
	KGFXshadertype type;
};

struct KGFXpipeline_t {
	ID3D12PipelineState* pipeline;
	ID3D12GraphicsCommandList* commandList;
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

constexpr DXGI_FORMAT datatypeToDxgiFormat(KGFXdatatype type) {
	switch (type) {
		case KGFX_DATATYPE_FLOAT: return DXGI_FORMAT_R32_FLOAT;
		case KGFX_DATATYPE_FLOAT2: return DXGI_FORMAT_R32G32_FLOAT;
		case KGFX_DATATYPE_FLOAT3: return DXGI_FORMAT_R32G32B32_FLOAT;
		case KGFX_DATATYPE_FLOAT4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case KGFX_DATATYPE_INT: return DXGI_FORMAT_R32_SINT;
		case KGFX_DATATYPE_INT2: return DXGI_FORMAT_R32G32_SINT;
		case KGFX_DATATYPE_INT3: return DXGI_FORMAT_R32G32B32_SINT;
		case KGFX_DATATYPE_INT4: return DXGI_FORMAT_R32G32B32A32_SINT;
		case KGFX_DATATYPE_UINT: return DXGI_FORMAT_R32_UINT;
		case KGFX_DATATYPE_UINT2: return DXGI_FORMAT_R32G32_UINT;
		case KGFX_DATATYPE_UINT3: return DXGI_FORMAT_R32G32B32_UINT;
		case KGFX_DATATYPE_UINT4: return DXGI_FORMAT_R32G32B32A32_UINT;
		default: return DXGI_FORMAT_UNKNOWN;
	}
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

KGFX_API KGFXshader kgfxCreateShader(KGFXcontext ctx, KGFXshaderdesc shaderDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createShader(shaderDesc);
}

KGFX_API void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyShader(shader);
}

KGFX_API KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createPipeline(pipelineDesc);
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
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyPipeline(pipeline);
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

	hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to get DXGI debug interface");
		return hr;
	} else {
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		IDXGIInfoQueue* infoQueue;
		hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&infoQueue));
		if (SUCCEEDED(hr)) {
			infoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE);
			infoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			infoQueue->Release();
		}
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

	RECT extent = getWindowExtent();
	viewport.Width = static_cast<f32>(extent.right - extent.left);
	viewport.Height = static_cast<f32>(extent.bottom - extent.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	scissor.left = 0;
	scissor.top = 0;
	scissor.right = static_cast<long>(viewport.Width);
	scissor.bottom = static_cast<long>(viewport.Height);

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

		RELEASE(signature);
	}

	{
		const float vertices[] = {
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
		};

		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = sizeof(vertices);
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create vertex buffer");
			return hr;
		}

		void* mapped;
		D3D12_RANGE range = { 0 };
		hr = vertexBuffer->Map(0, &range, &mapped);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to map vertex buffer");
			return hr;
		}
		memcpy(mapped, vertices, sizeof(vertices));
		vertexBuffer->Unmap(0, nullptr);

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(float) * 3;
		vertexBufferView.SizeInBytes = sizeof(vertices);
	}

	hr = createSyncUtilities();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create sync utilities");
		return hr;
	}

	DEBUG_OUT("KGFX and D3D12 initialized properly");
	return S_OK;
}

void D3D12::destroy() {
	#ifdef KGFX_DEBUG
	RELEASE(debug);
	#endif
	

	RELEASE(inFlightFence);
	if (inFlightFenceEvent != nullptr) {
		CloseHandle(inFlightFenceEvent);
		#ifdef KGFX_DEBUG
		inFlightFenceEvent = nullptr;
		#endif
	}

	RELEASE(swapchain);
	RELEASE(rtvDescHeap);
	RELEASE(rootSignature);
	RELEASE(commandAllocator);

	for (u32 i = 0; i < KGFX_D3D_TARGET_FRAMES; ++i) {
		RELEASE(renderTargets[i]);
	}

	RELEASE(vertexBuffer);

	RELEASE(commandQueue);
	RELEASE(device);
	RELEASE(adapter);
	RELEASE(factory);

	#ifdef KGFX_DEBUG
	dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	RELEASE(dxgiDebug);
	#endif
}

void D3D12::render(KGFXpipeline pipeline) {
	u32 value = inFlightFenceValue;
	
	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to reset command allocator");
		return;
	}

	hr = pipeline->commandList->Reset(commandAllocator, pipeline->pipeline);
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to reset command list");
		return;
	}

	pipeline->commandList->SetGraphicsRootSignature(rootSignature);
	pipeline->commandList->RSSetViewports(1, &viewport);
	pipeline->commandList->RSSetScissorRects(1, &scissor);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = renderTargets[frameIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	pipeline->commandList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += frameIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	pipeline->commandList->OMSetRenderTargets(1, &handle, FALSE, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	pipeline->commandList->ClearRenderTargetView(handle, clearColor, 0, nullptr);

	pipeline->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pipeline->commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	pipeline->commandList->DrawInstanced(6, 1, 0, 0);

	D3D12_RESOURCE_BARRIER presentBarrier = barrier;
	presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	pipeline->commandList->ResourceBarrier(1, &presentBarrier);

	hr = pipeline->commandList->Close();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to close command list");
		return;
	}

	commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&pipeline->commandList));

	{
		u32 flags = 0;
		#ifdef KGFX_DEBUG
		//flags |= DXGI_PRESENT_TEST;
		#endif

		DXGI_PRESENT_PARAMETERS params = {};
		hr = swapchain->Present1(1, flags, &params);
		if (FAILED(hr)) {
			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
				DEBUG_OUT("Device removed or reset");
				return;
			} else {
				DEBUG_OUT("Failed to present");
				return;
			}
		}

		u32 value = inFlightFenceValue;

		hr = commandQueue->Signal(inFlightFence, value);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to signal in flight fence");
			return;
		}

		++inFlightFenceValue;

		u64 completed = inFlightFence->GetCompletedValue();
		if (inFlightFence->GetCompletedValue() < value) {
			hr = inFlightFence->SetEventOnCompletion(value, inFlightFenceEvent);
			if (FAILED(hr)) {
				DEBUG_OUT("Failed to set event on completion");
				return;
			}

			WaitForSingleObject(inFlightFenceEvent, INFINITE);
		}

		frameIndex = swapchain->GetCurrentBackBufferIndex();
	}
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
		
		RELEASE(adapter);
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
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = viewport.Width;
	swapchainDesc.Height = viewport.Height;
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

HRESULT D3D12::createSyncUtilities() {
	HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&inFlightFence));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create in flight fence");
		return hr;
	}
	++inFlightFenceValue;

	inFlightFenceEvent = CreateEvent(0, FALSE, FALSE, NULL);
	if (inFlightFenceEvent == NULL) {
		DEBUG_OUT("Failed to create in flight fence event");
		return E_FAIL;
	}

	return S_OK;
}

KGFXbuffer D3D12::createBuffer(KGFXbufferdesc bufferDesc) {
	return KGFX_HANDLE_NULL;
}

KGFXresult D3D12::uploadBuffer(KGFXbuffer buffer, u32 size, void* data) {
	return KGFX_SUCCESS;
}

void* D3D12::mapBuffer(KGFXbuffer buffer) {
	return KGFX_HANDLE_NULL;
}

void D3D12::unmapBuffer(KGFXbuffer buffer) {

}

KGFXresult D3D12::copyBuffer(KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	return KGFX_SUCCESS;
}

KGFXresult D3D12::copyBufferToTexture(KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	return KGFX_SUCCESS;
}

KGFXmesh D3D12::createMesh(KGFXmeshdesc meshDesc) {
	return KGFX_HANDLE_NULL;
}

KGFXtexture D3D12::createTexture(KGFXtexturedesc textureDesc) {
	return KGFX_HANDLE_NULL;
}

KGFXsampler D3D12::createSampler(KGFXsamplerdesc samplerDesc) {
	return KGFX_HANDLE_NULL;
}

KGFXshader D3D12::createShader(KGFXshaderdesc shaderDesc) {
	if (shaderDesc.pData == nullptr) {
		DEBUG_OUT("Invalid shader data");
		return nullptr;
	}

	if (shaderDesc.size == 0) {
		DEBUG_OUT("Invalid shader size");
		return nullptr;
	}

	ID3DBlob* blob;
	ID3DBlob* error;

	u32 compileFlags = 0;
	#ifdef KGFX_DEBUG
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	const char* entry = (shaderDesc.entryName == nullptr) ? "main" : shaderDesc.entryName;
	std::string target = "*s_5_0";
	if (shaderDesc.type == KGFX_SHADERTYPE_VERTEX) {
		target[0] = 'v';
	} else if (shaderDesc.type == KGFX_SHADERTYPE_FRAGMENT) {
		target[0] = 'p';
	} else {
		DEBUG_OUT("Invalid KGFXshadertype");
		return nullptr;
	}

	HRESULT hr = D3DCompile(shaderDesc.pData, shaderDesc.size, nullptr, nullptr, nullptr, entry, target.c_str(), compileFlags, 0, &blob, &error);
	if (FAILED(hr)) {
		char* buf = new char[error->GetBufferSize()];
		memcpy(buf, error->GetBufferPointer(), error->GetBufferSize());
		DEBUG_OUTF("Failed to compile shader: {}", buf);
		delete[] buf;
		RELEASE(error);
		return nullptr;
	}

	RELEASE(error);

	KGFXshader shader = new KGFXshader_t{};
	shader->blob = blob;
	shader->type = shaderDesc.type;
	return shader;
}

KGFXpipeline D3D12::createPipeline(KGFXpipelinedesc pipelineDesc) {
	if (pipelineDesc.pShaders == nullptr) {
		DEBUG_OUT("Invalid KGFXpipelinedesc");
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.shaderCount == 0) {
		DEBUG_OUT("Invalid KGFXpipelinedesc");
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.pShaders[0] == nullptr) {
		DEBUG_OUT("Invalid KGFXpipelinedesc");
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.cullMode > KGFX_CULLMODE_MAX || pipelineDesc.cullMode < KGFX_CULLMODE_MIN) {
		DEBUG_OUT("Invalid KGFXcullmode");
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.fillMode > KGFX_FILLMODE_MAX || pipelineDesc.fillMode < KGFX_FILLMODE_MIN) {
		DEBUG_OUT("Invalid KGFXfillmode");
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.frontFace > KGFX_FRONTFACE_MAX || pipelineDesc.frontFace < KGFX_FRONTFACE_MIN) {
		DEBUG_OUT("Invalid KGFXfrontface");
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.topology > KGFX_TOPOLOGY_MAX || pipelineDesc.topology < KGFX_TOPOLOGY_MIN) {
		DEBUG_OUT("Invalid KGFXtopology");
		return KGFX_HANDLE_NULL;
	}

	u32 totalAttributeCount = 0;
	for (u32 i = 0; i < pipelineDesc.layout.bindingCount; ++i) {
		totalAttributeCount += pipelineDesc.layout.pBindings[i].attributeCount;
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs(pipelineDesc.layout.pBindings[0].attributeCount);

	u32 currentElementIndex = 0;
	for (u32 i = 0; i < pipelineDesc.layout.bindingCount; ++i) {
		for (u32 j = 0; j < pipelineDesc.layout.pBindings[i].attributeCount; ++j) {
			inputElementDescs[currentElementIndex].SemanticName = pipelineDesc.layout.pBindings[i].pAttributes[j].semanticName;
			inputElementDescs[currentElementIndex].SemanticIndex = j;
			inputElementDescs[currentElementIndex].Format = datatypeToDxgiFormat(pipelineDesc.layout.pBindings[i].pAttributes[j].type);
			inputElementDescs[currentElementIndex].InputSlot = i;
			inputElementDescs[currentElementIndex].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElementDescs[currentElementIndex].InputSlotClass = (pipelineDesc.layout.pBindings[i].inputRate == KGFX_VERTEX_INPUT_RATE_VERTEX) ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
			inputElementDescs[currentElementIndex].InstanceDataStepRate = 0;
			++currentElementIndex;
		}
	}

	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = (pipelineDesc.fillMode == KGFX_FILLMODE_SOLID) ? D3D12_FILL_MODE_SOLID : D3D12_FILL_MODE_WIREFRAME;
	rasterizerDesc.CullMode = (pipelineDesc.cullMode == KGFX_CULLMODE_BACK) ? D3D12_CULL_MODE_BACK : (pipelineDesc.cullMode == KGFX_CULLMODE_FRONT) ? D3D12_CULL_MODE_FRONT : D3D12_CULL_MODE_NONE;
	rasterizerDesc.FrontCounterClockwise = (pipelineDesc.frontFace == KGFX_FRONTFACE_CCW);
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

	D3D12_SHADER_BYTECODE bytecodes[3] = {};
	for (u32 i = 0; i < pipelineDesc.shaderCount; ++i) {
		u32 index = 0;
		if (pipelineDesc.pShaders[i]->type == KGFX_SHADERTYPE_VERTEX) {
			index = 0;
		} else if (pipelineDesc.pShaders[i]->type == KGFX_SHADERTYPE_FRAGMENT) {
			index = 1;
		} else if (pipelineDesc.pShaders[i]->type == KGFX_SHADERTYPE_GEOMETRY) {
			index = 2;
		} else {
			DEBUG_OUT("Invalid KGFXshadertype");
			return KGFX_HANDLE_NULL;
		}

		if (bytecodes[index].pShaderBytecode != nullptr || bytecodes[i].BytecodeLength != 0) {
			DEBUG_OUTF("More than one {} shader provided in KGFXpipelinedesc", (index == 0) ? "vertex" : (index == 1) ? "fragment" : "geometry");
			return KGFX_HANDLE_NULL;
		}

		bytecodes[index].pShaderBytecode = pipelineDesc.pShaders[i]->blob->GetBufferPointer();
		bytecodes[index].BytecodeLength = pipelineDesc.pShaders[i]->blob->GetBufferSize();
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = rootSignature;
	psoDesc.VS = bytecodes[0];
	psoDesc.PS = bytecodes[1];
	psoDesc.GS = bytecodes[2];
	psoDesc.StreamOutput = { nullptr, 0, nullptr, 0, 0 };
	psoDesc.BlendState = blendDesc;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.InputLayout = { inputElementDescs.data(), static_cast<u32>(inputElementDescs.size()) };
	psoDesc.PrimitiveTopologyType = (pipelineDesc.topology == KGFX_TOPOLOGY_POINTS) ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT : (pipelineDesc.topology == KGFX_TOPOLOGY_LINES) ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	ID3D12PipelineState* pipeline;
	HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create D3D12 pipeline state");
		return KGFX_HANDLE_NULL;
	}

	ID3D12GraphicsCommandList* commandList;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipeline, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create D3D12 command list");
		RELEASE(pipeline);
		return KGFX_HANDLE_NULL;
	}

	commandList->Close();

	KGFXpipeline kgfxPipeline = new KGFXpipeline_t{};
	kgfxPipeline->pipeline = pipeline;
	kgfxPipeline->commandList = commandList;
	return kgfxPipeline;
}

KGFXresult D3D12::pipelineUpdateDescriptorSets(KGFXpipeline pipeline) {
	return KGFX_SUCCESS;
}

void D3D12::pipelineUnbindDescriptorSet(KGFXpipeline pipeline, struct KGFXdescriptorset_t& set) {

}

KGFXpipelinemesh D3D12::pipelineAddMesh(KGFXpipeline pipeline, KGFXmesh mesh, u32 binding) {
	return KGFX_HANDLE_NULL;
}

void D3D12::pipelineRemoveMesh(KGFXpipeline pipeline, KGFXpipelinemesh mesh) {

}

KGFXpipelinetexture D3D12::pipelineBindDescriptorSetTexture(KGFXpipeline pipeline, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	return KGFX_HANDLE_NULL;
}

void D3D12::pipelineUnbindDescriptorSetTexture(KGFXpipeline pipeline, KGFXpipelinetexture pipelineTexture) {

}

KGFXuniformbuffer D3D12::pipelineBindDescriptorSetBuffer(KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding, u32 offset) {
	return KGFX_HANDLE_NULL;
}

void D3D12::pipelineUnbindDescriptorSetBuffer(KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer) {

}

void D3D12::destroyShader(KGFXshader shader) {
	if (shader == nullptr) {
		DEBUG_OUT("Invalid KGFXshader");
		return;
	}

	RELEASE(shader->blob);
	delete shader;
}

void D3D12::destroyPipeline(KGFXpipeline pipeline) {
	if (pipeline == nullptr) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	RELEASE(pipeline->commandList);
	RELEASE(pipeline->pipeline);
	delete pipeline;
}

void D3D12::destroyBuffer(KGFXbuffer buffer) {

}

void D3D12::destroyMesh(KGFXmesh mesh) {

}

void D3D12::destroyTexture(KGFXtexture texture) {

}

void D3D12::destroySampler(KGFXsampler sampler) {

}
