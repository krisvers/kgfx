#ifndef PRISM_H
#define PRISM_H

#if defined(PRISM_D3D12) && !defined(WIN32)
#error "D3D12 only supported on windows platforms"
#endif /* #if defined(PRISM_D3D12) && !defined(WIN32) */

#if defined(PRISM_METAL) && !defined(__APPLE__)
#error "Metal only supported on Apple platforms"
#endif /* #if defined(PRISM_METAL) && !defined(__APPLE__) */

#include <stdint.h>

#define PRISM_DEFINE_HANDLE(object) typedef struct PrismObject* object

#ifndef PRISM_API
#define PRISM_API
#endif /* PRISM_API */

#ifdef PRISM_PLUSPLUS
namespace prism {

namespace cprism {
#endif /* #ifdef PRISM_PLUSPLUS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int8_t PrismBool;

#define PRISM_TRUE 1
#define PRISM_FALSE 0

PRISM_DEFINE_HANDLE(PrismInstance);
PRISM_DEFINE_HANDLE(PrismDevice);
PRISM_DEFINE_HANDLE(PrismBuffer);
PRISM_DEFINE_HANDLE(PrismTexture);
PRISM_DEFINE_HANDLE(PrismSwapchain);
PRISM_DEFINE_HANDLE(PrismQueue);
PRISM_DEFINE_HANDLE(PrismCommandPool);
PRISM_DEFINE_HANDLE(PrismCommandList);
PRISM_DEFINE_HANDLE(PrismShader);
PRISM_DEFINE_HANDLE(PrismGraphicsPipeline);

typedef enum PrismResult {
    /* feature not implemented yet */
    PRISM_RESULT_ERROR_UNIMPLEMENTED = -2,
    /* when error parsing or a new result has not been added yet*/
    PRISM_RESULT_ERROR_TEMPORARY_ERROR = -1,
    PRISM_RESULT_SUCCESS = 0,
    PRISM_RESULT_ENUMERATION_DONE = PRISM_RESULT_SUCCESS,
    PRISM_RESULT_ENUMERATION_IN_PROGRESS = 1,
    PRISM_RESULT_ERROR_UNKNOWN = 16,
    /* an argument passed to prism was invalid */
    PRISM_RESULT_ERROR_INVALID_ARGUMENT = 17,
    /* a feature requested is not supported (internal or external) */
    PRISM_RESULT_ERROR_UNSUPPORTED = 18,
    /* objects cannot be allocated based on the given space */
    PRISM_RESULT_ERROR_OUT_OF_MEMORY = 19,
} PrismResult;

typedef enum PrismInstanceAPI {
    PRISM_INSTANCE_API_VULKAN = 1,
    PRISM_INSTANCE_API_METAL = 2,
    PRISM_INSTANCE_API_D3D12 = 3,
    PRISM_INSTANCE_API_COUNT,
} PrismInstanceAPI;

typedef enum PrismInstanceCreateFlags {
    PRISM_INSTANCE_CREATE_FLAG_NONE = 0,
    PRISM_INSTANCE_CREATE_FLAG_DEBUG = 1,
    PRISM_INSTANCE_CREATE_FLAG_VALIDATION = 2,
    PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL = 4,
} PrismInstanceCreateFlags;
typedef uint32_t PrismInstanceCreateFlagBits;

typedef enum PrismDebugSeverity {
    PRISM_DEBUG_SEVERITY_VERBOSE = 1,
    PRISM_DEBUG_SEVERITY_INFO = 2,
    PRISM_DEBUG_SEVERITY_WARNING = 3,
    PRISM_DEBUG_SEVERITY_ERROR = 4,
} PrismDebugSeverity;

typedef enum PrismDebugSource {
    PRISM_DEBUG_SOURCE_PRISM = 1,
    PRISM_DEBUG_SOURCE_UNDERLYING_API = 2,
} PrismDebugSource;

typedef enum PrismAdapterType {
    PRISM_ADAPTER_TYPE_UNKNOWN = 0,
    PRISM_ADAPTER_TYPE_CPU = 1,
    PRISM_ADAPTER_TYPE_DEDICATED_GPU = 2,
    PRISM_ADAPTER_TYPE_INTEGRATED_GPU = 3,
    PRISM_ADAPTER_TYPE_UMA_GPU = 4,
    PRISM_ADAPTER_TYPE_VIRTUAL = 5,
} PrismAdapterType;

typedef enum PrismAdapterVendor {
    PRISM_ADAPTER_VENDOR_UNKNOWN = 0,
    PRISM_ADAPTER_VENDOR_AMD = 1,
    PRISM_ADAPTER_VENDOR_INTEL = 2,
    PRISM_ADAPTER_VENDOR_NVIDIA = 3,
    PRISM_ADAPTER_VENDOR_ARM = 4,
    PRISM_ADAPTER_VENDOR_QUALCOMM = 5,
    PRISM_ADAPTER_VENDOR_IMGTEC = 6,
    PRISM_ADAPTER_VENDOR_APPLE = 7,
} PrismAdapterVendor;

typedef enum PrismFormat {
    PRISM_FORMAT_UNKNOWN = 0,
    
    PRISM_FORMAT_R8_UINT             = 1,
    PRISM_FORMAT_R8_SINT             = 2,
    PRISM_FORMAT_R8_UNORM            = 3,
    PRISM_FORMAT_R8_SNORM            = 4,
    
    PRISM_FORMAT_R8G8_UINT           = 5,
    PRISM_FORMAT_R8G8_SINT           = 6,
    PRISM_FORMAT_R8G8_UNORM          = 7,
    PRISM_FORMAT_R8G8_SNORM          = 8,
    
    PRISM_FORMAT_R8G8B8A8_UINT       = 9,
    PRISM_FORMAT_R8G8B8A8_SINT       = 10,
    PRISM_FORMAT_R8G8B8A8_UNORM      = 11,
    PRISM_FORMAT_R8G8B8A8_SNORM      = 12,
    
    PRISM_FORMAT_R8G8B8A8_SRGB       = 13,
    PRISM_FORMAT_B8G8R8A8_SRGB       = 14,
    
    PRISM_FORMAT_R16_UINT            = 15,
    PRISM_FORMAT_R16_SINT            = 16,
    PRISM_FORMAT_R16_UNORM           = 17,
    PRISM_FORMAT_R16_SNORM           = 18,
    PRISM_FORMAT_R16_FLOAT           = 19,
    
    PRISM_FORMAT_R16G16_UINT         = 20,
    PRISM_FORMAT_R16G16_SINT         = 21,
    PRISM_FORMAT_R16G16_UNORM        = 22,
    PRISM_FORMAT_R16G16_SNORM        = 23,
    PRISM_FORMAT_R16G16_FLOAT        = 24,
    
    PRISM_FORMAT_R16G16B16A16_UINT   = 25,
    PRISM_FORMAT_R16G16B16A16_SINT   = 26,
    PRISM_FORMAT_R16G16B16A16_UNORM  = 27,
    PRISM_FORMAT_R16G16B16A16_SNORM  = 28,
    PRISM_FORMAT_R16G16B16A16_FLOAT  = 29,
    
    PRISM_FORMAT_R32_UINT            = 30,
    PRISM_FORMAT_R32_SINT            = 31,
    PRISM_FORMAT_R32_FLOAT           = 32,
    
    PRISM_FORMAT_R32G32_UINT         = 33,
    PRISM_FORMAT_R32G32_SINT         = 34,
    PRISM_FORMAT_R32G32_FLOAT        = 35,
    
    PRISM_FORMAT_R32G32B32A32_UINT   = 36,
    PRISM_FORMAT_R32G32B32A32_SINT   = 37,
    PRISM_FORMAT_R32G32B32A32_FLOAT  = 38,
    
    PRISM_FORMAT_D32_FLOAT           = 39,
    PRISM_FORMAT_D24_UNORM_S8_UINT   = 40,
    PRISM_FORMAT_D16_UNORM           = 41,
    
    PRISM_FORMAT_B8G8R8A8_UNORM      = 42,
    
    PRISM_FORMAT_R32G32B32_UINT      = 43,
    PRISM_FORMAT_R32G32B32_SINT      = 44,
    PRISM_FORMAT_R32G32B32_FLOAT     = 45,
} PrismFormat;

typedef enum PrismShaderStage {
    PRISM_SHADER_STAGE_VERTEX = 1,
    PRISM_SHADER_STAGE_TESS_CONTROL = 2,
    PRISM_SHADER_STAGE_TESS_EVAL = 4,
    PRISM_SHADER_STAGE_GEOMETRY = 8,
    PRISM_SHADER_STAGE_FRAGMENT = 16,
    PRISM_SHADER_STAGE_COMPUTE = 32,
    
    PRISM_SHADER_STAGE_GRAPHICS_MASK = PRISM_SHADER_STAGE_VERTEX | PRISM_SHADER_STAGE_TESS_CONTROL | PRISM_SHADER_STAGE_TESS_EVAL | PRISM_SHADER_STAGE_GEOMETRY | PRISM_SHADER_STAGE_FRAGMENT,
} PrismShaderStage;

typedef enum PrismCullMode {
    PRISM_CULL_MODE_NONE = 0,
    PRISM_CULL_MODE_FRONT = 1,
    PRISM_CULL_MODE_BACK = 2,
} PrismCullMode;

typedef enum PrismFillMode {
    PRISM_FILL_MODE_FILL = 1,
    PRISM_FILL_MODE_WIRE = 2,
    PRISM_FILL_MODE_POINT = 3,
} PrismFillMode;

typedef enum PrismTopology {
    PRISM_TOPOLOGY_TRIANGLES = 1,
    PRISM_TOPOLOGY_LINES = 2,
    PRISM_TOPOLOGY_POINTS = 3,
    PRISM_TOPOLOGY_TRIANGLE_STRIP = 4,
    PRISM_TOPOLOGY_LINE_STRIP = 5,
    PRISM_TOPOLOGY_PATCHES = 6,
} PrismTopology;

typedef enum PrismVertexInputRate {
    PRISM_VERTEX_INPUT_RATE_PER_INSTANCE = 1,
    PRISM_VERTEX_INPUT_RATE_PER_VERTEX = 2,
} PrismVertexInputRate;

typedef enum PrismBlendOp {
    PRISM_BLEND_OP_ADD = 1,
    PRISM_BLEND_OP_SUBTRACT = 2,
    PRISM_BLEND_OP_REVERSE_SUBTRACT = 3,
    PRISM_BLEND_OP_MIN = 4,
    PRISM_BLEND_OP_MAX = 5,
} PrismBlendOp;

typedef enum PrismLogicOp {
    PRISM_LOGIC_OP_DISABLED = 0,
    PRISM_LOGIC_OP_NO_OP = 1,
    PRISM_LOGIC_OP_CLEAR = 2,
    PRISM_LOGIC_OP_COPY = 3,
    PRISM_LOGIC_OP_INVERT = 4,
    PRISM_LOGIC_OP_AND = 5,
    PRISM_LOGIC_OP_OR = 6,
    PRISM_LOGIC_OP_XOR = 7,
    PRISM_LOGIC_OP_EQUALS = 8,
    PRISM_LOGIC_OP_NAND = 9,
    PRISM_LOGIC_OP_SET = 10,
    PRISM_LOGIC_OP_AND_INVERTED = 11,
    PRISM_LOGIC_OP_OR_INVERTED = 12,
    PRISM_LOGIC_OP_COPY_INVERTED = 13,
    PRISM_LOGIC_OP_AND_REVERSE = 14,
    PRISM_LOGIC_OP_OR_REVERSE = 15,
} PrismLogicOp;

typedef enum PrismRenderTargetOp {
    PRISM_RENDER_TARGET_OP_DONT_CARE = 0,
    PRISM_RENDER_TARGET_OP_LOAD = 1,
    PRISM_RENDER_TARGET_OP_STORE = 2,
    PRISM_RENDER_TARGET_OP_CLEAR = 3,
} PrismRenderTargetOp;

typedef enum PrismTextureLayout {
    PRISM_TEXTURE_LAYOUT_UNDEFINED = 0,
    PRISM_TEXTURE_LAYOUT_RENDER_TARGET = 1,
    PRISM_TEXTURE_LAYOUT_DEPTH_STENCIL_TARGET = 2,
    PRISM_TEXTURE_LAYOUT_DEPTH_STENCIL_READ = 3,
    PRISM_TEXTURE_LAYOUT_DEPTH_TARGET = 4,
    PRISM_TEXTURE_LAYOUT_DEPTH_READ = 5,
    PRISM_TEXTURE_LAYOUT_STENCIL_TARGET = 6,
    PRISM_TEXTURE_LAYOUT_STENCIL_READ = 7,
    PRISM_TEXTURE_LAYOUT_PRESENT = 8,
    PRISM_TEXTURE_LAYOUT_SHADER_READ = 9,
} PrismTextureLayout;

typedef enum PrismUniformBindPointType {
    PRISM_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX = 1,
    PRISM_UNIFORM_BIND_POINT_TYPE_D3D12_REGISTER = 2,
} PrismUniformBindPointType;

typedef enum PrismBlendFactor {
    PRISM_BLEND_FACTOR_ZERO = 1,
    PRISM_BLEND_FACTOR_ONE = 2,
    PRISM_BLEND_FACTOR_SOURCE = 3,
    PRISM_BLEND_FACTOR_DESTINATION = 4,
    PRISM_BLEND_FACTOR_INVERTED_SOURCE = 5,
    PRISM_BLEND_FACTOR_INVERTED_DESTINATION = 6,
} PrismBlendFactor;

typedef enum PrismColorMaskFlag {
    PRISM_COLOR_MASK_FLAG_RED = 1,
    PRISM_COLOR_MASK_FLAG_GREEN = 2,
    PRISM_COLOR_MASK_FLAG_BLUE = 4,
    PRISM_COLOR_MASK_FLAG_ALPHA = 8,
    
    PRISM_COLOR_MASK_ALL = PRISM_COLOR_MASK_FLAG_RED | PRISM_COLOR_MASK_FLAG_GREEN | PRISM_COLOR_MASK_FLAG_BLUE | PRISM_COLOR_MASK_FLAG_ALPHA,
} PrismColorMaskFlag;
typedef uint8_t PrismColorMaskBits;

typedef enum PrismPresentMode {
    /* only supported by D3D12 */
    PRISM_PRESENT_MODE_VSYNC_DISCARD_D3D12 = -3,
    /* only supported by Vulkan */
    PRISM_PRESENT_MODE_MAILBOX_VK = -2,
    /* not supported by D3D12 */
    PRISM_PRESENT_MODE_IMMEDIATE_VK_MTL = -1,
    
    PRISM_PRESENT_MODE_VSYNC = 1,
    PRISM_PRESENT_MODE_NO_SYNC = 2,
} PrismPresentMode;

typedef enum PrismQueueType {
    PRISM_QUEUE_TYPE_GENERIC = 0,
    PRISM_QUEUE_TYPE_GRAPHICS = 1,
    PRISM_QUEUE_TYPE_COMPUTE = 2,
    PRISM_QUEUE_TYPE_TRANSFER = 3,
} PrismQueueType;

typedef struct PrismAdapterDetails {
    PrismAdapterType type;
    PrismAdapterVendor vendor;
    
    uint32_t maxTextureDimensions[3];
    uint32_t maxFramebufferDimensions[3];
    uint32_t maxViewportDimensions[2];
    
    uint32_t maxViewports;
    uint32_t maxScissorRects;
    uint32_t maxVertexInputAttributes;
    uint32_t maxVertexInputBindings;
    uint32_t maxVertexInputAttributeOffset;
    uint32_t maxVertexInputBindingStride;
    uint32_t maxVertexOutputComponents;
    uint32_t maxColorAttachments;
    float maxAnisotropy;
    
    PrismBool supportsGraphics;
    PrismBool supportsCompute;
    PrismBool supportsRayTracing;
    
    uint64_t dedicatedMemoryGPU;
    uint64_t dedicatedMemoryCPU;
    uint64_t sharedMemory;
    uint64_t maxMemory;
    
    const char name[64];
} PrismAdapterDetails;

typedef struct PrismVertexBindingDesc {
    uint32_t binding;
    uint32_t stride;
    PrismVertexInputRate inputRate;
} PrismVertexBindingDesc;

typedef struct PrismVertexAttributeDesc {
    uint32_t binding;
    uint32_t location;
    uint32_t offset;
    PrismFormat format;
} PrismVertexAttributeDesc;

typedef struct PrismVertexInputDesc {
    uint32_t bindingCount;
    PrismVertexBindingDesc* pBindings;
    uint32_t attributeCount;
    PrismVertexAttributeDesc* pAttributes;
} PrismVertexInputDesc;

typedef struct PrismRenderTargetDesc {
    PrismFormat format;
    PrismBool enableBlending;
    PrismBlendOp colorBlendOp;
    PrismBlendFactor srcColorBlendFactor;
    PrismBlendFactor dstColorBlendFactor;
    PrismBlendOp alphaBlendOp;
    PrismBlendFactor srcAlphaBlendFactor;
    PrismBlendFactor dstAlphaBlendFactor;
    PrismColorMaskBits colorWriteMask;
    
    PrismRenderTargetOp loadOp;
    PrismRenderTargetOp storeOp;
    PrismTextureLayout finalLayout;
} PrismRenderTargetDesc;

typedef struct PrismUniformDesc {
    PrismUniformBindPointType bindPointType;
    uint32_t bindPoint;
} PrismUniformDesc;

typedef struct PrismUniformSignatureDesc {
    
} PrismUniformSignatureDesc;

typedef struct PrismGraphicsPipelineDesc {
    PrismCullMode cullMode;
    PrismBool counterClockwiseFrontFaceWinding;
    PrismFillMode fillMode;
    PrismTopology topology;
    uint32_t shaderCount;
    PrismShader* pShaders;
    /* TODO: add blending stuff */
    PrismVertexInputDesc vertexInputDesc;
    
    PrismLogicOp enableBlendLogicOp;
    uint32_t renderTargetDescCount;
    PrismRenderTargetDesc* pRenderTargetDescs;
    PrismFormat depthTargetFormat;
} PrismGraphicsPipelineDesc;

typedef struct PrismSwapchainDesc {
    uint32_t width;
    uint32_t height;
    uint32_t imageCount;
    PrismFormat format;
    PrismPresentMode presentMode;
} PrismSwapchainDesc;

typedef void (*PrismDebugCallbackPFN)(PrismInstance instance, PrismDebugSeverity severity, PrismDebugSource source, const char* message);

PRISM_API PrismResult prismCreateInstance(PrismInstanceAPI api, PrismInstanceCreateFlagBits flags, PrismInstance* pInstance);
PRISM_API void prismDestroyInstance(PrismInstance instance);

PRISM_API PrismResult prismDebugRegisterCallback(PrismInstance instance, PrismDebugCallbackPFN callback);

PRISM_API PrismResult prismEnumerateAdapters(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails);
PRISM_API PrismResult prismCreateDevice(PrismInstance instance, uint32_t adapterID, PrismDevice* pDevice);
PRISM_API void prismDestroyDevice(PrismDevice device);

PRISM_API PrismResult prismCreateGraphicsPipeline(PrismDevice device, const PrismGraphicsPipelineDesc* pPipelineDesc, PrismGraphicsPipeline* pPipeline);
PRISM_API void prismDestroyGraphicsPipeline(PrismGraphicsPipeline pipeline);

PRISM_API PrismResult prismCreateCommandPool(PrismDevice device, uint32_t maxCommandLists, PrismQueueType queueType, PrismCommandPool* pCommandPool);
PRISM_API void prismDestroyCommandPool(PrismCommandPool commandPool);

PRISM_API PrismResult prismCreateCommandList(PrismCommandPool commandPool, PrismCommandList* pCommandList);
PRISM_API void prismDestroyCommandList(PrismCommandList commandList);

#ifdef PRISM_WIN32
PRISM_API PrismResult prismCreateSwapchainWin32(PrismDevice device, void* hwnd, void* hinstance, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
#endif /* #ifdef PRISM_WIN32 */

#ifdef PRISM_XLIB
PRISM_API PrismResult prismCreateSwapchainXlib(PrismDevice device, void* display, unsigned long window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
#endif /* #ifdef PRISM_XLIB */

#ifdef PRISM_XCB
PRISM_API PrismResult prismCreateSwapchainXcb(PrismDevice device, void* connection, uint32_t window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
#endif /* #ifdef PRISM_XCB */

#ifdef PRISM_WAYLAND
PRISM_API PrismResult prismCreateSwapchainWayland(PrismDevice device, void* display, void* surface, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
#endif /* #ifdef PRISM_WAYLAND */

#ifdef PRISM_COCOA
PRISM_API PrismResult prismCreateSwapchainCocoa(PrismDevice device, void* nsWindow, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
#endif /* #ifdef PRISM_COCOA */

PRISM_API void prismDestroySwapchain(PrismSwapchain swapchain);
PRISM_API PrismTexture prismGetSwapchainBackbuffer(PrismSwapchain swapchain);

#if defined(PRISM_IMPLEMENTATION) || defined(PRISM_VULKAN_IMPLEMENTATION) || defined(PRISM_D3D12_IMPLEMENTATION) || defined(PRISM_METAL_IMPLEMENTATION)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PrismObject {
    PrismInstanceAPI api;
    PrismInstance instance;
} PrismObject;

#define PRISM_LIST(T_) struct { T_* data; uint32_t length; }
#define PRISM_LIST_INIT(list_, length_, T_) list_.data = (T_*) malloc(sizeof(T_) * length_); if (list_.data == NULL) { abort(); } list_.length = length_
#define PRISM_LIST_FREE(list_) free(list_.data)
#define PRISM_LIST_ZERO(list_) list_.data = NULL; list_.length = 0

#define PRISM_LIST_RESIZE(list_, length_, T_) if (list_.data != NULL) { list_.data = (T_*) realloc(list_.data, sizeof(T_) * length_); } else { list_.data = (T_*) malloc(sizeof(T_) * length_); } if (list_.data == NULL) { abort(); } list_.length = length_
#define PRISM_LIST_APPEND(list_, value_, T_) PRISM_LIST_RESIZE(list_, list_.length + 1, T_); memcpy(list_.data + sizeof(T_) * (list_.length - 1), &value_, sizeof(T_))
#define PRISM_LIST_REMOVE(list_, index_, T_) if (list_.length - 1 > index_) { memcpy(list_.data + sizeof(T_) * index_, list_.data + sizeof(T_) * (index_ + 1), sizeof(T_) * (list_.length - index_ - 1)); } PRISM_LIST_RESIZE(list_, list_.length - 1, T_)

#define PRISM_LOG(f_, inst_, sev_, src_, msg_) if (f_ != NULL) { f_(inst_, sev_, src_, msg_); }

#ifdef PRISM_VULKAN

PrismResult prismCreateInstance_vulkan(PrismInstanceCreateFlagBits flags, PrismInstance* pInstance);
void prismDestroyInstance_vulkan(PrismInstance instance);

PrismResult prismDebugRegisterCallback_vulkan(PrismInstance instance, PrismDebugCallbackPFN callback);

PrismResult prismEnumerateAdapters_vulkan(PrismInstance instance, uint32_t deviceID, PrismAdapterDetails* pAdapterDetails);
PrismResult prismCreateDevice_vulkan(PrismInstance instance, uint32_t deviceID, PrismDevice* pDevice);
void prismDestroyDevice_vulkan(PrismDevice device);

PrismResult prismCreateCommandPool_vulkan(PrismDevice device, uint32_t maxCommandLists, PrismQueueType queueType, PrismCommandPool* pCommandPool);
void prismDestroyCommandPool_vulkan(PrismCommandPool commandPool);

PrismResult prismCreateCommandList_vulkan(PrismCommandPool commandPool, PrismCommandList* pCommandList);
void prismDestroyCommandList_vulkan(PrismCommandList commandList);

PrismResult prismCreateSwapchainWin32_vulkan(PrismDevice device, void* hwnd, void* hinstance, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
PrismResult prismCreateSwapchainXlib_vulkan(PrismDevice device, void* display, unsigned long window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
PrismResult prismCreateSwapchainXcb_vulkan(PrismDevice device, void* connection, uint32_t window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
PrismResult prismCreateSwapchainWayland_vulkan(PrismDevice device, void* display, void* surface, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
PrismResult prismCreateSwapchainCocoa_vulkan(PrismDevice device, void* nsWindow, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
void prismDestroySwapchain_vulkan(PrismSwapchain swapchain);

#endif /* #ifdef PRISM_VULKAN */

#ifdef PRISM_D3D12

PrismResult prismCreateInstance_d3d12(PrismInstanceCreateFlagBits flags, PrismInstance* pInstance);
void prismDestroyInstance_d3d12(PrismInstance instance);

PrismResult prismDebugRegisterCallback_d3d12(PrismInstance instance, PrismDebugCallbackPFN callback);

PrismResult prismEnumerateAdapters_d3d12(PrismInstance instance, uint32_t deviceID, PrismAdapterDetails* pAdapterDetails);
PrismResult prismCreateDevice_d3d12(PrismInstance instance, uint32_t deviceID, PrismDevice* pDevice);
void prismDestroyDevice_d3d12(PrismDevice device);

PrismResult prismCreateSwapchainWin32_d3d12(PrismDevice device, void* hwnd, void* hinstance, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
void prismDestroySwapchain_d3d12(PrismSwapchain swapchain);

#endif /* #ifdef PRISM_D3D12 */

#ifdef PRISM_METAL

PrismResult prismCreateInstance_metal(PrismInstanceCreateFlagBits flags, PrismInstance* pInstance);
void prismDestroyInstance_metal(PrismInstance instance);

PrismResult prismDebugRegisterCallback_metal(PrismInstance instance, PrismDebugCallbackPFN callback);

PrismResult prismEnumerateAdapters_metal(PrismInstance instance, uint32_t deviceID, PrismAdapterDetails* pAdapterDetails);
PrismResult prismCreateDevice_metal(PrismInstance instance, uint32_t deviceID, PrismDevice* pDevice);
void prismDestroyDevice_metal(PrismDevice device);

PrismResult prismCreateSwapchainCocoa_metal(PrismDevice device, void* nsWindow, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain);
void prismDestroySwapchain_metal(PrismSwapchain swapchain);

#endif /* #ifdef PRISM_METAL */

#endif /* #if defined(PRISM_IMPLEMENTATION) || defined(PRISM_VULKAN_IMPLEMENTATION) || defined(PRISM_D3D12_IMPLEMENTATION) || defined(PRISM_METAL_IMPLEMENTATION) */

#ifdef PRISM_VULKAN_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__linux__) || defined(__gnu_linux__) || defined(linux) || defined(__linux)
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
#define VK_USE_PLATFORM_WAYLAND_KHR
#elif defined(__APPLE__)
#define VK_USE_PLATFORM_MACOS_MVK
#define VK_USE_PLATFORM_METAL_EXT
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */

#include <vulkan/vulkan.h>
#include <string.h>
#include <stdlib.h>

typedef struct PrismInstance_Vulkan_t {
    PrismObject obj;
    
    PrismInstanceCreateFlagBits flags;
    PrismDebugCallbackPFN debugCallback;
    PRISM_LIST(struct Prism_Vulkan_Surface_t) surfaces;
    
    struct {
        VkDebugUtilsMessengerEXT debugUtilsMessenger;
        PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
        
        VkInstance instance;        
        PRISM_LIST(VkPhysicalDevice) physicalDevices;
    } vk;
    
#ifdef __APPLE__
    struct {
        PrismBool macOSSurface;
        PrismBool metalSurface;
    } apple;
#endif /* #ifdef __APPLE__ */
} PrismInstance_Vulkan_t;

/* Pass in PrismInstance_Vulkan_t* */
#define PRISM_INSTVK_LOG(pinst_, ...) PRISM_LOG(pinst_->debugCallback, &pinst->obj, __VA_ARGS__)

/* Pass in PrismInstance */
#define PRISM_INST_LOG(inst_, ...) PRISM_INSTVK_LOG((PrismInstance_Vulkan_t*) inst_, __VA_ARGS__)

typedef struct PrismDevice_Vulkan_t {
    PrismObject obj;
    
    struct {
        VkDevice device;
        
        VkQueue graphicsQueue;
        VkQueue computeQueue;
        VkQueue transferQueue;
        VkQueue genericQueue;
        
        uint32_t graphicsQueueIndex;
        uint32_t computeQueueIndex;
        uint32_t transferQueueIndex;
        uint32_t genericQueueIndex;
    } vk;
} PrismDevice_Vulkan_t;

typedef struct PrismSwapchain_Vulkan_t {
    PrismObject obj;
    PrismDevice device;
    
    struct {
        VkSwapchainKHR swapchain;
        VkSurfaceKHR surface;
    } vk;
} PrismSwapchain_Vulkan_t;

typedef struct PrismCommandPool_Vulkan_t {
    PrismObject obj;
    PrismDevice device;
    PrismQueueType queueType;
    
    uint32_t maxCommandLists;
    uint32_t currentListCount;
    
    struct {
        VkQueue queue;
        uint32_t queueFamilyIndex;
        VkCommandPool commandPool;
    } vk;
} PrismCommandPool_Vulkan_t;

typedef struct PrismCommandList_Vulkan_t {
    PrismObject obj;
    PrismCommandPool commandPool;
    
    struct {
        VkCommandBuffer commandBuffer;
    } vk;
} PrismCommandList_Vulkan_t;

typedef enum Prism_Vulkan_WindowType_t {
    PRISM_INTERNAL_VULKAN_WINDOW_TYPE_WIN32 = 1,
    PRISM_INTERNAL_VULKAN_WINDOW_TYPE_XLIB = 2,
    PRISM_INTERNAL_VULKAN_WINDOW_TYPE_XCB = 3,
    PRISM_INTERNAL_VULKAN_WINDOW_TYPE_WAYLAND = 4,
    PRISM_INTERNAL_VULKAN_WINDOW_TYPE_COCOA = 5,
} Prism_Vulkan_WindowType_t;

typedef struct Prism_Vulkan_Surface_t {
    Prism_Vulkan_WindowType_t type;
    union {
        struct { void* hwnd; void* hinstance; } win32;
        struct { void* display; unsigned long window; } xlib;
        struct { void* connection; uint32_t window; } xcb;
        struct { void* display; void* surface; } wayland;
        struct { void* nsWindow; void* layer; } cocoa;
    } window;
    
    uint32_t referenceCount;
    struct {
        VkSurfaceKHR surface;
    } vk;
} Prism_Vulkan_Surface_t;

typedef struct Prism_Vulkan_ExtensionRequest_t {
    const char name[VK_MAX_EXTENSION_NAME_SIZE];
    PrismBool isNecessary;
} Prism_Vulkan_ExtensionRequest_t;

typedef PRISM_LIST(Prism_Vulkan_ExtensionRequest_t) Prism_Vulkan_ExtensionRequestList_t;
typedef PRISM_LIST(const char*) Prism_Vulkan_ActualExtensionList_t;

typedef Prism_Vulkan_ExtensionRequest_t Prism_Vulkan_LayerRequest_t;
typedef PRISM_LIST(Prism_Vulkan_LayerRequest_t) Prism_Vulkan_LayerRequestList_t;
typedef PRISM_LIST(const char*) Prism_Vulkan_ActualLayerList_t;

PrismResult prism_vulkan_getExtensions(VkPhysicalDevice vkPhysicalDevice, Prism_Vulkan_ExtensionRequestList_t requestedExtensionList, Prism_Vulkan_ActualExtensionList_t* pList) {
    PRISM_LIST(VkExtensionProperties) vkExtensionProperties = { NULL, 0 };
    Prism_Vulkan_ActualExtensionList_t actualExtensions = { NULL, 0 };
    uint32_t actualExtensionIndex = 0;
    if (requestedExtensionList.length != 0) {
        PRISM_LIST_INIT(actualExtensions, requestedExtensionList.length, const char*);
        if (vkPhysicalDevice == NULL) {
            vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionProperties.length, NULL);
        } else {
            vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, NULL, &vkExtensionProperties.length, NULL);
        }
        
        PRISM_LIST_INIT(vkExtensionProperties, vkExtensionProperties.length, VkExtensionProperties);
        if (vkPhysicalDevice == NULL) {
            vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionProperties.length, vkExtensionProperties.data);
        } else {
            vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, NULL, &vkExtensionProperties.length, vkExtensionProperties.data);
        }
        
        for (uint32_t j = 0; j < requestedExtensionList.length; ++j) {
            PrismBool added = PRISM_FALSE;
            for (uint32_t i = 0; i < vkExtensionProperties.length; ++i) {
                if (strcmp(vkExtensionProperties.data[i].extensionName, requestedExtensionList.data[j].name) == 0) {
                    actualExtensions.data[actualExtensionIndex++] = requestedExtensionList.data[j].name;
                    added = PRISM_TRUE;
                    break;
                }
            }
            
            if (!added && requestedExtensionList.data[j].isNecessary) {
                PRISM_LIST_FREE(vkExtensionProperties);
                PRISM_LIST_FREE(actualExtensions);
                return PRISM_RESULT_ERROR_UNSUPPORTED;
            }
        }
    }
    
    if (actualExtensionIndex == 0) {
        PRISM_LIST_FREE(actualExtensions);
        actualExtensions.data = NULL;
        actualExtensions.length = 0;
    } else {
        PRISM_LIST_RESIZE(actualExtensions, actualExtensionIndex, const char*);
    }
    PRISM_LIST_FREE(vkExtensionProperties);
    *pList = actualExtensions;
    return PRISM_RESULT_SUCCESS;
}

PrismResult prism_vulkan_getLayers(VkPhysicalDevice vkPhysicalDevice, Prism_Vulkan_LayerRequestList_t requestedLayerList, Prism_Vulkan_ActualLayerList_t* pList) {
    PRISM_LIST(VkLayerProperties) vkLayerProperties = { NULL, 0 };
    Prism_Vulkan_ActualLayerList_t actualLayers = { NULL, 0 };
    uint32_t actualLayerIndex = 0;
    if (requestedLayerList.length != 0) {
        PRISM_LIST_INIT(actualLayers, requestedLayerList.length, const char*);
        if (vkPhysicalDevice == NULL) {
            VkResult result = vkEnumerateInstanceLayerProperties(&vkLayerProperties.length, NULL);
        } else {
            vkEnumerateDeviceLayerProperties(vkPhysicalDevice, &vkLayerProperties.length, NULL);
        }
        
        PRISM_LIST_INIT(vkLayerProperties, vkLayerProperties.length, VkLayerProperties);
        if (vkPhysicalDevice == NULL) {
            vkEnumerateInstanceLayerProperties(&vkLayerProperties.length, vkLayerProperties.data);
        } else {
            vkEnumerateDeviceLayerProperties(vkPhysicalDevice, &vkLayerProperties.length, vkLayerProperties.data);
        }
        
        for (uint32_t j = 0; j < requestedLayerList.length; ++j) {
            PrismBool added = PRISM_FALSE;
            for (uint32_t i = 0; i < vkLayerProperties.length; ++i) {
                if (strcmp(vkLayerProperties.data[i].layerName, requestedLayerList.data[j].name) == 0) {
                    actualLayers.data[actualLayerIndex++] = requestedLayerList.data[j].name;
                    added = PRISM_TRUE;
                    break;
                }
            }
            
            if (!added && requestedLayerList.data[j].isNecessary) {
                PRISM_LIST_FREE(vkLayerProperties);
                PRISM_LIST_FREE(actualLayers);
                return PRISM_RESULT_ERROR_UNSUPPORTED;
            }
        }
    }
    
    if (actualLayerIndex == 0) {
        PRISM_LIST_FREE(actualLayers);
        actualLayers.data = NULL;
        actualLayers.length = 0;
    } else {
        PRISM_LIST_RESIZE(actualLayers, actualLayerIndex, const char*);
    }
    PRISM_LIST_FREE(vkLayerProperties);
    *pList = actualLayers;
    return PRISM_RESULT_SUCCESS;
}

VkFormat prism_vulkan_vkFormat(PrismFormat format) {
    switch (format) {
        case PRISM_FORMAT_R8_UINT: return VK_FORMAT_R8_UINT;
        case PRISM_FORMAT_R8_SINT: return VK_FORMAT_R8_SINT;
        case PRISM_FORMAT_R8_UNORM: return VK_FORMAT_R8_UNORM;
        case PRISM_FORMAT_R8_SNORM: return VK_FORMAT_R8_SNORM;
        case PRISM_FORMAT_R8G8_UINT: return VK_FORMAT_R8G8_UINT;
        case PRISM_FORMAT_R8G8_SINT: return VK_FORMAT_R8G8_SINT;
        case PRISM_FORMAT_R8G8_UNORM: return VK_FORMAT_R8G8_UNORM;
        case PRISM_FORMAT_R8G8_SNORM: return VK_FORMAT_R8G8_SNORM;
        case PRISM_FORMAT_R8G8B8A8_UINT: return VK_FORMAT_R8G8B8A8_UINT;
        case PRISM_FORMAT_R8G8B8A8_SINT: return VK_FORMAT_R8G8B8A8_SINT;
        case PRISM_FORMAT_R8G8B8A8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
        case PRISM_FORMAT_R8G8B8A8_SNORM: return VK_FORMAT_R8G8B8A8_SNORM;
        case PRISM_FORMAT_R8G8B8A8_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
        case PRISM_FORMAT_B8G8R8A8_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
        case PRISM_FORMAT_R16_UINT: return VK_FORMAT_R16_UINT;
        case PRISM_FORMAT_R16_SINT: return VK_FORMAT_R16_SINT;
        case PRISM_FORMAT_R16_UNORM: return VK_FORMAT_R16_UNORM;
        case PRISM_FORMAT_R16_SNORM: return VK_FORMAT_R16_SNORM;
        case PRISM_FORMAT_R16_FLOAT: return VK_FORMAT_R16_SFLOAT;
        case PRISM_FORMAT_R16G16_UINT: return VK_FORMAT_R16G16_UINT;
        case PRISM_FORMAT_R16G16_SINT: return VK_FORMAT_R16G16_SINT;
        case PRISM_FORMAT_R16G16_UNORM: return VK_FORMAT_R16G16_UNORM;
        case PRISM_FORMAT_R16G16_SNORM: return VK_FORMAT_R16G16_SNORM;
        case PRISM_FORMAT_R16G16_FLOAT: return VK_FORMAT_R16G16_SFLOAT;
        case PRISM_FORMAT_R16G16B16A16_UINT: return VK_FORMAT_R16G16B16A16_UINT;
        case PRISM_FORMAT_R16G16B16A16_SINT: return VK_FORMAT_R16G16B16A16_SINT;
        case PRISM_FORMAT_R16G16B16A16_UNORM: return VK_FORMAT_R16G16B16A16_UNORM;
        case PRISM_FORMAT_R16G16B16A16_SNORM: return VK_FORMAT_R16G16B16A16_SNORM;
        case PRISM_FORMAT_R16G16B16A16_FLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
        case PRISM_FORMAT_R32_UINT: return VK_FORMAT_R32_UINT;
        case PRISM_FORMAT_R32_SINT: return VK_FORMAT_R32_SINT;
        case PRISM_FORMAT_R32_FLOAT: return VK_FORMAT_R32_SFLOAT;
        case PRISM_FORMAT_R32G32_UINT: return VK_FORMAT_R32G32_UINT;
        case PRISM_FORMAT_R32G32_SINT: return VK_FORMAT_R32G32_SINT;
        case PRISM_FORMAT_R32G32_FLOAT: return VK_FORMAT_R32G32_SFLOAT;
        case PRISM_FORMAT_R32G32B32A32_UINT: return VK_FORMAT_R32G32B32A32_UINT;
        case PRISM_FORMAT_R32G32B32A32_SINT: return VK_FORMAT_R32G32B32A32_SINT;
        case PRISM_FORMAT_R32G32B32A32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case PRISM_FORMAT_D32_FLOAT: return VK_FORMAT_D32_SFLOAT;
        case PRISM_FORMAT_D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
        case PRISM_FORMAT_D16_UNORM: return VK_FORMAT_D16_UNORM;
        case PRISM_FORMAT_B8G8R8A8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
        case PRISM_FORMAT_R32G32B32_UINT: return VK_FORMAT_R32G32B32_UINT;
        case PRISM_FORMAT_R32G32B32_SINT: return VK_FORMAT_R32G32B32_SINT;
        case PRISM_FORMAT_R32G32B32_FLOAT: return VK_FORMAT_R32G32B32_SFLOAT;
            
        default:
        case PRISM_FORMAT_UNKNOWN:
            return VK_FORMAT_UNDEFINED;
    }
}

VkPresentModeKHR prism_vulkan_vkPresentMode(PrismPresentMode presentMode) {
    switch (presentMode) {
        case PRISM_PRESENT_MODE_MAILBOX_VK: return VK_PRESENT_MODE_MAILBOX_KHR;
        case PRISM_PRESENT_MODE_IMMEDIATE_VK_MTL: return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case PRISM_PRESENT_MODE_VSYNC: return VK_PRESENT_MODE_FIFO_KHR;
        case PRISM_PRESENT_MODE_NO_SYNC: return VK_PRESENT_MODE_IMMEDIATE_KHR;
            
        default:
        case PRISM_PRESENT_MODE_VSYNC_DISCARD_D3D12:
            return VK_PRESENT_MODE_MAX_ENUM_KHR;
    }
}

VkBool32 prism_vulkan_debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) pUserData;
    if (vulkanInstance->debugCallback == NULL) {
        return VK_FALSE;
    }
    
    PrismDebugSeverity severity = 0;
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        severity = PRISM_DEBUG_SEVERITY_ERROR;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        severity = PRISM_DEBUG_SEVERITY_WARNING;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        severity = PRISM_DEBUG_SEVERITY_VERBOSE;
    } else {
        severity = PRISM_DEBUG_SEVERITY_INFO;
    }
    
    vulkanInstance->debugCallback(&vulkanInstance->obj, severity, PRISM_DEBUG_SOURCE_UNDERLYING_API, pCallbackData->pMessage);
    return VK_FALSE;
}

PrismResult prismCreateInstance_vulkan(PrismInstanceCreateFlagBits flags, PrismInstance* pInstance) {
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "Prism";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Prism";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;
    
    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = NULL;
    
    Prism_Vulkan_ExtensionRequestList_t requestedExtensionList = { NULL, 0 };
#ifdef __APPLE__
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    ++requestedExtensionList.length;
#endif /* #ifdef __APPLE__ */
    
    /* surface extensions */
    if (flags & PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL) {
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
        ++requestedExtensionList.length;
#elif defined(__linux__) || defined(__gnu_linux__) || defined(linux) || defined(__linux)
        /* X11/Xlib, Xcb, Wayland */
        requestedExtensionList.length += 3;
#elif defined(__APPLE__)
        /* VK_MVK_macos_surface and VK_EXT_metal_surface */
        requestedExtensionList.length += 2;
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */
        ++requestedExtensionList.length;
    }
    
    if (flags & PRISM_INSTANCE_CREATE_FLAG_DEBUG) {
        ++requestedExtensionList.length;
    }
    
    if (requestedExtensionList.length != 0) {
        PRISM_LIST_INIT(requestedExtensionList, requestedExtensionList.length, Prism_Vulkan_ExtensionRequest_t);
    }
    
    uint32_t requestedExtensionIndex = 0;
    
    /* surface extensions */
    if (flags & PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL) {
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_SURFACE_EXTENSION_NAME, strlen(VK_KHR_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
        
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, strlen(VK_KHR_WIN32_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
#elif defined(__linux__) || defined(__gnu_linux__) || defined(linux) || defined(__linux)
        /* X11/Xlib, Xcb, Wayland */
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_XLIB_SURFACE_EXTENSION_NAME, strlen(VK_KHR_XLIB_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_FALSE;
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_XCB_SURFACE_EXTENSION_NAME, strlen(VK_KHR_XCB_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_FALSE;
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, strlen(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_FALSE;
#elif defined(__APPLE__)
        /* VK_MVK_macos_surface and VK_EXT_metal_surface */
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_MVK_MACOS_SURFACE_EXTENSION_NAME, strlen(VK_MVK_MACOS_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_EXT_METAL_SURFACE_EXTENSION_NAME, strlen(VK_EXT_METAL_SURFACE_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */
    }
    
    if (flags & PRISM_INSTANCE_CREATE_FLAG_DEBUG) {
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
    }
    
#ifdef __APPLE__
    memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, strlen(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) + 1);
    requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
#endif /* #ifdef __APPLE__ */
    
    Prism_Vulkan_ActualExtensionList_t actualExtensions = { NULL, 0 };
    PrismResult presult = prism_vulkan_getExtensions(NULL, requestedExtensionList, &actualExtensions);
    if (presult != PRISM_RESULT_SUCCESS) {
        PRISM_LIST_FREE(requestedExtensionList);
        return presult;
    }
    
    createInfo.enabledExtensionCount = actualExtensions.length;
    createInfo.ppEnabledExtensionNames = actualExtensions.data;
    
    Prism_Vulkan_LayerRequestList_t requestedLayerList = { NULL, 0 };
    
    if (flags & PRISM_INSTANCE_CREATE_FLAG_VALIDATION) {
        ++requestedLayerList.length;
    }
    
    if (requestedLayerList.length != 0) {
        PRISM_LIST_INIT(requestedLayerList, requestedLayerList.length, Prism_Vulkan_LayerRequest_t);
    }
    
    uint32_t requestedLayerIndex = 0;
    if (flags & PRISM_INSTANCE_CREATE_FLAG_VALIDATION) {
        memcpy((void*) requestedLayerList.data[requestedLayerIndex].name, "VK_LAYER_KHRONOS_validation", strlen("VK_LAYER_KHRONOS_validation") + 1);
        requestedLayerList.data[requestedLayerIndex++].isNecessary = PRISM_TRUE;
    }
    
    Prism_Vulkan_ActualLayerList_t actualLayers = { NULL, 0 };
    presult = prism_vulkan_getLayers(NULL, requestedLayerList, &actualLayers);
    if (presult != PRISM_RESULT_SUCCESS) {
        PRISM_LIST_FREE(requestedLayerList);
        PRISM_LIST_FREE(requestedExtensionList);
        PRISM_LIST_FREE(actualExtensions);
        return presult;
    }
    
    createInfo.enabledLayerCount = actualLayers.length;
    createInfo.ppEnabledLayerNames = actualLayers.data;
    
    VkInstance vkInstance;
    VkResult result = vkCreateInstance(&createInfo, NULL, &vkInstance);
    
    PRISM_LIST_FREE(requestedLayerList);
    PRISM_LIST_FREE(requestedExtensionList);
    PRISM_LIST_FREE(actualLayers);
    PRISM_LIST_FREE(actualExtensions);
    
    if (result != VK_SUCCESS) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    PrismInstance_Vulkan_t* instance = (PrismInstance_Vulkan_t*) malloc(sizeof(PrismInstance_Vulkan_t));
    if (instance == NULL) {
        vkDestroyInstance(vkInstance, NULL);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    instance->obj.api = PRISM_INSTANCE_API_VULKAN;
    instance->flags = flags;
    instance->debugCallback = NULL;
    instance->vk.instance = vkInstance;
    instance->vk.debugUtilsMessenger = NULL;
    if (flags & PRISM_INSTANCE_CREATE_FLAG_DEBUG) {
        VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo;
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        dbgCreateInfo.pNext = NULL;
        dbgCreateInfo.flags = 0;
        dbgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        dbgCreateInfo.pfnUserCallback = prism_vulkan_debugUtilsMessengerCallback;
        dbgCreateInfo.pUserData = (void*) instance;
        
        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance->vk.instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != NULL) {
            func(instance->vk.instance, &dbgCreateInfo, NULL, &instance->vk.debugUtilsMessenger);
        }
        
        PFN_vkSetDebugUtilsObjectNameEXT setDbgName = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetInstanceProcAddr(instance->vk.instance, "vkSetDebugUtilsObjectNameEXT");
        instance->vk.vkSetDebugUtilsObjectNameEXT = setDbgName;
    }
    
    uint32_t adapterCount = 0;
    vkEnumeratePhysicalDevices(instance->vk.instance, &adapterCount, NULL);
    
    PRISM_LIST_INIT(instance->vk.physicalDevices, adapterCount, VkPhysicalDevice);
    vkEnumeratePhysicalDevices(instance->vk.instance, &adapterCount, instance->vk.physicalDevices.data);
    
    PRISM_LIST_ZERO(instance->surfaces);
    
    *pInstance = &instance->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyInstance_vulkan(PrismInstance instance) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    if (vulkanInstance->vk.debugUtilsMessenger != NULL) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkanInstance->vk.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != NULL) {
            func(vulkanInstance->vk.instance, vulkanInstance->vk.debugUtilsMessenger, NULL);
        }
    }
    
    vkDestroyInstance(vulkanInstance->vk.instance, NULL);
    PRISM_LIST_FREE(vulkanInstance->vk.physicalDevices);
    free(vulkanInstance);
}

PrismResult prismDebugRegisterCallback_vulkan(PrismInstance instance, PrismDebugCallbackPFN callback) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    if ((vulkanInstance->flags & PRISM_INSTANCE_CREATE_FLAG_DEBUG) == 0) {
        return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
    
    vulkanInstance->debugCallback = callback;
    return PRISM_RESULT_SUCCESS;
}

PrismResult prismEnumerateAdapters_vulkan(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    if (adapterID >= vulkanInstance->vk.physicalDevices.length) {
        return PRISM_RESULT_ENUMERATION_DONE;
    }
    
    VkPhysicalDevice vkPhysicalDevice = vulkanInstance->vk.physicalDevices.data[adapterID];
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
    
    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            pAdapterDetails->type = PRISM_ADAPTER_TYPE_CPU;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            pAdapterDetails->type = PRISM_ADAPTER_TYPE_DEDICATED_GPU;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            pAdapterDetails->type = PRISM_ADAPTER_TYPE_INTEGRATED_GPU;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            pAdapterDetails->type = PRISM_ADAPTER_TYPE_VIRTUAL;
            break;
        default:
            pAdapterDetails->type = PRISM_ADAPTER_TYPE_UNKNOWN;
            break;
    }
    
    switch (properties.vendorID) {
        case 0x1002:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_AMD;
            break;
        case 0x8086:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_INTEL;
            break;
        case 0x10DE:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_NVIDIA;
            break;
        case 0x13B5:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_ARM;
            break;
        case 0x5143:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_QUALCOMM;
            break;
        case 0x1010:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_IMGTEC;
            break;
        case 0x106B:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_APPLE;
            pAdapterDetails->type = PRISM_ADAPTER_TYPE_UMA_GPU;
            break;
        default:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_UNKNOWN;
            break;
    }
    
    pAdapterDetails->maxTextureDimensions[0] = properties.limits.maxImageDimension1D;
    pAdapterDetails->maxTextureDimensions[1] = properties.limits.maxImageDimension2D;
    pAdapterDetails->maxTextureDimensions[2] = properties.limits.maxImageDimension3D;
    
    pAdapterDetails->maxFramebufferDimensions[0] = properties.limits.maxFramebufferWidth;
    pAdapterDetails->maxFramebufferDimensions[1] = properties.limits.maxFramebufferHeight;
    pAdapterDetails->maxFramebufferDimensions[2] = properties.limits.maxFramebufferLayers;
    
    pAdapterDetails->maxViewportDimensions[0] = properties.limits.maxViewportDimensions[0];
    pAdapterDetails->maxViewportDimensions[1] = properties.limits.maxViewportDimensions[1];
    
    pAdapterDetails->maxViewports = properties.limits.maxViewports;
    pAdapterDetails->maxScissorRects = properties.limits.maxFramebufferWidth;
    pAdapterDetails->maxVertexInputAttributes = properties.limits.maxVertexInputAttributes;
    pAdapterDetails->maxVertexInputBindings = properties.limits.maxVertexInputBindings;
    pAdapterDetails->maxVertexInputAttributeOffset = properties.limits.maxVertexInputAttributeOffset;
    pAdapterDetails->maxVertexInputBindingStride = properties.limits.maxVertexInputBindingStride;
    pAdapterDetails->maxVertexOutputComponents = properties.limits.maxVertexOutputComponents;
    pAdapterDetails->maxColorAttachments = properties.limits.maxColorAttachments;
    pAdapterDetails->maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    
    pAdapterDetails->supportsGraphics = properties.limits.maxComputeWorkGroupCount[0] > 0;
    pAdapterDetails->supportsCompute = properties.limits.maxComputeWorkGroupCount[0] > 0;
    pAdapterDetails->supportsRayTracing = VK_VERSION_MAJOR(properties.apiVersion) >= 1 && VK_VERSION_MINOR(properties.apiVersion) >= 2;
    
    /* detect memory */
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);
    
    pAdapterDetails->dedicatedMemoryGPU = 0;
    pAdapterDetails->dedicatedMemoryCPU = 0;
    pAdapterDetails->sharedMemory = 0;
    pAdapterDetails->maxMemory = 0;
    
    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; i++) {
        VkMemoryHeap heap = memoryProperties.memoryHeaps[i];
        if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                pAdapterDetails->dedicatedMemoryGPU += heap.size;
            } else {
                pAdapterDetails->sharedMemory += heap.size;
            }
        } else {
            pAdapterDetails->dedicatedMemoryCPU += heap.size;
        }
        
        pAdapterDetails->maxMemory += heap.size;
    }
    
    snprintf((char*) pAdapterDetails->name, 64, "%s", properties.deviceName);
    return PRISM_RESULT_ENUMERATION_IN_PROGRESS;
}

PrismResult prismCreateDevice_vulkan(PrismInstance instance, uint32_t adapterID, PrismDevice* pDevice) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    if (adapterID >= vulkanInstance->vk.physicalDevices.length) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    VkPhysicalDevice vkPhysicalDevice = vulkanInstance->vk.physicalDevices.data[adapterID];
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &features);
    
    PRISM_LIST(VkQueueFamilyProperties) vkQueueFamilies = { NULL, 0 };
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &vkQueueFamilies.length, NULL);
    
    PRISM_LIST_INIT(vkQueueFamilies, vkQueueFamilies.length, VkQueueFamilyProperties);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &vkQueueFamilies.length, vkQueueFamilies.data);
    
    uint32_t gfxIndexBest = UINT32_MAX;
    int32_t gfxScoreBest = 0;
    uint32_t cmpIndexBest = UINT32_MAX;
    int32_t cmpScoreBest = 0;
    uint32_t trnIndexBest = UINT32_MAX;
    int32_t trnScoreBest = 0;
    uint32_t genericIndexBest = UINT32_MAX;
    int32_t genericScoreBest = 0;
    
    for (uint32_t i = 0; i < vkQueueFamilies.length; ++i) {
        int32_t gfxScore = 0;
        int32_t cmpScore = 0;
        int32_t trnScore = 0;
        int32_t genericScore = 0;
        
        if (vkQueueFamilies.data[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            ++gfxScore;
            --cmpScore;
            --trnScore;
            ++genericScore;
        }
        
        if (vkQueueFamilies.data[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            --gfxScore;
            ++cmpScore;
            --trnScore;
            ++genericScore;
        }
        
        if (vkQueueFamilies.data[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            --gfxScore;
            --cmpScore;
            ++trnScore;
            ++genericScore;
        }
        
        if (gfxScore > gfxScoreBest) {
            gfxScoreBest = gfxScore;
            gfxIndexBest = i;
        }
        
        if (cmpScore > cmpScoreBest) {
            cmpScoreBest =  cmpScore;
            cmpIndexBest = i;
        }
        
        if (trnScore > trnScoreBest) {
            trnScoreBest = trnScore;
            trnIndexBest = i;
        }
        
        if (genericScore > genericScoreBest) {
            genericScoreBest = genericScore;
            genericIndexBest = i;
        }
    }
    
    PRISM_LIST_FREE(vkQueueFamilies);
    
    float queuePriority = 1.0f;
    uint32_t uniqueQueueFamilyCount = 0;
    VkDeviceQueueCreateInfo uniqueQueueFamilyIndices[4];
    
    VkDeviceQueueCreateInfo base;
    base.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    base.pNext = NULL;
    base.flags = 0;
    base.queueFamilyIndex = 0;
    base.queueCount = 1;
    base.pQueuePriorities = &queuePriority;
    
    if (gfxIndexBest != UINT32_MAX) {
        memcpy(&uniqueQueueFamilyIndices[uniqueQueueFamilyCount], &base, sizeof(VkDeviceQueueCreateInfo));
        uniqueQueueFamilyIndices[uniqueQueueFamilyCount++].queueFamilyIndex = gfxIndexBest;
    }
    
    if (trnIndexBest != gfxIndexBest && trnIndexBest != UINT32_MAX) {
        memcpy(&uniqueQueueFamilyIndices[uniqueQueueFamilyCount], &base, sizeof(VkDeviceQueueCreateInfo));
        uniqueQueueFamilyIndices[uniqueQueueFamilyCount++].queueFamilyIndex = trnIndexBest;
    }
    
    if (cmpIndexBest != gfxIndexBest && cmpIndexBest != trnIndexBest && cmpIndexBest != UINT32_MAX) {
        memcpy(&uniqueQueueFamilyIndices[uniqueQueueFamilyCount], &base, sizeof(VkDeviceQueueCreateInfo));
        uniqueQueueFamilyIndices[uniqueQueueFamilyCount++].queueFamilyIndex = cmpIndexBest;
    }
    
    if (genericIndexBest != gfxIndexBest && genericIndexBest != trnIndexBest && genericIndexBest != cmpIndexBest && genericIndexBest != UINT32_MAX) {
        memcpy(&uniqueQueueFamilyIndices[uniqueQueueFamilyCount], &base, sizeof(VkDeviceQueueCreateInfo));
        uniqueQueueFamilyIndices[uniqueQueueFamilyCount++].queueFamilyIndex = genericIndexBest;
    }
    
    if (gfxIndexBest == UINT32_MAX) {
        gfxIndexBest = genericIndexBest;
    }
    
    if (cmpIndexBest == UINT32_MAX) {
        cmpIndexBest = genericIndexBest;
    }
    
    if (trnIndexBest == UINT32_MAX) {
        trnIndexBest = genericIndexBest;
    }
    
    VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = uniqueQueueFamilyCount;
    createInfo.pQueueCreateInfos = uniqueQueueFamilyIndices;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = NULL;
    createInfo.pEnabledFeatures = &features;
    
    Prism_Vulkan_ExtensionRequestList_t requestedExtensionList = { NULL, 0 };
    
#ifdef __APPLE__
    ++requestedExtensionList.length;
#endif /* #ifdef __APPLE__ */
    
    ++requestedExtensionList.length;
    
    if (vulkanInstance->flags & PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL) {
        ++requestedExtensionList.length;
    }
    
    if (requestedExtensionList.length != 0) {
        PRISM_LIST_INIT(requestedExtensionList, requestedExtensionList.length, Prism_Vulkan_ExtensionRequest_t);
    }
    
    uint32_t requestedExtensionIndex = 0;
#ifdef __APPLE__
    memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, "VK_KHR_portability_subset", strlen("VK_KHR_portability_subset") + 1);
    requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
#endif /* #ifdef __APPLE__ */
    
    memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, "VK_KHR_push_descriptor", strlen("VK_KHR_push_descriptor") + 1);
    requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
    
    if (vulkanInstance->flags & PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL) {
        memcpy((void*) requestedExtensionList.data[requestedExtensionIndex].name, VK_KHR_SWAPCHAIN_EXTENSION_NAME, strlen(VK_KHR_SWAPCHAIN_EXTENSION_NAME) + 1);
        requestedExtensionList.data[requestedExtensionIndex++].isNecessary = PRISM_TRUE;
    }
    
    Prism_Vulkan_ActualExtensionList_t actualExtensions = { NULL, 0 };
    PrismResult presult = prism_vulkan_getExtensions(vkPhysicalDevice, requestedExtensionList, &actualExtensions);
    if (presult != PRISM_RESULT_SUCCESS) {
        return presult;
    }
    
    createInfo.enabledExtensionCount = actualExtensions.length;
    createInfo.ppEnabledExtensionNames = actualExtensions.data;
    
    Prism_Vulkan_LayerRequestList_t requestedLayerList = { NULL, 0 };
    if (requestedLayerList.length != 0) {
        PRISM_LIST_INIT(requestedLayerList, requestedLayerList.length, Prism_Vulkan_LayerRequest_t);
    }
    
    uint32_t requestedLayerIndex = 0;
    Prism_Vulkan_ActualLayerList_t actualLayers = { NULL, 0 };
    presult = prism_vulkan_getLayers(NULL, requestedLayerList, &actualLayers);
    if (presult != PRISM_RESULT_SUCCESS) {
        PRISM_LIST_FREE(requestedLayerList);
        PRISM_LIST_FREE(requestedExtensionList);
        PRISM_LIST_FREE(actualExtensions);
        return presult;
    }
    
    createInfo.enabledLayerCount = actualLayers.length;
    createInfo.ppEnabledLayerNames = actualLayers.data;
    
    PrismDevice_Vulkan_t* device = (PrismDevice_Vulkan_t*) malloc(sizeof(PrismDevice_Vulkan_t));
    VkResult result = vkCreateDevice(vkPhysicalDevice, &createInfo, NULL, &device->vk.device);
    
    PRISM_LIST_FREE(requestedLayerList);
    PRISM_LIST_FREE(requestedExtensionList);
    PRISM_LIST_FREE(actualLayers);
    PRISM_LIST_FREE(actualExtensions);
    
    if (result != VK_SUCCESS) {
        free(device);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT != NULL) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = VK_OBJECT_TYPE_DEVICE;
        nameInfo.objectHandle = device->vk.device;
        nameInfo.pObjectName = "Prism Logical Device";
        vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT(device->vk.device, &nameInfo);
    }
    
    device->obj.api = PRISM_INSTANCE_API_VULKAN;
    device->obj.instance = instance;
    device->vk.graphicsQueueIndex = gfxIndexBest;
    device->vk.computeQueueIndex = cmpIndexBest;
    device->vk.transferQueueIndex = trnIndexBest;
    device->vk.genericQueueIndex = genericIndexBest;
    vkGetDeviceQueue(device->vk.device, gfxIndexBest, 0, &device->vk.graphicsQueue);
    vkGetDeviceQueue(device->vk.device, cmpIndexBest, 0, &device->vk.computeQueue);
    vkGetDeviceQueue(device->vk.device, trnIndexBest, 0, &device->vk.transferQueue);
    vkGetDeviceQueue(device->vk.device, genericIndexBest, 0, &device->vk.genericQueue);
    *pDevice = &device->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyDevice_vulkan(PrismDevice device) {
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) device;
    vkDestroyDevice(vulkanDevice->vk.device, NULL);
    free(vulkanDevice);
}

PrismResult prismCreateCommandPool_vulkan(PrismDevice device, uint32_t maxCommandLists, PrismQueueType queueType, PrismCommandPool* pCommandPool) {
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) device;
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) device->instance;
    if (maxCommandLists == 0) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    VkQueue queue = vulkanDevice->vk.genericQueue;
    uint32_t queueFamilyIndex = vulkanDevice->vk.genericQueueIndex;
    if (queueType == PRISM_QUEUE_TYPE_GRAPHICS) {
        queue = vulkanDevice->vk.graphicsQueue;
        queueFamilyIndex = vulkanDevice->vk.graphicsQueueIndex;
    } else if (queueType == PRISM_QUEUE_TYPE_COMPUTE) {
        queue = vulkanDevice->vk.computeQueue;
        queueFamilyIndex = vulkanDevice->vk.computeQueueIndex;
    } else if (queueType == PRISM_QUEUE_TYPE_TRANSFER) {
        queue = vulkanDevice->vk.transferQueue;
        queueFamilyIndex = vulkanDevice->vk.transferQueueIndex;
    }
    
    VkCommandPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.queueFamilyIndex = queueFamilyIndex;
    
    VkCommandPool vkCommandPool;
    if (vkCreateCommandPool(vulkanDevice->vk.device, &createInfo, NULL, &vkCommandPool) != VK_SUCCESS) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT != NULL) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = VK_OBJECT_TYPE_COMMAND_POOL;
        nameInfo.objectHandle = vkCommandPool;
        nameInfo.pObjectName = "Prism Command Pool";
        vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT(vulkanDevice->vk.device, &nameInfo);
    }
    
    PrismCommandPool_Vulkan_t* commandPool = (PrismCommandPool_Vulkan_t*) malloc(sizeof(PrismCommandPool_Vulkan_t));
    if (commandPool == NULL) {
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCommandPool, NULL);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    commandPool->obj.api = PRISM_INSTANCE_API_VULKAN;
    commandPool->obj.instance = device->instance;
    commandPool->device = device;
    commandPool->queueType = queueType;
    commandPool->maxCommandLists = maxCommandLists;
    commandPool->currentListCount = 0;
    commandPool->vk.queue = queue;
    commandPool->vk.queueFamilyIndex = queueFamilyIndex;
    commandPool->vk.commandPool = vkCommandPool;
    
    *pCommandPool = &commandPool->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyCommandPool_vulkan(PrismCommandPool commandPool) {
    PrismCommandPool_Vulkan_t* vulkanCommandPool = (PrismCommandPool_Vulkan_t*) commandPool;
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) vulkanCommandPool->device;
    vkDestroyCommandPool(vulkanDevice->vk.device, vulkanCommandPool->vk.commandPool, NULL);
    free(commandPool);
}

PrismResult prismCreateCommandList_vulkan(PrismCommandPool commandPool, PrismCommandList* pCommandList) {
    PrismCommandPool_Vulkan_t* vulkanCommandPool = (PrismCommandPool_Vulkan_t*) commandPool;
    if (vulkanCommandPool->maxCommandLists == vulkanCommandPool->currentListCount) {
        return PRISM_RESULT_ERROR_OUT_OF_MEMORY;
    }
    
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) vulkanCommandPool->device;
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) vulkanCommandPool->device->instance;
    
    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.commandPool = vulkanCommandPool->vk.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer vkCommandBuffer;
    if (vkAllocateCommandBuffers(vulkanDevice->vk.device, &allocInfo, &vkCommandBuffer) != VK_SUCCESS) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT != NULL) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = VK_OBJECT_TYPE_COMMAND_BUFFER;
        nameInfo.objectHandle = vkCommandBuffer;
        nameInfo.pObjectName = "Prism Command List";
        vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT(vulkanDevice->vk.device, &nameInfo);
    }
    
    PrismCommandList_Vulkan_t* vulkanCommandList = (PrismCommandList_Vulkan_t*) malloc(sizeof(PrismCommandList_Vulkan_t));
    if (vulkanCommandList == NULL) {
        vkFreeCommandBuffers(vulkanDevice->vk.device, vulkanCommandPool->vk.commandPool, 1, &vkCommandBuffer);
        return PRISM_RESULT_ERROR_UNKNOWN;
    }
    
    vulkanCommandList->obj.api = PRISM_INSTANCE_API_VULKAN;
    vulkanCommandList->obj.instance = vulkanDevice->obj.instance;
    vulkanCommandList->commandPool = commandPool;
    vulkanCommandList->vk.commandBuffer = vkCommandBuffer;
    
    *pCommandList = &vulkanCommandList->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyCommandList_vulkan(PrismCommandList commandList) {
    PrismCommandList_Vulkan_t* vulkanCommandList = (PrismCommandList_Vulkan_t*) commandList;
    PrismCommandPool_Vulkan_t* vulkanCommandPool = (PrismCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) vulkanCommandPool->device;
    vkFreeCommandBuffers(vulkanDevice->vk.device, vulkanCommandPool->vk.commandPool, 1, &vulkanCommandList->vk.commandBuffer);
    free(commandList);
}

#ifdef WIN32
PrismResult prismCreateSwapchainWin32_vulkan(PrismDevice device, void* hwnd, void* hinstance, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    /* TODO: implement win32 swapchain */
    return PRISM_RESULT_ERROR_UNIMPLEMENTED;
}
#endif /* #ifdef WIN32 */

#if defined(__linux__) || defined(__gnu_linux__)
PrismResult prismCreateSwapchainXlib_vulkan(PrismDevice device, void* display, unsigned long window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    /* TODO: implement Xlib swapchain */
    return PRISM_RESULT_ERROR_UNIMPLEMENTED;
}

PrismResult prismCreateSwapchainXcb_vulkan(PrismDevice device, void* connection, uint32_t window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    /* TODO: implement Xcb swapchain */
    return PRISM_RESULT_ERROR_UNIMPLEMENTED;
}

PrismResult prismCreateSwapchainWayland_vulkan(PrismDevice device, void* display, void* surface, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    /* TODO: implement Wayland swapchain */
    return PRISM_RESULT_ERROR_UNIMPLEMENTED;
}
#endif /* #if defined(__linux__) || defined(__gnu_linux__) */

#ifdef __APPLE__
#include <objc/runtime.h>
#include <objc/message.h>

void prism_vulkan_releaseLayer(void* pLayer);

PrismResult prism_vulkan_createSurfaceCocoa(VkInstance instance, void* nsWindow, VkSurfaceKHR* pSurface, void** ppLayer);

PrismResult prismCreateSwapchainCocoa_vulkan(PrismDevice device, void* nsWindow, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    if (pSwapchainDesc->format == PRISM_FORMAT_UNKNOWN) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) device;
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    Prism_Vulkan_Surface_t* surface = NULL;
    uint32_t surfaceIndex = 0;
    for (uint32_t i = 0; i < vulkanInstance->surfaces.length; ++i) {
        if (nsWindow == vulkanInstance->surfaces.data[i].window.cocoa.nsWindow) {
            surface = &vulkanInstance->surfaces.data[i];
            surfaceIndex = i;
            break;
        }
    }
    
    if (surface == NULL) {
        VkSurfaceKHR vkSurface;
        void* layer;
        PrismResult result = prism_vulkan_createSurfaceCocoa(vulkanInstance->vk.instance, nsWindow, &vkSurface, &layer);
        if (result != PRISM_RESULT_SUCCESS) {
            return result;
        }
        
        Prism_Vulkan_Surface_t surf;
        surf.vk.surface = vkSurface;
        surf.type = PRISM_INTERNAL_VULKAN_WINDOW_TYPE_COCOA;
        surf.window.cocoa.nsWindow = nsWindow;
        surf.window.cocoa.layer = layer;
        surf.referenceCount = 0;
        
        surfaceIndex = vulkanInstance->surfaces.length;
        if (vulkanInstance->surfaces.data == NULL) {
            PRISM_LIST_INIT(vulkanInstance->surfaces, 1, Prism_Vulkan_Surface_t);
            memcpy(vulkanInstance->surfaces.data, &surf, sizeof(surf));
        } else {
            PRISM_LIST_APPEND(vulkanInstance->surfaces, surf, Prism_Vulkan_Surface_t);
        }
        surface = &vulkanInstance->surfaces.data[vulkanInstance->surfaces.length - 1];
    }
    
    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.surface = surface->vk.surface;
    createInfo.minImageCount = pSwapchainDesc->imageCount;
    createInfo.imageFormat = prism_vulkan_vkFormat(pSwapchainDesc->format);
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = (VkExtent2D) { pSwapchainDesc->width, pSwapchainDesc->height };
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = prism_vulkan_vkPresentMode(pSwapchainDesc->presentMode);
    if (createInfo.presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        PRISM_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, Prism_Vulkan_Surface_t);
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = NULL;
    
    VkSwapchainKHR vkSwapchain;
    if (vkCreateSwapchainKHR(vulkanDevice->vk.device, &createInfo, NULL, &vkSwapchain) != VK_SUCCESS) {
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        PRISM_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, Prism_Vulkan_Surface_t);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT != NULL) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = VK_OBJECT_TYPE_SWAPCHAIN_KHR;
        nameInfo.objectHandle = vkSwapchain;
        nameInfo.pObjectName = "Prism Cocoa Swapchain";
        vulkanInstance->vk.vkSetDebugUtilsObjectNameEXT(vulkanDevice->vk.device, &nameInfo);
    }
    
    PrismSwapchain_Vulkan_t* vulkanSwapchain = (PrismSwapchain_Vulkan_t*) malloc(sizeof(PrismSwapchain_Vulkan_t));
    if (vulkanSwapchain == NULL) {
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        PRISM_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, Prism_Vulkan_Surface_t);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    vulkanSwapchain->obj.api = PRISM_INSTANCE_API_VULKAN;
    vulkanSwapchain->obj.instance = device->instance;
    vulkanSwapchain->device = device;
    vulkanSwapchain->vk.surface = surface->vk.surface;
    vulkanSwapchain->vk.swapchain = vkSwapchain;
    
    *pSwapchain = &vulkanSwapchain->obj;
    ++surface->referenceCount;
    return PRISM_RESULT_SUCCESS;
}

#endif /* #ifdef __APPLE__ */

void prismDestroySwapchain_vulkan(PrismSwapchain swapchain) {
    PrismSwapchain_Vulkan_t* vulkanSwapchain = (PrismSwapchain_Vulkan_t*) swapchain;
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) vulkanSwapchain->device;
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) vulkanSwapchain->obj.instance;
    Prism_Vulkan_Surface_t* surface = NULL;
    uint32_t surfaceIndex = 0;
    for (uint32_t i = 0; i < vulkanInstance->surfaces.length; ++i) {
        if (vulkanInstance->surfaces.data[i].vk.surface == vulkanSwapchain->vk.surface) {
            surface = &vulkanInstance->surfaces.data[i];
            surfaceIndex = i;
            break;
        }
    }
    
    if (surface == NULL) {
        /* ??? */
        return;
    }
    
    vkDestroySwapchainKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, NULL);
    
    --surface->referenceCount;
    if (surface->referenceCount == 0) {
#ifdef __APPLE__
        prism_vulkan_releaseLayer(surface->window.cocoa.layer);
#endif /* #ifdef __APPLE__ */
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        PRISM_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, Prism_Vulkan_Surface_t);
    }
}

#endif /* #ifdef PRISM_VULKAN_IMPLEMENTATION */

#ifdef PRISM_VULKAN_COCOA_IMPLEMENTATION

#ifndef __OBJC__
#error "Objective-C is required for the Cocoa Vulkan implementation"
#endif /* #ifndef __OBJC__ */

#define VK_USE_PLATFORM_MACOS_MVK
#define VK_USE_PLATFORM_METAL_EXT
#include <vulkan/vulkan.h>

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

void prism_vulkan_releaseLayer(void* pLayer) {
    CAMetalLayer* layer = (__bridge CAMetalLayer*) pLayer;
    [layer release];
}

PrismResult prism_vulkan_createSurfaceCocoa(VkInstance instance, void* nsWindow, VkSurfaceKHR* pSurface, void** ppLayer) {
    CAMetalLayer* layer = [CAMetalLayer layer];
    if (layer == nil) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    NSWindow* window = (__bridge NSWindow*) nsWindow;
    
    [layer setContentsScale:[window backingScaleFactor]];
    [window.contentView setLayer:layer];
    [window.contentView setWantsLayer:YES];
    
    VkSurfaceKHR surface;
    PFN_vkCreateMetalSurfaceEXT createMetalSurface = (PFN_vkCreateMetalSurfaceEXT) vkGetInstanceProcAddr(instance, "vkCreateMetalSurfaceEXT");
    if (createMetalSurface != NULL) {
        VkMetalSurfaceCreateInfoEXT metalInfo = {};
        metalInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
        metalInfo.pLayer = layer;
        
        if (createMetalSurface(instance, &metalInfo, NULL, &surface) == VK_SUCCESS) {
            *pSurface = surface;
            *ppLayer = (__bridge void*) layer;
            return PRISM_RESULT_SUCCESS;
        }
    }
    
    PFN_vkCreateMacOSSurfaceMVK createMacOSSurface = (PFN_vkCreateMacOSSurfaceMVK) vkGetInstanceProcAddr(instance, "vkCreateMacOSSurfaceMVK");
    if (createMacOSSurface != NULL) {
        VkMacOSSurfaceCreateInfoMVK macOSInfo = {};
        macOSInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
        macOSInfo.pView = (__bridge void*) window.contentView;
        
        if (createMacOSSurface(instance, &macOSInfo, NULL, &surface) == VK_SUCCESS) {
            *pSurface = surface;
            *ppLayer = (__bridge void*) layer;
            return PRISM_RESULT_SUCCESS;
        }
        
        macOSInfo.pView = (__bridge void*) layer;
        if (createMacOSSurface(instance, &macOSInfo, NULL, &surface) == VK_SUCCESS) {
            *pSurface = surface;
            *ppLayer = (__bridge void*) layer;
            return PRISM_RESULT_SUCCESS;
        }
    }
    
    return PRISM_RESULT_ERROR_UNSUPPORTED;
}

#endif /* PRISM_VULKAN_COCOA_IMPLEMENTATION */

#ifdef PRISM_D3D12_IMPLEMENTATION

#include <d3d12.h>
#include <dxgi1_6.h>

#ifdef __cplusplus
#define PRISM_D3D12_IID(T_, obj_) __uuidof(T_), (void**) &obj_
#define PRISM_D3D12_CALL(obj_, method_, ...) obj_->method_(__VA_ARGS__)
#else
#define PRISM_D3D12_IID(T_, obj_) &IID_##T_, (void**) &obj_
#define PRISM_D3D12_CALL(obj_, method_, ...) obj_->lpVtbl->method_(obj_, __VA_ARGS__)
#endif /* #ifdef __cplusplus */

typedef struct PrismInstance_D3D12 {
    PrismObject obj;
    
    PrismInstanceCreateFlagBits flags;
    IDXGIFactory4* factory;
} PrismInstance_D3D12_t;

typedef struct PrismDevice_D3D12 {
    PrismObject obj;
    
    ID3D12Device* device;
} PrismDevice_D3D12_t;

PrismResult prismCreateInstance_d3d12(PrismInstanceCreateFlagBits flags, PrismInstance* pInstance) {
    IDXGIFactory4* factory;
    HRESULT result = CreateDXGIFactory2(flags & PRISM_INSTANCE_CREATE_FLAG_DEBUG ? DXGI_CREATE_FACTORY_DEBUG : 0, PRISM_D3D12_IID(IDXGIFactory4, factory));
    if (FAILED(result)) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    PrismInstance_D3D12_t* instance = (PrismInstance_D3D12_t*) malloc(sizeof(PrismInstance_D3D12_t));
    if (instance == NULL) {
        PRISM_D3D12_CALL(factory, Release);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    instance->obj.api = PRISM_INSTANCE_API_D3D12;
    instance->flags = flags;
    instance->factory = factory;
    
    *pInstance = &instance->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyInstance_d3d12(PrismInstance instance) {
    PrismInstance_D3D12_t* d3d12Instance = (PrismInstance_D3D12_t*) instance;
    PRISM_D3D12_CALL(d3d12Instance->factory, Release);
    free(d3d12Instance);
}

PrismResult prismEnumerateAdapters_d3d12(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails) {
    PrismInstance_D3D12_t* d3d12Instance = (PrismInstance_D3D12_t*) instance;
    
    IDXGIAdapter1* adapter;
    HRESULT result = PRISM_D3D12_CALL(d3d12Instance->factory, EnumAdapters1, adapterID, &adapter);
    if (FAILED(result)) {
        return PRISM_RESULT_ENUMERATION_DONE;
    }
    
    DXGI_ADAPTER_DESC1 desc;
    PRISM_D3D12_CALL(adapter, GetDesc1, &desc);
    
    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        pAdapterDetails->type = PRISM_ADAPTER_TYPE_CPU;
    } else if (desc.DedicatedVideoMemory > 0) {
        pAdapterDetails->type = PRISM_ADAPTER_TYPE_DEDICATED_GPU;
    } else if (desc.DedicatedSystemMemory > 0) {
        pAdapterDetails->type = PRISM_ADAPTER_TYPE_INTEGRATED_GPU;
    } else {
        pAdapterDetails->type = PRISM_ADAPTER_TYPE_UNKNOWN;
    }
    
    switch (desc.VendorId) {
        case 0x1002:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_AMD;
            break;
        case 0x8086:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_INTEL;
            break;
        case 0x10DE:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_NVIDIA;
            break;
        case 0x13B5:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_ARM;
            break;
        case 0x5143:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_QUALCOMM;
            break;
        case 0x1010:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_IMGTEC;
            break;
        case 0x106B:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_APPLE;
            break;
        default:
            pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_UNKNOWN;
            break;
    }
    
    ID3D12Device* device;
    result = D3D12CreateDevice((IUnknown*) adapter, D3D_FEATURE_LEVEL_11_0, PRISM_D3D12_IID(ID3D12Device, device));
    if (FAILED(result)) {
        PRISM_D3D12_CALL(adapter, Release);
        return PRISM_RESULT_ENUMERATION_DONE;
    }
    
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featuresSupport;
    PRISM_D3D12_CALL(device, CheckFeatureSupport, D3D12_FEATURE_D3D12_OPTIONS5, &featuresSupport, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5));
    PRISM_D3D12_CALL(device, Release);
    
    pAdapterDetails->maxTextureDimensions[0] = 16834;
    pAdapterDetails->maxTextureDimensions[1] = 16834;
    pAdapterDetails->maxTextureDimensions[2] = 2048;
    pAdapterDetails->maxFramebufferDimensions[0] = 16384;
    pAdapterDetails->maxFramebufferDimensions[1] = 16384;
    pAdapterDetails->maxFramebufferDimensions[2] = 2048;
    pAdapterDetails->maxViewportDimensions[0] = 16384;
    pAdapterDetails->maxViewportDimensions[1] = 16384;
    
    pAdapterDetails->maxViewports = UINT32_MAX;
    pAdapterDetails->maxScissorRects = UINT32_MAX;
    pAdapterDetails->maxVertexInputAttributes = UINT32_MAX;
    pAdapterDetails->maxVertexInputBindings = UINT32_MAX;
    pAdapterDetails->maxVertexInputAttributeOffset = UINT32_MAX;
    pAdapterDetails->maxVertexInputBindingStride = UINT32_MAX;
    pAdapterDetails->maxVertexOutputComponents = UINT32_MAX;
    pAdapterDetails->maxColorAttachments = UINT32_MAX;
    pAdapterDetails->maxAnisotropy = 16.0f;
    
    pAdapterDetails->supportsGraphics = PRISM_TRUE;
    pAdapterDetails->supportsCompute = PRISM_TRUE;
    pAdapterDetails->supportsRayTracing = featuresSupport.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
    
    pAdapterDetails->dedicatedMemoryGPU = desc.DedicatedVideoMemory;
    pAdapterDetails->dedicatedMemoryCPU = desc.DedicatedSystemMemory;
    pAdapterDetails->sharedMemory = desc.SharedSystemMemory;
    pAdapterDetails->maxMemory = desc.DedicatedVideoMemory + desc.DedicatedSystemMemory + desc.SharedSystemMemory;
    
    snprintf((char*) pAdapterDetails->name, 64, "%ls", desc.Description);
    PRISM_D3D12_CALL(adapter, Release);
    return PRISM_RESULT_ENUMERATION_IN_PROGRESS;
}

PrismResult prismCreateDevice_d3d12(PrismInstance instance, uint32_t adapterID, PrismDevice* pDevice) {
    PrismInstance_D3D12_t* d3d12Instance = (PrismInstance_D3D12_t*) instance;
    
    IDXGIAdapter1* adapter;
    HRESULT result = PRISM_D3D12_CALL(d3d12Instance->factory, EnumAdapters1, adapterID, &adapter);
    if (FAILED(result)) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    ID3D12Device* device;
    result = D3D12CreateDevice((IUnknown*) adapter, D3D_FEATURE_LEVEL_11_0, PRISM_D3D12_IID(ID3D12Device, device));
    if (FAILED(result)) {
        PRISM_D3D12_CALL(adapter, Release);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    PrismDevice_D3D12_t* d3d12Device = (PrismDevice_D3D12_t*) malloc(sizeof(PrismDevice_D3D12_t));
    if (d3d12Device == NULL) {
        PRISM_D3D12_CALL(device, Release);
        PRISM_D3D12_CALL(adapter, Release);
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    d3d12Device->obj.api = PRISM_INSTANCE_API_D3D12;
    d3d12Device->obj.instance = instance;
    d3d12Device->device = device;
    
    *pDevice = &d3d12Device->obj;
    PRISM_D3D12_CALL(adapter, Release);
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyDevice_d3d12(PrismDevice device) {
    PrismDevice_D3D12_t* d3d12Device = (PrismDevice_D3D12_t*) device;
    PRISM_D3D12_CALL(d3d12Device->device, Release);
    free(d3d12Device);
}

PrismResult prismCreateSwapchainWin32_d3d12(PrismDevice device, void* hwnd, void* hinstance, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    /* TODO: implement D3D12 windows swapchain */
    return PRISM_RESULT_ERROR_UNIMPLMENTED;
}

void prismDestroySwapchain_d3d12(PrismSwapchain swapchain) {
    /* TODO: implement D3D12 windows swapchain */
    return;
}

#endif /* #ifdef PRISM_D3D12_IMPLEMENTATION */

#ifdef PRISM_METAL_IMPLEMENTATION

#ifndef __OBJC__
#error "Objective-C is required for Prism Metal implementation"
#endif /* #ifndef __objc */

#import <Metal/Metal.h>
#import <sys/sysctl.h>

typedef struct PrismInstance_Metal_t {
    PrismObject obj;
    
    NSArray<id<MTLDevice>>* devices;
} PrismInstance_Metal_t;

typedef struct PrismDevice_Metal_t {
    PrismObject obj;
    
    id<MTLDevice> device;
} PrismDevice_Metal_t;

PrismResult prismCreateInstance_metal(PrismInstanceCreateFlagBits flags, PrismInstance* pInstance) {
    PrismInstance_Metal_t* instance = (PrismInstance_Metal_t*) malloc(sizeof(PrismInstance_Metal_t));
    if (instance == NULL) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    instance->obj.api = PRISM_INSTANCE_API_METAL;
    instance->obj.instance = NULL;
    instance->devices = MTLCopyAllDevices();
    
    *pInstance = &instance->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyInstance_metal(PrismInstance instance) {
    PrismInstance_Metal_t* mtlInstance = (PrismInstance_Metal_t*) instance;
    [mtlInstance->devices release];
    free(mtlInstance);
}

PrismResult prismEnumerateAdapters_metal(PrismInstance instance, uint32_t deviceID, PrismAdapterDetails* pAdapterDetails) {
    PrismInstance_Metal_t* mtlInstance = (PrismInstance_Metal_t*) instance;
    if (deviceID >= [mtlInstance->devices count]) {
        return PRISM_RESULT_ENUMERATION_DONE;
    }
    
    id<MTLDevice> dev = mtlInstance->devices[deviceID];
    pAdapterDetails->type = (dev.lowPower) ? PRISM_ADAPTER_TYPE_INTEGRATED_GPU : PRISM_ADAPTER_TYPE_DEDICATED_GPU;
    if (dev.hasUnifiedMemory) {
        pAdapterDetails->type = PRISM_ADAPTER_TYPE_UMA_GPU;
    }
    
    NSString* nameLower = [dev.name lowercaseString];
    if ([nameLower containsString:@"apple"]) {
        pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_APPLE;
    } else if ([nameLower containsString:@"amd"]) {
        pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_AMD;
    } else if ([nameLower containsString:@"intel"]) {
        pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_INTEL;
    } else if ([nameLower containsString:@"nvidia"]) {
        pAdapterDetails->vendor = PRISM_ADAPTER_VENDOR_NVIDIA;
    } else {
        pAdapterDetails->vendor = PRISM_ADAPTER_TYPE_UNKNOWN;
    }
    [nameLower release];
    
    if ([dev supportsFamily:MTLGPUFamilyApple3]) {
        pAdapterDetails->maxTextureDimensions[0] = 16834;
        pAdapterDetails->maxTextureDimensions[1] = 16834;
    } else {
        pAdapterDetails->maxTextureDimensions[0] = 8192;
        pAdapterDetails->maxTextureDimensions[1] = 8192;
    }
    pAdapterDetails->maxTextureDimensions[2] = 2048;
    
    pAdapterDetails->maxFramebufferDimensions[0] = pAdapterDetails->maxTextureDimensions[0];
    pAdapterDetails->maxFramebufferDimensions[1] = pAdapterDetails->maxTextureDimensions[1];
    pAdapterDetails->maxFramebufferDimensions[2] = pAdapterDetails->maxTextureDimensions[2];
    
    pAdapterDetails->maxViewportDimensions[0] = pAdapterDetails->maxFramebufferDimensions[0];
    pAdapterDetails->maxViewportDimensions[1] = pAdapterDetails->maxFramebufferDimensions[1];
    
    if ([dev supportsFamily:MTLGPUFamilyApple5]) {
        pAdapterDetails->maxViewports = 16;
    } else {
        pAdapterDetails->maxViewports = 1;
    }
    
    pAdapterDetails->maxScissorRects = pAdapterDetails->maxViewports;
    
    pAdapterDetails->maxVertexInputAttributes = 31;
    pAdapterDetails->maxVertexInputBindings = 31;
    pAdapterDetails->maxVertexInputAttributeOffset = 1073741823;
    pAdapterDetails->maxVertexInputBindingStride = 1073741824;
    if ([dev supportsFamily:MTLGPUFamilyApple4]) {
        pAdapterDetails->maxVertexOutputComponents = 124;
    } else {
        pAdapterDetails->maxVertexOutputComponents = 60;
    }
    
    pAdapterDetails->maxColorAttachments = 8;
    pAdapterDetails->maxAnisotropy = 16.0f;
    
    pAdapterDetails->supportsGraphics = PRISM_TRUE;
    pAdapterDetails->supportsCompute = PRISM_TRUE;
    pAdapterDetails->supportsRayTracing = PRISM_TRUE;
    
    pAdapterDetails->dedicatedMemoryGPU = 0;
    pAdapterDetails->dedicatedMemoryCPU = 0;
    pAdapterDetails->sharedMemory = 0;
    /* calculate system memory */
    if ([dev hasUnifiedMemory]) {
        pAdapterDetails->maxMemory = 0;
        size_t size = sizeof(pAdapterDetails->maxMemory);
        sysctlbyname("hw.memsize", &pAdapterDetails->maxMemory, &size, NULL, 0);
        pAdapterDetails->sharedMemory = pAdapterDetails->maxMemory;
    } else {
        /* no good way to detect actual memory */
        pAdapterDetails->maxMemory = dev.recommendedMaxWorkingSetSize;
        pAdapterDetails->dedicatedMemoryGPU = pAdapterDetails->maxMemory;
    }
    
    [dev.name getBytes:(char*) pAdapterDetails->name maxLength:63 usedLength:NULL encoding:NSUTF8StringEncoding options:0 range:NSMakeRange(0, [dev.name length]) remainingRange:NULL];
    
    return PRISM_RESULT_ENUMERATION_IN_PROGRESS;
}

PrismResult prismCreateDevice_metal(PrismInstance instance, uint32_t deviceID, PrismDevice* pDevice) {
    PrismInstance_Metal_t* mtlInstance = (PrismInstance_Metal_t*) instance;
    
    if (deviceID >= [mtlInstance->devices count]) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    PrismDevice_Metal_t* device = (PrismDevice_Metal_t*) malloc(sizeof(PrismDevice_Metal_t));
    if (device == NULL) {
        return PRISM_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    device->obj.api = PRISM_INSTANCE_API_METAL;
    device->obj.instance = instance;
    device->device = mtlInstance->devices[deviceID];
    
    *pDevice = &device->obj;
    return PRISM_RESULT_SUCCESS;
}

void prismDestroyDevice_metal(PrismDevice device) {
    free(device);
}

PrismResult prismCreateSwapchainCocoa_metal(PrismDevice device, void* nsWindow, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
    /* TODO: Implement Metal Cocoa swapchain */
    return PRISM_RESULT_ERROR_UNIMPLEMENTED;
}

void prismDestroySwapchain_metal(PrismSwapchain swapchain) {
    /* TODO: Implement Metal Cocoa swapchain */
    return;
}

#endif /* #ifdef PRISM_METAL_IMPLEMENTATION */

#ifdef PRISM_IMPLEMENTATION

PrismResult prismCreateInstance(PrismInstanceAPI api, PrismInstanceCreateFlagBits flags, PrismInstance* pInstance) {
    if (pInstance == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateInstance_vulkan(flags, pInstance);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            return prismCreateInstance_d3d12(flags, pInstance);
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            return prismCreateInstance_metal(flags, pInstance);
#endif /* PRISM_METAL */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
}

PRISM_API void prismDestroyInstance(PrismInstance instance) {
    if (instance == NULL) {
        return;
    }
    
    switch (instance->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            prismDestroyInstance_vulkan(instance);
            break;
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            prismDestroyInstance_d3d12(instance);
            break;
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            prismDestroyInstance_metal(instance);
            break;
#endif /* PRISM_METAL */
        default:
            break;
    }
}

PrismResult prismDebugRegisterCallback(PrismInstance instance, PrismDebugCallbackPFN callback) {
    if (instance == NULL || callback == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (instance->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismDebugRegisterCallback_vulkan(instance, callback);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            return prismDebugRegisterCallback_d3d12(instance, callback);
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            //return prismDebugRegisterCallback_metal(instance, callback);
#endif /* PRISM_METAL */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
}

PrismResult prismEnumerateAdapters(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails) {
    if (instance == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (instance->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismEnumerateAdapters_vulkan(instance, adapterID, pAdapterDetails);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            return prismEnumerateAdapters_d3d12(instance, adapterID, pAdapterDetails);
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            return prismEnumerateAdapters_metal(instance, adapterID, pAdapterDetails);
#endif /* PRISM_METAL */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
}

PrismResult prismCreateDevice(PrismInstance instance, uint32_t adapterID, PrismDevice* pDevice) {
    if (instance == NULL || pDevice == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (instance->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateDevice_vulkan(instance, adapterID, pDevice);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            return prismCreateDevice_d3d12(instance, adapterID, pDevice);
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            return prismCreateDevice_metal(instance, adapterID, pDevice);
#endif /* PRISM_METAL */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
}

void prismDestroyDevice(PrismDevice device) {
    if (device == NULL) {
        return;
    }
    
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            prismDestroyDevice_vulkan(device);
            break;
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            prismDestroyDevice_d3d12(device);
            break;
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            prismDestroyDevice_metal(device);
            break;
#endif /* PRISM_METAL */
        default:
            break;
    }
}

PrismResult prismCreateCommandPool(PrismDevice device, uint32_t maxCommandLists, PrismQueueType queueType, PrismCommandPool* pCommandPool) {
    if (device == NULL || maxCommandLists == 0 || pCommandPool == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateCommandPool_vulkan(device, maxCommandLists, queueType, pCommandPool);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            //return prismCreateCommandPool_d3d12(device, maxCommandLists, queueType, pCommandPool);
            return PRISM_RESULT_ERROR_UNIMPLEMENTED;
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            //return prismCreateCommandPool_metal(device, maxCommandLists, queueType, pCommandPool);
            return PRISM_RESULT_ERROR_UNIMPLEMENTED;
#endif /* PRISM_METAL */
        default:
            return PRISM_RESULT_ERROR_UNIMPLEMENTED;
    }
}

void prismDestroyCommandPool(PrismCommandPool commandPool) {
    if (commandPool == NULL) {
        return;
    }
    
    switch (commandPool->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            prismDestroyCommandPool_vulkan(commandPool);
            break;
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            //prismDestroyCommandPool_d3d12(commandPool);
            break;
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            //prismDestroyCommandPool_metal(commandPool);
            break;
#endif /* PRISM_METAL */
        default:
            break;
    }
}

PrismResult prismCreateCommandList(PrismCommandPool commandPool, PrismCommandList* pCommandList) {
    if (commandPool == NULL || pCommandList == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (commandPool->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateCommandList_vulkan(commandPool, pCommandList);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            //return prismCreateCommandList_d3d12(commandPool, pCommandList);
            return PRISM_RESULT_ERROR_UNIMPLEMENTED;
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            //return prismCreateCommandList_metal(commandPool, pCommandList);
            return PRISM_RESULT_ERROR_UNIMPLEMENTED;
#endif /* PRISM_METAL */
        default:
            return PRISM_RESULT_ERROR_UNIMPLEMENTED;
    }
    
}

void prismDestroyCommandList(PrismCommandList commandList) {
    if (commandList == NULL) {
        return;
    }
    
    switch (commandList->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            prismDestroyCommandList_vulkan(commandList);
            break;
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            //prismDestroyCommandList_d3d12(commandList);
            break;
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            //prismDestroyCommandList_metal(commandList);
            break;
#endif /* PRISM_METAL */
        default:
            break;
    }
}

PrismResult prismCreateSwapchainWin32(PrismDevice device, void* hwnd, void* hinstance, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
#ifndef WIN32
    return PRISM_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateSwapchainWin32_vulkan(device, hwnd, hinstance, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            return prismCreateSwapchainWin32_d3d12(device, hwnd, hinstance, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_D3D12 */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #ifndef WIN32 */
}

PrismResult prismCreateSwapchainXlib(PrismDevice device, void* display, unsigned long window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
#if !defined(__linux__) && !defined(__gnu_linux__)
    return PRISM_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateSwapchainXlib_vulkan(device, display, window, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_VULKAN */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !defined(__linux__) && !defined(__gnu_linux__) */
}

PrismResult prismCreateSwapchainXcb(PrismDevice device, void* connection, uint32_t window, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
#if !defined(__linux__) && !defined(__gnu_linux__)
    return PRISM_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateSwapchainXcb_vulkan(device, connection, window, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_VULKAN */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !defined(__linux__) && !defined(__gnu_linux__) */
}

PrismResult prismCreateSwapchainWayland(PrismDevice device, void* display, void* surface, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
#if !defined(__linux__) && !defined(__gnu_linux__)
    return PRISM_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateSwapchainWayland_vulkan(device, display, surface, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_VULKAN */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !defined(__linux__) && !defined(__gnu_linux__) */
}

PrismResult prismCreateSwapchainCocoa(PrismDevice device, void* nsWindow, const PrismSwapchainDesc* pSwapchainDesc, PrismSwapchain* pSwapchain) {
#ifndef __APPLE__
    return PRISM_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateSwapchainCocoa_vulkan(device, nsWindow, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_VULKAN */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            return prismCreateSwapchainCocoa_metal(device, nsWindow, pSwapchainDesc, pSwapchain);
#endif /* #ifdef PRISM_VULKAN */
        default:
            return PRISM_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #ifndef __APPLE__ */
}

void prismDestroySwapchain(PrismSwapchain swapchain) {
    switch (swapchain->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            prismDestroySwapchain_vulkan(swapchain);
            break;
#endif /* #ifdef PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            prismDestroySwapchain_d3d12(swapchain);
            break;
#endif /* #ifdef PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            prismDestroySwapchain_metal(swapchain);
            break;
#endif /* #ifdef PRISM_METAL */
        default:
            break;
    }
}

#endif /* PRISM_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef PRISM_PLUSPLUS
} /* namespace cprism */

} /* namespace prism */

#include <exception>
#include <functional>
#include <string>
#include <cstring>
#include <algorithm>

namespace prism {

enum class Result {
    ErrorUnimplemented = cprism::PRISM_RESULT_ERROR_UNIMPLEMENTED,
    ErrorTemporaryError = cprism::PRISM_RESULT_ERROR_TEMPORARY_ERROR,
    Success = cprism::PRISM_RESULT_SUCCESS,
    EnumerationDone = cprism::PRISM_RESULT_ENUMERATION_DONE,
    EnumerationInProgress = cprism::PRISM_RESULT_ENUMERATION_IN_PROGRESS,
    ErrorUnknown = cprism::PRISM_RESULT_ERROR_UNKNOWN,
    ErrorInvalidArgument = cprism::PRISM_RESULT_ERROR_INVALID_ARGUMENT,
    ErrorUnsupported = cprism::PRISM_RESULT_ERROR_UNSUPPORTED,
};

enum class InstanceAPI {
    Vulkan = cprism::PRISM_INSTANCE_API_VULKAN,
    Metal = cprism::PRISM_INSTANCE_API_METAL,
    D3D12 = cprism::PRISM_INSTANCE_API_D3D12,
};

enum class InstanceCreateFlags {
    None = cprism::PRISM_INSTANCE_CREATE_FLAG_NONE,
    Debug = cprism::PRISM_INSTANCE_CREATE_FLAG_DEBUG,
    Validation = cprism::PRISM_INSTANCE_CREATE_FLAG_VALIDATION,
    Graphical = cprism::PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL,
};

inline InstanceCreateFlags operator|(InstanceCreateFlags a, InstanceCreateFlags b) {
    return static_cast<InstanceCreateFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline InstanceCreateFlags operator&(InstanceCreateFlags a, InstanceCreateFlags b) {
    return static_cast<InstanceCreateFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline bool operator==(InstanceCreateFlags a, uint32_t b) {
    return (static_cast<uint32_t>(a) == b);
}

inline bool operator!=(InstanceCreateFlags a, uint32_t b) {
    return (static_cast<uint32_t>(a) != b);
}

enum class DebugSeverity {
    Verbose = cprism::PRISM_DEBUG_SEVERITY_VERBOSE,
    Info = cprism::PRISM_DEBUG_SEVERITY_INFO,
    Warning = cprism::PRISM_DEBUG_SEVERITY_WARNING,
    Error = cprism::PRISM_DEBUG_SEVERITY_ERROR,
};

enum class DebugSource {
    Prism = cprism::PRISM_DEBUG_SOURCE_PRISM,
    UnderlyingAPI = cprism::PRISM_DEBUG_SOURCE_UNDERLYING_API,
};

enum class AdapterType {
    Unknown = cprism::PRISM_ADAPTER_TYPE_UNKNOWN,
    CPU = cprism::PRISM_ADAPTER_TYPE_CPU,
    DedicatedGPU = cprism::PRISM_ADAPTER_TYPE_DEDICATED_GPU,
    IntegratedGPU = cprism::PRISM_ADAPTER_TYPE_INTEGRATED_GPU,
    UmaGPU = cprism::PRISM_ADAPTER_TYPE_UMA_GPU,
    Virtual = cprism::PRISM_ADAPTER_TYPE_VIRTUAL,
};

enum class AdapterVendor {
    Unknown = cprism::PRISM_ADAPTER_VENDOR_UNKNOWN,
    AMD = cprism::PRISM_ADAPTER_VENDOR_AMD,
    Intel = cprism::PRISM_ADAPTER_VENDOR_INTEL,
    Nvidia = cprism::PRISM_ADAPTER_VENDOR_NVIDIA,
    ARM = cprism::PRISM_ADAPTER_VENDOR_ARM,
    Qualcomm = cprism::PRISM_ADAPTER_VENDOR_QUALCOMM,
    ImgTec = cprism::PRISM_ADAPTER_VENDOR_IMGTEC,
    Apple = cprism::PRISM_ADAPTER_VENDOR_APPLE,
};

namespace detail {

constexpr const char* resultToString(Result result) {
    switch (result) {
        case Result::ErrorUnimplemented:
            return "Error Unimplemented";
        case Result::ErrorTemporaryError:
            return "Error Temporary Error";
        case Result::Success:
            return "Success";
        case Result::EnumerationInProgress:
            return "Enumeration In Progress";
        case Result::ErrorUnknown:
            return "Error Unknown";
        case Result::ErrorInvalidArgument:
            return "Error Invalid Argument";
        case Result::ErrorUnsupported:
            return "Error Unsupported";
        default:
            return "Unknown";
    }
}

} /* namespace detail */

struct AdapterDetails {
    AdapterType type;
    AdapterVendor vendor;
    
    uint32_t maxTextureDimensions[3];
    uint32_t maxFramebufferDimensions[3];
    uint32_t maxViewportDimensions[3];
    
    uint32_t maxViewports;
    uint32_t maxScissorRects;
    uint32_t maxVertexInputAttributes;
    uint32_t maxVertexInputBindings;
    uint32_t maxVertexInputAttributeOffset;
    uint32_t maxVertexInputBindingStride;
    uint32_t maxVertexOutputComponents;
    uint32_t maxColorAttachments;
    float maxAnisotropy;
    
    bool supportsGraphics;
    bool supportsCompute;
    bool supportsRayTracing;
    
    uint64_t dedicatedMemoryGPU;
    uint64_t dedicatedMemoryCPU;
    uint64_t sharedMemory;
    uint64_t maxMemory;
    
    const char name[64];
    
    operator cprism::PrismAdapterDetails() {
        return {};
    }
    
    AdapterDetails operator=(cprism::PrismAdapterDetails const& details) {
        type = static_cast<AdapterType>(details.type);
        vendor = static_cast<AdapterVendor>(details.vendor);
        
        maxTextureDimensions[0] = details.maxTextureDimensions[0];
        maxTextureDimensions[1] = details.maxTextureDimensions[1];
        maxTextureDimensions[2] = details.maxTextureDimensions[2];
        
        maxFramebufferDimensions[0] = details.maxFramebufferDimensions[0];
        maxFramebufferDimensions[1] = details.maxFramebufferDimensions[1];
        maxFramebufferDimensions[2] = details.maxFramebufferDimensions[2];
        
        maxViewportDimensions[0] = details.maxViewportDimensions[0];
        maxViewportDimensions[1] = details.maxViewportDimensions[1];
        
        maxViewports = details.maxViewports;
        maxScissorRects = details.maxScissorRects;
        maxVertexInputAttributes = details.maxVertexInputAttributes;
        maxVertexInputBindings = details.maxVertexInputBindings;
        maxVertexInputAttributeOffset = details.maxVertexInputAttributeOffset;
        maxVertexInputBindingStride = details.maxVertexInputBindingStride;
        maxVertexOutputComponents = details.maxVertexOutputComponents;
        maxColorAttachments = details.maxColorAttachments;
        maxAnisotropy = details.maxAnisotropy;
        
        supportsGraphics = details.supportsGraphics;
        supportsCompute = details.supportsCompute;
        supportsRayTracing = details.supportsRayTracing;
        
        dedicatedMemoryGPU = details.dedicatedMemoryGPU;
        dedicatedMemoryCPU = details.dedicatedMemoryCPU;
        sharedMemory = details.sharedMemory;
        maxMemory = details.maxMemory;
        
        std::strcpy(const_cast<char*>(name), details.name);
        return *this;
    }
    
    AdapterDetails(cprism::PrismAdapterDetails const& details) :
        type(static_cast<AdapterType>(details.type)), vendor(static_cast<AdapterVendor>(details.vendor)),
        maxTextureDimensions {
            details.maxTextureDimensions[0],
            details.maxTextureDimensions[1],
            details.maxTextureDimensions[2]
        },
        maxFramebufferDimensions {
            details.maxFramebufferDimensions[0],
            details.maxFramebufferDimensions[1],
            details.maxFramebufferDimensions[2]
        },
        maxViewportDimensions {
            details.maxViewportDimensions[0],
            details.maxViewportDimensions[1]
        },
        maxViewports(details.maxViewports), maxScissorRects(details.maxScissorRects),
        maxVertexInputAttributes(details.maxVertexInputAttributes),
        maxVertexInputBindings(details.maxVertexInputBindings),
        maxVertexInputAttributeOffset(details.maxVertexInputAttributeOffset),
        maxVertexInputBindingStride(details.maxVertexInputBindingStride),
        maxVertexOutputComponents(details.maxVertexOutputComponents),
        maxColorAttachments(details.maxColorAttachments),
        maxAnisotropy(details.maxAnisotropy),
        supportsGraphics(details.supportsGraphics),
        supportsCompute(details.supportsCompute),
        supportsRayTracing(details.supportsRayTracing),
        dedicatedMemoryGPU(details.dedicatedMemoryGPU),
        dedicatedMemoryCPU(details.dedicatedMemoryCPU),
        sharedMemory(details.sharedMemory),
        maxMemory(details.maxMemory),
        name("")
    {
        std::strcpy(const_cast<char*>(name), details.name);
    }
    
    AdapterDetails(AdapterDetails const& details) :
        type(details.type), vendor(details.vendor),
        maxTextureDimensions {
            details.maxTextureDimensions[0],
            details.maxTextureDimensions[1],
            details.maxTextureDimensions[2]
        },
        maxFramebufferDimensions {
            details.maxFramebufferDimensions[0],
            details.maxFramebufferDimensions[1],
            details.maxFramebufferDimensions[2]
        },
        maxViewportDimensions {
            details.maxViewportDimensions[0],
            details.maxViewportDimensions[1]
        },
        maxViewports(details.maxViewports), maxScissorRects(details.maxScissorRects),
        maxVertexInputAttributes(details.maxVertexInputAttributes),
        maxVertexInputBindings(details.maxVertexInputBindings),
        maxVertexInputAttributeOffset(details.maxVertexInputAttributeOffset),
        maxVertexInputBindingStride(details.maxVertexInputBindingStride),
        maxVertexOutputComponents(details.maxVertexOutputComponents),
        maxColorAttachments(details.maxColorAttachments),
        maxAnisotropy(details.maxAnisotropy),
        supportsGraphics(details.supportsGraphics),
        supportsCompute(details.supportsCompute),
        supportsRayTracing(details.supportsRayTracing),
        dedicatedMemoryGPU(details.dedicatedMemoryGPU),
        dedicatedMemoryCPU(details.dedicatedMemoryCPU),
        sharedMemory(details.sharedMemory),
        maxMemory(details.maxMemory),
        name("")
    {
        std::strcpy(const_cast<char*>(name), details.name);
    }
};

using DebugCallbackFunction = std::function<void(class Instance*, DebugSeverity, DebugSource, const char*)>;

class Error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    
    Error(std::string const& message, Result result) : std::runtime_error(std::string(detail::resultToString(result)) + ": " + message), message(message), result(result) {}
    Error(const char* message, Result result) : std::runtime_error(std::string(detail::resultToString(result)) + ": " + message), message(message), result(result) {}
    Error(Result result) : std::runtime_error(detail::resultToString(result)), result(result) {}
    Error(cprism::PrismResult result) : std::runtime_error(detail::resultToString(static_cast<Result>(result))), result(static_cast<Result>(result)) {}
    
    Error(Error const& error) : std::runtime_error(error.what()), message(error.message), result(error.result) {}
    Error(Error&& error) : std::runtime_error(error.what()), message(std::move(error.message)), result(std::move(error.result)) {}
    
    const char* what() const noexcept override {
        std::string what = detail::resultToString(result);
        if (message != "") {
            what += ": " + message;
        }
    }
    
private:
    std::string message = "";
    Result result;
};

class Instance {
public:
    Instance(InstanceAPI api, InstanceCreateFlags flags) {
        cprism::PrismResult r = cprism::prismCreateInstance(static_cast<cprism::PrismInstanceAPI>(api), static_cast<cprism::PrismInstanceCreateFlagBits>(flags), &handle);
        if (r != cprism::PRISM_RESULT_SUCCESS) {
            throw Error(r);
        }
    }
    
    ~Instance() {
        cprism::prismDestroyInstance(handle);
    }
    
    Result debugRegisterCallback(DebugCallbackFunction callback) {
        debugCallbacks[handle] = DebugCallbackPair(this, callback);
        return static_cast<Result>(cprism::prismDebugRegisterCallback(handle, debugCallback));
    }
    
    Result enumerateAdapters(uint32_t deviceID, AdapterDetails& adapterDetails) {
        cprism::PrismAdapterDetails cAdapterDetails = {};
        Result result = static_cast<Result>(cprism::prismEnumerateAdapters(handle, deviceID, &cAdapterDetails));
        adapterDetails = cAdapterDetails;
    }
    
private:
    static void debugCallback(cprism::PrismInstance instance, cprism::PrismDebugSeverity severity, cprism::PrismDebugSource source, const char* message) {
        for (auto pair : debugCallbacks) {
            if (pair.second.instance->handle == instance) {
                pair.second.func(pair.second.instance, static_cast<DebugSeverity>(severity), static_cast<DebugSource>(source), message);
                return;
            }
        }
    }
    
    struct DebugCallbackPair {
        Instance* instance;
        DebugCallbackFunction func;
        
        DebugCallbackPair() = default;
        
        DebugCallbackPair(Instance* instance, DebugCallbackFunction func) : instance(instance), func(func) {}
        DebugCallbackPair(DebugCallbackPair const& pair) : instance(pair.instance), func(pair.func) {}
    };
    
    static std::unordered_map<cprism::PrismInstance, DebugCallbackPair> debugCallbacks;
    cprism::PrismInstance handle;
};

} /* namespace prism */
#endif /* #ifdef PRISM_PLUSPLUS */

#endif /* PRISM_H */
