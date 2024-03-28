#include <kgfx/kgfx.h>
#define WIN32_LEAN_OR_MEAN
#define NOMINMAX
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#ifdef KGFX_DEBUG
#define KGFX_D3D12_VALIDATION
#define KGFX_D3D12_BREADCRUMBS
#define KGFX_LOG_TO_FILE "kgfx_d3d12.log"
#include <dxgidebug.h>
#endif
#include <d3dcompiler.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <type_traits>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(0, 1, 0)
#define KGFX_BACKEND KGFX_BACKEND_D3D12
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

static usize contextCount = 0;

#ifdef KGFX_LOG_TO_FILE
static FILE* logFile = nullptr;
static void debugFuncToFile(std::wstring string) {
	if (logFile == nullptr) {
		fopen_s(&logFile, KGFX_LOG_TO_FILE, "w");
		if (logFile == nullptr) {
			return;
		}
		freopen_s(&logFile, KGFX_LOG_TO_FILE, "a", logFile);
	}

	fwprintf(logFile, L"%s\n", string.c_str());
	fflush(logFile);
}
static void debugCloseLogFile() {
	if (logFile != nullptr) {
		fclose(logFile);
	}
}
#endif

static void debugFuncConcat(std::wstringstream& stream, std::wstring& format) {
	stream << format;
}

template<typename T, typename... Types>
static void debugFuncConcat(std::wstringstream& stream, std::wstring& format, T& arg, Types&... args) {
	usize index = format.find(L"{}");
	if (index == std::wstring::npos) {
		stream << format;
		return;
	}

	stream << std::wstring(format.begin(), format.begin() + index);
	stream << arg;
	
	if (index + 2 < format.size()) {
		format = std::wstring(format.begin() + index + 2, format.end());
		debugFuncConcat(stream, format, args...);
	}
}

static void debugFunc(const char* format) {
	size_t len = ::MultiByteToWideChar(CP_UTF8, 0, format, strlen(format), nullptr, 0);
	std::wstring fmt(len, L'\0');
	::MultiByteToWideChar(CP_UTF8, 0, format, strlen(format), &fmt[0], len);

	std::wcout << L"KGFX debug: " << fmt << std::endl;
	#ifdef KGFX_LOG_TO_FILE
	debugFuncToFile(fmt);
	#endif
}

template<typename... Types>
static void debugFunc(const char* format, Types... types) {
	size_t len = ::MultiByteToWideChar(CP_UTF8, 0, format, strlen(format), nullptr, 0);
	std::wstring fmt(len, L'\0');
	::MultiByteToWideChar(CP_UTF8, 0, format, strlen(format), &fmt[0], len);
	std::wstringstream stream;

	debugFuncConcat(stream, fmt, types...);

	std::wcout << L"KGFX debug: " << stream.str() << std::endl;
	#ifdef KGFX_LOG_TO_FILE
	debugFuncToFile(stream.str());
	#endif
}

#ifdef KGFX_D3D12_BREADCRUMBS
static std::wstring breadcrumbOperationToString(D3D12_AUTO_BREADCRUMB_OP op) {
	switch (op) { case D3D12_AUTO_BREADCRUMB_OP_SETMARKER: return L"D3D12_AUTO_BREADCRUMB_OP_SETMARKER"; case D3D12_AUTO_BREADCRUMB_OP_BEGINEVENT: return L"D3D12_AUTO_BREADCRUMB_OP_BEGINEVENT"; case D3D12_AUTO_BREADCRUMB_OP_ENDEVENT: return L"D3D12_AUTO_BREADCRUMB_OP_ENDEVENT"; case D3D12_AUTO_BREADCRUMB_OP_DRAWINSTANCED: return L"D3D12_AUTO_BREADCRUMB_OP_DRAWINSTANCED"; case D3D12_AUTO_BREADCRUMB_OP_DRAWINDEXEDINSTANCED: return L"D3D12_AUTO_BREADCRUMB_OP_DRAWINDEXEDINSTANCED"; case D3D12_AUTO_BREADCRUMB_OP_EXECUTEINDIRECT: return L"D3D12_AUTO_BREADCRUMB_OP_EXECUTEINDIRECT"; case D3D12_AUTO_BREADCRUMB_OP_DISPATCH: return L"D3D12_AUTO_BREADCRUMB_OP_DISPATCH"; case D3D12_AUTO_BREADCRUMB_OP_COPYBUFFERREGION: return L"D3D12_AUTO_BREADCRUMB_OP_COPYBUFFERREGION"; case D3D12_AUTO_BREADCRUMB_OP_COPYTEXTUREREGION: return L"D3D12_AUTO_BREADCRUMB_OP_COPYTEXTUREREGION"; case D3D12_AUTO_BREADCRUMB_OP_COPYRESOURCE: return L"D3D12_AUTO_BREADCRUMB_OP_COPYRESOURCE"; case D3D12_AUTO_BREADCRUMB_OP_COPYTILES: return L"D3D12_AUTO_BREADCRUMB_OP_COPYTILES"; case D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCE: return L"D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCE"; case D3D12_AUTO_BREADCRUMB_OP_CLEARRENDERTARGETVIEW: return L"D3D12_AUTO_BREADCRUMB_OP_CLEARRENDERTARGETVIEW"; case D3D12_AUTO_BREADCRUMB_OP_CLEARUNORDEREDACCESSVIEW: return L"D3D12_AUTO_BREADCRUMB_OP_CLEARUNORDEREDACCESSVIEW"; case D3D12_AUTO_BREADCRUMB_OP_CLEARDEPTHSTENCILVIEW: return L"D3D12_AUTO_BREADCRUMB_OP_CLEARDEPTHSTENCILVIEW"; case D3D12_AUTO_BREADCRUMB_OP_RESOURCEBARRIER: return L"D3D12_AUTO_BREADCRUMB_OP_RESOURCEBARRIER"; case D3D12_AUTO_BREADCRUMB_OP_EXECUTEBUNDLE: return L"D3D12_AUTO_BREADCRUMB_OP_EXECUTEBUNDLE"; case D3D12_AUTO_BREADCRUMB_OP_PRESENT: return L"D3D12_AUTO_BREADCRUMB_OP_PRESENT"; case D3D12_AUTO_BREADCRUMB_OP_RESOLVEQUERYDATA: return L"D3D12_AUTO_BREADCRUMB_OP_RESOLVEQUERYDATA"; case D3D12_AUTO_BREADCRUMB_OP_BEGINSUBMISSION: return L"D3D12_AUTO_BREADCRUMB_OP_BEGINSUBMISSION"; case D3D12_AUTO_BREADCRUMB_OP_ENDSUBMISSION: return L"D3D12_AUTO_BREADCRUMB_OP_ENDSUBMISSION"; case D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME: return L"D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME"; case D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES: return L"D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES"; case D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT: return L"D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT"; case D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT64: return L"D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT64"; case D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCEREGION: return L"D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCEREGION"; case D3D12_AUTO_BREADCRUMB_OP_WRITEBUFFERIMMEDIATE: return L"D3D12_AUTO_BREADCRUMB_OP_WRITEBUFFERIMMEDIATE"; case D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME1: return L"D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME1"; case D3D12_AUTO_BREADCRUMB_OP_SETPROTECTEDRESOURCESESSION: return L"D3D12_AUTO_BREADCRUMB_OP_SETPROTECTEDRESOURCESESSION"; case D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME2: return L"D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME2"; case D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES1: return L"D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES1"; case D3D12_AUTO_BREADCRUMB_OP_BUILDRAYTRACINGACCELERATIONSTRUCTURE: return L"D3D12_AUTO_BREADCRUMB_OP_BUILDRAYTRACINGACCELERATIONSTRUCTURE"; case D3D12_AUTO_BREADCRUMB_OP_EMITRAYTRACINGACCELERATIONSTRUCTUREPOSTBUILDINFO: return L"D3D12_AUTO_BREADCRUMB_OP_EMITRAYTRACINGACCELERATIONSTRUCTUREPOSTBUILDINFO"; case D3D12_AUTO_BREADCRUMB_OP_COPYRAYTRACINGACCELERATIONSTRUCTURE: return L"D3D12_AUTO_BREADCRUMB_OP_COPYRAYTRACINGACCELERATIONSTRUCTURE"; case D3D12_AUTO_BREADCRUMB_OP_DISPATCHRAYS: return L"D3D12_AUTO_BREADCRUMB_OP_DISPATCHRAYS"; case D3D12_AUTO_BREADCRUMB_OP_INITIALIZEMETACOMMAND: return L"D3D12_AUTO_BREADCRUMB_OP_INITIALIZEMETACOMMAND"; case D3D12_AUTO_BREADCRUMB_OP_EXECUTEMETACOMMAND: return L"D3D12_AUTO_BREADCRUMB_OP_EXECUTEMETACOMMAND"; case D3D12_AUTO_BREADCRUMB_OP_ESTIMATEMOTION: return L"D3D12_AUTO_BREADCRUMB_OP_ESTIMATEMOTION"; case D3D12_AUTO_BREADCRUMB_OP_RESOLVEMOTIONVECTORHEAP: return L"D3D12_AUTO_BREADCRUMB_OP_RESOLVEMOTIONVECTORHEAP"; case D3D12_AUTO_BREADCRUMB_OP_SETPIPELINESTATE1: return L"D3D12_AUTO_BREADCRUMB_OP_SETPIPELINESTATE1"; case D3D12_AUTO_BREADCRUMB_OP_INITIALIZEEXTENSIONCOMMAND: return L"D3D12_AUTO_BREADCRUMB_OP_INITIALIZEEXTENSIONCOMMAND"; case D3D12_AUTO_BREADCRUMB_OP_EXECUTEEXTENSIONCOMMAND: return L"D3D12_AUTO_BREADCRUMB_OP_EXECUTEEXTENSIONCOMMAND"; case D3D12_AUTO_BREADCRUMB_OP_DISPATCHMESH: return L"D3D12_AUTO_BREADCRUMB_OP_DISPATCHMESH"; case D3D12_AUTO_BREADCRUMB_OP_ENCODEFRAME: return L"D3D12_AUTO_BREADCRUMB_OP_ENCODEFRAME"; case D3D12_AUTO_BREADCRUMB_OP_RESOLVEENCODEROUTPUTMETADATA: return L"D3D12_AUTO_BREADCRUMB_OP_RESOLVEENCODEROUTPUTMETADATA"; }
}

static std::wstring dredAllocationTypeToString(D3D12_DRED_ALLOCATION_TYPE type) {
	switch (type) { case D3D12_DRED_ALLOCATION_TYPE_COMMAND_QUEUE: return L"D3D12_DRED_ALLOCATION_TYPE_COMMAND_QUEUE"; case D3D12_DRED_ALLOCATION_TYPE_COMMAND_ALLOCATOR: return L"D3D12_DRED_ALLOCATION_TYPE_COMMAND_ALLOCATOR"; case D3D12_DRED_ALLOCATION_TYPE_PIPELINE_STATE: return L"D3D12_DRED_ALLOCATION_TYPE_PIPELINE_STATE"; case D3D12_DRED_ALLOCATION_TYPE_COMMAND_LIST: return L"D3D12_DRED_ALLOCATION_TYPE_COMMAND_LIST"; case D3D12_DRED_ALLOCATION_TYPE_FENCE: return L"D3D12_DRED_ALLOCATION_TYPE_FENCE"; case D3D12_DRED_ALLOCATION_TYPE_DESCRIPTOR_HEAP: return L"D3D12_DRED_ALLOCATION_TYPE_DESCRIPTOR_HEAP"; case D3D12_DRED_ALLOCATION_TYPE_HEAP: return L"D3D12_DRED_ALLOCATION_TYPE_HEAP"; case D3D12_DRED_ALLOCATION_TYPE_QUERY_HEAP: return L"D3D12_DRED_ALLOCATION_TYPE_QUERY_HEAP"; case D3D12_DRED_ALLOCATION_TYPE_COMMAND_SIGNATURE: return L"D3D12_DRED_ALLOCATION_TYPE_COMMAND_SIGNATURE"; case D3D12_DRED_ALLOCATION_TYPE_PIPELINE_LIBRARY: return L"D3D12_DRED_ALLOCATION_TYPE_PIPELINE_LIBRARY"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_PROCESSOR: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_PROCESSOR"; case D3D12_DRED_ALLOCATION_TYPE_RESOURCE: return L"D3D12_DRED_ALLOCATION_TYPE_RESOURCE"; case D3D12_DRED_ALLOCATION_TYPE_PASS: return L"D3D12_DRED_ALLOCATION_TYPE_PASS"; case D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSION: return L"D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSION"; case D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSIONPOLICY: return L"D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSIONPOLICY"; case D3D12_DRED_ALLOCATION_TYPE_PROTECTEDRESOURCESESSION: return L"D3D12_DRED_ALLOCATION_TYPE_PROTECTEDRESOURCESESSION"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER_HEAP: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER_HEAP"; case D3D12_DRED_ALLOCATION_TYPE_COMMAND_POOL: return L"D3D12_DRED_ALLOCATION_TYPE_COMMAND_POOL"; case D3D12_DRED_ALLOCATION_TYPE_COMMAND_RECORDER: return L"D3D12_DRED_ALLOCATION_TYPE_COMMAND_RECORDER"; case D3D12_DRED_ALLOCATION_TYPE_STATE_OBJECT: return L"D3D12_DRED_ALLOCATION_TYPE_STATE_OBJECT"; case D3D12_DRED_ALLOCATION_TYPE_METACOMMAND: return L"D3D12_DRED_ALLOCATION_TYPE_METACOMMAND"; case D3D12_DRED_ALLOCATION_TYPE_SCHEDULINGGROUP: return L"D3D12_DRED_ALLOCATION_TYPE_SCHEDULINGGROUP"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_ESTIMATOR: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_ESTIMATOR"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_VECTOR_HEAP: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_VECTOR_HEAP"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_EXTENSION_COMMAND: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_EXTENSION_COMMAND"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_ENCODER: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_ENCODER"; case D3D12_DRED_ALLOCATION_TYPE_VIDEO_ENCODER_HEAP: return L"D3D12_DRED_ALLOCATION_TYPE_VIDEO_ENCODER_HEAP"; case D3D12_DRED_ALLOCATION_TYPE_INVALID: return L"D3D12_DRED_ALLOCATION_TYPE_INVALID"; }
}
#endif

struct D3D12 {
	KGFXcontext ctx;

	#ifdef KGFX_DEBUG
	ID3D12Debug1* debug;
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
	
	ID3D12RootSignature* emptyRootSignature;
	ID3D12DescriptorHeap* defaultSamplerHeap;
	ID3D12CommandAllocator* commandAllocator;

	u32 frameIndex;
	IDXGISwapChain4* swapchain;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissor;

	HRESULT init();
	void destroy();

	void render(KGFXpipeline pipeline);

	HRESULT selectAdapter();
	HRESULT createDevice();
	HRESULT createCommandListUtilities();
	HRESULT createSwapchain();
	HRESULT createSyncUtilities();

	RECT getWindowExtent();
	HRESULT recreateSwapchain();

	KGFXbuffer createBuffer(KGFXbufferdesc bufferDesc);
	KGFXresult uploadBuffer(KGFXbuffer buffer, u32 size, void* data);
	void* mapBuffer(KGFXbuffer buffer);
	void unmapBuffer(KGFXbuffer buffer);
	u32 bufferSize(KGFXbuffer buffer);
	KGFXresult copyBuffer(KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset);
	KGFXresult copyBufferToTexture(KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset);
	void destroyBuffer(KGFXbuffer buffer);

	KGFXtexture createTexture(KGFXtexturedesc textureDesc);
	void destroyTexture(KGFXtexture texture);

	KGFXsampler createSampler(KGFXsamplerdesc samplerDesc);
	void destroySampler(KGFXsampler sampler);

	KGFXshader createShader(KGFXshaderdesc shaderDesc);
	void destroyShader(KGFXshader shader);

	KGFXpipeline createPipeline(KGFXpipelinedesc pipelineDesc);
	void destroyPipeline(KGFXpipeline pipeline);

	KGFXcommandlist createCommandList();
	void resetCommand(KGFXcommandlist commandList);
	void bindPipelineCommand(KGFXcommandlist commandList, KGFXpipeline pipeline);
	void bindVertexBufferCommand(KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding);
	void bindIndexBufferCommand(KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding);
	void bindDescriptorSetBufferCommand(KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding);
	void bindDescriptorSetTextureCommand(KGFXcommandlist commandList, KGFXtexture texture, KGFXsampler sampler, u32 binding);
	void drawCommand(KGFXcommandlist commandList, u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
	void drawIndexedCommand(KGFXcommandlist commandList, u32 indexCount, u32 instanceCount, u32 firstIndex, s32 vertexOffset, u32 firstInstance);
	void presentCommand(KGFXcommandlist commandList);
	void submitCommands(KGFXcommandlist commandList);
	void destroyCommandList(KGFXcommandlist commandList);

	#ifdef KGFX_D3D12_BREADCRUMBS
	void collectBreadcrumbs(KGFXcommandlist commandList, KGFXpipeline pipeline) {
		ID3D12DeviceRemovedExtendedData1* dred;
		HRESULT hr = device->QueryInterface(IID_PPV_ARGS(&dred));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to query DRED interface");
			return;
		}

		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT1 breadcrumbs;
		hr = dred->GetAutoBreadcrumbsOutput1(&breadcrumbs);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to get breadcrumbs");
			return;
		}

		const D3D12_AUTO_BREADCRUMB_NODE1* node = breadcrumbs.pHeadAutoBreadcrumbNode;
		u32 i = 0;
		for (; node != nullptr; node = node->pNext) {
			DEBUG_OUTF("    Breadcrumb {}:", i);
			if (node->pCommandListDebugNameA != nullptr) {
				DEBUG_OUTF("    pCommandListDebugNameA: \"{}\"", node->pCommandListDebugNameA);
			} else {
				DEBUG_OUT("        pCommandListDebugNameA: null");
			}
			if (node->pCommandListDebugNameW != nullptr) {
				DEBUG_OUTF("        pCommandListDebugNameW: \"{}\"", node->pCommandListDebugNameW);
			} else {
				DEBUG_OUT("        pCommandListDebugNameW: null");
			}
			if (node->pCommandQueueDebugNameA != nullptr) {
				DEBUG_OUTF("        pCommandQueueDebugNameA: \"{}\"", node->pCommandQueueDebugNameA);
			} else {
				DEBUG_OUT("        pCommandQueueDebugNameA: null");
			}
			if (node->pCommandQueueDebugNameW != nullptr) {
				DEBUG_OUTF("        pCommandQueueDebugNameW: \"{}\"", node->pCommandQueueDebugNameW);
			} else {
				DEBUG_OUT("        pCommandQueueDebugNameW: null");
			}
			if (node->pCommandList != nullptr) {
				DEBUG_OUTF("        pCommandList: {}", node->pCommandList);
			} else {
				DEBUG_OUT("        pCommandList: null");
			}
			if (node->pCommandQueue != nullptr) {
				DEBUG_OUTF("        pCommandQueue: {}", node->pCommandQueue);
			} else {
				DEBUG_OUT("        pCommandQueue: null");
			}
			DEBUG_OUTF("        BreadcrumbCount: {}", node->BreadcrumbCount);
			if (node->pLastBreadcrumbValue != nullptr) {
				DEBUG_OUTF("        pLastBreadcrumbValue: {} ({})", node->pLastBreadcrumbValue, *node->pLastBreadcrumbValue);
			} else {
				DEBUG_OUT("        pLastBreadcrumbValue: null");
			}
			if (node->pCommandHistory != nullptr) {
				DEBUG_OUTF("        pCommandHistory: {}", node->pCommandHistory);
				for (u32 j = 0; j < node->BreadcrumbCount; ++j) {
					DEBUG_OUTF("            {}Operation {}: {}", (j < *node->pLastBreadcrumbValue) ? "Completed " : (j == *node->pLastBreadcrumbValue) ? "[Error] " : "", j, breadcrumbOperationToString(node->pCommandHistory[j]));
				}
			} else {
				DEBUG_OUT("        pCommandHistory: null");
			}
			if (node->pNext != nullptr) {
				DEBUG_OUTF("        pNext: {}", node->pNext);
			} else {
				DEBUG_OUT("        pNext: null");
			}
			DEBUG_OUTF("        BreadcrumbContextsCount: {}", node->BreadcrumbContextsCount);
			if (node->pBreadcrumbContexts != nullptr) {
				DEBUG_OUTF("        pBreadcrumbContexts: {}", node->pBreadcrumbContexts);
			} else {
				DEBUG_OUT("        pBreadcrumbContexts: null");
			}
			++i;
		}

		D3D12_DRED_PAGE_FAULT_OUTPUT1 pageFaults;
		hr = dred->GetPageFaultAllocationOutput1(&pageFaults);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to get page faults");
			return;
		}

		DEBUG_OUTF("Page fault address: {}", pageFaults.PageFaultVA);
		DEBUG_OUT("Page fault allocations:");

		const D3D12_DRED_ALLOCATION_NODE1* allocation = pageFaults.pHeadExistingAllocationNode;
		if (allocation != nullptr) {
			for (; allocation != nullptr; allocation = allocation->pNext) {
				if (allocation->ObjectNameA != nullptr) {
					DEBUG_OUTF("    ObjectNameA: \"{}\"", allocation->ObjectNameA);
				} else {
					DEBUG_OUT("    ObjectNameA: null");
				}
				if (allocation->ObjectNameW != nullptr) {
					DEBUG_OUTF("    ObjectNameW: \"{}\"", allocation->ObjectNameW);
				} else {
					DEBUG_OUT("    ObjectNameW: null");
				}
				DEBUG_OUTF("    AllocationType: {}", dredAllocationTypeToString(allocation->AllocationType));
				DEBUG_OUTF("    pNext: {}", allocation->pNext);
				OLECHAR* guid;
				if (allocation->pObject == nullptr) {
					DEBUG_OUT("    pObject (invalid GUID): null");
				} else {
					StringFromCLSID(__uuidof(*allocation->pObject), &guid);
					DEBUG_OUTF("    pObject ({}): {}", guid, allocation->pObject);
					CoTaskMemFree(guid);
				}
			}
		} else {
			DEBUG_OUT("    none");
		}

		DEBUG_OUT("Page fault frees:");
		allocation = pageFaults.pHeadRecentFreedAllocationNode;
		if (allocation != nullptr) {
			for (; allocation != nullptr; allocation = allocation->pNext) {
				if (allocation->ObjectNameA != nullptr) {
					DEBUG_OUTF("    ObjectNameA: \"{}\"", allocation->ObjectNameA);
				} else {
					DEBUG_OUT("    ObjectNameA: null");
				}
				if (allocation->ObjectNameW != nullptr) {
					DEBUG_OUTF("    ObjectNameW: \"{}\"", allocation->ObjectNameW);
				} else {
					DEBUG_OUT("    ObjectNameW: null");
				}
				DEBUG_OUTF("    AllocationType: {}", dredAllocationTypeToString(allocation->AllocationType));
				DEBUG_OUTF("    pNext: {}", allocation->pNext);
				OLECHAR* guid;
				if (allocation->pObject == nullptr) {
					DEBUG_OUT("    pObject (invalid GUID): null");
				} else {
					StringFromCLSID(__uuidof(*allocation->pObject), &guid);
					DEBUG_OUTF("    pObject ({}): {}", guid, allocation->pObject);
					CoTaskMemFree(guid);
				}
			}
		} else {
			DEBUG_OUT("    none");
		}

		DebugBreak();
	}
	#endif
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

typedef enum {
	KGFX_D3D12_ROOT_PARAM_TYPE_UNIFORM_BUFFER,
	KGFX_D3D12_ROOT_PARAM_TYPE_TEXTURE,
	KGFX_D3D12_ROOT_PARAM_TYPE_SAMPLER,
} KGFXd3d12rootparamtype;

struct KGFXd3d12rootparam_t {
	u32 binding;
	KGFXd3d12rootparamtype type;

	bool operator==(const KGFXd3d12rootparam_t& other) const {
		return binding == other.binding && type == other.type;
	}
};

class KGFXd3d12rootparam_t_hash {
public:
	std::size_t operator()(const KGFXd3d12rootparam_t& param) const {
		return std::hash<u32>()(param.binding) ^ std::hash<u32>()(param.type) * 3;
	}
};

struct KGFXpipeline_t {
	ID3D12PipelineState* pipeline;
	ID3D12GraphicsCommandList2* commandList;
	ID3D12RootSignature* rootSignature;

	D3D_PRIMITIVE_TOPOLOGY d3dTopology;
	u32 vertexStride;

	/* meta data */
	std::unordered_map<KGFXd3d12rootparam_t, u32, KGFXd3d12rootparam_t_hash> rootParameterMap;
};

struct KGFXrenderpass_t {
	s8 temp;
};

struct KGFXbuffer_t {
	ID3D12Resource* resource;
	u32 size;
};

struct KGFXtexture_t {
	ID3D12Resource* resource;
	ID3D12DescriptorHeap* heap;
	DXGI_FORMAT format;
	u32 width;
	u32 height;
	u32 depth;
};

struct KGFXsampler_t {
	ID3D12DescriptorHeap* heap;
};

struct KGFXcommandlist_t {
	KGFXpipeline pipeline;
	u32 vertexBinding;
	KGFXbuffer vertexBuffer;
	u32 indexBinding;
	KGFXbuffer indexBuffer;
	bool presentCommand;
};

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

constexpr u32 shaderDatatypeSize(KGFXdatatype type) {
	switch (type) {
		case KGFX_DATATYPE_FLOAT: return 4;
		case KGFX_DATATYPE_FLOAT2: return 8;
		case KGFX_DATATYPE_FLOAT3: return 12;
		case KGFX_DATATYPE_FLOAT4: return 16;
		case KGFX_DATATYPE_INT: return 4;
		case KGFX_DATATYPE_INT2: return 8;
		case KGFX_DATATYPE_INT3: return 12;
		case KGFX_DATATYPE_INT4: return 16;
		case KGFX_DATATYPE_UINT: return 4;
		case KGFX_DATATYPE_UINT2: return 8;
		case KGFX_DATATYPE_UINT3: return 12;
		case KGFX_DATATYPE_UINT4: return 16;
		case KGFX_DATATYPE_MAT2: return 16;
		case KGFX_DATATYPE_MAT3: return 36;
		case KGFX_DATATYPE_MAT4: return 64;
		default: return 0;
	}
}

constexpr DXGI_FORMAT textureFormatToDxgiFormat(KGFXtextureformat format) {
	switch (format) {
		case KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case KGFX_TEXTURE_FORMAT_R8G8B8A8_SRGB: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case KGFX_TEXTURE_FORMAT_R32G32B32A32_SFLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case KGFX_TEXTURE_FORMAT_R32G32B32_SFLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
		case KGFX_TEXTURE_FORMAT_R32G32_SFLOAT: return DXGI_FORMAT_R32G32_FLOAT;
		case KGFX_TEXTURE_FORMAT_R32_SFLOAT: return DXGI_FORMAT_R32_FLOAT;
		case KGFX_TEXTURE_FORMAT_DEPTH: return DXGI_FORMAT_D32_FLOAT;
		default: return DXGI_FORMAT_UNKNOWN;
	}
}

KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context) {
	if (window.hwnd == INVALID_HANDLE_VALUE) {
		return KGFX_INVALID_ARGUMENT;
	}

	if (context == nullptr) {
		return KGFX_NULL_OUTPUT_ARGUMENT;
	}

	if (KGFX_MAJOR(version) != KGFX_ANY_MAJOR) {
		if (KGFX_MAJOR(version) != KGFX_MAJOR(KGFX_IMPL_VER)) {
			return KGFX_VERSION_MAJOR_MISMATCH;
		}
	}
	
	if (KGFX_MINOR(version) != KGFX_ANY_MINOR) {
		if (KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VER)) {
			return KGFX_VERSION_NOT_SUPPORTED;
		}
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
void kgfxDestroyContext(KGFXcontext ctx) {
	if (ctx == nullptr) {
		return;
	}

	ctx->d3d12.destroy();
	delete ctx;
}

KGFXshader kgfxCreateShader(KGFXcontext ctx, KGFXshaderdesc shaderDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createShader(shaderDesc);
}

void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyShader(shader);
}

KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createPipeline(pipelineDesc);
}

void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyPipeline(pipeline);
}

KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createBuffer(bufferDesc);
}

KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_INVALID_ARGUMENT;
	}

	return ctx->d3d12.uploadBuffer(buffer, size, data);
}

void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.mapBuffer(buffer);
}

void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.unmapBuffer(buffer);
}

KGFXresult kgfxBufferCopy(KGFXcontext ctx, KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_INVALID_ARGUMENT;
	}

	return ctx->d3d12.copyBuffer(dstBuffer, srcBuffer, size, dstOffset, srcOffset);
}

u32 kgfxBufferSize(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return 0;
	}

	return ctx->d3d12.bufferSize(buffer);
}

void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyBuffer(buffer);
}

KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createTexture(textureDesc);
}

KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_INVALID_ARGUMENT;
	}

	return ctx->d3d12.copyBufferToTexture(dstTexture, srcBuffer, srcOffset);
}

void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyTexture(texture);
}

KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createSampler(samplerDesc);
}

void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroySampler(sampler);
}

KGFXcommandlist kgfxCreateCommandList(KGFXcontext ctx) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->d3d12.createCommandList();
}

void kgfxCommandReset(KGFXcontext ctx, KGFXcommandlist commandList) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.resetCommand(commandList);
}

void kgfxCommandBindPipeline(KGFXcontext ctx, KGFXcommandlist commandList, KGFXpipeline pipeline) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.bindPipelineCommand(commandList, pipeline);
}

void kgfxCommandBindVertexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.bindVertexBufferCommand(commandList, buffer, binding);
}

void kgfxCommandBindIndexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.bindIndexBufferCommand(commandList, buffer, binding);
}

void kgfxCommandBindDescriptorSetBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.bindDescriptorSetBufferCommand(commandList, buffer, binding);
}

void kgfxCommandBindDescriptorSetTexture(KGFXcontext ctx, KGFXcommandlist commandList, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.bindDescriptorSetTextureCommand(commandList, texture, sampler, binding);
}

void kgfxCommandDraw(KGFXcontext ctx, KGFXcommandlist commandList, u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.drawCommand(commandList, vertexCount, instanceCount, firstVertex, firstInstance);
}

void kgfxCommandDrawIndexed(KGFXcontext ctx, KGFXcommandlist commandList, u32 indexCount, u32 instanceCount, u32 firstIndex, s32 vertexOffset, u32 firstInstance) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.drawIndexedCommand(commandList, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void kgfxCommandPresent(KGFXcontext ctx, KGFXcommandlist commandList) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.presentCommand(commandList);
}

void kgfxCommandListSubmit(KGFXcontext ctx, KGFXcommandlist commandList) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.submitCommands(commandList);
}

void kgfxDestroyCommandList(KGFXcontext ctx, KGFXcommandlist commandList) {
	if (ctx == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->d3d12.destroyCommandList(commandList);
}

/* returns implementation version */
u32 kgfxGetImplementationVersion(void) {
	return KGFX_IMPL_VER;
}

KGFXbackend kgfxGetBackend(void) {
	return KGFX_BACKEND;
}

HRESULT D3D12::init() {
	HRESULT hr = S_OK;
	#ifdef KGFX_DEBUG
	{
		ID3D12Debug* _debug;
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&_debug));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to get debug interface");
			return hr;
		} else {
			hr = _debug->QueryInterface(IID_PPV_ARGS(&debug));
			if (FAILED(hr)) {
				DEBUG_OUT("Failed to query debug interface");
				return hr;
			}
			debug->EnableDebugLayer();
			#ifdef KGFX_D3D12_VALIDATION
			debug->SetEnableGPUBasedValidation(true);
			#endif
			_debug->Release();
		}
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

	#ifdef KGFX_D3D12_BREADCRUMBS
	{
		ID3D12DeviceRemovedExtendedDataSettings1* dred;
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&dred));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to get DRED interface");
			return hr;
		}

		dred->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dred->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dred->Release();
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

	hr = createCommandListUtilities();
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

		hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&emptyRootSignature));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create root signature");
			return hr;
		}

		RELEASE(signature);
	}

	{
		D3D12_SAMPLER_DESC desc = {};
		desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1.0f;
		desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		desc.MinLOD = 0.0f;
		desc.MaxLOD = D3D12_FLOAT32_MAX;

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;

		ID3D12DescriptorHeap* heap;
		HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create sampler descriptor heap");
			return KGFX_HANDLE_NULL;
		}

		device->CreateSampler(&desc, heap->GetCPUDescriptorHandleForHeapStart());
		defaultSamplerHeap = heap;
	}

	hr = createSyncUtilities();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create sync utilities");
		return hr;
	}

	DEBUG_OUT("KGFX and D3D12 initialized properly");
	++contextCount;
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
	RELEASE(defaultSamplerHeap);
	RELEASE(emptyRootSignature);
	RELEASE(commandAllocator);

	for (u32 i = 0; i < KGFX_D3D_TARGET_FRAMES; ++i) {
		RELEASE(renderTargets[i]);
	}

	RELEASE(commandQueue);
	RELEASE(device);
	RELEASE(adapter);
	RELEASE(factory);

	#ifdef KGFX_DEBUG
	dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	RELEASE(dxgiDebug);
	#endif

	--contextCount;
	#ifdef KGFX_LOG_TO_FILE
	if (contextCount == 0) {
		debugCloseLogFile();
	}
	#endif
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

		score += static_cast<u32>(desc.DedicatedVideoMemory / 1024);

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

HRESULT D3D12::createCommandListUtilities() {
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
	swapchainDesc.Width = static_cast<u32>(viewport.Width);
	swapchainDesc.Height = static_cast<u32>(viewport.Height);
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
	if (bufferDesc.size == 0) {
		KGFXbuffer buffer = new KGFXbuffer_t{};
		buffer->size = 0;
		buffer->resource = nullptr;
		return buffer;
	}

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = (bufferDesc.location == KGFX_BUFFER_LOCATION_CPU) ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = bufferDesc.size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* resource;
	HRESULT hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, (bufferDesc.location == KGFX_BUFFER_LOCATION_CPU) ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create KGFXbuffer");
		return KGFX_HANDLE_NULL;
	}

	KGFXbuffer buffer = new KGFXbuffer_t{};
	buffer->size = bufferDesc.size;
	buffer->resource = resource;

	if (bufferDesc.pData == nullptr) {
		return buffer;
	}

	if (bufferDesc.location == KGFX_BUFFER_LOCATION_CPU) {
		void* mapped;
		D3D12_RANGE range = { 0 };
		hr = buffer->resource->Map(0, &range, &mapped);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to map KGFXbuffer");
			return KGFX_HANDLE_NULL;
		}

		memcpy(mapped, bufferDesc.pData, bufferDesc.size);
		buffer->resource->Unmap(0, nullptr);
		return buffer;
	}

	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	uploadHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	uploadHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	uploadHeapProperties.CreationNodeMask = 0;
	uploadHeapProperties.VisibleNodeMask = 0;

	ID3D12Resource* upload;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upload));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create upload KGFXbuffer");
		return KGFX_HANDLE_NULL;
	}

	void* mapped;
	D3D12_RANGE range = { 0 };
	hr = upload->Map(0, &range, &mapped);
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to map upload buffer");
		return KGFX_HANDLE_NULL;
	}

	memcpy(mapped, bufferDesc.pData, bufferDesc.size);
	upload->Unmap(0, nullptr);

	commandAllocator->Reset();

	ID3D12GraphicsCommandList* commandList;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command list");
		return KGFX_HANDLE_NULL;
	}

	commandList->CopyBufferRegion(buffer->resource, 0, upload, 0, bufferDesc.size);

	D3D12_RESOURCE_BARRIER resourceBarrier = {};
	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = buffer->resource;
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &resourceBarrier);
	commandList->Close();

	commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&commandList));
	commandQueue->Signal(inFlightFence, inFlightFenceValue);
	inFlightFence->SetEventOnCompletion(inFlightFenceValue, inFlightFenceEvent);
	WaitForSingleObject(inFlightFenceEvent, INFINITE);
	++inFlightFenceValue;
	RELEASE(commandList);
	RELEASE(upload);

	return buffer;
}

KGFXresult D3D12::uploadBuffer(KGFXbuffer buffer, u32 size, void* data) {
	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer provided");
		return KGFX_INVALID_ARGUMENT;
	}

	if (size == 0) {
		DEBUG_OUT("Invalid size provided");
		return KGFX_INVALID_ARGUMENT;
	}

	if (data == nullptr) {
		DEBUG_OUT("Invalid data pointer provided");
		return KGFX_INVALID_ARGUMENT;
	}

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	uploadHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	uploadHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	uploadHeapProperties.CreationNodeMask = 0;
	uploadHeapProperties.VisibleNodeMask = 0;

	ID3D12Resource* upload;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upload));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create upload KGFXbuffer");
		return KGFX_GENERIC_ERROR;
	}

	void* mapped;
	D3D12_RANGE range = { 0 };
	hr = upload->Map(0, &range, &mapped);
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to map upload buffer");
		return KGFX_GENERIC_ERROR;
	}

	memcpy(mapped, data, size);
	upload->Unmap(0, nullptr);

	commandAllocator->Reset();

	ID3D12GraphicsCommandList* commandList;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command list");
		return KGFX_GENERIC_ERROR;
	}

	commandList->CopyBufferRegion(buffer->resource, 0, upload, 0, size);

	D3D12_RESOURCE_BARRIER resourceBarrier = {};
	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = buffer->resource;
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &resourceBarrier);
	commandList->Close();

	commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&commandList));
	commandQueue->Signal(inFlightFence, inFlightFenceValue);
	inFlightFence->SetEventOnCompletion(inFlightFenceValue, inFlightFenceEvent);
	WaitForSingleObject(inFlightFenceEvent, INFINITE);
	++inFlightFenceValue;
	RELEASE(commandList);
	RELEASE(upload);

	return KGFX_SUCCESS;
}

void* D3D12::mapBuffer(KGFXbuffer buffer) {
	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer provided");
		return nullptr;
	}

	void* mapped;
	D3D12_RANGE range = { 0 };
	HRESULT hr = buffer->resource->Map(0, &range, &mapped);
	if (FAILED(hr)) {
		DEBUG_OUT("KGFXbuffer does not support mapping");
		return nullptr;
	}

	return mapped;
}

void D3D12::unmapBuffer(KGFXbuffer buffer) {
	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer provided");
		return;
	}

	buffer->resource->Unmap(0, nullptr);
}

u32 D3D12::bufferSize(KGFXbuffer buffer) {
	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer provided");
		return 0;
	}

	return buffer->size;
}

KGFXresult D3D12::copyBuffer(KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	commandAllocator->Reset();

	ID3D12GraphicsCommandList* commandList;
	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command list");
		return KGFX_GENERIC_ERROR;
	}

	D3D12_RESOURCE_BARRIER resourceBarrier = {};
	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = dstBuffer->resource;
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &resourceBarrier);
	commandList->CopyBufferRegion(dstBuffer->resource, dstOffset, srcBuffer->resource, srcOffset, size);

	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = dstBuffer->resource;
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &resourceBarrier);
	commandList->Close();

	commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&commandList));
	commandQueue->Signal(inFlightFence, inFlightFenceValue);
	inFlightFence->SetEventOnCompletion(inFlightFenceValue, inFlightFenceEvent);
	WaitForSingleObject(inFlightFenceEvent, INFINITE);
	++inFlightFenceValue;
	RELEASE(commandList);

	return KGFX_SUCCESS;
}

KGFXresult D3D12::copyBufferToTexture(KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	commandAllocator->Reset();

	ID3D12GraphicsCommandList* commandList;
	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create command list");
		return KGFX_GENERIC_ERROR;
	}

	D3D12_RESOURCE_BARRIER resourceBarrier = {};
	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = dstTexture->resource;
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &resourceBarrier);
	commandList->CopyBufferRegion(dstTexture->resource, 0, srcBuffer->resource, srcOffset, dstTexture->width * dstTexture->height * dstTexture->depth * sizeof(u32));

	resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resourceBarrier.Transition.pResource = dstTexture->resource;
	resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &resourceBarrier);
	commandList->Close();

	commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&commandList));
	commandQueue->Signal(inFlightFence, inFlightFenceValue);
	inFlightFence->SetEventOnCompletion(inFlightFenceValue, inFlightFenceEvent);
	WaitForSingleObject(inFlightFenceEvent, INFINITE);
	++inFlightFenceValue;
	RELEASE(commandList);
	return KGFX_SUCCESS;
}

KGFXtexture D3D12::createTexture(KGFXtexturedesc textureDesc) {
	if (textureDesc.width == 0) {
		DEBUG_OUT("Invalid KGFXtexturedesc");
		return KGFX_HANDLE_NULL;
	}

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = textureDesc.width;
	resourceDesc.Height = (textureDesc.height == 0) ? 1 : textureDesc.height;
	resourceDesc.DepthOrArraySize = (textureDesc.depth == 0) ? 1 : textureDesc.depth;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = textureFormatToDxgiFormat(textureDesc.format);
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	ID3D12Resource* resource;
	HRESULT hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&resource));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create KGFXtexture");
		return KGFX_HANDLE_NULL;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = resourceDesc.Format;
	srvDesc.ViewDimension = (textureDesc.height == 0) ? D3D12_SRV_DIMENSION_TEXTURE1D : (textureDesc.depth == 0) ? D3D12_SRV_DIMENSION_TEXTURE2D : D3D12_SRV_DIMENSION_TEXTURE3D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	if (srvDesc.ViewDimension == D3D12_SRV_DIMENSION_TEXTURE1D) {
		srvDesc.Texture1D.MostDetailedMip = 0;
		srvDesc.Texture1D.MipLevels = 1;
		srvDesc.Texture1D.ResourceMinLODClamp = 0.0f;
	} else if (srvDesc.ViewDimension == D3D12_SRV_DIMENSION_TEXTURE2D) {
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.PlaneSlice = 0;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	} else {
		srvDesc.Texture3D.MostDetailedMip = 0;
		srvDesc.Texture3D.MipLevels = 1;
		srvDesc.Texture3D.ResourceMinLODClamp = 0.0f;
	}

	/* create srv heap */
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;

	ID3D12DescriptorHeap* srvDescHeap;
	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&srvDescHeap));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create srv descriptor heap");
		RELEASE(resource);
		return KGFX_HANDLE_NULL;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvDescHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(resource, &srvDesc, srvHandle);

	KGFXtexture texture = new KGFXtexture_t{};
	texture->width = textureDesc.width;
	texture->height = textureDesc.height;
	texture->depth = textureDesc.depth;
	texture->format = resourceDesc.Format;
	texture->resource = resource;
	texture->heap = srvDescHeap;
	return texture;
}

KGFXsampler D3D12::createSampler(KGFXsamplerdesc samplerDesc) {
	D3D12_SAMPLER_DESC desc = {};
	if (samplerDesc.minFilter == KGFX_SAMPLER_FILTER_NEAREST) {
		desc.Filter = (samplerDesc.magFilter == KGFX_SAMPLER_FILTER_NEAREST) ? D3D12_FILTER_MIN_MAG_MIP_POINT : D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	} else {
		desc.Filter = (samplerDesc.magFilter == KGFX_SAMPLER_FILTER_NEAREST) ? D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	}
	
	desc.AddressU = (samplerDesc.addressModeU == KGFX_SAMPLER_ADDRESS_MODE_CLAMP) ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : (samplerDesc.addressModeU == KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT) ? D3D12_TEXTURE_ADDRESS_MODE_MIRROR : D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.AddressV = (samplerDesc.addressModeV == KGFX_SAMPLER_ADDRESS_MODE_CLAMP) ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : (samplerDesc.addressModeV == KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT) ? D3D12_TEXTURE_ADDRESS_MODE_MIRROR : D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.AddressW = (samplerDesc.addressModeW == KGFX_SAMPLER_ADDRESS_MODE_CLAMP) ? D3D12_TEXTURE_ADDRESS_MODE_CLAMP : (samplerDesc.addressModeW == KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT) ? D3D12_TEXTURE_ADDRESS_MODE_MIRROR : D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1.0f;
	desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	desc.MinLOD = 0.0f;
	desc.MaxLOD = D3D12_FLOAT32_MAX;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;

	ID3D12DescriptorHeap* heap;
	HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create sampler descriptor heap");
		return KGFX_HANDLE_NULL;
	}

	device->CreateSampler(&desc, heap->GetCPUDescriptorHandleForHeapStart());
	KGFXsampler sampler = new KGFXsampler_t{};
	sampler->heap = heap;
	return sampler;
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

	if (shaderDesc.medium != KGFX_MEDIUM_HLSL) {
		DEBUG_OUT("Implementation only supports HLSL");
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
	u32 vertexStride = 0;
	for (u32 i = 0; i < pipelineDesc.layout.bindingCount; ++i) {
		totalAttributeCount += pipelineDesc.layout.pBindings[i].attributeCount;
		for (u32 j = 0; j < pipelineDesc.layout.pBindings[i].attributeCount; ++j) {
			vertexStride += shaderDatatypeSize(pipelineDesc.layout.pBindings[i].pAttributes[j].type);
		}
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

	u32 uniformBufferCount = 0;
	u32 textureCount = 0;
	for (u32 i = 0; i < pipelineDesc.layout.descriptorSetCount; ++i) {
		if (pipelineDesc.layout.pDescriptorSets[i].usage == KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER) {
			++uniformBufferCount;
		} else if (pipelineDesc.layout.pDescriptorSets[i].usage == KGFX_DESCRIPTOR_USAGE_TEXTURE) {
			++textureCount;
		}
	}

	KGFXpipeline kgfxPipeline = new KGFXpipeline_t{};
	ID3D12RootSignature* rootSignature;
	HRESULT hr = S_OK;
	if (uniformBufferCount == 0) {
		rootSignature = emptyRootSignature;
	} else {
		std::vector<D3D12_ROOT_PARAMETER> parameters(uniformBufferCount + textureCount * 2);
		std::vector<D3D12_DESCRIPTOR_RANGE> ranges(textureCount * 2);
		u32 current = 0;
		u32 textureCurrent = 0;
		KGFXd3d12rootparam_t rootParam = {};
		for (u32 i = 0; i < pipelineDesc.layout.descriptorSetCount; ++i) {
			if (pipelineDesc.layout.pDescriptorSets[i].usage == KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER) {
				parameters[current].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				parameters[current].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				parameters[current].Constants.RegisterSpace = 0;
				parameters[current].Constants.ShaderRegister = pipelineDesc.layout.pDescriptorSets[i].binding;
				parameters[current].Constants.Num32BitValues = pipelineDesc.layout.pDescriptorSets[i].size / 4 + (pipelineDesc.layout.pDescriptorSets[i].size % 4 != 0);

				rootParam.binding = pipelineDesc.layout.pDescriptorSets[i].binding;
				rootParam.type = KGFX_D3D12_ROOT_PARAM_TYPE_UNIFORM_BUFFER;

				kgfxPipeline->rootParameterMap[rootParam] = current;
				++current;
			} else if (pipelineDesc.layout.pDescriptorSets[i].usage == KGFX_DESCRIPTOR_USAGE_TEXTURE) {
				ranges[textureCurrent].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				ranges[textureCurrent].NumDescriptors = 1;
				ranges[textureCurrent].BaseShaderRegister = pipelineDesc.layout.pDescriptorSets[i].binding;
				ranges[textureCurrent].RegisterSpace = 0;

				ranges[textureCurrent + 1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
				ranges[textureCurrent + 1].NumDescriptors = 1;
				ranges[textureCurrent + 1].BaseShaderRegister = pipelineDesc.layout.pDescriptorSets[i].binding;
				ranges[textureCurrent + 1].RegisterSpace = 0;

				parameters[current].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				parameters[current].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				parameters[current].DescriptorTable.NumDescriptorRanges = 1;
				parameters[current].DescriptorTable.pDescriptorRanges = &ranges[textureCurrent];

				parameters[current + 1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				parameters[current + 1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				parameters[current + 1].DescriptorTable.NumDescriptorRanges = 1;
				parameters[current + 1].DescriptorTable.pDescriptorRanges = &ranges[textureCurrent + 1];

				rootParam.binding = pipelineDesc.layout.pDescriptorSets[i].binding;
				rootParam.type = KGFX_D3D12_ROOT_PARAM_TYPE_TEXTURE;
				kgfxPipeline->rootParameterMap[rootParam] = current;

				rootParam.type = KGFX_D3D12_ROOT_PARAM_TYPE_SAMPLER;
				kgfxPipeline->rootParameterMap[rootParam] = current + 1;

				textureCurrent += 2;
				current += 2;
			}
		}

		for (auto const& key : kgfxPipeline->rootParameterMap) {
			DEBUG_OUTF("{}: Binding: {}, Type: {}", key.second, key.first.binding, key.first.type);
		}

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
		rootSignatureDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.Desc_1_0.NumParameters = parameters.size();
		rootSignatureDesc.Desc_1_0.pParameters = const_cast<const D3D12_ROOT_PARAMETER*>(parameters.data());
		rootSignatureDesc.Desc_1_0.NumStaticSamplers = 0;
		rootSignatureDesc.Desc_1_0.pStaticSamplers = nullptr;

		ID3DBlob* signature;
		ID3DBlob* error;
		hr = D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signature, &error);
		if (FAILED(hr)) {
			char* buf = new char[error->GetBufferSize()];
			memcpy(buf, error->GetBufferPointer(), error->GetBufferSize());
			DEBUG_OUTF("Failed to serialize root signature: {}", buf);
			delete kgfxPipeline;
			delete[] buf;
			RELEASE(error);
			return KGFX_HANDLE_NULL;
		}
		RELEASE(error);

		hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to create root signature");
			delete kgfxPipeline;
			RELEASE(signature);
			return KGFX_HANDLE_NULL;
		}

		RELEASE(signature);
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
	hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipeline));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create D3D12 pipeline state");
		delete kgfxPipeline;
		if (rootSignature != emptyRootSignature) {
			RELEASE(rootSignature);
		}
		return KGFX_HANDLE_NULL;
	}

	ID3D12GraphicsCommandList2* commandList;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipeline, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to create D3D12 command list");
		RELEASE(pipeline);
		delete kgfxPipeline;
		if (rootSignature != emptyRootSignature) {
			RELEASE(rootSignature);
		}
		return KGFX_HANDLE_NULL;
	}

	commandList->Close();

	kgfxPipeline->pipeline = pipeline;
	kgfxPipeline->commandList = commandList;
	kgfxPipeline->rootSignature = rootSignature;

	kgfxPipeline->d3dTopology = (pipelineDesc.topology == KGFX_TOPOLOGY_POINTS) ? D3D_PRIMITIVE_TOPOLOGY_POINTLIST : (pipelineDesc.topology == KGFX_TOPOLOGY_LINES) ? D3D_PRIMITIVE_TOPOLOGY_LINELIST : D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	kgfxPipeline->vertexStride = vertexStride;
	return kgfxPipeline;
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

	if (pipeline->rootSignature != emptyRootSignature) {
		RELEASE(pipeline->rootSignature);
	}
	RELEASE(pipeline->commandList);
	RELEASE(pipeline->pipeline);
	delete pipeline;
}

void D3D12::destroyBuffer(KGFXbuffer buffer) {
	if (buffer == nullptr) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return;
	}

	RELEASE(buffer->resource);
	delete buffer;
}

void D3D12::destroyTexture(KGFXtexture texture) {
	if (texture == nullptr) {
		DEBUG_OUT("Invalid KGFXtexture");
		return;
	}

	RELEASE(texture->heap);
	RELEASE(texture->resource);
	delete texture;
}

void D3D12::destroySampler(KGFXsampler sampler) {
	if (sampler == nullptr) {
		DEBUG_OUT("Invalid KGFXsampler");
		return;
	}

	RELEASE(sampler->heap);
	delete sampler;
}

KGFXcommandlist D3D12::createCommandList() {
	KGFXcommandlist commandList = new KGFXcommandlist_t{};
	resetCommand(commandList);
	return commandList;
}

void D3D12::resetCommand(KGFXcommandlist commandList) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	commandList->vertexBuffer = KGFX_HANDLE_NULL;
	commandList->indexBuffer = KGFX_HANDLE_NULL;
	commandList->pipeline = KGFX_HANDLE_NULL;
	commandList->presentCommand = false;
}

void D3D12::bindPipelineCommand(KGFXcommandlist commandList, KGFXpipeline pipeline) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to reset D3D12 command allocator");
		return;
	}

	hr = pipeline->commandList->Reset(commandAllocator, pipeline->pipeline);
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to reset D3D12 command list");
		return;
	}

	pipeline->commandList->SetGraphicsRootSignature(pipeline->rootSignature);
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

	commandList->pipeline = pipeline;
}

void D3D12::bindVertexBufferCommand(KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return;
	}

	commandList->vertexBinding = binding;
	commandList->vertexBuffer = buffer;
}

void D3D12::bindIndexBufferCommand(KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return;
	}

	commandList->indexBinding = binding;
	commandList->indexBuffer = buffer;
}

void D3D12::bindDescriptorSetBufferCommand(KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return;
	}

	if (commandList->pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (commandList->pipeline->commandList == nullptr) {
		DEBUG_OUT("Invalid D3D12 command list");
		return;
	}

	D3D12_GPU_VIRTUAL_ADDRESS address = buffer->resource->GetGPUVirtualAddress();
	commandList->pipeline->commandList->SetGraphicsRootConstantBufferView(binding, address);
}

void D3D12::bindDescriptorSetTextureCommand(KGFXcommandlist commandList, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (texture == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXtexture");
		return;
	}

	if (commandList->pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Pipeline not bound to command list yet");
		return;
	}

	KGFXd3d12rootparam_t rootParam = {};
	rootParam.binding = binding;
	rootParam.type = KGFX_D3D12_ROOT_PARAM_TYPE_TEXTURE;

	commandList->pipeline->commandList->SetDescriptorHeaps(1, &texture->heap);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = texture->heap->GetGPUDescriptorHandleForHeapStart();
	commandList->pipeline->commandList->SetGraphicsRootDescriptorTable(commandList->pipeline->rootParameterMap[rootParam], handle);

	if (sampler != KGFX_HANDLE_NULL) {
		if (sampler->heap == nullptr) {
			DEBUG_OUT("Invalid sampler D3D12 descriptor heap");
			return;
		}

		rootParam.type = KGFX_D3D12_ROOT_PARAM_TYPE_SAMPLER;

		commandList->pipeline->commandList->SetDescriptorHeaps(1, &sampler->heap);
		handle = sampler->heap->GetGPUDescriptorHandleForHeapStart();
		commandList->pipeline->commandList->SetGraphicsRootDescriptorTable(commandList->pipeline->rootParameterMap[rootParam], handle);
	} else {
		rootParam.type = KGFX_D3D12_ROOT_PARAM_TYPE_SAMPLER;

		handle = defaultSamplerHeap->GetGPUDescriptorHandleForHeapStart();
		commandList->pipeline->commandList->SetGraphicsRootDescriptorTable(commandList->pipeline->rootParameterMap[rootParam], handle);
	}
}

void D3D12::drawCommand(KGFXcommandlist commandList, u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (commandList->pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (commandList->vertexBuffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid vertex KGFXbuffer");
		return;
	}

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = commandList->vertexBuffer->resource->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = commandList->pipeline->vertexStride;
	vertexBufferView.SizeInBytes = commandList->vertexBuffer->size;

	commandList->pipeline->commandList->IASetPrimitiveTopology(commandList->pipeline->d3dTopology);
	commandList->pipeline->commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList->pipeline->commandList->DrawInstanced(vertexCount, instanceCount, firstVertex, firstInstance);
}

void D3D12::drawIndexedCommand(KGFXcommandlist commandList, u32 indexCount, u32 instanceCount, u32 firstIndex, s32 vertexOffset, u32 firstInstance) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (commandList->pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (commandList->vertexBuffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid vertex KGFXbuffer");
		return;
	}

	if (commandList->indexBuffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid index KGFXbuffer");
		return;
	}

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = commandList->vertexBuffer->resource->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = commandList->pipeline->vertexStride;
	vertexBufferView.SizeInBytes = commandList->vertexBuffer->size;

	D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
	indexBufferView.BufferLocation = commandList->indexBuffer->resource->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = commandList->indexBuffer->size;

	commandList->pipeline->commandList->IASetPrimitiveTopology(commandList->pipeline->d3dTopology);
	commandList->pipeline->commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList->pipeline->commandList->IASetIndexBuffer(&indexBufferView);
	commandList->pipeline->commandList->DrawIndexedInstanced(indexCount, instanceCount, firstIndex, vertexOffset, 0);
}

void D3D12::presentCommand(KGFXcommandlist commandList) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (commandList->pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (commandList->pipeline->commandList == nullptr) {
		DEBUG_OUT("Invalid D3D12 command list");
		return;
	}

	commandList->presentCommand = true;
}

void D3D12::submitCommands(KGFXcommandlist commandList) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	if (commandList->pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (commandList->pipeline->commandList == nullptr) {
		DEBUG_OUT("Invalid D3D12 command list");
		return;
	}

	D3D12_RESOURCE_BARRIER presentBarrier = {};
	presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	presentBarrier.Transition.pResource = renderTargets[frameIndex];
	presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	presentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->pipeline->commandList->ResourceBarrier(1, &presentBarrier);

	HRESULT hr = commandList->pipeline->commandList->Close();
	if (FAILED(hr)) {
		DEBUG_OUT("Failed to close D3D12 command list");
		return;
	}

	commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&commandList->pipeline->commandList));

	if (commandList->presentCommand) {
		u32 flags = 0;

		DXGI_PRESENT_PARAMETERS params = {};
		hr = swapchain->Present1(1, flags, &params);
		if (FAILED(hr)) {
			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
				DEBUG_OUT("D3D12 Device removed or reset");
				#ifdef KGFX_D3D12_BREADCRUMBS
				collectBreadcrumbs(commandList, commandList->pipeline);
				#endif
				return;
			} else {
				DEBUG_OUT("Failed to present");
				return;
			}
		}

		u64 value = inFlightFenceValue;

		hr = commandQueue->Signal(inFlightFence, value);
		if (FAILED(hr)) {
			DEBUG_OUT("Failed to signal in flight fence");
			return;
		}

		++inFlightFenceValue;
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

	commandList->presentCommand = false;
}

void D3D12::destroyCommandList(KGFXcommandlist commandList) {
	if (commandList == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcommandlist");
		return;
	}

	delete commandList;
}