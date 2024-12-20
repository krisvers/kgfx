#ifndef KGFX_H
#define KGFX_H

#if defined(KGFX_D3D12) && !(defined(_WIN32) || defined(_WIN64) || defined(__WIN32__))
#error "D3D12 only supported on windows platforms"
#endif /* #if defined(KGFX_D3D12) && !(defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)) */

#if defined(KGFX_METAL) && !defined(__APPLE__)
#error "Metal only supported on Apple platforms"
#endif /* #if defined(KGFX_METAL) && !defined(__APPLE__) */

#include <stdint.h>

#define KGFX_DEFINE_HANDLE(object) typedef struct KGFXObject* object

#ifndef KGFX_API
#define KGFX_API
#endif /* KGFX_API */

#ifdef KGFX_PLUSPLUS
namespace kgfx {

namespace ckgfx {
#endif /* #ifdef KGFX_PLUSPLUS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int8_t KGFXBool;

#define KGFX_TRUE 1
#define KGFX_FALSE 0

KGFX_DEFINE_HANDLE(KGFXInstance);
KGFX_DEFINE_HANDLE(KGFXDevice);
KGFX_DEFINE_HANDLE(KGFXBuffer);
KGFX_DEFINE_HANDLE(KGFXTexture);
KGFX_DEFINE_HANDLE(KGFXSampler);
KGFX_DEFINE_HANDLE(KGFXFramebuffer);
KGFX_DEFINE_HANDLE(KGFXSwapchain);
KGFX_DEFINE_HANDLE(KGFXCommandPool);
KGFX_DEFINE_HANDLE(KGFXCommandList);
KGFX_DEFINE_HANDLE(KGFXShader);
KGFX_DEFINE_HANDLE(KGFXGraphicsPipeline);

typedef enum KGFXResult {
    /* feature not implemented yet */
    KGFX_RESULT_ERROR_UNIMPLEMENTED = -2,
    /* when error parsing or a new result has not been added yet*/
    KGFX_RESULT_ERROR_TEMPORARY_ERROR = -1,
    KGFX_RESULT_SUCCESS = 0,
    KGFX_RESULT_ENUMERATION_DONE = KGFX_RESULT_SUCCESS,
    KGFX_RESULT_ENUMERATION_IN_PROGRESS = 1,
    KGFX_RESULT_ERROR_UNKNOWN = 16,
    /* an argument passed to kgfx was invalid */
    KGFX_RESULT_ERROR_INVALID_ARGUMENT = 17,
    /* a feature requested is not supported (internal or external) */
    KGFX_RESULT_ERROR_UNSUPPORTED = 18,
    /* objects cannot be allocated based on the given space */
    KGFX_RESULT_ERROR_OUT_OF_MEMORY = 19,
    /* operation is illegal or cannot be performed */
    KGFX_RESULT_ERROR_INVALID_OPERATION = 20,
} KGFXResult;

typedef enum KGFXInstanceAPI {
    KGFX_INSTANCE_API_VULKAN = 1,
    KGFX_INSTANCE_API_METAL = 2,
    KGFX_INSTANCE_API_D3D12 = 3,
    KGFX_INSTANCE_API_D3D11 = 4,
    KGFX_INSTANCE_API_COUNT,
} KGFXInstanceAPI;

typedef enum KGFXInstanceCreateFlags {
    KGFX_INSTANCE_CREATE_FLAG_NONE = 0,
    KGFX_INSTANCE_CREATE_FLAG_DEBUG = 1,
    KGFX_INSTANCE_CREATE_FLAG_VALIDATION = 2,
    KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL = 4,
} KGFXInstanceCreateFlags;
typedef uint32_t KGFXInstanceCreateFlagBits;

typedef enum KGFXDebugSeverity {
    KGFX_DEBUG_SEVERITY_VERBOSE = 1,
    KGFX_DEBUG_SEVERITY_INFO = 2,
    KGFX_DEBUG_SEVERITY_WARNING = 3,
    KGFX_DEBUG_SEVERITY_ERROR = 4,
} KGFXDebugSeverity;

typedef enum KGFXDebugSource {
    KGFX_DEBUG_SOURCE_KGFX = 1,
    KGFX_DEBUG_SOURCE_UNDERLYING_API = 2,
} KGFXDebugSource;

typedef enum KGFXAdapterType {
    KGFX_ADAPTER_TYPE_UNKNOWN = 0,
    KGFX_ADAPTER_TYPE_CPU = 1,
    KGFX_ADAPTER_TYPE_DEDICATED_GPU = 2,
    KGFX_ADAPTER_TYPE_INTEGRATED_GPU = 3,
    KGFX_ADAPTER_TYPE_UMA_GPU = 4,
    KGFX_ADAPTER_TYPE_VIRTUAL = 5,
} KGFXAdapterType;

typedef enum KGFXAdapterVendor {
    KGFX_ADAPTER_VENDOR_UNKNOWN = 0,
    KGFX_ADAPTER_VENDOR_AMD = 1,
    KGFX_ADAPTER_VENDOR_INTEL = 2,
    KGFX_ADAPTER_VENDOR_NVIDIA = 3,
    KGFX_ADAPTER_VENDOR_ARM = 4,
    KGFX_ADAPTER_VENDOR_QUALCOMM = 5,
    KGFX_ADAPTER_VENDOR_IMGTEC = 6,
    KGFX_ADAPTER_VENDOR_APPLE = 7,
    KGFX_ADAPTER_VENDOR_MESA = 16,
} KGFXAdapterVendor;

typedef enum KGFXFormat {
    KGFX_FORMAT_UNKNOWN = 0,
    
    KGFX_FORMAT_R8_UINT             = 1,
    KGFX_FORMAT_R8_SINT             = 2,
    KGFX_FORMAT_R8_UNORM            = 3,
    KGFX_FORMAT_R8_SNORM            = 4,
    
    KGFX_FORMAT_R8G8_UINT           = 5,
    KGFX_FORMAT_R8G8_SINT           = 6,
    KGFX_FORMAT_R8G8_UNORM          = 7,
    KGFX_FORMAT_R8G8_SNORM          = 8,
    
    KGFX_FORMAT_R8G8B8A8_UINT       = 9,
    KGFX_FORMAT_R8G8B8A8_SINT       = 10,
    KGFX_FORMAT_R8G8B8A8_UNORM      = 11,
    KGFX_FORMAT_R8G8B8A8_SNORM      = 12,
    
    KGFX_FORMAT_R8G8B8A8_SRGB       = 13,
    KGFX_FORMAT_B8G8R8A8_SRGB       = 14,
    
    KGFX_FORMAT_R16_UINT            = 15,
    KGFX_FORMAT_R16_SINT            = 16,
    KGFX_FORMAT_R16_UNORM           = 17,
    KGFX_FORMAT_R16_SNORM           = 18,
    KGFX_FORMAT_R16_FLOAT           = 19,
    
    KGFX_FORMAT_R16G16_UINT         = 20,
    KGFX_FORMAT_R16G16_SINT         = 21,
    KGFX_FORMAT_R16G16_UNORM        = 22,
    KGFX_FORMAT_R16G16_SNORM        = 23,
    KGFX_FORMAT_R16G16_FLOAT        = 24,
    
    KGFX_FORMAT_R16G16B16A16_UINT   = 25,
    KGFX_FORMAT_R16G16B16A16_SINT   = 26,
    KGFX_FORMAT_R16G16B16A16_UNORM  = 27,
    KGFX_FORMAT_R16G16B16A16_SNORM  = 28,
    KGFX_FORMAT_R16G16B16A16_FLOAT  = 29,
    
    KGFX_FORMAT_R32_UINT            = 30,
    KGFX_FORMAT_R32_SINT            = 31,
    KGFX_FORMAT_R32_FLOAT           = 32,
    
    KGFX_FORMAT_R32G32_UINT         = 33,
    KGFX_FORMAT_R32G32_SINT         = 34,
    KGFX_FORMAT_R32G32_FLOAT        = 35,
    
    KGFX_FORMAT_R32G32B32A32_UINT   = 36,
    KGFX_FORMAT_R32G32B32A32_SINT   = 37,
    KGFX_FORMAT_R32G32B32A32_FLOAT  = 38,
    
    KGFX_FORMAT_D32_FLOAT           = 39,
    KGFX_FORMAT_D24_UNORM_S8_UINT   = 40,
    KGFX_FORMAT_D16_UNORM           = 41,
    
    KGFX_FORMAT_B8G8R8A8_UNORM      = 42,
    
    KGFX_FORMAT_R32G32B32_UINT      = 43,
    KGFX_FORMAT_R32G32B32_SINT      = 44,
    KGFX_FORMAT_R32G32B32_FLOAT     = 45,
} KGFXFormat;

typedef enum KGFXShaderStage {
    KGFX_SHADER_STAGE_VERTEX = 1,
    KGFX_SHADER_STAGE_TESS_CONTROL = 2,
    KGFX_SHADER_STAGE_TESS_EVAL = 4,
    KGFX_SHADER_STAGE_GEOMETRY = 8,
    KGFX_SHADER_STAGE_FRAGMENT = 16,
    KGFX_SHADER_STAGE_PIXEL = KGFX_SHADER_STAGE_FRAGMENT,
    
    KGFX_SHADER_STAGE_COMPUTE = 32,
    
    KGFX_SHADER_STAGE_GRAPHICS_MASK = KGFX_SHADER_STAGE_VERTEX | KGFX_SHADER_STAGE_TESS_CONTROL | KGFX_SHADER_STAGE_TESS_EVAL | KGFX_SHADER_STAGE_GEOMETRY | KGFX_SHADER_STAGE_FRAGMENT,
} KGFXShaderStage;
typedef uint32_t KGFXShaderStageMask;

typedef enum KGFXCullMode {
    KGFX_CULL_MODE_NONE = 0,
    KGFX_CULL_MODE_FRONT = 1,
    KGFX_CULL_MODE_BACK = 2,
} KGFXCullMode;

typedef enum KGFXFillMode {
    KGFX_FILL_MODE_FILL = 1,
    KGFX_FILL_MODE_WIRE = 2,
    KGFX_FILL_MODE_POINT = 3,
} KGFXFillMode;

typedef enum KGFXTopology {
    KGFX_TOPOLOGY_TRIANGLES = 1,
    KGFX_TOPOLOGY_LINES = 2,
    KGFX_TOPOLOGY_POINTS = 3,
    KGFX_TOPOLOGY_TRIANGLE_STRIP = 4,
    KGFX_TOPOLOGY_LINE_STRIP = 5,
    KGFX_TOPOLOGY_PATCHES = 6,
} KGFXTopology;

typedef enum KGFXVertexInputRate {
    KGFX_VERTEX_INPUT_RATE_PER_INSTANCE = 1,
    KGFX_VERTEX_INPUT_RATE_PER_VERTEX = 2,
} KGFXVertexInputRate;

typedef enum KGFXBlendOp {
    KGFX_BLEND_OP_ADD = 1,
    KGFX_BLEND_OP_SUBTRACT = 2,
    KGFX_BLEND_OP_REVERSE_SUBTRACT = 3,
    KGFX_BLEND_OP_MIN = 4,
    KGFX_BLEND_OP_MAX = 5,
} KGFXBlendOp;

typedef enum KGFXLogicOp {
    KGFX_LOGIC_OP_DISABLED = 0,
    KGFX_LOGIC_OP_NO_OP = 1,
    KGFX_LOGIC_OP_CLEAR = 2,
    KGFX_LOGIC_OP_COPY = 3,
    KGFX_LOGIC_OP_INVERT = 4,
    KGFX_LOGIC_OP_AND = 5,
    KGFX_LOGIC_OP_OR = 6,
    KGFX_LOGIC_OP_XOR = 7,
    KGFX_LOGIC_OP_EQUALS = 8,
    KGFX_LOGIC_OP_NAND = 9,
    KGFX_LOGIC_OP_SET = 10,
    KGFX_LOGIC_OP_AND_INVERTED = 11,
    KGFX_LOGIC_OP_OR_INVERTED = 12,
    KGFX_LOGIC_OP_COPY_INVERTED = 13,
    KGFX_LOGIC_OP_AND_REVERSE = 14,
    KGFX_LOGIC_OP_OR_REVERSE = 15,
} KGFXLogicOp;

typedef enum KGFXRenderTargetOp {
    KGFX_RENDER_TARGET_OP_DONT_CARE = 0,
    KGFX_RENDER_TARGET_OP_LOAD = 1,
    KGFX_RENDER_TARGET_OP_STORE = 2,
    KGFX_RENDER_TARGET_OP_CLEAR = 3,
} KGFXRenderTargetOp;

typedef enum KGFXStencilOp {
    KGFX_STENCIL_OP_KEEP = 0,
    KGFX_STENCIL_OP_ZERO = 1,
    KGFX_STENCIL_OP_REPLACE = 2,
    KGFX_STENCIL_OP_INCREM_CLAMP = 3,
    KGFX_STENCIL_OP_DECREM_CLAMP = 4,
    KGFX_STENCIL_OP_INVERT = 5,
    KGFX_STENCIL_OP_INCREM = 6,
    KGFX_STENCIL_OP_DECREM = 7,
} KGFXStencilOp;

typedef enum KGFXCompareOp {
    KGFX_COMPARE_OP_NEVER = 0,
    KGFX_COMPARE_OP_LESS = 1,
    KGFX_COMPARE_OP_EQUAL = 2,
    KGFX_COMPARE_OP_GREATER = 3,
    KGFX_COMPARE_OP_LEQUAL = 4,
    KGFX_COMPARE_OP_NEQUAL = 5,
    KGFX_COMPARE_OP_GEQUAL = 6,
    KGFX_COMPARE_OP_ALWAYS = 7,
} KGFXCompareOp;

typedef enum KGFXTextureLayout {
    KGFX_TEXTURE_LAYOUT_UNDEFINED = 0,
    KGFX_TEXTURE_LAYOUT_RENDER_TARGET = 1,
    KGFX_TEXTURE_LAYOUT_DEPTH_STENCIL_TARGET = 2,
    KGFX_TEXTURE_LAYOUT_DEPTH_STENCIL_READ = 3,
    KGFX_TEXTURE_LAYOUT_DEPTH_TARGET = 4,
    KGFX_TEXTURE_LAYOUT_DEPTH_READ = 5,
    KGFX_TEXTURE_LAYOUT_STENCIL_TARGET = 6,
    KGFX_TEXTURE_LAYOUT_STENCIL_READ = 7,
    KGFX_TEXTURE_LAYOUT_PRESENT = 8,
    KGFX_TEXTURE_LAYOUT_SHADER_READ = 9,
    KGFX_TEXTURE_LAYOUT_SHADER_WRITE = 10,
    KGFX_TEXTURE_LAYOUT_TRANSFER_SRC = 11,
    KGFX_TEXTURE_LAYOUT_TRANSFER_DST = 12,
} KGFXTextureLayout;

typedef enum KGFXUniformBindPointType {
    KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX = 1,
    KGFX_UNIFORM_BIND_POINT_TYPE_D3D12_REGISTER = 2,
} KGFXUniformBindPointType;

typedef enum KGFXDimensionType {
    KGFX_DIMENSION_TYPE_1D = 1,
    KGFX_DIMENSION_TYPE_2D = 2,
    KGFX_DIMENSION_TYPE_3D = 3,
    KGFX_DIMENSION_TYPE_CUBE = 4,
    KGFX_DIMENSION_TYPE_1D_ARRAY = 5,
    KGFX_DIMENSION_TYPE_2D_ARRAY = 6,
    KGFX_DIMENSION_TYPE_CUBE_ARRAY = 7,
} KGFXDimensionType;

typedef enum KGFXBlendFactor {
    KGFX_BLEND_FACTOR_ZERO = 1,
    KGFX_BLEND_FACTOR_ONE = 2,
    KGFX_BLEND_FACTOR_SRC_COLOR = 3,
    KGFX_BLEND_FACTOR_DST_COLOR = 4,
    KGFX_BLEND_FACTOR_INVERTED_SRC_COLOR = 5,
    KGFX_BLEND_FACTOR_INVERTED_DST_COLOR = 6,
    KGFX_BLEND_FACTOR_SRC_ALPHA = 7,
    KGFX_BLEND_FACTOR_DST_ALPHA = 8,
    KGFX_BLEND_FACTOR_INVERTED_SRC_ALPHA = 9,
    KGFX_BLEND_FACTOR_INVERTED_DST_ALPHA = 10,
} KGFXBlendFactor;

typedef enum KGFXColorMaskFlag {
    KGFX_COLOR_MASK_FLAG_RED = 1,
    KGFX_COLOR_MASK_FLAG_GREEN = 2,
    KGFX_COLOR_MASK_FLAG_BLUE = 4,
    KGFX_COLOR_MASK_FLAG_ALPHA = 8,
    
    KGFX_COLOR_MASK_ALL = KGFX_COLOR_MASK_FLAG_RED | KGFX_COLOR_MASK_FLAG_GREEN | KGFX_COLOR_MASK_FLAG_BLUE | KGFX_COLOR_MASK_FLAG_ALPHA,
} KGFXColorMaskFlag;
typedef uint32_t KGFXColorMask;

typedef enum KGFXPresentMode {
    /* only supported by D3D12 */
    KGFX_PRESENT_MODE_VSYNC_DISCARD_D3D = -3,
    /* only supported by Vulkan */
    KGFX_PRESENT_MODE_MAILBOX_VK = -2,
    /* not supported by D3D12 */
    KGFX_PRESENT_MODE_IMMEDIATE_VK_MTL = -1,
    
    KGFX_PRESENT_MODE_VSYNC = 1,
    KGFX_PRESENT_MODE_NO_SYNC = 2,
} KGFXPresentMode;

typedef enum KGFXQueueType {
    KGFX_QUEUE_TYPE_GENERIC = 0,
    KGFX_QUEUE_TYPE_GRAPHICS = 1,
    KGFX_QUEUE_TYPE_COMPUTE = 2,
    KGFX_QUEUE_TYPE_TRANSFER = 3,
} KGFXQueueType;

typedef enum KGFXResourceType {
    KGFX_RESOURCE_TYPE_UNIFORM_BUFFER = 1,
    KGFX_RESOURCE_TYPE_STORAGE_BUFFER = 2,
    KGFX_RESOURCE_TYPE_UNIFORM_TEXTURE = 3,
    KGFX_RESOURCE_TYPE_STORAGE_TEXTURE = 4,
    KGFX_RESOURCE_TYPE_SAMPLER = 5,
    /* TODO: multipass rendering KGFX_RESOURCE_TYPE_INPUT_ATTACHMENT */
} KGFXResourceType;

typedef enum KGFXBufferUsage {
    KGFX_BUFFER_USAGE_VERTEX_BUFFER = 1,
    KGFX_BUFFER_USAGE_INDEX_BUFFER = 2,
    KGFX_BUFFER_USAGE_UNIFORM_BUFFER = 4,
    KGFX_BUFFER_USAGE_STORAGE_BUFFER = 8,
    KGFX_BUFFER_USAGE_INDIRECT_BUFFER = 16,
    KGFX_BUFFER_USAGE_TRANSFER_SRC = 32,
    KGFX_BUFFER_USAGE_TRANSFER_DST = 64,

    KGFX_BUFFER_USAGE_VERTEX_BUFFER_INDEX_BUFFER = KGFX_BUFFER_USAGE_VERTEX_BUFFER | KGFX_BUFFER_USAGE_INDEX_BUFFER,
    KGFX_BUFFER_USAGE_UNIFORM_BUFFER_STORAGE_BUFFER = KGFX_BUFFER_USAGE_UNIFORM_BUFFER | KGFX_BUFFER_USAGE_STORAGE_BUFFER,
    KGFX_BUFFER_USAGE_TRANSFER_ALL = KGFX_BUFFER_USAGE_TRANSFER_SRC | KGFX_BUFFER_USAGE_TRANSFER_DST,
} KGFXBufferUsage;
typedef uint32_t KGFXBufferUsageMask;

typedef enum KGFXTextureUsage {
    KGFX_TEXTURE_USAGE_UNIFORM_TEXTURE = 1,
    KGFX_TEXTURE_USAGE_STORAGE_TEXTURE = 2,
    KGFX_TEXTURE_USAGE_RENDER_TARGET = 4,
    KGFX_TEXTURE_USAGE_DEPTH_STENCIL_TARGET = 8,
    KGFX_TEXTURE_USAGE_TRANSFER_SRC = 16,
    KGFX_TEXTURE_USAGE_TRANSFER_DST = 32,

    KGFX_TEXTURE_USAGE_TRANSFER_ALL = KGFX_TEXTURE_USAGE_TRANSFER_SRC | KGFX_TEXTURE_USAGE_TRANSFER_DST,
} KGFXTextureUsage;
typedef uint32_t KGFXTextureUsageMask;

typedef enum KGFXResourceLocation {
    KGFX_RESOURCE_LOCATION_DEVICE = 1,
    KGFX_RESOURCE_LOCATION_HOST = 2,
    KGFX_RESOURCE_LOCATION_SHARED = 3,
} KGFXResourceLocation;

typedef enum KGFXIndexType {
    KGFX_INDEX_TYPE_U16 = 1,
    KGFX_INDEX_TYPE_U32 = 2,
} KGFXIndexType;

typedef enum KGFXClearValueType {
    KGFX_CLEAR_VALUE_TYPE_F32 = 0,
    KGFX_CLEAR_VALUE_TYPE_S32 = 1,
    KGFX_CLEAR_VALUE_TYPE_U32 = 2,
    KGFX_CLEAR_VALUE_TYPE_DEPTH_STENCIL = 3,
} KGFXClearValueType;

typedef enum KGFXTextureFilter {
    KGFX_TEXTURE_FILTER_NEAREST = 1,
    KGFX_TEXTURE_FILTER_LINEAR = 2,
} KGFXTextureFilter;

typedef enum KGFXSampleMode {
    KGFX_SAMPLE_MODE_CLAMP = 1,
    KGFX_SAMPLE_MODE_REPEAT = 2,
    KGFX_SAMPLE_MODE_CLAMP_BORDER = 3,
    KGFX_SAMPLE_MODE_MIRROR = 4,
} KGFXSampleMode;

typedef enum KGFXSampleBorder {
    KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_FLOAT = 1,
    KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_INT = 2,
    KGFX_SAMPLE_BORDER_OPAQUE_BLACK_FLOAT = 3,
    KGFX_SAMPLE_BORDER_OPAQUE_BLACK_INT = 4,
    KGFX_SAMPLE_BORDER_OPAQUE_WHITE_FLOAT = 5,
    KGFX_SAMPLE_BORDER_OPAQUE_WHITE_INT = 6,
} KGFXSampleBorder;

typedef struct KGFXAdapterDetails {
    KGFXAdapterType type;
    KGFXAdapterVendor vendor;
    
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
    
    KGFXBool supportsGraphics;
    KGFXBool supportsCompute;
    KGFXBool supportsRayTracing;
    
    uint64_t dedicatedMemoryGPU;
    uint64_t dedicatedMemoryCPU;
    uint64_t sharedMemory;
    uint64_t maxMemory;
    
    const char name[64];
} KGFXAdapterDetails;

typedef struct KGFXClearValue {
    KGFXClearValueType type;
    union {
        float f32[4];
        int32_t s32[4];
        uint32_t u32[4];
        struct { float depth; uint32_t stencil; } depthStencil;
    } value;
} KGFXClearValue;

typedef struct KGFXViewport {
    float x, y;
    float width, height;
    float minDepth, maxDepth;
} KGFXViewport;

typedef struct KGFXScissor {
    int32_t x, y;
    uint32_t width, height;
} KGFXScissor;

typedef struct KGFXMacroDefineHLSL {
    const char* macroName;
    const char* value;
} KGFXMacroDefineHLSL;

typedef struct KGFXVertexBindingDesc {
    uint32_t binding;
    uint32_t stride;
    KGFXVertexInputRate inputRate;
} KGFXVertexBindingDesc;

typedef struct KGFXVertexAttributeDesc {
    uint32_t binding;
    uint32_t offset;
    KGFXFormat format;

    /* Vulkan/OpenGL style attribute referencing */
    uint32_t location;

    /* D3D style attribute referencing (defaults to INPUT[location] (like INPUT0 or INPUT2) */
    const char* semanticName; /* (if semanticName is NULL, default to INPUT) */
    uint32_t semanticIndex; /* (if semanticName is NULL, default to location) */
} KGFXVertexAttributeDesc;

typedef struct KGFXVertexInputDesc {
    uint32_t bindingCount;
    KGFXVertexBindingDesc* pBindings;
    uint32_t attributeCount;
    KGFXVertexAttributeDesc* pAttributes;
} KGFXVertexInputDesc;

typedef struct KGFXRenderTargetDesc {
    KGFXFormat format;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
    KGFXBool enableBlending;
    KGFXBlendOp colorBlendOp;
    KGFXBlendFactor srcColorBlendFactor;
    KGFXBlendFactor dstColorBlendFactor;
    KGFXBlendOp alphaBlendOp;
    KGFXBlendFactor srcAlphaBlendFactor;
    KGFXBlendFactor dstAlphaBlendFactor;
    KGFXColorMask colorWriteMask;
    
    KGFXRenderTargetOp loadOp;
    KGFXRenderTargetOp storeOp;
    KGFXRenderTargetOp stencilLoadOp;
    KGFXRenderTargetOp stencilStoreOp;
    KGFXTextureUsageMask usage;
    KGFXTextureLayout finalLayout;
} KGFXRenderTargetDesc;

typedef struct KGFXUniformBinding {
    KGFXUniformBindPointType type;
    union {
        struct { uint32_t binding; } bindingIndex;
        struct { uint32_t reg; uint32_t space; } d3d12Register;
    } bindPoint;
} KGFXUniformBinding;

typedef struct KGFXUniformDesc {
    KGFXResourceType resourceType;
    KGFXUniformBinding binding;
    KGFXShaderStageMask stages;
    
    /* only applies to arrays of uniform; otherwise set to 1 */
    uint32_t arrayLength;
} KGFXUniformDesc;

typedef struct KGFXUniformSignatureDesc {
    uint32_t uniformCount;
    KGFXUniformDesc* pUniforms;
} KGFXUniformSignatureDesc;

typedef struct KGFXStencilOpDesc {
    KGFXStencilOp failOp;
    KGFXStencilOp failDepthOp;
    KGFXStencilOp passOp;
    KGFXCompareOp compareOp;
    uint32_t reference;
} KGFXStencilOpDesc;

typedef struct KGFXDepthStencilDesc {
    KGFXFormat format;
    uint32_t width;
    uint32_t height;
    uint32_t layers;

    KGFXCompareOp compareOp;
    KGFXBool writeDepth;
    KGFXBool testDepth;
    KGFXBool writeStencil;
    KGFXBool testStencil;
    uint8_t readMaskStencil;
    uint8_t writeMaskStencil;
    KGFXStencilOpDesc frontStencilOpDesc;
    KGFXStencilOpDesc backStencilOpDesc;
    float minimumDepth;
    float maximumDepth;
    
    KGFXRenderTargetOp loadOp;
    KGFXRenderTargetOp storeOp;
    KGFXRenderTargetOp stencilLoadOp;
    KGFXRenderTargetOp stencilStoreOp;
    KGFXTextureLayout finalLayout;
} KGFXDepthStencilDesc;

typedef struct KGFXGraphicsPipelineDesc {
    KGFXCullMode cullMode;
    KGFXBool counterClockwiseFrontFaceWinding;
    KGFXFillMode fillMode;
    KGFXTopology topology;
    uint32_t viewportAndScissorCount;
    KGFXVertexInputDesc vertexInputDesc;
    
    uint32_t shaderCount;
    KGFXShader* pShaders;
    
    KGFXLogicOp blendLogicOp;
    uint32_t framebufferWidth;
    uint32_t framebufferHeight;
    uint32_t framebufferLayers;
    uint32_t renderTargetCount;
    KGFXRenderTargetDesc* pRenderTargetDescs;
    KGFXDepthStencilDesc depthStencilDesc;
    KGFXUniformSignatureDesc uniformSignatureDesc;
} KGFXGraphicsPipelineDesc;

typedef struct KGFXSwapchainDesc {
    uint32_t width;
    uint32_t height;
    uint32_t imageCount;
    KGFXFormat format;
    KGFXPresentMode presentMode;
} KGFXSwapchainDesc;

typedef struct KGFXTextureDesc {
    KGFXDimensionType dimensionType;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t layers;
    uint32_t mipMapLevels;
    KGFXFormat format;
    KGFXTextureUsageMask usage;
    KGFXResourceLocation location;
} KGFXTextureDesc;

typedef struct KGFXTextureTransferDesc {
    uint32_t textureX;
    uint32_t textureY;
    uint32_t textureZ;
    uint32_t textureWidth;
    uint32_t textureHeight;
    uint32_t textureDepth;
    uint32_t textureFirstLayer;
    uint32_t textureLayerCount;
} KGFXTextureTransferDesc;

typedef struct KGFXTextureCopyDesc {
    uint32_t srcX;
    uint32_t srcY;
    uint32_t srcZ;
    uint32_t srcFirstLayer;
    uint32_t srcLayerCount;

    uint32_t dstX;
    uint32_t dstY;
    uint32_t dstZ;
    uint32_t dstFirstLayer;
    uint32_t dstLayerCount;

    uint32_t width;
    uint32_t height;
    uint32_t depth;
} KGFXTextureCopyDesc;

typedef struct KGFXSamplerDesc {
    KGFXTextureFilter magFilter;
    KGFXTextureFilter minFilter;
    KGFXTextureFilter mipMapFilter;
    KGFXSampleMode sampleModeU;
    KGFXSampleMode sampleModeV;
    KGFXSampleMode sampleModeW;
    KGFXSampleBorder border;
    float anisotropy;
    float maxLod;
    float minLod;
} KGFXSamplerDesc;

typedef void (*KGFXDebugCallbackPFN)(KGFXInstance instance, KGFXDebugSeverity severity, KGFXDebugSource source, const char* message);

KGFX_API KGFXResult kgfxCreateInstance(KGFXInstanceAPI api, KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance);
KGFX_API void kgfxDestroyInstance(KGFXInstance instance);

KGFX_API KGFXResult kgfxDebugRegisterCallback(KGFXInstance instance, KGFXDebugCallbackPFN callback);

KGFX_API KGFXResult kgfxEnumerateAdapters(KGFXInstance instance, uint32_t adapterID, KGFXAdapterDetails* pAdapterDetails);
KGFX_API KGFXResult kgfxCreateDevice(KGFXInstance instance, uint32_t adapterID, KGFXDevice* pDevice);
KGFX_API void kgfxDestroyDevice(KGFXDevice device);

KGFX_API KGFXResult kgfxCreateBuffer(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer);
KGFX_API void kgfxDestroyBuffer(KGFXBuffer buffer);

KGFX_API KGFXResult kgfxUploadBuffer(KGFXBuffer buffer, const void* pData, uint64_t size);
KGFX_API KGFXResult kgfxDownloadBuffer(KGFXBuffer buffer, void* pData, uint64_t size);

KGFX_API KGFXResult kgfxMapBuffer(KGFXBuffer buffer, void** ppData);
KGFX_API void kgfxUnmapBuffer(KGFXBuffer buffer);

KGFX_API KGFXResult kgfxCreateTexture(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture);
KGFX_API void kgfxDestroyTexture(KGFXTexture texture);

KGFX_API KGFXResult kgfxUploadTexture(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);
KGFX_API KGFXResult kgfxDownloadTexture(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);

KGFX_API KGFXResult kgfxCreateSampler(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler);
KGFX_API void kgfxDestroySampler(KGFXSampler sampler);

KGFX_API KGFXResult kgfxCreateShaderSPIRV(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFX_API KGFXResult kgfxCreateShaderDXBC(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFX_API KGFXResult kgfxCreateShaderGLSL(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader);
KGFX_API KGFXResult kgfxCreateShaderHLSL(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader);
KGFX_API void kgfxDestroyShader(KGFXShader shader);

KGFX_API KGFXResult kgfxCreateGraphicsPipeline(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline);
KGFX_API KGFXResult kgfxResizeGraphicsPipelineRenderTargets(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height);
KGFX_API void kgfxDestroyGraphicsPipeline(KGFXGraphicsPipeline pipeline);

KGFX_API KGFXResult kgfxCreateCommandPool(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool);
KGFX_API void kgfxDestroyCommandPool(KGFXCommandPool commandPool);

KGFX_API KGFXResult kgfxCreateCommandList(KGFXCommandPool commandPool, KGFXCommandList* pCommandList);
KGFX_API void kgfxDestroyCommandList(KGFXCommandList commandList);

KGFX_API void kgfxResetCommandList(KGFXCommandList commandList);
KGFX_API KGFXResult kgfxOpenCommandList(KGFXCommandList commandList, KGFXBool isOneTime);
KGFX_API KGFXResult kgfxCloseCommandList(KGFXCommandList commandList);

/* TODO: explicit sync */
KGFX_API KGFXResult kgfxSubmitCommandList(KGFXCommandList commandList);

KGFX_API void kgfxCmdBindGraphicsPipeline(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline);
KGFX_API void kgfxCmdSetViewportAndScissor(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors);
KGFX_API void kgfxCmdBindRenderTargets(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget);
KGFX_API void kgfxCmdBeginRendering(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue);
KGFX_API void kgfxCmdEndRendering(KGFXCommandList commandList);

KGFX_API void kgfxCmdBindUniformBuffer(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
KGFX_API void kgfxCmdBindStorageBuffer(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
KGFX_API void kgfxCmdBindUniformTexture(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
KGFX_API void kgfxCmdBindStorageTexture(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
KGFX_API void kgfxCmdBindSampler(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler);

KGFX_API void kgfxCmdBindIndexBuffer(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType);
KGFX_API void kgfxCmdBindVertexBuffers(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets);

KGFX_API void kgfxCmdDraw(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex);
KGFX_API void kgfxCmdDrawIndexed(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
KGFX_API void kgfxCmdDrawIndirect(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);
KGFX_API void kgfxCmdDrawIndexedIndirect(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);

KGFX_API void kgfxCmdCopyTexture(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc);

KGFX_API KGFXResult kgfxPresentSwapchain(KGFXSwapchain swapchain);

#ifdef KGFX_WIN32
KGFX_API KGFXResult kgfxCreateSwapchainWin32(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
#endif /* #ifdef KGFX_WIN32 */

#ifdef KGFX_XLIB
KGFX_API KGFXResult kgfxCreateSwapchainXlib(KGFXDevice device, void* display, unsigned long window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
#endif /* #ifdef KGFX_XLIB */

#ifdef KGFX_XCB
KGFX_API KGFXResult kgfxCreateSwapchainXcb(KGFXDevice device, void* connection, uint32_t window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
#endif /* #ifdef KGFX_XCB */

#ifdef KGFX_WAYLAND
KGFX_API KGFXResult kgfxCreateSwapchainWayland(KGFXDevice device, void* display, void* surface, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
#endif /* #ifdef KGFX_WAYLAND */

#ifdef KGFX_COCOA
KGFX_API KGFXResult kgfxCreateSwapchainCocoa(KGFXDevice device, void* nsWindow, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
#endif /* #ifdef KGFX_COCOA */

KGFX_API KGFXResult kgfxRecreateSwapchain(KGFXSwapchain swapchain, uint32_t width, uint32_t height);
KGFX_API void kgfxDestroySwapchain(KGFXSwapchain swapchain);
KGFX_API KGFXTexture kgfxGetSwapchainBackbuffer(KGFXSwapchain swapchain);

#if defined(KGFX_IMPLEMENTATION) || defined(KGFX_VULKAN_IMPLEMENTATION) || defined(KGFX_D3D12_IMPLEMENTATION) || defined(KGFX_METAL_IMPLEMENTATION)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KGFXObject {
    KGFXInstanceAPI api;
    KGFXInstance instance;
} KGFXObject;

#define KGFX_LIST(T_) struct { T_* data; uint32_t length; }
#define KGFX_LIST_INIT(list_, length_, T_) list_.data = (T_*) malloc(sizeof(T_) * length_); if (list_.data == NULL) { abort(); } list_.length = length_
#define KGFX_LIST_FREE(list_) free(list_.data)
#define KGFX_LIST_ZERO(list_) list_.data = NULL; list_.length = 0

#define KGFX_LIST_RESIZE(list_, length_, T_) if (list_.data != NULL) { list_.data = (T_*) realloc(list_.data, sizeof(T_) * length_); } else { list_.data = (T_*) malloc(sizeof(T_) * length_); } if (list_.data == NULL) { abort(); } list_.length = length_
#define KGFX_LIST_APPEND(list_, value_, T_) KGFX_LIST_RESIZE(list_, list_.length + 1, T_); memcpy(list_.data + sizeof(T_) * (list_.length - 1), &value_, sizeof(T_))
#define KGFX_LIST_REMOVE(list_, index_, T_) if (list_.length - 1 > index_) { memcpy(list_.data + sizeof(T_) * index_, list_.data + sizeof(T_) * (index_ + 1), sizeof(T_) * (list_.length - index_ - 1)); } KGFX_LIST_RESIZE(list_, list_.length - 1, T_)

#define KGFX_LOG(f_, inst_, sev_, src_, msg_) if (f_ != NULL) { f_(inst_, sev_, src_, msg_); }

#ifdef KGFX_VULKAN

KGFXResult kgfxCreateInstance_vulkan(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance);
void kgfxDestroyInstance_vulkan(KGFXInstance instance);

KGFXResult kgfxDebugRegisterCallback_vulkan(KGFXInstance instance, KGFXDebugCallbackPFN callback);

KGFXResult kgfxEnumerateAdapters_vulkan(KGFXInstance instance, uint32_t deviceID, KGFXAdapterDetails* pAdapterDetails);
KGFXResult kgfxCreateDevice_vulkan(KGFXInstance instance, uint32_t deviceID, KGFXDevice* pDevice);
void kgfxDestroyDevice_vulkan(KGFXDevice device);

KGFXResult kgfxCreateBuffer_vulkan(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer);
void kgfxDestroyBuffer_vulkan(KGFXBuffer buffer);

KGFXResult kgfxMapBuffer_vulkan(KGFXBuffer buffer, void** ppData);
void kgfxUnmapBuffer_vulkan(KGFXBuffer buffer);

KGFXResult kgfxUploadBuffer_vulkan(KGFXBuffer buffer, const void* pData, uint64_t size);
KGFXResult kgfxDownloadBuffer_vulkan(KGFXBuffer buffer, void* pData, uint64_t size);

KGFXResult kgfxCreateTexture_vulkan(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture);
void kgfxDestroyTexture_vulkan(KGFXTexture texture);

KGFXResult kgfxUploadTexture_vulkan(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);
KGFXResult kgfxDownloadTexture_vulkan(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);

KGFXResult kgfxCreateSampler_vulkan(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler);
void kgfxDestroySampler_vulkan(KGFXSampler sampler);

KGFXResult kgfxCreateShaderSPIRV_vulkan(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderDXBC_vulkan(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderGLSL_vulkan(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader);
KGFXResult kgfxCreateShaderHLSL_vulkan(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader);
void kgfxDestroyShader_vulkan(KGFXShader shader);

KGFXResult kgfxCreateCommandPool_vulkan(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool);
void kgfxDestroyCommandPool_vulkan(KGFXCommandPool commandPool);

KGFXResult kgfxCreateCommandList_vulkan(KGFXCommandPool commandPool, KGFXCommandList* pCommandList);
void kgfxDestroyCommandList_vulkan(KGFXCommandList commandList);
void kgfxResetCommandList_vulkan(KGFXCommandList commandList);

KGFXResult kgfxOpenCommandList_vulkan(KGFXCommandList commandList, KGFXBool isOneTime);
KGFXResult kgfxCloseCommandList_vulkan(KGFXCommandList commandList);
KGFXResult kgfxSubmitCommandList_vulkan(KGFXCommandList commandList);

void kgfxCmdBindGraphicsPipeline_vulkan(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline);
void kgfxCmdSetViewportAndScissor_vulkan(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors);
void kgfxCmdBindRenderTargets_vulkan(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget);
void kgfxCmdBeginRendering_vulkan(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue);
void kgfxCmdEndRendering_vulkan(KGFXCommandList commandList);

void kgfxCmdBindUniformBuffer_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindStorageBuffer_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindUniformTexture_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindStorageTexture_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindSampler_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler);

void kgfxCmdBindIndexBuffer_vulkan(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType);
void kgfxCmdBindVertexBuffers_vulkan(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets);
void kgfxCmdDraw_vulkan(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex);
void kgfxCmdDrawIndexed_vulkan(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void kgfxCmdDrawIndirect_vulkan(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);
void kgfxCmdDrawIndexedIndirect_vulkan(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);

void kgfxCmdCopyTexture_vulkan(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc);

KGFXResult kgfxPresentSwapchain_vulkan(KGFXSwapchain swapchain);

KGFXResult kgfxCreateSwapchainWin32_vulkan(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxCreateSwapchainXlib_vulkan(KGFXDevice device, void* display, unsigned long window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxCreateSwapchainXcb_vulkan(KGFXDevice device, void* connection, uint32_t window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxCreateSwapchainWayland_vulkan(KGFXDevice device, void* display, void* surface, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxCreateSwapchainCocoa_vulkan(KGFXDevice device, void* nsWindow, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxRecreateSwapchain_vulkan(KGFXSwapchain swapchain, uint32_t width, uint32_t height);
void kgfxDestroySwapchain_vulkan(KGFXSwapchain swapchain);
KGFXTexture kgfxGetSwapchainBackbuffer_vulkan(KGFXSwapchain swapchain);

#endif /* #ifdef KGFX_VULKAN */

#ifdef KGFX_D3D11

KGFXResult kgfxCreateInstance_d3d11(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance);
void kgfxDestroyInstance_d3d11(KGFXInstance instance);

KGFXResult kgfxDebugRegisterCallback_d3d11(KGFXInstance instance, KGFXDebugCallbackPFN callback);

KGFXResult kgfxEnumerateAdapters_d3d11(KGFXInstance instance, uint32_t deviceID, KGFXAdapterDetails* pAdapterDetails);
KGFXResult kgfxCreateDevice_d3d11(KGFXInstance instance, uint32_t deviceID, KGFXDevice* pDevice);
void kgfxDestroyDevice_d3d11(KGFXDevice device);

KGFXResult kgfxCreateBuffer_d3d11(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer);
void kgfxDestroyBufferd3d11(KGFXBuffer buffer);

KGFXResult kgfxMapBuffer_d3d11(KGFXBuffer buffer, void** ppData);
void kgfxUnmapBuffer_d3d11(KGFXBuffer buffer);

KGFXResult kgfxUploadBuffer_d3d11(KGFXBuffer buffer, const void* pData, uint64_t size);
KGFXResult kgfxDownloadBuffer_d3d11(KGFXBuffer buffer, void* pData, uint64_t size);

KGFXResult kgfxCreateTexture_d3d11(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture);
void kgfxDestroyTexture_d3d11(KGFXTexture texture);

KGFXResult kgfxUploadTexture_d3d11(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);
KGFXResult kgfxDownloadTexture_d3d11(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);

KGFXResult kgfxCreateSampler_d3d11(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler);
void kgfxDestroySampler_d3d11(KGFXSampler sampler);

KGFXResult kgfxCreateShaderSPIRV_d3d11(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderDXBC_d3d11(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderGLSL_d3d11(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader);
KGFXResult kgfxCreateShaderHLSL_d3d11(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader);
void kgfxDestroyShader_d3d11(KGFXShader shader);

KGFXResult kgfxCreateGraphicsPipeline_d3d11(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline);
void kgfxDestroyGraphicsPipeline_d3d11(KGFXGraphicsPipeline pipeline);

KGFXResult kgfxCreateCommandPool_d3d11(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool);
void kgfxDestroyCommandPool_d3d11(KGFXCommandPool commandPool);

KGFXResult kgfxCreateCommandList_d3d11(KGFXCommandPool commandPool, KGFXCommandList* pCommandList);
void kgfxDestroyCommandList_d3d11(KGFXCommandList commandList);
void kgfxResetCommandList_d3d11(KGFXCommandList commandList);

KGFXResult kgfxOpenCommandList_d3d11(KGFXCommandList commandList, KGFXBool isOneTime);
KGFXResult kgfxCloseCommandList_d3d11(KGFXCommandList commandList);
KGFXResult kgfxSubmitCommandList_d3d11(KGFXCommandList commandList);

void kgfxCmdBindGraphicsPipeline_d3d11(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline);
void kgfxCmdSetViewportAndScissor_d3d11(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors);
void kgfxCmdBindRenderTargets_d3d11(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget);
void kgfxCmdBeginRendering_d3d11(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue);
void kgfxCmdEndRendering_d3d11(KGFXCommandList commandList);

void kgfxCmdBindUniformBuffer_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindStorageBuffer_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindUniformTexture_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindStorageTexture_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindSampler_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler);

void kgfxCmdBindIndexBuffer_d3d11(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType);
void kgfxCmdBindVertexBuffers_d3d11(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets);
void kgfxCmdDraw_d3d11(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex);
void kgfxCmdDrawIndexed_d3d11(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void kgfxCmdDrawIndirect_d3d11(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);
void kgfxCmdDrawIndexedIndirect_d3d11(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);

void kgfxCmdCopyTexture_d3d11(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc);

KGFXResult kgfxPresentSwapchain_d3d11(KGFXSwapchain swapchain);
KGFXResult kgfxCreateSwapchainWin32_d3d11(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxRecreateSwapchain_d3d11(KGFXSwapchain swapchain, uint32_t width, uint32_t height);
void kgfxDestroySwapchain_d3d11(KGFXSwapchain swapchain);
KGFXTexture kgfxGetSwapchainBackbuffer_d3d11(KGFXSwapchain swapchain);

#endif /* #ifdef KGFX_D3D11 */

#ifdef KGFX_D3D12

KGFXResult kgfxCreateInstance_d3d12(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance);
void kgfxDestroyInstance_d3d12(KGFXInstance instance);

KGFXResult kgfxDebugRegisterCallback_d3d12(KGFXInstance instance, KGFXDebugCallbackPFN callback);

KGFXResult kgfxEnumerateAdapters_d3d12(KGFXInstance instance, uint32_t deviceID, KGFXAdapterDetails* pAdapterDetails);
KGFXResult kgfxCreateDevice_d3d12(KGFXInstance instance, uint32_t deviceID, KGFXDevice* pDevice);
void kgfxDestroyDevice_d3d12(KGFXDevice device);

KGFXResult kgfxCreateBuffer_d3d12(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer);
void kgfxDestroyBuffer_d3d12(KGFXBuffer buffer);

KGFXResult kgfxMapBuffer_d3d12(KGFXBuffer buffer, void** ppData);
void kgfxUnmapBuffer_d3d12(KGFXBuffer buffer);

KGFXResult kgfxUploadBuffer_d3d12(KGFXBuffer buffer, const void* pData, uint64_t size);
KGFXResult kgfxDownloadBuffer_d3d12(KGFXBuffer buffer, void* pData, uint64_t size);

KGFXResult kgfxCreateTexture_d3d12(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture);
void kgfxDestroyTexture_d3d12(KGFXTexture texture);

KGFXResult kgfxUploadTexture_d3d12(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);
KGFXResult kgfxDownloadTexture_d3d12(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);

KGFXResult kgfxCreateSampler_d3d12(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler);
void kgfxDestroySampler_d3d12(KGFXSampler sampler);

KGFXResult kgfxCreateShaderSPIRV_d3d12(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderDXBC_d3d12(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderGLSL_d3d12(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader);
KGFXResult kgfxCreateShaderHLSL_d3d12(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader);
void kgfxDestroyShader_d3d12(KGFXShader shader);

KGFXResult kgfxCreateGraphicsPipeline_d3d12(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline);
void kgfxDestroyGraphicsPipeline_d3d12(KGFXGraphicsPipeline pipeline);

KGFXResult kgfxCreateCommandPool_d3d12(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool);
void kgfxDestroyCommandPool_d3d12(KGFXCommandPool commandPool);

KGFXResult kgfxCreateCommandList_d3d12(KGFXCommandPool commandPool, KGFXCommandList* pCommandList);
void kgfxDestroyCommandList_d3d12(KGFXCommandList commandList);
void kgfxResetCommandList_d3d12(KGFXCommandList commandList);

KGFXResult kgfxOpenCommandList_d3d12(KGFXCommandList commandList, KGFXBool isOneTime);
KGFXResult kgfxCloseCommandList_d3d12(KGFXCommandList commandList);
KGFXResult kgfxSubmitCommandList_d3d12(KGFXCommandList commandList);

void kgfxCmdBindGraphicsPipeline_d3d12(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline);
void kgfxCmdSetViewportAndScissor_d3d12(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors);
void kgfxCmdBindRenderTargets_d3d12(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget);
void kgfxCmdBeginRendering_d3d12(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue);
void kgfxCmdEndRendering_d3d12(KGFXCommandList commandList);

void kgfxCmdBindUniformBuffer_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindStorageBuffer_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindUniformTexture_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindStorageTexture_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindSampler_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler);
void kgfxCmdBindIndexBuffer_d3d12(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType);
void kgfxCmdBindVertexBuffers_d3d12(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets);
void kgfxCmdDraw(KGFXCommandList _d3d12commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex);
void kgfxCmdDrawIndexed_d3d12(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void kgfxCmdDrawIndirect_d3d12(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);
void kgfxCmdDrawIndexedIndirect_d3d12(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);

void kgfxCmdCopyTexture_d3d12(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc);

KGFXResult kgfxPresentSwapchain_d3d12(KGFXSwapchain swapchain);
KGFXResult kgfxCreateSwapchainWin32_d3d12(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxRecreateSwapchain_d3d12(KGFXSwapchain swapchain, uint32_t width, uint32_t height);
void kgfxDestroySwapchain_d3d12(KGFXSwapchain swapchain);
KGFXTexture kgfxGetSwapchainBackbuffer_d3d12(KGFXSwapchain swapchain);

#endif /* #ifdef KGFX_D3D12 */

#ifdef KGFX_METAL

KGFXResult kgfxCreateInstance_metal(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance);
void kgfxDestroyInstance_metal(KGFXInstance instance);

KGFXResult kgfxDebugRegisterCallback_metal(KGFXInstance instance, KGFXDebugCallbackPFN callback);

KGFXResult kgfxEnumerateAdapters_metal(KGFXInstance instance, uint32_t deviceID, KGFXAdapterDetails* pAdapterDetails);
KGFXResult kgfxCreateDevice_metal(KGFXInstance instance, uint32_t deviceID, KGFXDevice* pDevice);
void kgfxDestroyDevice_metal(KGFXDevice device);

KGFXResult kgfxCreateBuffer_metal(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer);
void kgfxDestroyBuffer_metal(KGFXBuffer buffer);

KGFXResult kgfxMapBuffer_metal(KGFXBuffer buffer, void** ppData);
void kgfxUnmapBuffer_metal(KGFXBuffer buffer);

KGFXResult kgfxUploadBuffer_metal(KGFXBuffer buffer, const void* pData, uint64_t size);
KGFXResult kgfxDownloadBuffer_metal(KGFXBuffer buffer, void* pData, uint64_t size);

KGFXResult kgfxCreateTexture_metal(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture);
void kgfxDestroyTexture_metal(KGFXTexture texture);

KGFXResult kgfxUploadTexture_metal(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);
KGFXResult kgfxDownloadTexture_metal(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc);

KGFXResult kgfxCreateSampler_metal(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler);
void kgfxDestroySampler_metal(KGFXSampler sampler);

KGFXResult kgfxCreateShaderSPIRV_metal(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderDXBC_metal(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader);
KGFXResult kgfxCreateShaderGLSL_metal(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader);
KGFXResult kgfxCreateShaderHLSL_metal(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader);
void kgfxDestroyShader_metal(KGFXShader shader);

KGFXResult kgfxCreateGraphicsPipeline_metal(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline);
void kgfxDestroyGraphicsPipeline_metal(KGFXGraphicsPipeline pipeline);

KGFXResult kgfxResizeGraphicsPipelineRenderTargets_metal(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height);

KGFXResult kgfxCreateCommandPool_metal(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool);
void kgfxDestroyCommandPool_metal(KGFXCommandPool commandPool);

KGFXResult kgfxCreateCommandList_metal(KGFXCommandPool commandPool, KGFXCommandList* pCommandList);
void kgfxDestroyCommandList_metal(KGFXCommandList commandList);
void kgfxResetCommandList_metal(KGFXCommandList commandList);

KGFXResult kgfxOpenCommandList_metal(KGFXCommandList commandList, KGFXBool isOneTime);
KGFXResult kgfxCloseCommandList_metal(KGFXCommandList commandList);
KGFXResult kgfxSubmitCommandList_metal(KGFXCommandList commandList);

void kgfxCmdBindGraphicsPipeline_metal(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline);
void kgfxCmdSetViewportAndScissor_metal(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors);
void kgfxCmdBindRenderTargets_metal(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget);
void kgfxCmdBeginRendering_metal(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue);
void kgfxCmdEndRendering_metal(KGFXCommandList commandList);

void kgfxCmdBindUniformBuffer_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindStorageBuffer_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size);
void kgfxCmdBindUniformTexture_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindStorageTexture_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture);
void kgfxCmdBindSampler_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler);
void kgfxCmdBindIndexBuffer_metal(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType);
void kgfxCmdBindVertexBuffers_metal(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets);
void kgfxCmdDraw_metal(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex);
void kgfxCmdDrawIndexed_metal(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void kgfxCmdDrawIndirect_metal(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);
void kgfxCmdDrawIndexedIndirect_metal(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride);

void kgfxCmdCopyTexture_metal(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc);

KGFXResult kgfxPresentSwapchain_metal(KGFXSwapchain swapchain);
KGFXResult kgfxCreateSwapchainCocoa_metal(KGFXDevice device, void* nsWindow, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain);
KGFXResult kgfxRecreateSwapchain_metal(KGFXSwapchain swapchain, uint32_t width, uint32_t height);
void kgfxDestroySwapchain_metal(KGFXSwapchain swapchain);
KGFXTexture kgfxGetSwapchainBackbuffer_metal(KGFXSwapchain swapchain);

#endif /* #ifdef KGFX_METAL */

#endif /* #if defined(KGFX_IMPLEMENTATION) || defined(KGFX_VULKAN_IMPLEMENTATION) || defined(KGFX_D3D12_IMPLEMENTATION) || defined(KGFX_METAL_IMPLEMENTATION) */

#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#ifdef KGFX_VULKAN_IMPLEMENTATION

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
#include <stdarg.h>

typedef struct KGFXInstance_Vulkan_t {
    KGFXObject obj;
    
    KGFXInstanceCreateFlagBits flags;
    KGFXDebugCallbackPFN debugCallback;
    KGFX_LIST(struct KGFX_Vulkan_Surface_t) surfaces;
    
    struct {
        VkDebugUtilsMessengerEXT debugUtilsMessenger;
        struct {
            PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
            PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
        } functions;
        
        VkInstance instance;        
        KGFX_LIST(VkPhysicalDevice) physicalDevices;

        uint32_t currentDeviceID;
        uint32_t currentSurfaceID;
    } vk;
    
#ifdef __APPLE__
    struct {
        KGFXBool macOSSurface;
        KGFXBool metalSurface;
    } apple;
#endif /* #ifdef __APPLE__ */
} KGFXInstance_Vulkan_t;

/* Pass in KGFXInstance_Vulkan_t* */
#define KGFX_INSTVK_LOG(pinst_, ...) KGFX_LOG(pinst_->debugCallback, &pinst->obj, __VA_ARGS__)

/* Pass in KGFXInstance */
#define KGFX_INST_LOG(inst_, ...) KGFX_INSTVK_LOG((KGFXInstance_Vulkan_t*) inst_, __VA_ARGS__)

typedef struct KGFXDevice_Vulkan_t {
    KGFXObject obj;
    KGFXBool isCPU;
    
    struct {
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        
        VkQueue graphicsQueue;
        VkQueue computeQueue;
        VkQueue transferQueue;
        VkQueue genericQueue;
        
        uint32_t graphicsQueueIndex;
        uint32_t computeQueueIndex;
        uint32_t transferQueueIndex;
        uint32_t genericQueueIndex;

        uint32_t currentShaderID;
        uint32_t currentRenderPassID;
        uint32_t currentFramebufferID;
        uint32_t currentDescriptorSetLayoutID;
        uint32_t currentPipelineLayoutID;
        uint32_t currentGraphicsPipelineID;
        uint32_t currentCommandPoolID;
        uint32_t currentSwapchainID;
        uint32_t currentBufferID;
        uint32_t currentTextureID;
        uint32_t currentSamplerID;
    } vk;
} KGFXDevice_Vulkan_t;

typedef struct KGFXBuffer_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;
    
    KGFXBool mapped;
    void* pMappedData;
    uint64_t size;
    KGFXBufferUsageMask usage;
    KGFXResourceLocation location;
    
    struct {
        VkBuffer buffer;
        VkDeviceMemory memory;
    } vk;
} KGFXBuffer_Vulkan_t;

typedef struct KGFXTexture_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;

    KGFXFormat format;
    KGFXTextureUsageMask usage;
    KGFXResourceLocation location;
    KGFXDimensionType dimensionType;
    
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t layers;

    struct {
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory memory;
        VkFormat format;
        VkImageLayout layout;
    } vk;

    KGFXBool isSwapchainTexture;
} KGFXTexture_Vulkan_t;

typedef struct KGFXSampler_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;
    
    struct {
        VkSampler sampler;
    } vk;
} KGFXSampler_Vulkan_t;

typedef struct KGFXSwapchainTexture_Vulkan_t {
    KGFXTexture_Vulkan_t base;
    struct KGFXSwapchain_Vulkan_t* swapchain;
} KGFXSwapchainTexture_Vulkan_t;

typedef struct KGFXSwapchain_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;

    KGFXFormat format;
    KGFXPresentMode presentMode;
    KGFXBool minimized;
    KGFXBool acquired;

    KGFXSwapchainTexture_Vulkan_t backbuffer;
    KGFXSwapchainTexture_Vulkan_t* currentBackbuffer;
    KGFX_LIST(KGFXSwapchainTexture_Vulkan_t) internalBackbuffers;
    
    struct {
        VkSwapchainKHR swapchain;
        VkSurfaceKHR surface;
        KGFX_LIST(VkImage) images;
        KGFX_LIST(VkImageView) imageViews;

        VkSemaphore imageAvailableSemaphore;
        uint32_t imageIndex;
    } vk;
} KGFXSwapchain_Vulkan_t;

typedef struct KGFXShader_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;
    
    const char* entry;
    KGFXShaderStage stage;
    
    struct {
        VkShaderModule shader;
    } vk;
} KGFXShader_Vulkan_t;

typedef struct KGFXGraphicsPipeline_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;

    uint32_t framebufferWidth;
    uint32_t framebufferHeight;
    uint32_t framebufferLayers;
    
    KGFX_LIST(KGFXRenderTargetDesc) renderTargetDescs;
    KGFXDepthStencilDesc depthStencilDesc;
    
    struct {
        VkDescriptorSetLayout descLayout;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
        VkFramebuffer framebuffer;
    } vk;
} KGFXGraphicsPipeline_Vulkan_t;

typedef struct KGFXCommandPool_Vulkan_t {
    KGFXObject obj;
    KGFXDevice device;
    KGFXQueueType queueType;
    
    uint32_t maxCommandLists;
    uint32_t currentListCount;
    
    struct {
        VkQueue queue;
        uint32_t queueFamilyIndex;
        VkCommandPool commandPool;
    } vk;
} KGFXCommandPool_Vulkan_t;

typedef struct KGFXCommandList_Vulkan_t {
    KGFXObject obj;
    KGFXCommandPool commandPool;
    
    struct {
        VkCommandBuffer commandBuffer;
        VkFence inUseFence;
    } vk;

    struct {
        KGFX_LIST(KGFXTexture_Vulkan_t*) renderTargets;
        KGFXTexture_Vulkan_t* depthStencilTarget;
        KGFXGraphicsPipeline_Vulkan_t* graphicsPipeline;
    } bound;
} KGFXCommandList_Vulkan_t;

typedef enum KGFX_Vulkan_WindowType_t {
    KGFX_INTERNAL_VULKAN_WINDOW_TYPE_WIN32 = 1,
    KGFX_INTERNAL_VULKAN_WINDOW_TYPE_XLIB = 2,
    KGFX_INTERNAL_VULKAN_WINDOW_TYPE_XCB = 3,
    KGFX_INTERNAL_VULKAN_WINDOW_TYPE_WAYLAND = 4,
    KGFX_INTERNAL_VULKAN_WINDOW_TYPE_COCOA = 5,
} KGFX_Vulkan_WindowType_t;

typedef struct KGFX_Vulkan_Surface_t {
    KGFX_Vulkan_WindowType_t type;
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
} KGFX_Vulkan_Surface_t;

typedef struct KGFX_Vulkan_ExtensionRequest_t {
    const char name[VK_MAX_EXTENSION_NAME_SIZE];
    KGFXBool isNecessary;
} KGFX_Vulkan_ExtensionRequest_t;

typedef KGFX_LIST(KGFX_Vulkan_ExtensionRequest_t) KGFX_Vulkan_ExtensionRequestList_t;
typedef KGFX_LIST(const char*) KGFX_Vulkan_ActualExtensionList_t;

typedef KGFX_Vulkan_ExtensionRequest_t KGFX_Vulkan_LayerRequest_t;
typedef KGFX_LIST(KGFX_Vulkan_LayerRequest_t) KGFX_Vulkan_LayerRequestList_t;
typedef KGFX_LIST(const char*) KGFX_Vulkan_ActualLayerList_t;

#define KGFX_ADD_EXTENSION(list_, index_, name_, necessary_) \
    memcpy((void*) list_.data[index_].name, name_, strlen(name_) + 1); \
    list_.data[index_++].isNecessary = necessary_;
#define KGFX_ADD_LAYER(list_, index_, name_, necessary_) KGFX_ADD_EXTENSION(list_, index_, name_, necessary_)

KGFXResult kgfx_vulkan_getExtensions(VkPhysicalDevice vkPhysicalDevice, KGFX_Vulkan_ExtensionRequestList_t requestedExtensionList, KGFX_Vulkan_ActualExtensionList_t* pList) {
    KGFX_LIST(VkExtensionProperties) vkExtensionProperties = { NULL, 0 };
    KGFX_Vulkan_ActualExtensionList_t actualExtensions = { NULL, 0 };
    uint32_t actualExtensionIndex = 0;
    if (requestedExtensionList.length != 0) {
        KGFX_LIST_INIT(actualExtensions, requestedExtensionList.length, const char*);
        if (vkPhysicalDevice == NULL) {
            vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionProperties.length, NULL);
        } else {
            vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, NULL, &vkExtensionProperties.length, NULL);
        }
        
        KGFX_LIST_INIT(vkExtensionProperties, vkExtensionProperties.length, VkExtensionProperties);
        if (vkPhysicalDevice == NULL) {
            vkEnumerateInstanceExtensionProperties(NULL, &vkExtensionProperties.length, vkExtensionProperties.data);
        } else {
            vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, NULL, &vkExtensionProperties.length, vkExtensionProperties.data);
        }
        
        for (uint32_t j = 0; j < requestedExtensionList.length; ++j) {
            KGFXBool added = KGFX_FALSE;
            for (uint32_t i = 0; i < vkExtensionProperties.length; ++i) {
                if (strcmp(vkExtensionProperties.data[i].extensionName, requestedExtensionList.data[j].name) == 0) {
                    actualExtensions.data[actualExtensionIndex++] = requestedExtensionList.data[j].name;
                    added = KGFX_TRUE;
                    break;
                }
            }
            
            if (!added && requestedExtensionList.data[j].isNecessary) {
                KGFX_LIST_FREE(vkExtensionProperties);
                KGFX_LIST_FREE(actualExtensions);
                return KGFX_RESULT_ERROR_UNSUPPORTED;
            }
        }
    }
    
    if (actualExtensionIndex == 0) {
        KGFX_LIST_FREE(actualExtensions);
        actualExtensions.data = NULL;
        actualExtensions.length = 0;
    } else {
        KGFX_LIST_RESIZE(actualExtensions, actualExtensionIndex, const char*);
    }
    KGFX_LIST_FREE(vkExtensionProperties);
    *pList = actualExtensions;
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfx_vulkan_getLayers(VkPhysicalDevice vkPhysicalDevice, KGFX_Vulkan_LayerRequestList_t requestedLayerList, KGFX_Vulkan_ActualLayerList_t* pList) {
    KGFX_LIST(VkLayerProperties) vkLayerProperties = { NULL, 0 };
    KGFX_Vulkan_ActualLayerList_t actualLayers = { NULL, 0 };
    uint32_t actualLayerIndex = 0;
    if (requestedLayerList.length != 0) {
        KGFX_LIST_INIT(actualLayers, requestedLayerList.length, const char*);
        if (vkPhysicalDevice == NULL) {
            VkResult result = vkEnumerateInstanceLayerProperties(&vkLayerProperties.length, NULL);
        } else {
            vkEnumerateDeviceLayerProperties(vkPhysicalDevice, &vkLayerProperties.length, NULL);
        }
        
        KGFX_LIST_INIT(vkLayerProperties, vkLayerProperties.length, VkLayerProperties);
        if (vkPhysicalDevice == NULL) {
            vkEnumerateInstanceLayerProperties(&vkLayerProperties.length, vkLayerProperties.data);
        } else {
            vkEnumerateDeviceLayerProperties(vkPhysicalDevice, &vkLayerProperties.length, vkLayerProperties.data);
        }
        
        for (uint32_t j = 0; j < requestedLayerList.length; ++j) {
            KGFXBool added = KGFX_FALSE;
            for (uint32_t i = 0; i < vkLayerProperties.length; ++i) {
                if (strcmp(vkLayerProperties.data[i].layerName, requestedLayerList.data[j].name) == 0) {
                    actualLayers.data[actualLayerIndex++] = requestedLayerList.data[j].name;
                    added = KGFX_TRUE;
                    break;
                }
            }
            
            if (!added && requestedLayerList.data[j].isNecessary) {
                KGFX_LIST_FREE(vkLayerProperties);
                KGFX_LIST_FREE(actualLayers);
                return KGFX_RESULT_ERROR_UNSUPPORTED;
            }
        }
    }
    
    if (actualLayerIndex == 0) {
        KGFX_LIST_FREE(actualLayers);
        actualLayers.data = NULL;
        actualLayers.length = 0;
    } else {
        KGFX_LIST_RESIZE(actualLayers, actualLayerIndex, const char*);
    }
    KGFX_LIST_FREE(vkLayerProperties);
    *pList = actualLayers;
    return KGFX_RESULT_SUCCESS;
}

KGFXBool kgfx_vulkan_vkFormat(KGFXFormat format, VkFormat* pFormat) {
    switch (format) {
        case KGFX_FORMAT_R8_UINT:
            *pFormat = VK_FORMAT_R8_UINT;
            break;
        case KGFX_FORMAT_R8_SINT:
            *pFormat = VK_FORMAT_R8_SINT;
            break;
        case KGFX_FORMAT_R8_UNORM:
            *pFormat = VK_FORMAT_R8_UNORM;
            break;
        case KGFX_FORMAT_R8_SNORM:
            *pFormat = VK_FORMAT_R8_SNORM;
            break;
        case KGFX_FORMAT_R8G8_UINT:
            *pFormat = VK_FORMAT_R8G8_UINT;
            break;
        case KGFX_FORMAT_R8G8_SINT:
            *pFormat = VK_FORMAT_R8G8_SINT;
            break;
        case KGFX_FORMAT_R8G8_UNORM:
            *pFormat = VK_FORMAT_R8G8_UNORM;
            break;
        case KGFX_FORMAT_R8G8_SNORM:
            *pFormat = VK_FORMAT_R8G8_SNORM;
            break;
        case KGFX_FORMAT_R8G8B8A8_UINT:
            *pFormat = VK_FORMAT_R8G8B8A8_UINT;
            break;
        case KGFX_FORMAT_R8G8B8A8_SINT:
            *pFormat = VK_FORMAT_R8G8B8A8_SINT;
            break;
        case KGFX_FORMAT_R8G8B8A8_UNORM:
            *pFormat = VK_FORMAT_R8G8B8A8_UNORM;
            break;
        case KGFX_FORMAT_R8G8B8A8_SNORM:
            *pFormat = VK_FORMAT_R8G8B8A8_SNORM;
            break;
        case KGFX_FORMAT_R8G8B8A8_SRGB:
            *pFormat = VK_FORMAT_R8G8B8A8_SRGB;
            break;
        case KGFX_FORMAT_B8G8R8A8_SRGB:
            *pFormat = VK_FORMAT_B8G8R8A8_SRGB;
            break;
        case KGFX_FORMAT_R16_UINT:
            *pFormat = VK_FORMAT_R16_UINT;
            break;
        case KGFX_FORMAT_R16_SINT:
            *pFormat = VK_FORMAT_R16_SINT;
            break;
        case KGFX_FORMAT_R16_UNORM:
            *pFormat = VK_FORMAT_R16_UNORM;
            break;
        case KGFX_FORMAT_R16_SNORM:
            *pFormat = VK_FORMAT_R16_SNORM;
            break;
        case KGFX_FORMAT_R16_FLOAT:
            *pFormat = VK_FORMAT_R16_SFLOAT;
            break;
        case KGFX_FORMAT_R16G16_UINT:
            *pFormat = VK_FORMAT_R16G16_UINT;
            break;
        case KGFX_FORMAT_R16G16_SINT:
            *pFormat = VK_FORMAT_R16G16_SINT;
            break;
        case KGFX_FORMAT_R16G16_UNORM:
            *pFormat = VK_FORMAT_R16G16_UNORM;
            break;
        case KGFX_FORMAT_R16G16_SNORM:
            *pFormat = VK_FORMAT_R16G16_SNORM;
            break;
        case KGFX_FORMAT_R16G16_FLOAT:
            *pFormat = VK_FORMAT_R16G16_SFLOAT;
            break;
        case KGFX_FORMAT_R16G16B16A16_UINT:
            *pFormat = VK_FORMAT_R16G16B16A16_UINT;
            break;
        case KGFX_FORMAT_R16G16B16A16_SINT:
            *pFormat = VK_FORMAT_R16G16B16A16_SINT;
            break;
        case KGFX_FORMAT_R16G16B16A16_UNORM:
            *pFormat = VK_FORMAT_R16G16B16A16_UNORM;
            break;
        case KGFX_FORMAT_R16G16B16A16_SNORM:
            *pFormat = VK_FORMAT_R16G16B16A16_SNORM;
            break;
        case KGFX_FORMAT_R16G16B16A16_FLOAT:
            *pFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
            break;
        case KGFX_FORMAT_R32_UINT:
            *pFormat = VK_FORMAT_R32_UINT;
            break;
        case KGFX_FORMAT_R32_SINT:
            *pFormat = VK_FORMAT_R32_SINT;
            break;
        case KGFX_FORMAT_R32_FLOAT:
            *pFormat = VK_FORMAT_R32_SFLOAT;
            break;
        case KGFX_FORMAT_R32G32_UINT:
            *pFormat = VK_FORMAT_R32G32_UINT;
            break;
        case KGFX_FORMAT_R32G32_SINT:
            *pFormat = VK_FORMAT_R32G32_SINT;
            break;
        case KGFX_FORMAT_R32G32_FLOAT:
            *pFormat = VK_FORMAT_R32G32_SFLOAT;
            break;
        case KGFX_FORMAT_R32G32B32A32_UINT:
            *pFormat = VK_FORMAT_R32G32B32A32_UINT;
            break;
        case KGFX_FORMAT_R32G32B32A32_SINT:
            *pFormat = VK_FORMAT_R32G32B32A32_SINT;
            break;
        case KGFX_FORMAT_R32G32B32A32_FLOAT:
            *pFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
            break;
        case KGFX_FORMAT_D32_FLOAT:
            *pFormat = VK_FORMAT_D32_SFLOAT;
            break;
        case KGFX_FORMAT_D24_UNORM_S8_UINT:
            *pFormat = VK_FORMAT_D24_UNORM_S8_UINT;
            break;
        case KGFX_FORMAT_D16_UNORM:
            *pFormat = VK_FORMAT_D16_UNORM;
            break;
        case KGFX_FORMAT_B8G8R8A8_UNORM:
            *pFormat = VK_FORMAT_B8G8R8A8_UNORM;
            break;
        case KGFX_FORMAT_R32G32B32_UINT:
            *pFormat = VK_FORMAT_R32G32B32_UINT;
            break;
        case KGFX_FORMAT_R32G32B32_SINT:
            *pFormat = VK_FORMAT_R32G32B32_SINT;
            break;
        case KGFX_FORMAT_R32G32B32_FLOAT:
            *pFormat = VK_FORMAT_R32G32B32_SFLOAT;
            break;
        case KGFX_FORMAT_UNKNOWN:
            *pFormat = VK_FORMAT_UNDEFINED;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkPresentMode(KGFXPresentMode presentMode, VkPresentModeKHR* pPresentMode) {
    switch (presentMode) {
        case KGFX_PRESENT_MODE_MAILBOX_VK:
            *pPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        case KGFX_PRESENT_MODE_IMMEDIATE_VK_MTL:
            *pPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            break;
        case KGFX_PRESENT_MODE_VSYNC:
            *pPresentMode = VK_PRESENT_MODE_FIFO_KHR;
            break;
        case KGFX_PRESENT_MODE_NO_SYNC:
            *pPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkShaderStage(KGFXShaderStageMask stages, VkShaderStageFlagBits* pStages) {
    VkShaderStageFlags flags = 0;
    if (stages & KGFX_SHADER_STAGE_VERTEX) {
        flags |= VK_SHADER_STAGE_VERTEX_BIT;
    }
    
    if (stages & KGFX_SHADER_STAGE_TESS_CONTROL) {
        flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    
    if (stages & KGFX_SHADER_STAGE_TESS_EVAL) {
        flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    
    if (stages & KGFX_SHADER_STAGE_GEOMETRY) {
        flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    
    if (stages & KGFX_SHADER_STAGE_FRAGMENT) {
        flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    
    if (stages & KGFX_SHADER_STAGE_COMPUTE) {
        flags |= VK_SHADER_STAGE_COMPUTE_BIT;
    }
    
    if (flags == 0) {
        return KGFX_FALSE;
    }
    
    *pStages = (VkShaderStageFlagBits) flags;
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkVertexInputRate(KGFXVertexInputRate inputRate, VkVertexInputRate* pInputRate) {
    switch (inputRate) {
        case KGFX_VERTEX_INPUT_RATE_PER_INSTANCE:
            *pInputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
            break;
        case KGFX_VERTEX_INPUT_RATE_PER_VERTEX:
            *pInputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkPrimitiveTopology(KGFXTopology topology, VkPrimitiveTopology* pTopology) {
    switch (topology) {
        case KGFX_TOPOLOGY_TRIANGLES:
            *pTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            break;
        case KGFX_TOPOLOGY_LINES:
            *pTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            break;
        case KGFX_TOPOLOGY_POINTS:
            *pTopology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            break;
        case KGFX_TOPOLOGY_TRIANGLE_STRIP:
            *pTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            break;
        case KGFX_TOPOLOGY_LINE_STRIP:
            *pTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            break;
        case KGFX_TOPOLOGY_PATCHES:
            *pTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkPolygonMode(KGFXFillMode fillMode, VkPolygonMode* pPolyMode) {
    switch (fillMode) {
        case KGFX_FILL_MODE_FILL:
            *pPolyMode = VK_POLYGON_MODE_FILL;
            break;
        case KGFX_FILL_MODE_WIRE:
            *pPolyMode = VK_POLYGON_MODE_LINE;
            break;
        case KGFX_FILL_MODE_POINT:
            *pPolyMode = VK_POLYGON_MODE_POINT;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkCullMode(KGFXCullMode cullMode, VkCullModeFlags* pCullMode) {
    switch (cullMode) {
        case KGFX_CULL_MODE_NONE:
            *pCullMode = VK_CULL_MODE_NONE;
            break;
        case KGFX_CULL_MODE_FRONT:
            *pCullMode = VK_CULL_MODE_FRONT_BIT;
            break;
        case KGFX_CULL_MODE_BACK:
            *pCullMode = VK_CULL_MODE_BACK_BIT;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkCompareOp(KGFXCompareOp op, VkCompareOp* pOp) {
    switch (op) {
        case KGFX_COMPARE_OP_NEVER:
            *pOp = VK_COMPARE_OP_NEVER;
            break;
        case KGFX_COMPARE_OP_LESS:
            *pOp = VK_COMPARE_OP_LESS;
            break;
        case KGFX_COMPARE_OP_EQUAL:
            *pOp = VK_COMPARE_OP_EQUAL;
            break;
        case KGFX_COMPARE_OP_GREATER:
            *pOp = VK_COMPARE_OP_GREATER;
            break;
        case KGFX_COMPARE_OP_LEQUAL:
            *pOp = VK_COMPARE_OP_LESS_OR_EQUAL;
            break;
        case KGFX_COMPARE_OP_NEQUAL:
            *pOp = VK_COMPARE_OP_NOT_EQUAL;
            break;
        case KGFX_COMPARE_OP_GEQUAL:
            *pOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
            break;
        case KGFX_COMPARE_OP_ALWAYS:
            *pOp = VK_COMPARE_OP_ALWAYS;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkStencilOp(KGFXStencilOp op, VkStencilOp* pOp) {
    switch (op) {
        case KGFX_STENCIL_OP_KEEP:
            *pOp = VK_STENCIL_OP_KEEP;
            break;
        case KGFX_STENCIL_OP_ZERO:
            *pOp = VK_STENCIL_OP_ZERO;
            break;
        case KGFX_STENCIL_OP_REPLACE:
            *pOp = VK_STENCIL_OP_REPLACE;
            break;
        case KGFX_STENCIL_OP_INCREM_CLAMP:
            *pOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
            break;
        case KGFX_STENCIL_OP_DECREM_CLAMP:
            *pOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
            break;
        case KGFX_STENCIL_OP_INVERT:
            *pOp = VK_STENCIL_OP_INVERT;
            break;
        case KGFX_STENCIL_OP_INCREM:
            *pOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
            break;
        case KGFX_STENCIL_OP_DECREM:
            *pOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkIndexType(KGFXIndexType type, VkIndexType* pType) {
    switch (type) {
        case KGFX_INDEX_TYPE_U16:
            *pType = VK_INDEX_TYPE_UINT16;
            break;
        case KGFX_INDEX_TYPE_U32:
            *pType = VK_INDEX_TYPE_UINT32;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkImageLayout(KGFXTextureLayout layout, VkImageLayout* pLayout) {
    switch (layout) {
        case KGFX_TEXTURE_LAYOUT_UNDEFINED:
            *pLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            break;
        case KGFX_TEXTURE_LAYOUT_RENDER_TARGET:
            *pLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            break;
        case KGFX_TEXTURE_LAYOUT_DEPTH_STENCIL_TARGET:
        case KGFX_TEXTURE_LAYOUT_STENCIL_TARGET:
        case KGFX_TEXTURE_LAYOUT_DEPTH_TARGET:
            *pLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            break;
        case KGFX_TEXTURE_LAYOUT_DEPTH_STENCIL_READ:
        case KGFX_TEXTURE_LAYOUT_STENCIL_READ:
        case KGFX_TEXTURE_LAYOUT_DEPTH_READ:
            *pLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            break;
        case KGFX_TEXTURE_LAYOUT_PRESENT:
            *pLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            break;
        case KGFX_TEXTURE_LAYOUT_SHADER_READ:
            *pLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            break;
        case KGFX_TEXTURE_LAYOUT_SHADER_WRITE:
            *pLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
            break;
        case KGFX_TEXTURE_LAYOUT_TRANSFER_SRC:
            *pLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            break;
        case KGFX_TEXTURE_LAYOUT_TRANSFER_DST:
            *pLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkBorderColor(KGFXSampleBorder border, VkBorderColor* pBorder) {
    switch (border) {
        case KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_FLOAT:
            *pBorder = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
            break;
        case KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_INT:
            *pBorder = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
            break;
        case KGFX_SAMPLE_BORDER_OPAQUE_BLACK_FLOAT:
            *pBorder = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
            break;
        case KGFX_SAMPLE_BORDER_OPAQUE_BLACK_INT:
            *pBorder = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            break;
        case KGFX_SAMPLE_BORDER_OPAQUE_WHITE_FLOAT:
            *pBorder = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
            break;
        case KGFX_SAMPLE_BORDER_OPAQUE_WHITE_INT:
            *pBorder = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkFilter(KGFXTextureFilter filter, VkFilter* pFilter) {
    switch (filter) {
        case KGFX_TEXTURE_FILTER_NEAREST:
            *pFilter = VK_FILTER_NEAREST;
            break;
        case KGFX_TEXTURE_FILTER_LINEAR:
            *pFilter = VK_FILTER_LINEAR;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkSamplerMipmapMode(KGFXTextureFilter filter, VkSamplerMipmapMode* pMode) {
    switch (filter) {
        case KGFX_TEXTURE_FILTER_NEAREST:
            *pMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
            break;
        case KGFX_TEXTURE_FILTER_LINEAR:
            *pMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkSamplerAddressMode(KGFXSampleMode mode, VkSamplerAddressMode* pMode) {
    switch (mode) {
        case KGFX_SAMPLE_MODE_CLAMP:
            *pMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            break;
        case KGFX_SAMPLE_MODE_REPEAT:
            *pMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            break;
        case KGFX_SAMPLE_MODE_CLAMP_BORDER:
            *pMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            break;
        case KGFX_SAMPLE_MODE_MIRROR:
            *pMode = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkBlendOp(KGFXBlendOp op, VkBlendOp* pOp) {
    switch (op) {
        case KGFX_BLEND_OP_ADD:
            *pOp = VK_BLEND_OP_ADD;
            break;
        case KGFX_BLEND_OP_SUBTRACT:
            *pOp = VK_BLEND_OP_SUBTRACT;
            break;
        case KGFX_BLEND_OP_REVERSE_SUBTRACT:
            *pOp = VK_BLEND_OP_REVERSE_SUBTRACT;
            break;
        case KGFX_BLEND_OP_MIN:
            *pOp = VK_BLEND_OP_MIN;
            break;
        case KGFX_BLEND_OP_MAX:
            *pOp = VK_BLEND_OP_MAX;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkBlendFactor(KGFXBlendFactor factor, VkBlendFactor* pFactor) {
    switch (factor) {
        case KGFX_BLEND_FACTOR_ZERO:
            *pFactor = VK_BLEND_FACTOR_ZERO;
            break;
        case KGFX_BLEND_FACTOR_ONE:
            *pFactor = VK_BLEND_FACTOR_ONE;
            break;
        case KGFX_BLEND_FACTOR_SRC_COLOR:
            *pFactor = VK_BLEND_FACTOR_SRC_COLOR;
            break;
        case KGFX_BLEND_FACTOR_DST_COLOR:
            *pFactor = VK_BLEND_FACTOR_DST_COLOR;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_SRC_COLOR:
            *pFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_DST_COLOR:
            *pFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            break;
        case KGFX_BLEND_FACTOR_SRC_ALPHA:
            *pFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            break;
        case KGFX_BLEND_FACTOR_DST_ALPHA:
            *pFactor = VK_BLEND_FACTOR_DST_ALPHA;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_SRC_ALPHA:
            *pFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_DST_ALPHA:
            *pFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkColorComponentFlags(KGFXColorMask colorMask, VkColorComponentFlags* pFlags) {
    VkColorComponentFlags flags = 0;
    if (colorMask & KGFX_COLOR_MASK_FLAG_RED) {
        flags |= VK_COLOR_COMPONENT_R_BIT;
    }
    
    if (colorMask & KGFX_COLOR_MASK_FLAG_GREEN) {
        flags |= VK_COLOR_COMPONENT_G_BIT;
    }
    
    if (colorMask & KGFX_COLOR_MASK_FLAG_BLUE) {
        flags |= VK_COLOR_COMPONENT_B_BIT;
    }
    
    if (colorMask & KGFX_COLOR_MASK_FLAG_ALPHA) {
        flags |= VK_COLOR_COMPONENT_A_BIT;
    }
    
    *pFlags = flags;
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkAttachmentLoadOp(KGFXRenderTargetOp op, VkAttachmentLoadOp* pOp) {
    switch (op) {
        case KGFX_RENDER_TARGET_OP_DONT_CARE:
            *pOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            break;
        case KGFX_RENDER_TARGET_OP_LOAD:
            *pOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            break;
        case KGFX_RENDER_TARGET_OP_CLEAR:
            *pOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkAttachmentStoreOp(KGFXRenderTargetOp op, VkAttachmentStoreOp* pOp) {
    switch (op) {
        case KGFX_RENDER_TARGET_OP_DONT_CARE:
            *pOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            break;
        case KGFX_RENDER_TARGET_OP_STORE:
            *pOp = VK_ATTACHMENT_STORE_OP_STORE;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkLogicOp(KGFXLogicOp op, VkLogicOp* pOp) {
    switch (op) {
        case KGFX_LOGIC_OP_NO_OP:
            *pOp = VK_LOGIC_OP_NO_OP;
            break;
        case KGFX_LOGIC_OP_CLEAR:
            *pOp = VK_LOGIC_OP_CLEAR;
            break;
        case KGFX_LOGIC_OP_COPY:
            *pOp = VK_LOGIC_OP_COPY;
            break;
        case KGFX_LOGIC_OP_INVERT:
            *pOp = VK_LOGIC_OP_INVERT;
            break;
        case KGFX_LOGIC_OP_AND:
            *pOp = VK_LOGIC_OP_AND;
            break;
        case KGFX_LOGIC_OP_OR:
            *pOp = VK_LOGIC_OP_OR;
            break;
        case KGFX_LOGIC_OP_XOR:
            *pOp = VK_LOGIC_OP_XOR;
            break;
        case KGFX_LOGIC_OP_EQUALS:
            *pOp = VK_LOGIC_OP_EQUIVALENT;
            break;
        case KGFX_LOGIC_OP_NAND:
            *pOp = VK_LOGIC_OP_NAND;
            break;
        case KGFX_LOGIC_OP_SET:
            *pOp = VK_LOGIC_OP_SET;
            break;
        case KGFX_LOGIC_OP_AND_INVERTED:
            *pOp = VK_LOGIC_OP_AND_INVERTED;
            break;
        case KGFX_LOGIC_OP_OR_INVERTED:
            *pOp = VK_LOGIC_OP_OR_INVERTED;
            break;
        case KGFX_LOGIC_OP_COPY_INVERTED:
            *pOp = VK_LOGIC_OP_COPY_INVERTED;
            break;
        case KGFX_LOGIC_OP_AND_REVERSE:
            *pOp = VK_LOGIC_OP_AND_REVERSE;
            break;
        case KGFX_LOGIC_OP_OR_REVERSE:
            *pOp = VK_LOGIC_OP_OR_REVERSE;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkDescriptorType(KGFXResourceType type, VkDescriptorType* pType) {
    switch (type) {
        case KGFX_RESOURCE_TYPE_UNIFORM_BUFFER:
            *pType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            break;
        case KGFX_RESOURCE_TYPE_STORAGE_BUFFER:
            *pType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            break;
        case KGFX_RESOURCE_TYPE_UNIFORM_TEXTURE:
            *pType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            break;
        case KGFX_RESOURCE_TYPE_STORAGE_TEXTURE:
            *pType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            break;
        case KGFX_RESOURCE_TYPE_SAMPLER:
            *pType = VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkBufferUsageFlags(KGFXBufferUsageMask usage, VkBufferUsageFlags* pUsage) {
    VkBufferUsageFlags flags = 0;
    if (usage & KGFX_BUFFER_USAGE_VERTEX_BUFFER) {
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }

    if (usage & KGFX_BUFFER_USAGE_INDEX_BUFFER) {
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }

    if (usage & KGFX_BUFFER_USAGE_UNIFORM_BUFFER) {
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }

    if (usage & KGFX_BUFFER_USAGE_STORAGE_BUFFER) {
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }

    if (usage & KGFX_BUFFER_USAGE_INDIRECT_BUFFER) {
        flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    }

    if (usage & KGFX_BUFFER_USAGE_TRANSFER_SRC) {
        flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    }

    if (usage & KGFX_BUFFER_USAGE_TRANSFER_DST) {
        flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    if (flags == 0) {
        return KGFX_FALSE;
    }

    *pUsage = flags;
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkMemoryPropertyFlags(KGFXResourceLocation location, VkMemoryPropertyFlags* pFlags) {
    switch (location) {
        case KGFX_RESOURCE_LOCATION_DEVICE:
            *pFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        case KGFX_RESOURCE_LOCATION_HOST:
            *pFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        case KGFX_RESOURCE_LOCATION_SHARED:
            *pFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkImageType(KGFXDimensionType type, VkImageType* pType) {
    switch (type) {
        case KGFX_DIMENSION_TYPE_1D:
            *pType = VK_IMAGE_TYPE_1D;
            break;
        case KGFX_DIMENSION_TYPE_2D:
            *pType = VK_IMAGE_TYPE_2D;
            break;
        case KGFX_DIMENSION_TYPE_3D:
            *pType = VK_IMAGE_TYPE_3D;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkImageViewType(KGFXDimensionType type, VkImageViewType* pType) {
    switch (type) {
        case KGFX_DIMENSION_TYPE_1D:
            *pType = VK_IMAGE_VIEW_TYPE_1D;
            break;
        case KGFX_DIMENSION_TYPE_2D:
            *pType = VK_IMAGE_VIEW_TYPE_2D;
            break;
        case KGFX_DIMENSION_TYPE_3D:
            *pType = VK_IMAGE_VIEW_TYPE_3D;
            break;
        case KGFX_DIMENSION_TYPE_CUBE:
            *pType = VK_IMAGE_VIEW_TYPE_CUBE;
            break;
        case KGFX_DIMENSION_TYPE_1D_ARRAY:
            *pType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
            break;
        case KGFX_DIMENSION_TYPE_2D_ARRAY:
            *pType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            break;
        case KGFX_DIMENSION_TYPE_CUBE_ARRAY:
            *pType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
            break;
        default:
            return KGFX_FALSE;
    }
    
    return KGFX_TRUE;
}

KGFXBool kgfx_vulkan_vkImageUsageFlags(KGFXTextureUsageMask usage, VkImageUsageFlags* pUsage) {
    VkImageUsageFlags flags = 0;
    if (usage & KGFX_TEXTURE_USAGE_UNIFORM_TEXTURE) {
        flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    
    if (usage & KGFX_TEXTURE_USAGE_STORAGE_TEXTURE) {
        flags |= VK_IMAGE_USAGE_STORAGE_BIT;
    }
    
    if (usage & KGFX_TEXTURE_USAGE_RENDER_TARGET) {
        flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    
    if (usage & KGFX_TEXTURE_USAGE_DEPTH_STENCIL_TARGET) {
        flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    
    if (usage & KGFX_TEXTURE_USAGE_TRANSFER_SRC) {
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    
    if (usage & KGFX_TEXTURE_USAGE_TRANSFER_DST) {
        flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    
    *pUsage = flags;
    return KGFX_TRUE;
}

VkBool32 kgfx_vulkan_debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) pUserData;
    if (vulkanInstance->debugCallback == NULL) {
        return VK_FALSE;
    }
    
    KGFXDebugSeverity severity = KGFX_DEBUG_SEVERITY_INFO;
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        severity = KGFX_DEBUG_SEVERITY_ERROR;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        severity = KGFX_DEBUG_SEVERITY_WARNING;
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        severity = KGFX_DEBUG_SEVERITY_VERBOSE;
    }
    
    vulkanInstance->debugCallback(&vulkanInstance->obj, severity, KGFX_DEBUG_SOURCE_UNDERLYING_API, pCallbackData->pMessage);
    return VK_FALSE;
}

void kgfx_vulkan_debugNameObject(KGFXDevice_Vulkan_t* vulkanDevice, VkObjectType type, void* handle, const char* name) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    if (vulkanInstance->vk.functions.vkSetDebugUtilsObjectNameEXT != NULL) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = type;
        nameInfo.objectHandle = (uint64_t) handle;
        nameInfo.pObjectName = name;

        vulkanInstance->vk.functions.vkSetDebugUtilsObjectNameEXT(vulkanDevice->vk.device, &nameInfo);
    }
}

void kgfx_vulkan_debugNameObjectWithID(KGFXDevice_Vulkan_t* vulkanDevice, VkObjectType type, void* handle, const char* name, uint32_t id) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    if (vulkanInstance->vk.functions.vkSetDebugUtilsObjectNameEXT != NULL) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = type;
        nameInfo.objectHandle = (uint64_t) handle;

        char buffer[256];
        snprintf(buffer, 256, "%s %u", name, id);
        nameInfo.pObjectName = buffer;
        vulkanInstance->vk.functions.vkSetDebugUtilsObjectNameEXT(vulkanDevice->vk.device, &nameInfo);
    }
}

void kgfx_vulkan_debugNameObjectPrintf(KGFXDevice_Vulkan_t* vulkanDevice, VkObjectType type, void* handle, const char* fmt, ...) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    if (vulkanInstance->vk.functions.vkSetDebugUtilsObjectNameEXT != NULL && !vulkanDevice->isCPU && type != VK_OBJECT_TYPE_SURFACE_KHR) {
        VkDebugUtilsObjectNameInfoEXT nameInfo;
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = NULL;
        nameInfo.objectType = type;
        nameInfo.objectHandle = (uint64_t) handle;

        char buffer[256];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, 256, fmt, args);
        nameInfo.pObjectName = buffer;
        vulkanInstance->vk.functions.vkSetDebugUtilsObjectNameEXT(vulkanDevice->vk.device, &nameInfo);
        va_end(args);
    }
}

KGFXResult kgfx_vulkan_acquireSwapchainImage(KGFXSwapchain_Vulkan_t* vulkanSwapchain) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;

    VkResult result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, VK_NULL_HANDLE, &vulkanSwapchain->vk.imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    vulkanSwapchain->currentBackbuffer = &vulkanSwapchain->internalBackbuffers.data[vulkanSwapchain->vk.imageIndex];
    return KGFX_RESULT_SUCCESS;
}

void kgfx_vulkan_transitionTexture(KGFXCommandList_Vulkan_t* vulkanCommandList, KGFXTexture_Vulkan_t* vulkanTexture, VkImageLayout layout, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanTexture->device;
    VkCommandPool vkPool = NULL;
    VkCommandBuffer vkBuffer = NULL;
    if (vulkanCommandList == NULL) {
        VkCommandPoolCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.queueFamilyIndex = vulkanDevice->vk.genericQueueIndex;

        vkCreateCommandPool(vulkanDevice->vk.device, &createInfo, NULL, &vkPool);
        kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_POOL, vkPool, "KGFX Temporary Texture Transition Command Pool");

        VkCommandBufferAllocateInfo allocInfo;
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.commandPool = vkPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(vulkanDevice->vk.device, &allocInfo, &vkBuffer);
        kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_POOL, vkPool, "KGFX Temporary Texture Transition Command Buffer");

        VkCommandBufferBeginInfo beginInfo;
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = NULL;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        beginInfo.pInheritanceInfo = NULL;

        vkBeginCommandBuffer(vkBuffer, &beginInfo);
    } else {
        vkBuffer = vulkanCommandList->vk.commandBuffer;
    }

    VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    barrier.srcAccessMask = srcAccess;
    barrier.dstAccessMask = dstAccess;
    barrier.oldLayout = vulkanTexture->vk.layout;
    barrier.newLayout = layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = vulkanTexture->vk.image;
    barrier.subresourceRange.aspectMask = 0;
    if (vulkanTexture->format == KGFX_FORMAT_D16_UNORM || vulkanTexture->format == KGFX_FORMAT_D24_UNORM_S8_UINT || vulkanTexture->format == KGFX_FORMAT_D32_FLOAT) {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
    } else {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (vulkanTexture->format == KGFX_FORMAT_D24_UNORM_S8_UINT) {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = vulkanTexture->layers;

    vkCmdPipelineBarrier(vkBuffer, srcStage, dstStage, 0, 0, NULL, 0, NULL, 1, &barrier);
    vulkanTexture->vk.layout = layout;

    if (vulkanCommandList == NULL) {
        vkEndCommandBuffer(vkBuffer);

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = NULL;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = NULL;
        submitInfo.pWaitDstStageMask = NULL;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkBuffer;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = NULL;

        vkQueueSubmit(vulkanDevice->vk.genericQueue, 1, &submitInfo, NULL);
        vkQueueWaitIdle(vulkanDevice->vk.genericQueue);

        vkFreeCommandBuffers(vulkanDevice->vk.device, vkPool, 1, &vkBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkPool, NULL);
    }
}

KGFXBool kgfx_vulkan_memoryType(VkPhysicalDevice vkPhysicalDevice, uint32_t memoryTypeBits, VkMemoryPropertyFlags props, uint32_t* pMemoryTypeIndex) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);
    
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if ((memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & props) == props) {
            *pMemoryTypeIndex = i;
            return KGFX_TRUE;
        }
    }
    
    return KGFX_FALSE;
}

KGFXBool kgfx_vulkan_recreateSwapchain(KGFXDevice_Vulkan_t* vulkanDevice, KGFXSwapchain_Vulkan_t* vulkanSwapchain, VkSurfaceKHR vkSurface, KGFXSwapchain_Vulkan_t** pNewVulkanSwapchain) {
    VkSurfaceCapabilitiesKHR caps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanDevice->vk.physicalDevice, vkSurface, &caps);
    if (caps.currentExtent.width == 0 || caps.currentExtent.height == 0) {
        vulkanSwapchain->minimized = KGFX_TRUE;
        *pNewVulkanSwapchain = vulkanSwapchain;
        return KGFX_TRUE;
    }

    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    KGFX_Vulkan_Surface_t* vulkanSurface = NULL;
    for (uint32_t i = 0; i < vulkanInstance->surfaces.length; ++i) {
        if (vulkanInstance->surfaces.data[i].vk.surface == vkSurface) {
            vulkanSurface = &vulkanInstance->surfaces.data[i];
            break;
        }
    }

    for (uint32_t i = 0; i < vulkanSwapchain->vk.imageViews.length; ++i) {
        vkDestroyImageView(vulkanDevice->vk.device, vulkanSwapchain->vk.imageViews.data[i], NULL);
    }
    vkDestroySwapchainKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, NULL);

    KGFXFormat oldFormat = vulkanSwapchain->format;
    KGFXPresentMode oldPresentMode = vulkanSwapchain->presentMode;

    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.surface = vulkanSurface->vk.surface;
    createInfo.minImageCount = vulkanSwapchain->vk.imageViews.length;
    if (!kgfx_vulkan_vkFormat(oldFormat, &createInfo.imageFormat)) {
        vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
        KGFX_LIST_FREE(vulkanSwapchain->vk.images);
        KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
        free(vulkanSwapchain);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent.width = caps.currentExtent.width;
    createInfo.imageExtent.height = caps.currentExtent.height;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    if (!kgfx_vulkan_vkPresentMode(oldPresentMode, &createInfo.presentMode)) {
        vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
        KGFX_LIST_FREE(vulkanSwapchain->vk.images);
        KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
        free(vulkanSwapchain);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    if (createInfo.presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
        vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
        KGFX_LIST_FREE(vulkanSwapchain->vk.images);
        KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
        free(vulkanSwapchain);
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = NULL;

    VkSwapchainKHR vkSwapchain;
    VkResult result = vkCreateSwapchainKHR(vulkanDevice->vk.device, &createInfo, NULL, &vkSwapchain);
    if (result != VK_SUCCESS) {
        vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
        KGFX_LIST_FREE(vulkanSwapchain->vk.images);
        KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
        free(vulkanSwapchain);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SWAPCHAIN_KHR, vkSwapchain, "KGFX Swapchain %u", vulkanDevice->vk.currentSwapchainID++);

    if (vkGetSwapchainImagesKHR(vulkanDevice->vk.device, vkSwapchain, &vulkanSwapchain->vk.images.length, vulkanSwapchain->vk.images.data) != VK_SUCCESS) {
        KGFX_LIST_FREE(vulkanSwapchain->vk.images);
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
        KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
        free(vulkanSwapchain);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    for (uint32_t i = 0; i < vulkanSwapchain->vk.images.length; ++i) {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = NULL;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = vulkanSwapchain->vk.images.data[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = createInfo.imageFormat;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(vulkanDevice->vk.device, &imageViewCreateInfo, NULL, &vulkanSwapchain->vk.imageViews.data[i]) != VK_SUCCESS) {
            for (uint32_t j = 0; j < i; ++j) {
                vkDestroyImageView(vulkanDevice->vk.device, vulkanSwapchain->vk.imageViews.data[j], NULL);
            }
            KGFX_LIST_FREE(vulkanSwapchain->vk.images);
            KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
            vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
            vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
            free(vulkanSwapchain);
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_IMAGE_VIEW, vulkanSwapchain->vk.imageViews.data[i], "KGFX Swapchain Image View %u for Swapchain %u", i, vulkanDevice->vk.currentSwapchainID - 1);
        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_IMAGE, vulkanSwapchain->vk.images.data[i], "KGFX Swapchain Image %u for Swapchain %u", i, vulkanDevice->vk.currentSwapchainID - 1);
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;

    if (vulkanSwapchain == NULL) {
        for (uint32_t i = 0; i < vulkanSwapchain->vk.images.length; ++i) {
            vkDestroyImageView(vulkanDevice->vk.device, vulkanSwapchain->vk.imageViews.data[i], NULL);
        }
        KGFX_LIST_FREE(vulkanSwapchain->vk.images);
        KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
        free(vulkanSwapchain);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    vulkanSwapchain->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanSwapchain->obj.instance = vulkanDevice->obj.instance;
    vulkanSwapchain->device = &vulkanDevice->obj;
    vulkanSwapchain->format = oldFormat;
    vulkanSwapchain->presentMode = oldPresentMode;
    vulkanSwapchain->backbuffer.base.obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanSwapchain->backbuffer.base.obj.instance = vulkanDevice->obj.instance;
    vulkanSwapchain->backbuffer.base.device = &vulkanDevice->obj;
    vulkanSwapchain->backbuffer.base.format = oldFormat;
    vulkanSwapchain->backbuffer.base.width = createInfo.imageExtent.width;
    vulkanSwapchain->backbuffer.base.height = createInfo.imageExtent.height;
    vulkanSwapchain->backbuffer.base.layers = 1;
    vulkanSwapchain->backbuffer.base.vk.image = VK_NULL_HANDLE;
    vulkanSwapchain->backbuffer.base.vk.imageView = VK_NULL_HANDLE;
    vulkanSwapchain->backbuffer.base.vk.memory = VK_NULL_HANDLE;
    vulkanSwapchain->backbuffer.base.vk.format = createInfo.imageFormat;
    vulkanSwapchain->backbuffer.base.vk.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    vulkanSwapchain->backbuffer.base.isSwapchainTexture = KGFX_TRUE;
    vulkanSwapchain->backbuffer.swapchain = vulkanSwapchain;
    for (uint32_t i = 0; i < vulkanSwapchain->internalBackbuffers.length; ++i) {
        vulkanSwapchain->internalBackbuffers.data[i].base.obj.api = KGFX_INSTANCE_API_VULKAN;
        vulkanSwapchain->internalBackbuffers.data[i].base.obj.instance = vulkanDevice->obj.instance;
        vulkanSwapchain->internalBackbuffers.data[i].base.device = &vulkanDevice->obj;
        vulkanSwapchain->internalBackbuffers.data[i].base.format = oldFormat;
        vulkanSwapchain->internalBackbuffers.data[i].base.width = createInfo.imageExtent.width;
        vulkanSwapchain->internalBackbuffers.data[i].base.height = createInfo.imageExtent.height;
        vulkanSwapchain->internalBackbuffers.data[i].base.layers = 1;
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.image = vulkanSwapchain->vk.images.data[i];
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.imageView = vulkanSwapchain->vk.imageViews.data[i];
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.memory = VK_NULL_HANDLE;
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.format = createInfo.imageFormat;
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.layout = VK_IMAGE_LAYOUT_UNDEFINED;
        vulkanSwapchain->internalBackbuffers.data[i].base.isSwapchainTexture = KGFX_TRUE;
        vulkanSwapchain->internalBackbuffers.data[i].swapchain = vulkanSwapchain;
    }

    vulkanSwapchain->vk.surface = vulkanSurface->vk.surface;
    vulkanSwapchain->vk.swapchain = vkSwapchain;
    vulkanSwapchain->vk.imageIndex = 0;

    *pNewVulkanSwapchain = (KGFXSwapchain_Vulkan_t*) vulkanSwapchain;
    return KGFX_TRUE;
}

KGFXResult kgfxCreateInstance_vulkan(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance) {
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "KGFX";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "KGFX";
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
    
    KGFX_Vulkan_ExtensionRequestList_t requestedExtensionList = { NULL, 0 };
#ifdef __APPLE__
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    ++requestedExtensionList.length;
#endif /* #ifdef __APPLE__ */
    
    /* surface extensions */
    if (flags & KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL) {
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
    
    if (flags & KGFX_INSTANCE_CREATE_FLAG_DEBUG) {
        ++requestedExtensionList.length;
    }
    
    if (requestedExtensionList.length != 0) {
        KGFX_LIST_INIT(requestedExtensionList, requestedExtensionList.length, KGFX_Vulkan_ExtensionRequest_t);
    }
    
    uint32_t requestedExtensionIndex = 0;
    
    /* surface extensions */
    if (flags & KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL) {
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_SURFACE_EXTENSION_NAME, KGFX_TRUE);
        
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, KGFX_TRUE);
#elif defined(__linux__) || defined(__gnu_linux__) || defined(linux) || defined(__linux)
        /* X11/Xlib, Xcb, Wayland */
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_XLIB_SURFACE_EXTENSION_NAME, KGFX_FALSE);
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_XCB_SURFACE_EXTENSION_NAME, KGFX_FALSE);
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, KGFX_FALSE);
#elif defined(__APPLE__)
        /* VK_MVK_macos_surface and VK_EXT_metal_surface */
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_MVK_MACOS_SURFACE_EXTENSION_NAME, KGFX_TRUE);
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_EXT_METAL_SURFACE_EXTENSION_NAME, KGFX_TRUE);
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */
    }
    
    if (flags & KGFX_INSTANCE_CREATE_FLAG_DEBUG) {
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, KGFX_TRUE);
    }
    
#ifdef __APPLE__
    KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, KGFX_TRUE);
#endif /* #ifdef __APPLE__ */
    
    KGFX_Vulkan_ActualExtensionList_t actualExtensions = { NULL, 0 };
    KGFXResult presult = kgfx_vulkan_getExtensions(NULL, requestedExtensionList, &actualExtensions);
    if (presult != KGFX_RESULT_SUCCESS) {
        KGFX_LIST_FREE(requestedExtensionList);
        return presult;
    }
    
    createInfo.enabledExtensionCount = actualExtensions.length;
    createInfo.ppEnabledExtensionNames = actualExtensions.data;
    
    KGFX_Vulkan_LayerRequestList_t requestedLayerList = { NULL, 0 };
    
    if (flags & KGFX_INSTANCE_CREATE_FLAG_VALIDATION) {
        ++requestedLayerList.length;
    }
    
    if (requestedLayerList.length != 0) {
        KGFX_LIST_INIT(requestedLayerList, requestedLayerList.length, KGFX_Vulkan_LayerRequest_t);
    }
    
    uint32_t requestedLayerIndex = 0;
    if (flags & KGFX_INSTANCE_CREATE_FLAG_VALIDATION) {
        KGFX_ADD_LAYER(requestedLayerList, requestedLayerIndex, "VK_LAYER_KHRONOS_validation", KGFX_TRUE);
    }
    
    KGFX_Vulkan_ActualLayerList_t actualLayers = { NULL, 0 };
    presult = kgfx_vulkan_getLayers(NULL, requestedLayerList, &actualLayers);
    if (presult != KGFX_RESULT_SUCCESS) {
        KGFX_LIST_FREE(requestedLayerList);
        KGFX_LIST_FREE(requestedExtensionList);
        KGFX_LIST_FREE(actualExtensions);
        return presult;
    }
    
    createInfo.enabledLayerCount = actualLayers.length;
    createInfo.ppEnabledLayerNames = actualLayers.data;
    
    VkInstance vkInstance;
    VkResult result = vkCreateInstance(&createInfo, NULL, &vkInstance);
    
    KGFX_LIST_FREE(requestedLayerList);
    KGFX_LIST_FREE(requestedExtensionList);
    KGFX_LIST_FREE(actualLayers);
    KGFX_LIST_FREE(actualExtensions);
    
    if (result != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    KGFXInstance_Vulkan_t* instance = (KGFXInstance_Vulkan_t*) malloc(sizeof(KGFXInstance_Vulkan_t));
    if (instance == NULL) {
        vkDestroyInstance(vkInstance, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    memset(instance, 0, sizeof(KGFXInstance_Vulkan_t));
    
    instance->obj.api = KGFX_INSTANCE_API_VULKAN;
    instance->flags = flags;
    instance->debugCallback = NULL;
    instance->vk.instance = vkInstance;
    instance->vk.debugUtilsMessenger = NULL;
    if (flags & KGFX_INSTANCE_CREATE_FLAG_DEBUG) {
        VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo;
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        dbgCreateInfo.pNext = NULL;
        dbgCreateInfo.flags = 0;
        dbgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        dbgCreateInfo.pfnUserCallback = kgfx_vulkan_debugUtilsMessengerCallback;
        dbgCreateInfo.pUserData = (void*) instance;
        
        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance->vk.instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != NULL) {
            func(instance->vk.instance, &dbgCreateInfo, NULL, &instance->vk.debugUtilsMessenger);
        }
        
        PFN_vkSetDebugUtilsObjectNameEXT setDbgName = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetInstanceProcAddr(instance->vk.instance, "vkSetDebugUtilsObjectNameEXT");
        instance->vk.functions.vkSetDebugUtilsObjectNameEXT = setDbgName;
    }
    
    instance->vk.functions.vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR) vkGetInstanceProcAddr(instance->vk.instance, "vkCmdPushDescriptorSetKHR");
    if (instance->vk.functions.vkCmdPushDescriptorSetKHR == NULL) {
        vkDestroyInstance(vkInstance, NULL);
        free(instance);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }
    
    uint32_t adapterCount = 0;
    vkEnumeratePhysicalDevices(instance->vk.instance, &adapterCount, NULL);
    
    KGFX_LIST_INIT(instance->vk.physicalDevices, adapterCount, VkPhysicalDevice);
    vkEnumeratePhysicalDevices(instance->vk.instance, &adapterCount, instance->vk.physicalDevices.data);
    
    KGFX_LIST_ZERO(instance->surfaces);
    
    *pInstance = &instance->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyInstance_vulkan(KGFXInstance instance) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) instance;
    if (vulkanInstance->vk.debugUtilsMessenger != NULL) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkanInstance->vk.instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != NULL) {
            func(vulkanInstance->vk.instance, vulkanInstance->vk.debugUtilsMessenger, NULL);
        }
    }
    
    vkDestroyInstance(vulkanInstance->vk.instance, NULL);
    KGFX_LIST_FREE(vulkanInstance->vk.physicalDevices);
    free(vulkanInstance);
}

KGFXResult kgfxDebugRegisterCallback_vulkan(KGFXInstance instance, KGFXDebugCallbackPFN callback) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) instance;
    if ((vulkanInstance->flags & KGFX_INSTANCE_CREATE_FLAG_DEBUG) == 0) {
        return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
    
    vulkanInstance->debugCallback = callback;
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxEnumerateAdapters_vulkan(KGFXInstance instance, uint32_t adapterID, KGFXAdapterDetails* pAdapterDetails) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) instance;
    if (adapterID >= vulkanInstance->vk.physicalDevices.length) {
        return KGFX_RESULT_ENUMERATION_DONE;
    }
    
    VkPhysicalDevice vkPhysicalDevice = vulkanInstance->vk.physicalDevices.data[adapterID];

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
    
    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            pAdapterDetails->type = KGFX_ADAPTER_TYPE_CPU;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            pAdapterDetails->type = KGFX_ADAPTER_TYPE_DEDICATED_GPU;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            pAdapterDetails->type = KGFX_ADAPTER_TYPE_INTEGRATED_GPU;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            pAdapterDetails->type = KGFX_ADAPTER_TYPE_VIRTUAL;
            break;
        default:
            pAdapterDetails->type = KGFX_ADAPTER_TYPE_UNKNOWN;
            break;
    }
    
    switch (properties.vendorID) {
        case 0x1002:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_AMD;
            break;
        case 0x8086:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_INTEL;
            break;
        case 0x10DE:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_NVIDIA;
            break;
        case 0x13B5:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_ARM;
            break;
        case 0x5143:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_QUALCOMM;
            break;
        case 0x1010:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_IMGTEC;
            break;
        case 0x106B:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_APPLE;
            pAdapterDetails->type = KGFX_ADAPTER_TYPE_UMA_GPU;
            break;
        case 0x10005:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_MESA;
            break;
        default:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_UNKNOWN;
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
    return KGFX_RESULT_ENUMERATION_IN_PROGRESS;
}

KGFXResult kgfxCreateDevice_vulkan(KGFXInstance instance, uint32_t adapterID, KGFXDevice* pDevice) {
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) instance;
    if (adapterID >= vulkanInstance->vk.physicalDevices.length) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    VkPhysicalDevice vkPhysicalDevice = vulkanInstance->vk.physicalDevices.data[adapterID];

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &props);

    VkPhysicalDeviceImagelessFramebufferFeatures imagelessFramebufferFeatures;
    imagelessFramebufferFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES;
    imagelessFramebufferFeatures.pNext = NULL;
    imagelessFramebufferFeatures.imagelessFramebuffer = VK_TRUE;

    VkPhysicalDeviceFeatures2 features2;
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &imagelessFramebufferFeatures;
    vkGetPhysicalDeviceFeatures2(vkPhysicalDevice, &features2);
    
    KGFX_LIST(VkQueueFamilyProperties) vkQueueFamilies = { NULL, 0 };
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &vkQueueFamilies.length, NULL);
    
    KGFX_LIST_INIT(vkQueueFamilies, vkQueueFamilies.length, VkQueueFamilyProperties);
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
    
    KGFX_LIST_FREE(vkQueueFamilies);
    
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
    createInfo.pNext = &imagelessFramebufferFeatures;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = uniqueQueueFamilyCount;
    createInfo.pQueueCreateInfos = uniqueQueueFamilyIndices;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = NULL;
    createInfo.pEnabledFeatures = &features2.features;
    
    KGFX_Vulkan_ExtensionRequestList_t requestedExtensionList = { NULL, 0 };
    
#ifdef __APPLE__
    ++requestedExtensionList.length;
#endif /* #ifdef __APPLE__ */
    
    ++requestedExtensionList.length;
    
    if (vulkanInstance->flags & KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL) {
        ++requestedExtensionList.length;
    }
    
    if (requestedExtensionList.length != 0) {
        KGFX_LIST_INIT(requestedExtensionList, requestedExtensionList.length, KGFX_Vulkan_ExtensionRequest_t);
    }
    
    uint32_t requestedExtensionIndex = 0;
#ifdef __APPLE__
    KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, "VK_KHR_portability_subset", KGFX_TRUE);
#endif /* #ifdef __APPLE__ */

    KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, "VK_KHR_push_descriptor", KGFX_TRUE);
    
    if (vulkanInstance->flags & KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL) {
        KGFX_ADD_EXTENSION(requestedExtensionList, requestedExtensionIndex, VK_KHR_SWAPCHAIN_EXTENSION_NAME, KGFX_TRUE);
    }
    
    KGFX_Vulkan_ActualExtensionList_t actualExtensions = { NULL, 0 };
    KGFXResult presult = kgfx_vulkan_getExtensions(vkPhysicalDevice, requestedExtensionList, &actualExtensions);
    if (presult != KGFX_RESULT_SUCCESS) {
        return presult;
    }
    
    createInfo.enabledExtensionCount = actualExtensions.length;
    createInfo.ppEnabledExtensionNames = actualExtensions.data;
    
    KGFX_Vulkan_LayerRequestList_t requestedLayerList = { NULL, 0 };
    if (requestedLayerList.length != 0) {
        KGFX_LIST_INIT(requestedLayerList, requestedLayerList.length, KGFX_Vulkan_LayerRequest_t);
    }
    
    uint32_t requestedLayerIndex = 0;
    KGFX_Vulkan_ActualLayerList_t actualLayers = { NULL, 0 };
    presult = kgfx_vulkan_getLayers(NULL, requestedLayerList, &actualLayers);
    if (presult != KGFX_RESULT_SUCCESS) {
        KGFX_LIST_FREE(requestedLayerList);
        KGFX_LIST_FREE(requestedExtensionList);
        KGFX_LIST_FREE(actualExtensions);
        return presult;
    }
    
    createInfo.enabledLayerCount = actualLayers.length;
    createInfo.ppEnabledLayerNames = actualLayers.data;
    
    KGFXDevice_Vulkan_t* device = (KGFXDevice_Vulkan_t*) malloc(sizeof(KGFXDevice_Vulkan_t));
    memset(device, 0, sizeof(KGFXDevice_Vulkan_t));

    VkResult result = vkCreateDevice(vkPhysicalDevice, &createInfo, NULL, &device->vk.device);
    
    KGFX_LIST_FREE(requestedLayerList);
    KGFX_LIST_FREE(requestedExtensionList);
    KGFX_LIST_FREE(actualLayers);
    KGFX_LIST_FREE(actualExtensions);
    if (result != VK_SUCCESS) {
        free(device);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    device->obj.api = KGFX_INSTANCE_API_VULKAN;
    device->obj.instance = instance;
    device->isCPU = (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU);
    device->vk.physicalDevice = vkPhysicalDevice;
    device->vk.graphicsQueueIndex = gfxIndexBest;
    device->vk.computeQueueIndex = cmpIndexBest;
    device->vk.transferQueueIndex = trnIndexBest;
    device->vk.genericQueueIndex = genericIndexBest;
    vkGetDeviceQueue(device->vk.device, gfxIndexBest, 0, &device->vk.graphicsQueue);
    vkGetDeviceQueue(device->vk.device, cmpIndexBest, 0, &device->vk.computeQueue);
    vkGetDeviceQueue(device->vk.device, trnIndexBest, 0, &device->vk.transferQueue);
    vkGetDeviceQueue(device->vk.device, genericIndexBest, 0, &device->vk.genericQueue);

    VkQueue uniqueQueues[4];
    uint32_t uniqueQueueCount = 0;
    uniqueQueues[uniqueQueueCount++] = device->vk.graphicsQueue;
    if (device->vk.computeQueueIndex != device->vk.graphicsQueueIndex) {
        uniqueQueues[uniqueQueueCount++] = device->vk.computeQueue;
    }

    if (device->vk.transferQueueIndex != device->vk.graphicsQueueIndex && device->vk.transferQueueIndex != device->vk.computeQueueIndex) {
        uniqueQueues[uniqueQueueCount++] = device->vk.transferQueue;
    }

    if (device->vk.genericQueueIndex != device->vk.graphicsQueueIndex && device->vk.genericQueueIndex != device->vk.computeQueueIndex && device->vk.genericQueueIndex != device->vk.transferQueueIndex) {
        uniqueQueues[uniqueQueueCount++] = device->vk.genericQueue;
    }

    for (uint32_t i = 0; i < uniqueQueueCount; ++i) {
        kgfx_vulkan_debugNameObjectPrintf(device, VK_OBJECT_TYPE_QUEUE, uniqueQueues[i], "KGFX Queue %u %s%c%c%c", i, (uniqueQueues[i] == device->vk.genericQueue) ? "Generic " : "", (uniqueQueues[i] == device->vk.graphicsQueue) ? 'G' : ' ', (uniqueQueues[i] == device->vk.computeQueue) ? 'C' : ' ', (uniqueQueues[i] == device->vk.transferQueue) ? 'T' : ' ');
    }

    kgfx_vulkan_debugNameObjectPrintf(device, VK_OBJECT_TYPE_DEVICE, device->vk.device, "KGFX Logical Device %u", vulkanInstance->vk.currentDeviceID++);
    
    *pDevice = &device->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyDevice_vulkan(KGFXDevice device) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    vkDestroyDevice(vulkanDevice->vk.device, NULL);
    free(vulkanDevice);
}

KGFXResult kgfxCreateBuffer_vulkan(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;

    VkBufferCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.size = (VkDeviceSize) size;
    if (!kgfx_vulkan_vkBufferUsageFlags(usage, &createInfo.usage)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;

    VkBuffer vkBuffer;
    if (vkCreateBuffer(vulkanDevice->vk.device, &createInfo, NULL, &vkBuffer) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_BUFFER, vkBuffer, "KGFX Buffer %u", vulkanDevice->vk.currentBufferID++);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice->vk.device, vkBuffer, &memRequirements);

    VkMemoryPropertyFlags memPropFlags;
    if (!kgfx_vulkan_vkMemoryPropertyFlags(location, &memPropFlags)) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.allocationSize = memRequirements.size;

    if (!kgfx_vulkan_memoryType(vulkanDevice->vk.physicalDevice, memRequirements.memoryTypeBits, memPropFlags, &allocInfo.memoryTypeIndex)) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkDeviceMemory vkMemory;
    if (vkAllocateMemory(vulkanDevice->vk.device, &allocInfo, NULL, &vkMemory) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_DEVICE_MEMORY, vkMemory, "KGFX Memory for Buffer %u", vulkanDevice->vk.currentBufferID - 1);

    if (vkBindBufferMemory(vulkanDevice->vk.device, vkBuffer, vkMemory, 0) != VK_SUCCESS) {
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) malloc(sizeof(KGFXBuffer_Vulkan_t));
    if (vulkanBuffer == NULL) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    memset(vulkanBuffer, 0, sizeof(KGFXBuffer_Vulkan_t));

    vulkanBuffer->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanBuffer->obj.instance = device->instance;
    vulkanBuffer->device = device;
    vulkanBuffer->vk.buffer = vkBuffer;
    vulkanBuffer->vk.memory = vkMemory;
    vulkanBuffer->size = size;
    vulkanBuffer->usage = usage;
    vulkanBuffer->location = location;

    *pBuffer = &vulkanBuffer->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyBuffer_vulkan(KGFXBuffer buffer) {
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanBuffer->device;
    vkDestroyBuffer(vulkanDevice->vk.device, vulkanBuffer->vk.buffer, NULL);
    vkFreeMemory(vulkanDevice->vk.device, vulkanBuffer->vk.memory, NULL);
    free(vulkanBuffer);
}

KGFXResult kgfxMapBuffer_vulkan(KGFXBuffer buffer, void** ppData) {
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanBuffer->device;
    
    if (vulkanBuffer->mapped) {
        return KGFX_RESULT_ERROR_INVALID_OPERATION;
    }
    
    if (vkMapMemory(vulkanDevice->vk.device, vulkanBuffer->vk.memory, 0, VK_WHOLE_SIZE, 0, &vulkanBuffer->pMappedData) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    vulkanBuffer->mapped = KGFX_TRUE;
    *ppData = vulkanBuffer->pMappedData;
    return KGFX_RESULT_SUCCESS;
}

void kgfxUnmapBuffer_vulkan(KGFXBuffer buffer) {
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanBuffer->device;
    
    if (!vulkanBuffer->mapped) {
        return;
    }
    
    vkUnmapMemory(vulkanDevice->vk.device, vulkanBuffer->vk.memory);
    vulkanBuffer->mapped = KGFX_FALSE;
    vulkanBuffer->pMappedData = NULL;
}

KGFXResult kgfxUploadBuffer_vulkan(KGFXBuffer buffer, const void* pData, uint64_t size) {
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanBuffer->device;
    
    if (size > vulkanBuffer->size) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    if (vulkanBuffer->location != KGFX_RESOURCE_LOCATION_DEVICE) {
        void* pMapped = vulkanBuffer->pMappedData;
        KGFXBool wasMapped = vulkanBuffer->mapped;
        if (!wasMapped) {
            KGFXResult result = kgfxMapBuffer_vulkan(buffer, &pMapped);
            if (result != KGFX_RESULT_SUCCESS) {
                return result;
            }
        }
        
        memcpy(pMapped, pData, size);
        if (!wasMapped) {
            kgfxUnmapBuffer_vulkan(buffer);
        }
        return KGFX_RESULT_SUCCESS;
    }
    
    VkBufferCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.size = (VkDeviceSize) size;
    createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;

    VkBuffer vkBuffer;
    if (vkCreateBuffer(vulkanDevice->vk.device, &createInfo, NULL, &vkBuffer) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_BUFFER, vkBuffer, "KGFX Temporary Upload Host Buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice->vk.device, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.allocationSize = memRequirements.size;

    if (!kgfx_vulkan_memoryType(vulkanDevice->vk.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex)) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkDeviceMemory vkMemory;
    if (vkAllocateMemory(vulkanDevice->vk.device, &allocInfo, NULL, &vkMemory) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_DEVICE_MEMORY, vkMemory, "KGFX Temporary Upload Host Buffer Memory");

    if (vkBindBufferMemory(vulkanDevice->vk.device, vkBuffer, vkMemory, 0) != VK_SUCCESS) {
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    void* pMapped;
    if (vkMapMemory(vulkanDevice->vk.device, vkMemory, 0, size, 0, &pMapped) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    memcpy(pMapped, pData, size);
    vkUnmapMemory(vulkanDevice->vk.device, vkMemory);
    
    VkCommandPoolCreateInfo poolCreateInfo;
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.pNext = NULL;
    poolCreateInfo.flags = 0;
    poolCreateInfo.queueFamilyIndex = vulkanDevice->vk.transferQueueIndex;
    
    VkCommandPool vkCmdPool;
    if (vkCreateCommandPool(vulkanDevice->vk.device, &poolCreateInfo, NULL, &vkCmdPool) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_POOL, vkCmdPool, "KGFX Temporary Upload Buffer Copy Command Pool");
    
    VkCommandBufferAllocateInfo cmdAllocInfo;
    cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAllocInfo.pNext = NULL;
    cmdAllocInfo.commandPool = vkCmdPool;
    cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAllocInfo.commandBufferCount = 1;
    
    VkCommandBuffer vkCmdBuffer;
    if (vkAllocateCommandBuffers(vulkanDevice->vk.device, &cmdAllocInfo, &vkCmdBuffer) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_BUFFER, vkCmdBuffer, "KGFX Temporary Upload Buffer Copy Command Buffer");
    
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = NULL;
    
    if (vkBeginCommandBuffer(vkCmdBuffer, &beginInfo) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkBufferCopy region;
    region.srcOffset = 0;
    region.dstOffset = 0;
    region.size = (VkDeviceSize) size;
    
    vkCmdCopyBuffer(vkCmdBuffer, vkBuffer, vulkanBuffer->vk.buffer, 1, &region);
    
    if (vkEndCommandBuffer(vkCmdBuffer) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCmdBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;
    
    VkResult vkResult = vkQueueSubmit(vulkanDevice->vk.transferQueue, 1, &submitInfo, NULL);
    vkQueueWaitIdle(vulkanDevice->vk.transferQueue);
    
    vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
    vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
    vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
    vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
    if (vkResult != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxDownloadBuffer_vulkan(KGFXBuffer buffer, void* pData, uint64_t size) {
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanBuffer->device;
    
    if (size > vulkanBuffer->size) {
        size = vulkanBuffer->size;
    }
    
    if (vulkanBuffer->location != KGFX_RESOURCE_LOCATION_DEVICE) {
        void* pMapped = vulkanBuffer->pMappedData;
        KGFXBool wasMapped = vulkanBuffer->mapped;
        if (!wasMapped) {
            KGFXResult result = kgfxMapBuffer_vulkan(buffer, &pMapped);
            if (result != KGFX_RESULT_SUCCESS) {
                return result;
            }
        }
        
        memcpy(pData, pMapped, size);
        if (!wasMapped) {
            kgfxUnmapBuffer_vulkan(buffer);
        }
        return KGFX_RESULT_SUCCESS;
    }
    
    VkBufferCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.size = (VkDeviceSize) size;
    createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;

    VkBuffer vkBuffer;
    if (vkCreateBuffer(vulkanDevice->vk.device, &createInfo, NULL, &vkBuffer) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_BUFFER, vkBuffer, "KGFX Temporary Upload Host Buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice->vk.device, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.allocationSize = memRequirements.size;

    if (!kgfx_vulkan_memoryType(vulkanDevice->vk.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex)) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkDeviceMemory vkMemory;
    if (vkAllocateMemory(vulkanDevice->vk.device, &allocInfo, NULL, &vkMemory) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_DEVICE_MEMORY, vkMemory, "KGFX Temporary Upload Host Buffer Memory");

    if (vkBindBufferMemory(vulkanDevice->vk.device, vkBuffer, vkMemory, 0) != VK_SUCCESS) {
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkCommandPoolCreateInfo poolCreateInfo;
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.pNext = NULL;
    poolCreateInfo.flags = 0;
    poolCreateInfo.queueFamilyIndex = vulkanDevice->vk.transferQueueIndex;
    
    VkCommandPool vkCmdPool;
    if (vkCreateCommandPool(vulkanDevice->vk.device, &poolCreateInfo, NULL, &vkCmdPool) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_POOL, vkCmdPool, "KGFX Temporary Upload Buffer Copy Command Pool");
    
    VkCommandBufferAllocateInfo cmdAllocInfo;
    cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAllocInfo.pNext = NULL;
    cmdAllocInfo.commandPool = vkCmdPool;
    cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAllocInfo.commandBufferCount = 1;
    
    VkCommandBuffer vkCmdBuffer;
    if (vkAllocateCommandBuffers(vulkanDevice->vk.device, &cmdAllocInfo, &vkCmdBuffer) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_BUFFER, vkCmdBuffer, "KGFX Temporary Upload Buffer Copy Command Buffer");
    
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = NULL;
    
    if (vkBeginCommandBuffer(vkCmdBuffer, &beginInfo) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkBufferCopy region;
    region.srcOffset = 0;
    region.dstOffset = 0;
    region.size = (VkDeviceSize) size;
    
    vkCmdCopyBuffer(vkCmdBuffer, vulkanBuffer->vk.buffer, vkBuffer, 1, &region);
    
    if (vkEndCommandBuffer(vkCmdBuffer) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCmdBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;
    
    VkResult vkResult = vkQueueSubmit(vulkanDevice->vk.transferQueue, 1, &submitInfo, NULL);
    vkQueueWaitIdle(vulkanDevice->vk.transferQueue);
    vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
    vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
    if (vkResult != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    void* pMapped;
    if (vkMapMemory(vulkanDevice->vk.device, vkMemory, 0, size, 0, &pMapped) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    memcpy(pData, pMapped, size);
    vkUnmapMemory(vulkanDevice->vk.device, vkMemory);
    
    vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
    vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
    return KGFX_RESULT_SUCCESS;
}

/* (high) TODO: Vulkan textures */
KGFXResult kgfxCreateTexture_vulkan(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    
    VkImageCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    if (!kgfx_vulkan_vkImageType(pTextureDesc->dimensionType, &createInfo.imageType)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkFormat(pTextureDesc->format, &createInfo.format)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    createInfo.extent.width = pTextureDesc->width;
    createInfo.extent.height = pTextureDesc->height;
    createInfo.extent.depth = pTextureDesc->depth;
    createInfo.mipLevels = pTextureDesc->mipMapLevels;
    createInfo.arrayLayers = pTextureDesc->layers;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    
    if (!kgfx_vulkan_vkImageUsageFlags(pTextureDesc->usage, &createInfo.usage)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    
    VkImage vkImage;
    if (vkCreateImage(vulkanDevice->vk.device, &createInfo, NULL, &vkImage) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_IMAGE, vkImage, "KGFX Texture %u", vulkanDevice->vk.currentTextureID++);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vulkanDevice->vk.device, vkImage, &memRequirements);

    VkMemoryPropertyFlags memPropFlags;
    if (!kgfx_vulkan_vkMemoryPropertyFlags(pTextureDesc->location, &memPropFlags)) {
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.allocationSize = memRequirements.size;

    if (!kgfx_vulkan_memoryType(vulkanDevice->vk.physicalDevice, memRequirements.memoryTypeBits, memPropFlags, &allocInfo.memoryTypeIndex)) {
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkDeviceMemory vkMemory;
    if (vkAllocateMemory(vulkanDevice->vk.device, &allocInfo, NULL, &vkMemory) != VK_SUCCESS) {
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_DEVICE_MEMORY, vkMemory, "KGFX Memory for Image %u", vulkanDevice->vk.currentTextureID - 1);

    if (vkBindImageMemory(vulkanDevice->vk.device, vkImage, vkMemory, 0) != VK_SUCCESS) {
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkImageViewCreateInfo viewCreateInfo;
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.pNext = NULL;
    viewCreateInfo.flags = 0;
    viewCreateInfo.image = vkImage;
    if (!kgfx_vulkan_vkImageViewType(pTextureDesc->dimensionType, &viewCreateInfo.viewType)) {
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    viewCreateInfo.format = createInfo.format;
    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewCreateInfo.subresourceRange.aspectMask = 0;
    if (pTextureDesc->format == KGFX_FORMAT_D16_UNORM || pTextureDesc->format == KGFX_FORMAT_D24_UNORM_S8_UINT || pTextureDesc->format == KGFX_FORMAT_D32_FLOAT) {
        viewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
    } else {
        viewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (pTextureDesc->format == KGFX_FORMAT_D24_UNORM_S8_UINT) {
        viewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = pTextureDesc->mipMapLevels;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = pTextureDesc->layers;
    
    VkImageView vkImageView;
    if (vkCreateImageView(vulkanDevice->vk.device, &viewCreateInfo, NULL, &vkImageView) != VK_SUCCESS) {
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) malloc(sizeof(KGFXTexture_Vulkan_t));
    if (vulkanTexture == NULL) {
        vkDestroyImageView(vulkanDevice->vk.device, vkImageView, NULL);
        vkDestroyImage(vulkanDevice->vk.device, vkImage, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }
    
    memset(vulkanTexture, 0, sizeof(KGFXTexture_Vulkan_t));
    
    vulkanTexture->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanTexture->obj.instance = device->instance;
    vulkanTexture->device = device;
    vulkanTexture->format = pTextureDesc->format;
    vulkanTexture->usage = pTextureDesc->usage;
    vulkanTexture->location = pTextureDesc->location;
    vulkanTexture->dimensionType = pTextureDesc->dimensionType;
    vulkanTexture->width = pTextureDesc->width;
    vulkanTexture->height = pTextureDesc->height;
    vulkanTexture->depth = pTextureDesc->depth;
    vulkanTexture->layers = pTextureDesc->layers;
    vulkanTexture->vk.image = vkImage;
    vulkanTexture->vk.imageView = vkImageView;
    vulkanTexture->vk.memory = vkMemory;
    vulkanTexture->vk.format = createInfo.format;
    vulkanTexture->vk.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    
    *pTexture = &vulkanTexture->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyTexture_vulkan(KGFXTexture texture) {
    KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) texture;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanTexture->device;
    vkDeviceWaitIdle(vulkanDevice->vk.device);
    vkDestroyImageView(vulkanDevice->vk.device, vulkanTexture->vk.imageView, NULL);
    vkDestroyImage(vulkanDevice->vk.device, vulkanTexture->vk.image, NULL);
    vkFreeMemory(vulkanDevice->vk.device, vulkanTexture->vk.memory, NULL);
    free(vulkanTexture);
}

/* (high) TODO: Vulkan texture loading operations */
KGFXResult kgfxUploadTexture_vulkan(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) texture;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanTexture->device;
    
    VkBufferCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.size = (VkDeviceSize) size;
    createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;

    VkBuffer vkBuffer;
    if (vkCreateBuffer(vulkanDevice->vk.device, &createInfo, NULL, &vkBuffer) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_BUFFER, vkBuffer, "KGFX Temporary Upload Host Buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice->vk.device, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.allocationSize = memRequirements.size;

    if (!kgfx_vulkan_memoryType(vulkanDevice->vk.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex)) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    VkDeviceMemory vkMemory;
    if (vkAllocateMemory(vulkanDevice->vk.device, &allocInfo, NULL, &vkMemory) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_DEVICE_MEMORY, vkMemory, "KGFX Temporary Upload Host Buffer Memory");

    if (vkBindBufferMemory(vulkanDevice->vk.device, vkBuffer, vkMemory, 0) != VK_SUCCESS) {
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    void* pMapped;
    if (vkMapMemory(vulkanDevice->vk.device, vkMemory, 0, size, 0, &pMapped) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    memcpy(pMapped, pData, size);
    vkUnmapMemory(vulkanDevice->vk.device, vkMemory);
    
    VkCommandPoolCreateInfo poolCreateInfo;
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.pNext = NULL;
    poolCreateInfo.flags = 0;
    poolCreateInfo.queueFamilyIndex = vulkanDevice->vk.transferQueueIndex;
    
    VkCommandPool vkCmdPool;
    if (vkCreateCommandPool(vulkanDevice->vk.device, &poolCreateInfo, NULL, &vkCmdPool) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_POOL, vkCmdPool, "KGFX Temporary Upload Buffer Copy Command Pool");
    
    VkCommandBufferAllocateInfo cmdAllocInfo;
    cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAllocInfo.pNext = NULL;
    cmdAllocInfo.commandPool = vkCmdPool;
    cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdAllocInfo.commandBufferCount = 1;
    
    VkCommandBuffer vkCmdBuffer;
    if (vkAllocateCommandBuffers(vulkanDevice->vk.device, &cmdAllocInfo, &vkCmdBuffer) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_BUFFER, vkCmdBuffer, "KGFX Temporary Upload Buffer Copy Command Buffer");
    
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = NULL;
    
    if (vkBeginCommandBuffer(vkCmdBuffer, &beginInfo) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkBufferImageCopy copy;
    copy.bufferOffset = 0;
    copy.bufferRowLength = 0;
    copy.bufferImageHeight = 0;
    copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    if (vulkanTexture->format == KGFX_FORMAT_D16_UNORM || vulkanTexture->format == KGFX_FORMAT_D24_UNORM_S8_UINT || vulkanTexture->format == KGFX_FORMAT_D32_FLOAT) {
        copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    
    if (vulkanTexture->format == KGFX_FORMAT_D24_UNORM_S8_UINT) {
        copy.imageSubresource.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    
    KGFXCommandPool_Vulkan_t vulkanCommandPool;
    vulkanCommandPool.obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanCommandPool.obj.instance = texture->instance;
    vulkanCommandPool.device = vulkanTexture->device;
    vulkanCommandPool.maxCommandLists = 1;
    vulkanCommandPool.currentListCount = 1;
    vulkanCommandPool.vk.queue = vulkanDevice->vk.transferQueue;
    vulkanCommandPool.vk.queueFamilyIndex = vulkanDevice->vk.transferQueueIndex;
    vulkanCommandPool.vk.commandPool = vkCmdPool;
    
    KGFXCommandList_Vulkan_t vulkanCommandList;
    vulkanCommandList.obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanCommandList.obj.instance = texture->instance;
    vulkanCommandList.commandPool = &vulkanCommandPool.obj;
    vulkanCommandList.vk.commandBuffer = vkCmdBuffer;
    vulkanCommandList.vk.inUseFence = NULL;
    KGFX_LIST_ZERO(vulkanCommandList.bound.renderTargets);
    vulkanCommandList.bound.depthStencilTarget = NULL;
    vulkanCommandList.bound.graphicsPipeline = NULL;
    
    kgfx_vulkan_transitionTexture(&vulkanCommandList, vulkanTexture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
    
    copy.imageSubresource.mipLevel = 0;
    copy.imageSubresource.baseArrayLayer = pTransferDesc->textureFirstLayer;
    copy.imageSubresource.layerCount = pTransferDesc->textureLayerCount;
    copy.imageOffset.x = pTransferDesc->textureX;
    copy.imageOffset.y = pTransferDesc->textureY;
    copy.imageOffset.z = pTransferDesc->textureZ;
    copy.imageExtent.width = pTransferDesc->textureWidth;
    copy.imageExtent.height = pTransferDesc->textureHeight;
    copy.imageExtent.depth = pTransferDesc->textureDepth;
    
    vkCmdCopyBufferToImage(vkCmdBuffer, vkBuffer, vulkanTexture->vk.image, vulkanTexture->vk.layout, 1, &copy);
    
    if (vkEndCommandBuffer(vkCmdBuffer) != VK_SUCCESS) {
        vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
        vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
        vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCmdBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;
    
    VkResult vkResult = vkQueueSubmit(vulkanDevice->vk.transferQueue, 1, &submitInfo, NULL);
    vkQueueWaitIdle(vulkanDevice->vk.transferQueue);
    
    vkDestroyBuffer(vulkanDevice->vk.device, vkBuffer, NULL);
    vkFreeMemory(vulkanDevice->vk.device, vkMemory, NULL);
    vkFreeCommandBuffers(vulkanDevice->vk.device, vkCmdPool, 1, &vkCmdBuffer);
    vkDestroyCommandPool(vulkanDevice->vk.device, vkCmdPool, NULL);
    if (vkResult != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxDownloadTexture_vulkan(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (high) TODO: Vulkan sampler creation */
KGFXResult kgfxCreateSampler_vulkan(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    
    VkSamplerCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    if (!kgfx_vulkan_vkFilter(pSamplerDesc->magFilter, &createInfo.magFilter)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkFilter(pSamplerDesc->minFilter, &createInfo.minFilter)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkSamplerMipmapMode(pSamplerDesc->mipMapFilter, &createInfo.mipmapMode)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkSamplerAddressMode(pSamplerDesc->sampleModeU, &createInfo.addressModeU)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkSamplerAddressMode(pSamplerDesc->sampleModeV, &createInfo.addressModeV)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkSamplerAddressMode(pSamplerDesc->sampleModeW, &createInfo.addressModeW)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    createInfo.mipLodBias = 0.0f;
    createInfo.anisotropyEnable = (pSamplerDesc->anisotropy != 1.0f);
    createInfo.maxAnisotropy = pSamplerDesc->anisotropy;
    createInfo.compareEnable = VK_FALSE;
    createInfo.compareOp = VK_COMPARE_OP_NEVER;
    createInfo.minLod = pSamplerDesc->minLod;
    createInfo.maxLod = pSamplerDesc->maxLod;
    createInfo.unnormalizedCoordinates = VK_FALSE;
    if (!kgfx_vulkan_vkBorderColor(pSamplerDesc->border, &createInfo.borderColor)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    VkSampler vkSampler;
    if (vkCreateSampler(vulkanDevice->vk.device, &createInfo, NULL, &vkSampler) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    KGFXSampler_Vulkan_t* sampler = (KGFXSampler_Vulkan_t*) malloc(sizeof(KGFXSampler_Vulkan_t));
    if (sampler == NULL) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    memset(sampler, 0, sizeof(KGFXSampler_Vulkan_t));
    
    sampler->obj.api = KGFX_INSTANCE_API_VULKAN;
    sampler->obj.instance = device->instance;
    sampler->device = device;
    sampler->vk.sampler = vkSampler;

    *pSampler = &sampler->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroySampler_vulkan(KGFXSampler sampler) {
    KGFXSampler_Vulkan_t* vulkanSampler = (KGFXSampler_Vulkan_t*) sampler;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSampler->device;
    vkDestroySampler(vulkanDevice->vk.device, vulkanSampler->vk.sampler, NULL);
    free(sampler);
}

KGFXResult kgfxCreateShaderSPIRV_vulkan(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    
    VkShaderModuleCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.codeSize = (size_t) size;
    createInfo.pCode = (const uint32_t*) pData;
    
    VkShaderModule vkShader;
    if (vkCreateShaderModule(vulkanDevice->vk.device, &createInfo, NULL, &vkShader) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SHADER_MODULE, vkShader, "KGFX Shader Module %u", vulkanDevice->vk.currentShaderID++);
    
    KGFXShader_Vulkan_t* vulkanShader = (KGFXShader_Vulkan_t*) malloc(sizeof(KGFXShader_Vulkan_t));
    if (vulkanShader == NULL) {
        vkDestroyShaderModule(vulkanDevice->vk.device, vkShader, NULL);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    memset(vulkanShader, 0, sizeof(KGFXShader_Vulkan_t));
    
    vulkanShader->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanShader->obj.instance = device->instance;
    vulkanShader->device = device;
    
    vulkanShader->entry = (char*) malloc(strlen(entryName) + 1);
    if (vulkanShader->entry == NULL) {
        vkDestroyShaderModule(vulkanDevice->vk.device, vkShader, NULL);
        free(vulkanShader);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }
    
    strcpy(((char*) vulkanShader->entry), entryName);
    vulkanShader->stage = stage;
    vulkanShader->vk.shader = vkShader;
    
    *pShader = &vulkanShader->obj;
    return KGFX_RESULT_SUCCESS;
}

/* (low) TODO: Vulkan DXBC shaders */
KGFXResult kgfxCreateShaderDXBC_vulkan(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (low) TODO: Vulkan GLSL shaders */
KGFXResult kgfxCreateShaderGLSL_vulkan(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (low-medium) TODO: Vulkan HLSL shaders */
KGFXResult kgfxCreateShaderHLSL_vulkan(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyShader_vulkan(KGFXShader shader) {
    KGFXShader_Vulkan_t* vulkanShader = (KGFXShader_Vulkan_t*) shader;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanShader->device;
    
    free((void*) vulkanShader->entry);
    vkDestroyShaderModule(vulkanDevice->vk.device, vulkanShader->vk.shader, NULL);
    free(vulkanShader);
}

/* (high) TODO: Vulkan graphics pipeline */
KGFXResult kgfxCreateGraphicsPipeline_vulkan(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    KGFX_LIST(VkPipelineShaderStageCreateInfo) pipelineStages = { NULL, 0 };
    KGFX_LIST_INIT(pipelineStages, pPipelineDesc->shaderCount, VkPipelineShaderStageCreateInfo);
    for (uint32_t i = 0; i < pPipelineDesc->shaderCount; ++i) {
        KGFXShader_Vulkan_t* vulkanShader = (KGFXShader_Vulkan_t*) pPipelineDesc->pShaders[i];
        pipelineStages.data[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineStages.data[i].pNext = NULL;
        pipelineStages.data[i].flags = 0;
        if (!kgfx_vulkan_vkShaderStage(vulkanShader->stage, &pipelineStages.data[i].stage)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        pipelineStages.data[i].module = vulkanShader->vk.shader;
        pipelineStages.data[i].pName = vulkanShader->entry;
        pipelineStages.data[i].pSpecializationInfo = NULL;
    }
    
    KGFX_LIST(VkVertexInputBindingDescription) vertexBindingDescriptions = { NULL, 0 };
    KGFX_LIST(VkVertexInputAttributeDescription) vertexAttributeDescriptions = { NULL, 0 };
    
    KGFX_LIST_INIT(vertexBindingDescriptions, pPipelineDesc->vertexInputDesc.bindingCount, VkVertexInputBindingDescription);
    KGFX_LIST_INIT(vertexAttributeDescriptions, pPipelineDesc->vertexInputDesc.attributeCount, VkVertexInputAttributeDescription);
    
    for (uint32_t i = 0; i < pPipelineDesc->vertexInputDesc.bindingCount; ++i) {
        vertexBindingDescriptions.data[i].binding = pPipelineDesc->vertexInputDesc.pBindings[i].binding;
        vertexBindingDescriptions.data[i].stride = pPipelineDesc->vertexInputDesc.pBindings[i].stride;
        if (!kgfx_vulkan_vkVertexInputRate(pPipelineDesc->vertexInputDesc.pBindings[i].inputRate, &vertexBindingDescriptions.data[i].inputRate)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
    }
    
    for (uint32_t i = 0; i < pPipelineDesc->vertexInputDesc.attributeCount; ++i) {
        vertexAttributeDescriptions.data[i].binding = pPipelineDesc->vertexInputDesc.pAttributes[i].binding;
        vertexAttributeDescriptions.data[i].location = pPipelineDesc->vertexInputDesc.pAttributes[i].location;
        vertexAttributeDescriptions.data[i].offset = pPipelineDesc->vertexInputDesc.pAttributes[i].offset;
        if (!kgfx_vulkan_vkFormat(pPipelineDesc->vertexInputDesc.pAttributes[i].format, &vertexAttributeDescriptions.data[i].format)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
    }
    
    VkPipelineVertexInputStateCreateInfo vinputCreateInfo;
    vinputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vinputCreateInfo.pNext = NULL;
    vinputCreateInfo.flags = 0;
    vinputCreateInfo.vertexBindingDescriptionCount = vertexBindingDescriptions.length;
    vinputCreateInfo.pVertexBindingDescriptions = vertexBindingDescriptions.data;
    vinputCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptions.length;
    vinputCreateInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data;
    
    VkPipelineInputAssemblyStateCreateInfo vassemblyCreateInfo;
    vassemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vassemblyCreateInfo.pNext = NULL;
    vassemblyCreateInfo.flags = 0;
    if (!kgfx_vulkan_vkPrimitiveTopology(pPipelineDesc->topology, &vassemblyCreateInfo.topology)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    vassemblyCreateInfo.primitiveRestartEnable = VK_FALSE;
    
    /* TODO: VkPipelineTessellationStateCreateInfo for tesselation */
    
    VkPipelineViewportStateCreateInfo viewportCreateInfo;
    viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportCreateInfo.pNext = NULL;
    viewportCreateInfo.flags = 0;
    viewportCreateInfo.viewportCount = pPipelineDesc->viewportAndScissorCount;
    viewportCreateInfo.pViewports = NULL;
    viewportCreateInfo.scissorCount = pPipelineDesc->viewportAndScissorCount;
    viewportCreateInfo.pScissors = NULL;
    
    VkPipelineRasterizationStateCreateInfo rasterCreateInfo;
    rasterCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterCreateInfo.pNext = NULL;
    rasterCreateInfo.flags = 0;
    rasterCreateInfo.depthClampEnable = VK_TRUE;
    rasterCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    if (!kgfx_vulkan_vkPolygonMode(pPipelineDesc->fillMode, &rasterCreateInfo.polygonMode)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    if (!kgfx_vulkan_vkCullMode(pPipelineDesc->cullMode, &rasterCreateInfo.cullMode)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    rasterCreateInfo.frontFace = (pPipelineDesc->counterClockwiseFrontFaceWinding) ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
    rasterCreateInfo.depthBiasEnable = VK_FALSE;
    rasterCreateInfo.depthBiasConstantFactor = 1.0f;
    rasterCreateInfo.depthBiasClamp = 1.0f;
    rasterCreateInfo.depthBiasSlopeFactor = 1.0f;
    rasterCreateInfo.lineWidth = 1.0f;
    
    /* TODO: VkPipelineMultisampleStateCreateInfo for multi-sampling */
    
    VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo;
    depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilCreateInfo.pNext = NULL;
    depthStencilCreateInfo.flags = 0;
    depthStencilCreateInfo.depthTestEnable = pPipelineDesc->depthStencilDesc.testDepth;
    depthStencilCreateInfo.depthWriteEnable = pPipelineDesc->depthStencilDesc.writeDepth;
    depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_NEVER;
    if (!kgfx_vulkan_vkCompareOp(pPipelineDesc->depthStencilDesc.compareOp, &depthStencilCreateInfo.depthCompareOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilCreateInfo.stencilTestEnable = pPipelineDesc->depthStencilDesc.testStencil;
    depthStencilCreateInfo.front.failOp = VK_STENCIL_OP_KEEP;
    if (!kgfx_vulkan_vkStencilOp(pPipelineDesc->depthStencilDesc.frontStencilOpDesc.failOp, &depthStencilCreateInfo.front.failOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.front.passOp = VK_STENCIL_OP_KEEP;
    if (!kgfx_vulkan_vkStencilOp(pPipelineDesc->depthStencilDesc.frontStencilOpDesc.passOp, &depthStencilCreateInfo.front.passOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.front.depthFailOp = VK_STENCIL_OP_KEEP;
    if (!kgfx_vulkan_vkStencilOp(pPipelineDesc->depthStencilDesc.frontStencilOpDesc.failDepthOp, &depthStencilCreateInfo.front.depthFailOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.front.compareOp = VK_COMPARE_OP_NEVER;
    if (!kgfx_vulkan_vkCompareOp(pPipelineDesc->depthStencilDesc.frontStencilOpDesc.compareOp, &depthStencilCreateInfo.front.compareOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
    if (!kgfx_vulkan_vkStencilOp(pPipelineDesc->depthStencilDesc.backStencilOpDesc.failOp, &depthStencilCreateInfo.back.failOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
    if (!kgfx_vulkan_vkStencilOp(pPipelineDesc->depthStencilDesc.backStencilOpDesc.passOp, &depthStencilCreateInfo.back.passOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
    if (!kgfx_vulkan_vkStencilOp(pPipelineDesc->depthStencilDesc.backStencilOpDesc.failDepthOp, &depthStencilCreateInfo.back.depthFailOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.back.compareOp = VK_COMPARE_OP_NEVER;
    if (!kgfx_vulkan_vkCompareOp(pPipelineDesc->depthStencilDesc.backStencilOpDesc.compareOp, &depthStencilCreateInfo.back.compareOp) && (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D32_FLOAT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D24_UNORM_S8_UINT || pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_D16_UNORM)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    depthStencilCreateInfo.front.reference = pPipelineDesc->depthStencilDesc.frontStencilOpDesc.reference;
    depthStencilCreateInfo.back.reference = pPipelineDesc->depthStencilDesc.frontStencilOpDesc.reference;
    
    depthStencilCreateInfo.front.compareMask = 0;
    for (uint32_t i = 0; i < 8; ++i) {
        uint32_t bit = (pPipelineDesc->depthStencilDesc.readMaskStencil & (1 << i)) >> i;
        uint32_t nibble = (bit * 0xF) << (i * 4);
        depthStencilCreateInfo.front.compareMask |= nibble;
    }
    depthStencilCreateInfo.back.compareMask = depthStencilCreateInfo.front.compareMask;
    
    depthStencilCreateInfo.front.writeMask = 0;
    for (uint32_t i = 0; i < 8; ++i) {
        uint32_t bit = (pPipelineDesc->depthStencilDesc.writeMaskStencil & (1 << i)) >> i;
        uint32_t nibble = (bit * 0xF) << (i * 4);
        depthStencilCreateInfo.front.writeMask |= nibble;
    }
    depthStencilCreateInfo.back.writeMask = depthStencilCreateInfo.front.writeMask;
    
    depthStencilCreateInfo.minDepthBounds = pPipelineDesc->depthStencilDesc.minimumDepth;
    depthStencilCreateInfo.maxDepthBounds = pPipelineDesc->depthStencilDesc.maximumDepth;
    
    KGFX_LIST(VkPipelineColorBlendAttachmentState) colorAttachmentBlends = { NULL, 0 };
    KGFX_LIST_INIT(colorAttachmentBlends, pPipelineDesc->renderTargetCount, VkPipelineColorBlendAttachmentState);
    
    KGFX_LIST(VkAttachmentDescription) attachmentDescriptions;
    attachmentDescriptions.length = pPipelineDesc->renderTargetCount + ((pPipelineDesc->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN) ? 1 : 0);
    attachmentDescriptions.data = (VkAttachmentDescription*) malloc(attachmentDescriptions.length * sizeof(VkAttachmentDescription));
    
    KGFX_LIST(VkAttachmentReference) colorAttachments;
    KGFX_LIST_INIT(colorAttachments, pPipelineDesc->renderTargetCount, VkAttachmentReference);

    KGFX_LIST(VkFramebufferAttachmentImageInfo) framebufferAttachmentImageInfos;
    framebufferAttachmentImageInfos.length = pPipelineDesc->renderTargetCount + ((pPipelineDesc->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN) ? 1 : 0);
    framebufferAttachmentImageInfos.data = (VkFramebufferAttachmentImageInfo*) malloc(framebufferAttachmentImageInfos.length * sizeof(VkFramebufferAttachmentImageInfo));
    
    for (uint32_t i = 0; i < pPipelineDesc->renderTargetCount; ++i) {
        colorAttachmentBlends.data[i].blendEnable = pPipelineDesc->pRenderTargetDescs[i].enableBlending;
        colorAttachmentBlends.data[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        if (!kgfx_vulkan_vkBlendFactor(pPipelineDesc->pRenderTargetDescs[i].srcColorBlendFactor, &colorAttachmentBlends.data[i].srcColorBlendFactor) && colorAttachmentBlends.data[i].blendEnable) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        colorAttachmentBlends.data[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        if (!kgfx_vulkan_vkBlendFactor(pPipelineDesc->pRenderTargetDescs[i].dstColorBlendFactor, &colorAttachmentBlends.data[i].dstColorBlendFactor) && colorAttachmentBlends.data[i].blendEnable) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        colorAttachmentBlends.data[i].colorBlendOp = VK_BLEND_OP_ADD;
        if (!kgfx_vulkan_vkBlendOp(pPipelineDesc->pRenderTargetDescs[i].colorBlendOp, &colorAttachmentBlends.data[i].colorBlendOp) && colorAttachmentBlends.data[i].blendEnable) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        colorAttachmentBlends.data[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        if (!kgfx_vulkan_vkBlendFactor(pPipelineDesc->pRenderTargetDescs[i].srcAlphaBlendFactor, &colorAttachmentBlends.data[i].srcAlphaBlendFactor) && colorAttachmentBlends.data[i].blendEnable) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        colorAttachmentBlends.data[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        if (!kgfx_vulkan_vkBlendFactor(pPipelineDesc->pRenderTargetDescs[i].dstAlphaBlendFactor, &colorAttachmentBlends.data[i].dstAlphaBlendFactor) && colorAttachmentBlends.data[i].blendEnable) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        colorAttachmentBlends.data[i].alphaBlendOp = VK_BLEND_OP_ADD;
        if (!kgfx_vulkan_vkBlendOp(pPipelineDesc->pRenderTargetDescs[i].alphaBlendOp, &colorAttachmentBlends.data[i].alphaBlendOp) && colorAttachmentBlends.data[i].blendEnable) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        if (!kgfx_vulkan_vkColorComponentFlags(pPipelineDesc->pRenderTargetDescs[i].colorWriteMask, &colorAttachmentBlends.data[i].colorWriteMask)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[i].flags = 0;
        if (!kgfx_vulkan_vkFormat(pPipelineDesc->pRenderTargetDescs[i].format, &attachmentDescriptions.data[i].format)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[i].samples = VK_SAMPLE_COUNT_1_BIT;
        if (!kgfx_vulkan_vkAttachmentLoadOp(pPipelineDesc->pRenderTargetDescs[i].loadOp, &attachmentDescriptions.data[i].loadOp)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        if (!kgfx_vulkan_vkAttachmentStoreOp(pPipelineDesc->pRenderTargetDescs[i].storeOp, &attachmentDescriptions.data[i].storeOp)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        if (!kgfx_vulkan_vkAttachmentLoadOp(pPipelineDesc->pRenderTargetDescs[i].stencilLoadOp, &attachmentDescriptions.data[i].stencilLoadOp) && attachmentDescriptions.data[i].format == VK_FORMAT_D24_UNORM_S8_UINT) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        if (!kgfx_vulkan_vkAttachmentStoreOp(pPipelineDesc->pRenderTargetDescs[i].stencilStoreOp, &attachmentDescriptions.data[i].stencilStoreOp) && attachmentDescriptions.data[i].format == VK_FORMAT_D24_UNORM_S8_UINT) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[i].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        if (!kgfx_vulkan_vkImageLayout(pPipelineDesc->pRenderTargetDescs[i].finalLayout, &attachmentDescriptions.data[i].finalLayout)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        colorAttachments.data[i].attachment = i;
        colorAttachments.data[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        framebufferAttachmentImageInfos.data[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO;
        framebufferAttachmentImageInfos.data[i].pNext = NULL;
        framebufferAttachmentImageInfos.data[i].flags = 0;
        if (!kgfx_vulkan_vkImageUsageFlags(pPipelineDesc->pRenderTargetDescs[i].usage, &framebufferAttachmentImageInfos.data[i].usage)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        framebufferAttachmentImageInfos.data[i].width = pPipelineDesc->pRenderTargetDescs[i].width;
        framebufferAttachmentImageInfos.data[i].height = pPipelineDesc->pRenderTargetDescs[i].height;
        framebufferAttachmentImageInfos.data[i].layerCount = pPipelineDesc->pRenderTargetDescs[i].layers;
        framebufferAttachmentImageInfos.data[i].viewFormatCount = 1;
        framebufferAttachmentImageInfos.data[i].pViewFormats = &attachmentDescriptions.data[i].format;
    }
    
    VkAttachmentReference depthStencilAttachmentRef;
    VkAttachmentReference* pDepthStencilRef = NULL;
    if (pPipelineDesc->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN) {
        attachmentDescriptions.data[attachmentDescriptions.length - 1].flags = 0;
        if (!kgfx_vulkan_vkFormat(pPipelineDesc->depthStencilDesc.format, &attachmentDescriptions.data[attachmentDescriptions.length - 1].format)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[attachmentDescriptions.length - 1].samples = VK_SAMPLE_COUNT_1_BIT;
        if (!kgfx_vulkan_vkAttachmentLoadOp(pPipelineDesc->depthStencilDesc.loadOp, &attachmentDescriptions.data[attachmentDescriptions.length - 1].loadOp)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        if (!kgfx_vulkan_vkAttachmentStoreOp(pPipelineDesc->depthStencilDesc.storeOp, &attachmentDescriptions.data[attachmentDescriptions.length - 1].storeOp)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        if (!kgfx_vulkan_vkAttachmentLoadOp(pPipelineDesc->depthStencilDesc.stencilLoadOp, &attachmentDescriptions.data[attachmentDescriptions.length - 1].stencilLoadOp)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        if (!kgfx_vulkan_vkAttachmentStoreOp(pPipelineDesc->depthStencilDesc.stencilStoreOp, &attachmentDescriptions.data[attachmentDescriptions.length - 1].stencilStoreOp)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        attachmentDescriptions.data[attachmentDescriptions.length - 1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        
        if (!kgfx_vulkan_vkImageLayout(pPipelineDesc->depthStencilDesc.finalLayout, &attachmentDescriptions.data[attachmentDescriptions.length - 1].finalLayout)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].pNext = NULL;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].flags = 0;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].width = pPipelineDesc->depthStencilDesc.width;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].height = pPipelineDesc->depthStencilDesc.height;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].layerCount = pPipelineDesc->depthStencilDesc.layers;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].viewFormatCount = 1;
        framebufferAttachmentImageInfos.data[attachmentDescriptions.length - 1].pViewFormats = &attachmentDescriptions.data[attachmentDescriptions.length - 1].format;
        
        depthStencilAttachmentRef.attachment = attachmentDescriptions.length - 1;
        depthStencilAttachmentRef.layout = attachmentDescriptions.data[attachmentDescriptions.length - 1].initialLayout;
        pDepthStencilRef = &depthStencilAttachmentRef;
    }
    
    VkPipelineColorBlendStateCreateInfo blendCreateInfo;
    blendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blendCreateInfo.pNext = NULL;
    blendCreateInfo.flags = 0;
    blendCreateInfo.logicOpEnable = (pPipelineDesc->blendLogicOp != KGFX_LOGIC_OP_DISABLED);
    blendCreateInfo.logicOp = VK_LOGIC_OP_CLEAR;
    if (!kgfx_vulkan_vkLogicOp(pPipelineDesc->blendLogicOp, &blendCreateInfo.logicOp) && blendCreateInfo.logicOpEnable) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    blendCreateInfo.attachmentCount = colorAttachmentBlends.length;
    blendCreateInfo.pAttachments = colorAttachmentBlends.data;
    blendCreateInfo.blendConstants[0] = 0.0f;
    blendCreateInfo.blendConstants[1] = 0.0f;
    blendCreateInfo.blendConstants[2] = 0.0f;
    blendCreateInfo.blendConstants[3] = 0.0f;
    
    VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    
    VkPipelineDynamicStateCreateInfo dynamicCreateInfo;
    dynamicCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicCreateInfo.pNext = NULL;
    dynamicCreateInfo.flags = 0;
    dynamicCreateInfo.dynamicStateCount = 2;
    dynamicCreateInfo.pDynamicStates = dynamicStates;
    
    VkSubpassDescription subpassDescription;
    subpassDescription.flags = 0;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = NULL;
    subpassDescription.colorAttachmentCount = colorAttachments.length;
    subpassDescription.pColorAttachments = colorAttachments.data;
    subpassDescription.pResolveAttachments = NULL;
    subpassDescription.pDepthStencilAttachment = pDepthStencilRef;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = NULL;
    
    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = NULL;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = attachmentDescriptions.length;
    renderPassCreateInfo.pAttachments = attachmentDescriptions.data;
    renderPassCreateInfo.subpassCount = 1; /* TODO: subpass support */
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = NULL;
    
    VkRenderPass vkRenderPass;
    if (vkCreateRenderPass(vulkanDevice->vk.device, &renderPassCreateInfo, NULL, &vkRenderPass) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_RENDER_PASS, vkRenderPass, "KGFX Render Pass for Graphics Pipeline %u", vulkanDevice->vk.currentGraphicsPipelineID);

    VkFramebufferAttachmentsCreateInfo framebufferAttachmentsCreateInfo;
    framebufferAttachmentsCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
    framebufferAttachmentsCreateInfo.pNext = NULL;
    framebufferAttachmentsCreateInfo.attachmentImageInfoCount = framebufferAttachmentImageInfos.length;
    framebufferAttachmentsCreateInfo.pAttachmentImageInfos = framebufferAttachmentImageInfos.data;

    VkFramebufferCreateInfo framebufferCreateInfo;
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.pNext = &framebufferAttachmentsCreateInfo;
    framebufferCreateInfo.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT;
    framebufferCreateInfo.renderPass = vkRenderPass;
    framebufferCreateInfo.attachmentCount = attachmentDescriptions.length;
    framebufferCreateInfo.pAttachments = NULL;
    framebufferCreateInfo.width = pPipelineDesc->framebufferWidth;
    framebufferCreateInfo.height = pPipelineDesc->framebufferHeight;
    framebufferCreateInfo.layers = pPipelineDesc->framebufferLayers;

    VkFramebuffer vkFramebuffer;
    if (vkCreateFramebuffer(vulkanDevice->vk.device, &framebufferCreateInfo, NULL, &vkFramebuffer) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_FRAMEBUFFER, vkFramebuffer, "KGFX Framebuffer for Graphics Pipeline %u", vulkanDevice->vk.currentGraphicsPipelineID);
    KGFX_LIST_FREE(framebufferAttachmentImageInfos);
    
    KGFX_LIST(VkDescriptorSetLayoutBinding) descSetLayoutBindings;
    KGFX_LIST_INIT(descSetLayoutBindings, pPipelineDesc->uniformSignatureDesc.uniformCount, VkDescriptorSetLayoutBinding);
    
    KGFX_LIST(VkDescriptorPoolSize) descPoolSizes = { NULL, 0 };
    KGFX_LIST_INIT(descPoolSizes, pPipelineDesc->uniformSignatureDesc.uniformCount, VkDescriptorPoolSize);
    
    for (uint32_t i = 0; i < pPipelineDesc->uniformSignatureDesc.uniformCount; ++i) {
        if (pPipelineDesc->uniformSignatureDesc.pUniforms[i].binding.type != KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        descSetLayoutBindings.data[i].binding = pPipelineDesc->uniformSignatureDesc.pUniforms[i].binding.bindPoint.bindingIndex.binding;
        if (!kgfx_vulkan_vkDescriptorType(pPipelineDesc->uniformSignatureDesc.pUniforms[i].resourceType, &descSetLayoutBindings.data[i].descriptorType)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        descSetLayoutBindings.data[i].descriptorCount = pPipelineDesc->uniformSignatureDesc.pUniforms[i].arrayLength;
        if (!kgfx_vulkan_vkShaderStage(pPipelineDesc->uniformSignatureDesc.pUniforms[i].stages, (VkShaderStageFlagBits*) &descSetLayoutBindings.data[i].stageFlags)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        descSetLayoutBindings.data[i].pImmutableSamplers = NULL;
        
        if (!kgfx_vulkan_vkDescriptorType(pPipelineDesc->uniformSignatureDesc.pUniforms[i].resourceType, &descPoolSizes.data[i].type)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        descPoolSizes.data[i].descriptorCount = 1;
    }
    
    VkDescriptorSetLayoutCreateInfo descLayoutCreateInfo;
    descLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descLayoutCreateInfo.pNext = NULL;
    descLayoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
    descLayoutCreateInfo.bindingCount = descSetLayoutBindings.length;
    descLayoutCreateInfo.pBindings = descSetLayoutBindings.data;
    
    VkDescriptorSetLayout vkDescLayout;
    if (vkCreateDescriptorSetLayout(vulkanDevice->vk.device, &descLayoutCreateInfo, NULL, &vkDescLayout) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, vkDescLayout, "KGFX Descriptor Set Layout for Graphics Pipeline %u", vulkanDevice->vk.currentGraphicsPipelineID);
    
    VkPipelineLayoutCreateInfo layoutCreateInfo;
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCreateInfo.pNext = NULL;
    layoutCreateInfo.flags = 0;
    layoutCreateInfo.setLayoutCount = 1;
    layoutCreateInfo.pSetLayouts = &vkDescLayout;
    layoutCreateInfo.pushConstantRangeCount = 0;
    layoutCreateInfo.pPushConstantRanges = NULL;
    
    VkPipelineLayout vkPipelineLayout;
    if (vkCreatePipelineLayout(vulkanDevice->vk.device, &layoutCreateInfo, NULL, &vkPipelineLayout) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_PIPELINE_LAYOUT, vkPipelineLayout, "KGFX Pipeline Layout for Graphics Pipeline %u", vulkanDevice->vk.currentGraphicsPipelineID);
    
    VkPipelineMultisampleStateCreateInfo msCreateInfo;
    msCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    msCreateInfo.pNext = NULL;
    msCreateInfo.flags = 0;
    msCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    msCreateInfo.sampleShadingEnable = VK_FALSE;
    msCreateInfo.minSampleShading = 1.0f;
    msCreateInfo.pSampleMask = NULL;
    msCreateInfo.alphaToCoverageEnable = VK_FALSE;
    msCreateInfo.alphaToOneEnable = VK_FALSE;
    
    VkGraphicsPipelineCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.stageCount = pipelineStages.length;
    createInfo.pStages = pipelineStages.data;
    createInfo.pVertexInputState = &vinputCreateInfo;
    createInfo.pInputAssemblyState = &vassemblyCreateInfo;
    createInfo.pTessellationState = NULL; /* TODO: tessellation support */
    createInfo.pViewportState = &viewportCreateInfo;
    createInfo.pRasterizationState = &rasterCreateInfo;
    createInfo.pMultisampleState = &msCreateInfo; /* TODO: multisample support */
    createInfo.pDepthStencilState = &depthStencilCreateInfo;
    createInfo.pColorBlendState = &blendCreateInfo;
    createInfo.pDynamicState = &dynamicCreateInfo;
    createInfo.layout = vkPipelineLayout;
    createInfo.renderPass = vkRenderPass;
    createInfo.subpass = 0; /* TODO: subpass support */
    createInfo.basePipelineHandle = NULL;
    createInfo.basePipelineIndex = 0;
    
    VkPipeline vkPipeline;
    VkResult result = vkCreateGraphicsPipelines(vulkanDevice->vk.device, NULL, 1, &createInfo, NULL, &vkPipeline);
    if (result != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_PIPELINE, vkPipeline, "KGFX Graphics Pipeline %u", vulkanDevice->vk.currentGraphicsPipelineID++);
    
    KGFXGraphicsPipeline_Vulkan_t* vulkanPipeline = (KGFXGraphicsPipeline_Vulkan_t*) malloc(sizeof(KGFXGraphicsPipeline_Vulkan_t));
    if (vulkanPipeline == NULL) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    memset(vulkanPipeline, 0, sizeof(KGFXGraphicsPipeline_Vulkan_t));
    
    vulkanPipeline->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanPipeline->obj.instance = device->instance;
    vulkanPipeline->device = device;
    vulkanPipeline->framebufferWidth = pPipelineDesc->framebufferWidth;
    vulkanPipeline->framebufferHeight = pPipelineDesc->framebufferHeight;
    vulkanPipeline->framebufferLayers = pPipelineDesc->framebufferLayers;
    if (pPipelineDesc->renderTargetCount == 0) {
        KGFX_LIST_ZERO(vulkanPipeline->renderTargetDescs);
    } else {
        KGFX_LIST_INIT(vulkanPipeline->renderTargetDescs, pPipelineDesc->renderTargetCount, KGFXRenderTargetDesc);
        memcpy(vulkanPipeline->renderTargetDescs.data, pPipelineDesc->pRenderTargetDescs, pPipelineDesc->renderTargetCount * sizeof(KGFXRenderTargetDesc));
    }
    
    if (pPipelineDesc->depthStencilDesc.format == KGFX_FORMAT_UNKNOWN) {
        memset(&vulkanPipeline->depthStencilDesc, 0, sizeof(KGFXDepthStencilDesc));
    } else {
        memcpy(&vulkanPipeline->depthStencilDesc, &pPipelineDesc->depthStencilDesc, sizeof(KGFXDepthStencilDesc));
    }
    
    vulkanPipeline->vk.descLayout = vkDescLayout;
    vulkanPipeline->vk.pipelineLayout = vkPipelineLayout;
    vulkanPipeline->vk.renderPass = vkRenderPass;
    vulkanPipeline->vk.pipeline = vkPipeline;
    vulkanPipeline->vk.framebuffer = vkFramebuffer;
    
    *pPipeline = &vulkanPipeline->obj;
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxResizeGraphicsPipelineRenderTargets_vulkan(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height) {
    KGFXGraphicsPipeline_Vulkan_t* vulkanPipeline = (KGFXGraphicsPipeline_Vulkan_t*) pipeline;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanPipeline->device;
    vulkanPipeline->framebufferWidth = width;
    vulkanPipeline->framebufferHeight = height;

    vkDestroyFramebuffer(vulkanDevice->vk.device, vulkanPipeline->vk.framebuffer, NULL);

    KGFX_LIST(VkFramebufferAttachmentImageInfo) framebufferAttachmentImageInfos;
    framebufferAttachmentImageInfos.length = vulkanPipeline->renderTargetDescs.length + ((vulkanPipeline->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN) ? 1 : 0);
    framebufferAttachmentImageInfos.data = (VkFramebufferAttachmentImageInfo*) malloc(framebufferAttachmentImageInfos.length * sizeof(VkFramebufferAttachmentImageInfo));

    for (uint32_t i = 0; i < vulkanPipeline->renderTargetDescs.length; ++i) {
        framebufferAttachmentImageInfos.data[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO;
        framebufferAttachmentImageInfos.data[i].pNext = NULL;
        framebufferAttachmentImageInfos.data[i].flags = 0;
        if (!kgfx_vulkan_vkImageUsageFlags(vulkanPipeline->renderTargetDescs.data[i].usage, &framebufferAttachmentImageInfos.data[i].usage)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        framebufferAttachmentImageInfos.data[i].width = width;
        framebufferAttachmentImageInfos.data[i].height = height;
        framebufferAttachmentImageInfos.data[i].layerCount = vulkanPipeline->renderTargetDescs.data[i].layers;
        framebufferAttachmentImageInfos.data[i].viewFormatCount = 1;

        VkFormat format;
        kgfx_vulkan_vkFormat(vulkanPipeline->renderTargetDescs.data[i].format, &format);
        framebufferAttachmentImageInfos.data[i].pViewFormats = &format;

        vulkanPipeline->renderTargetDescs.data[i].width = width;
        vulkanPipeline->renderTargetDescs.data[i].height = height;
    }

    if (vulkanPipeline->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN) {
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].pNext = NULL;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].flags = 0;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].width = width;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].height = height;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].layerCount = vulkanPipeline->depthStencilDesc.layers;
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].viewFormatCount = 1;

        VkFormat format;
        kgfx_vulkan_vkFormat(vulkanPipeline->depthStencilDesc.format, &format);
        framebufferAttachmentImageInfos.data[framebufferAttachmentImageInfos.length - 1].pViewFormats = &format;

        vulkanPipeline->depthStencilDesc.width = width;
        vulkanPipeline->depthStencilDesc.height = height;
    }

    VkFramebufferAttachmentsCreateInfo framebufferAttachmentsCreateInfo;
    framebufferAttachmentsCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
    framebufferAttachmentsCreateInfo.pNext = NULL;
    framebufferAttachmentsCreateInfo.attachmentImageInfoCount = framebufferAttachmentImageInfos.length;
    framebufferAttachmentsCreateInfo.pAttachmentImageInfos = framebufferAttachmentImageInfos.data;

    VkFramebufferCreateInfo framebufferCreateInfo;
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.pNext = &framebufferAttachmentsCreateInfo;
    framebufferCreateInfo.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT;
    framebufferCreateInfo.renderPass = vulkanPipeline->vk.renderPass;
    framebufferCreateInfo.attachmentCount = vulkanPipeline->renderTargetDescs.length + ((vulkanPipeline->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN) ? 1 : 0);
    framebufferCreateInfo.pAttachments = NULL;
    framebufferCreateInfo.width = width;
    framebufferCreateInfo.height = height;
    framebufferCreateInfo.layers = vulkanPipeline->framebufferLayers;

    VkFramebuffer vkFramebuffer;
    if (vkCreateFramebuffer(vulkanDevice->vk.device, &framebufferCreateInfo, NULL, &vkFramebuffer) != VK_SUCCESS) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_FRAMEBUFFER, vkFramebuffer, "KGFX Framebuffer for Graphics Pipeline %u", vulkanDevice->vk.currentGraphicsPipelineID);
    vulkanPipeline->vk.framebuffer = vkFramebuffer;
    KGFX_LIST_FREE(framebufferAttachmentImageInfos);

    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyGraphicsPipeline_vulkan(KGFXGraphicsPipeline pipeline) {
    KGFXGraphicsPipeline_Vulkan_t* vulkanPipeline = (KGFXGraphicsPipeline_Vulkan_t*) pipeline;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanPipeline->device;
    vkDestroyPipeline(vulkanDevice->vk.device, vulkanPipeline->vk.pipeline, NULL);
    vkDestroyPipelineLayout(vulkanDevice->vk.device, vulkanPipeline->vk.pipelineLayout, NULL);
    vkDestroyDescriptorSetLayout(vulkanDevice->vk.device, vulkanPipeline->vk.descLayout, NULL);
    vkDestroyFramebuffer(vulkanDevice->vk.device, vulkanPipeline->vk.framebuffer, NULL);
    vkDestroyRenderPass(vulkanDevice->vk.device, vulkanPipeline->vk.renderPass, NULL);
    free(vulkanPipeline);
}

KGFXResult kgfxCreateCommandPool_vulkan(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool) {
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) device->instance;
    if (maxCommandLists == 0) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    VkQueue queue = vulkanDevice->vk.genericQueue;
    uint32_t queueFamilyIndex = vulkanDevice->vk.genericQueueIndex;
    if (queueType == KGFX_QUEUE_TYPE_GRAPHICS) {
        queue = vulkanDevice->vk.graphicsQueue;
        queueFamilyIndex = vulkanDevice->vk.graphicsQueueIndex;
    } else if (queueType == KGFX_QUEUE_TYPE_COMPUTE) {
        queue = vulkanDevice->vk.computeQueue;
        queueFamilyIndex = vulkanDevice->vk.computeQueueIndex;
    } else if (queueType == KGFX_QUEUE_TYPE_TRANSFER) {
        queue = vulkanDevice->vk.transferQueue;
        queueFamilyIndex = vulkanDevice->vk.transferQueueIndex;
    }
    
    VkCommandPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    createInfo.queueFamilyIndex = queueFamilyIndex;
    
    VkCommandPool vkCommandPool;
    if (vkCreateCommandPool(vulkanDevice->vk.device, &createInfo, NULL, &vkCommandPool) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObject(vulkanDevice, VK_OBJECT_TYPE_COMMAND_POOL, vkCommandPool, "KGFX Command Pool");
    KGFXCommandPool_Vulkan_t* commandPool = (KGFXCommandPool_Vulkan_t*) malloc(sizeof(KGFXCommandPool_Vulkan_t));
    if (commandPool == NULL) {
        vkDestroyCommandPool(vulkanDevice->vk.device, vkCommandPool, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    memset(commandPool, 0, sizeof(KGFXCommandPool_Vulkan_t));
    
    commandPool->obj.api = KGFX_INSTANCE_API_VULKAN;
    commandPool->obj.instance = device->instance;
    commandPool->device = device;
    commandPool->queueType = queueType;
    commandPool->maxCommandLists = maxCommandLists;
    commandPool->currentListCount = 0;
    commandPool->vk.queue = queue;
    commandPool->vk.queueFamilyIndex = queueFamilyIndex;
    commandPool->vk.commandPool = vkCommandPool;
    
    *pCommandPool = &commandPool->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyCommandPool_vulkan(KGFXCommandPool commandPool) {
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    vkDestroyCommandPool(vulkanDevice->vk.device, vulkanCommandPool->vk.commandPool, NULL);
    free(commandPool);
}

KGFXResult kgfxCreateCommandList_vulkan(KGFXCommandPool commandPool, KGFXCommandList* pCommandList) {
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) commandPool;
    if (vulkanCommandPool->maxCommandLists == vulkanCommandPool->currentListCount) {
        return KGFX_RESULT_ERROR_OUT_OF_MEMORY;
    }
    
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanCommandPool->device->instance;
    
    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.commandPool = vulkanCommandPool->vk.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer vkCommandBuffer;
    if (vkAllocateCommandBuffers(vulkanDevice->vk.device, &allocInfo, &vkCommandBuffer) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_COMMAND_BUFFER, vkCommandBuffer, "KGFX Command List %u", vulkanCommandPool->currentListCount++);
    
    VkFenceCreateInfo fenceCreateInfo;
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = NULL;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    VkFence vkInUseFence;
    if (vkCreateFence(vulkanDevice->vk.device, &fenceCreateInfo, NULL, &vkInUseFence) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_FENCE, vkInUseFence, "KGFX Command List In Use Fence for Command List %u", vulkanCommandPool->currentListCount - 1);
    
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) malloc(sizeof(KGFXCommandList_Vulkan_t));
    if (vulkanCommandList == NULL) {
        vkFreeCommandBuffers(vulkanDevice->vk.device, vulkanCommandPool->vk.commandPool, 1, &vkCommandBuffer);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    memset(vulkanCommandList, 0, sizeof(KGFXCommandList_Vulkan_t));
    
    ++vulkanCommandPool->currentListCount;
    
    vulkanCommandList->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanCommandList->obj.instance = vulkanDevice->obj.instance;
    vulkanCommandList->commandPool = commandPool;
    vulkanCommandList->vk.commandBuffer = vkCommandBuffer;
    vulkanCommandList->vk.inUseFence = vkInUseFence;
    memset(&vulkanCommandList->bound, 0, sizeof(vulkanCommandList->bound));
    
    *pCommandList = &vulkanCommandList->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyCommandList_vulkan(KGFXCommandList commandList) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    vkWaitForFences(vulkanDevice->vk.device, 1, &vulkanCommandList->vk.inUseFence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(vulkanDevice->vk.device, vulkanCommandList->vk.inUseFence, NULL);
    vkFreeCommandBuffers(vulkanDevice->vk.device, vulkanCommandPool->vk.commandPool, 1, &vulkanCommandList->vk.commandBuffer);
    free(commandList);
    --vulkanCommandPool->currentListCount;
}

void kgfxResetCommandList_vulkan(KGFXCommandList commandList) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    vkWaitForFences(vulkanDevice->vk.device, 1, &vulkanCommandList->vk.inUseFence, VK_TRUE, UINT64_MAX);
    
    vkResetCommandBuffer(vulkanCommandList->vk.commandBuffer, 0);
    memset(&vulkanCommandList->bound, 0, sizeof(vulkanCommandList->bound));
}

KGFXResult kgfxOpenCommandList_vulkan(KGFXCommandList commandList, KGFXBool isOneTime) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    vkWaitForFences(vulkanDevice->vk.device, 1, &vulkanCommandList->vk.inUseFence, VK_TRUE, UINT64_MAX);
    
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = NULL;
    
    if (isOneTime) {
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    
    if (vkBeginCommandBuffer(vulkanCommandList->vk.commandBuffer, &beginInfo) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxCloseCommandList_vulkan(KGFXCommandList commandList) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    if (vkEndCommandBuffer(vulkanCommandList->vk.commandBuffer) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxSubmitCommandList_vulkan(KGFXCommandList commandList) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    
    if (vkResetFences(vulkanDevice->vk.device, 1, &vulkanCommandList->vk.inUseFence) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    /* TODO: Vulkan proper sync */
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vulkanCommandList->vk.commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;
    
    if (vkQueueSubmit(vulkanCommandPool->vk.queue, 1, &submitInfo, vulkanCommandList->vk.inUseFence) != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    return KGFX_RESULT_SUCCESS;
}

void kgfxCmdBindGraphicsPipeline_vulkan(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXGraphicsPipeline_Vulkan_t* vulkanPipeline = (KGFXGraphicsPipeline_Vulkan_t*) pipeline;
    vkCmdBindPipeline(vulkanCommandList->vk.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->vk.pipeline);
    vulkanCommandList->bound.graphicsPipeline = vulkanPipeline;
}

void kgfxCmdSetViewportAndScissor_vulkan(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFX_LIST(VkViewport) viewports = { NULL, 0 };
    KGFX_LIST(VkRect2D) scissors = { NULL, 0 };
    KGFX_LIST_INIT(viewports, viewportAndScissorCount, VkViewport);
    KGFX_LIST_INIT(scissors, viewportAndScissorCount, VkRect2D);

    for (uint32_t i = 0; i < viewportAndScissorCount; ++i) {
        viewports.data[i].x = pViewports[i].x;
        viewports.data[i].y = pViewports[i].height - pViewports[i].y;
        viewports.data[i].width = pViewports[i].width;
        viewports.data[i].height = -pViewports[i].height;
        viewports.data[i].minDepth = pViewports[i].minDepth;
        viewports.data[i].maxDepth = pViewports[i].maxDepth;
        
        scissors.data[i].offset.x = pScissors[i].x;
        scissors.data[i].offset.y = pScissors[i].y;
        scissors.data[i].extent.width = pScissors[i].width;
        scissors.data[i].extent.height = pScissors[i].height;
    }

    vkCmdSetViewport(vulkanCommandList->vk.commandBuffer, 0, viewportAndScissorCount, viewports.data);
    vkCmdSetScissor(vulkanCommandList->vk.commandBuffer, 0, viewportAndScissorCount, scissors.data);

    KGFX_LIST_FREE(viewports);
    KGFX_LIST_FREE(scissors);
}

void kgfxCmdBindRenderTargets_vulkan(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFX_LIST(KGFXTexture_Vulkan_t*) renderTargets = { NULL, 0 };
    KGFX_LIST_INIT(renderTargets, renderTargetCount, KGFXTexture_Vulkan_t*);

    memcpy(renderTargets.data, pRenderTargets, sizeof(KGFXTexture_Vulkan_t*) * renderTargetCount);
    vulkanCommandList->bound.renderTargets.data = renderTargets.data;
    vulkanCommandList->bound.renderTargets.length = renderTargetCount;
    vulkanCommandList->bound.depthStencilTarget = (KGFXTexture_Vulkan_t*) depthStencilTarget;
}

void kgfxCmdBeginRendering_vulkan(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFX_LIST(VkClearValue) clearValues = { NULL, 0 };
    clearValues.length = renderTargetClearValueCount + ((pDepthStencilClearValue != NULL) ? 1 : 0);
    clearValues.data = (VkClearValue*) malloc(clearValues.length * sizeof(VkClearValue));

    for (uint32_t i = 0; i < renderTargetClearValueCount; ++i) {
        switch (pRenderTargetClearValues[i].type) {
            case KGFX_CLEAR_VALUE_TYPE_F32:
                clearValues.data[i].color.float32[0] = pRenderTargetClearValues[i].value.f32[0];
                clearValues.data[i].color.float32[1] = pRenderTargetClearValues[i].value.f32[1];
                clearValues.data[i].color.float32[2] = pRenderTargetClearValues[i].value.f32[2];
                clearValues.data[i].color.float32[3] = pRenderTargetClearValues[i].value.f32[3];
                break;
            case KGFX_CLEAR_VALUE_TYPE_S32:
                clearValues.data[i].color.int32[0] = pRenderTargetClearValues[i].value.s32[0];
                clearValues.data[i].color.int32[1] = pRenderTargetClearValues[i].value.s32[1];
                clearValues.data[i].color.int32[2] = pRenderTargetClearValues[i].value.s32[2];
                clearValues.data[i].color.int32[3] = pRenderTargetClearValues[i].value.s32[3];
                break;
            case KGFX_CLEAR_VALUE_TYPE_U32:
                clearValues.data[i].color.uint32[0] = pRenderTargetClearValues[i].value.u32[0];
                clearValues.data[i].color.uint32[1] = pRenderTargetClearValues[i].value.u32[1];
                clearValues.data[i].color.uint32[2] = pRenderTargetClearValues[i].value.u32[2];
                clearValues.data[i].color.uint32[3] = pRenderTargetClearValues[i].value.u32[3];
                break;
            case KGFX_CLEAR_VALUE_TYPE_DEPTH_STENCIL:
                clearValues.data[i].depthStencil.depth = pRenderTargetClearValues[i].value.depthStencil.depth;
                clearValues.data[i].depthStencil.stencil = pRenderTargetClearValues[i].value.depthStencil.stencil;
                break;
        }
    }
    
    if (pDepthStencilClearValue != NULL) {
        switch (pDepthStencilClearValue->type) {
            case KGFX_CLEAR_VALUE_TYPE_F32:
                clearValues.data[clearValues.length - 1].color.float32[0] = pDepthStencilClearValue->value.f32[0];
                clearValues.data[clearValues.length - 1].color.float32[1] = pDepthStencilClearValue->value.f32[1];
                clearValues.data[clearValues.length - 1].color.float32[2] = pDepthStencilClearValue->value.f32[2];
                clearValues.data[clearValues.length - 1].color.float32[3] = pDepthStencilClearValue->value.f32[3];
                break;
            case KGFX_CLEAR_VALUE_TYPE_S32:
                clearValues.data[clearValues.length - 1].color.int32[0] = pDepthStencilClearValue->value.s32[0];
                clearValues.data[clearValues.length - 1].color.int32[1] = pDepthStencilClearValue->value.s32[1];
                clearValues.data[clearValues.length - 1].color.int32[2] = pDepthStencilClearValue->value.s32[2];
                clearValues.data[clearValues.length - 1].color.int32[3] = pDepthStencilClearValue->value.s32[3];
                break;
            case KGFX_CLEAR_VALUE_TYPE_U32:
                clearValues.data[clearValues.length - 1].color.uint32[0] = pDepthStencilClearValue->value.u32[0];
                clearValues.data[clearValues.length - 1].color.uint32[1] = pDepthStencilClearValue->value.u32[1];
                clearValues.data[clearValues.length - 1].color.uint32[2] = pDepthStencilClearValue->value.u32[2];
                clearValues.data[clearValues.length - 1].color.uint32[3] = pDepthStencilClearValue->value.u32[3];
                break;
            case KGFX_CLEAR_VALUE_TYPE_DEPTH_STENCIL:
                clearValues.data[clearValues.length - 1].depthStencil.depth = pDepthStencilClearValue->value.depthStencil.depth;
                clearValues.data[clearValues.length - 1].depthStencil.stencil = pDepthStencilClearValue->value.depthStencil.stencil;
                break;
        }
    }

    KGFX_LIST(VkImageView) attachments = { NULL, 0 };
    attachments.length = vulkanCommandList->bound.renderTargets.length + ((vulkanCommandList->bound.depthStencilTarget != NULL) ? 1 : 0);
    attachments.data = (VkImageView*) malloc(attachments.length * sizeof(VkImageView));
    
    for (uint32_t i = 0; i < vulkanCommandList->bound.renderTargets.length; ++i) {
        KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) vulkanCommandList->bound.renderTargets.data[i];
        if (vulkanTexture->isSwapchainTexture) {
            KGFXSwapchainTexture_Vulkan_t* vulkanSwapchainTexture = (KGFXSwapchainTexture_Vulkan_t*) vulkanTexture;
            KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) vulkanSwapchainTexture->swapchain;
            KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;
            VkSurfaceKHR vkSurface = vulkanSwapchain->vk.surface;

            if (!vulkanSwapchain->acquired) {
                VkResult result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
                if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                    KGFXSwapchain_Vulkan_t* newSwapchain;
                    if (!kgfx_vulkan_recreateSwapchain(vulkanDevice, vulkanSwapchain, vkSurface, &newSwapchain)) {
                        /* TODO: crash out safely */
                        return;
                    }

                    vulkanSwapchain = newSwapchain;
                    if (!newSwapchain->minimized) {
                        result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
                        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                            /* TODO: crash out safely */
                            return;
                        }
                    }
                } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                    /* TODO: crash out safely */
                    return;
                }

                vulkanSwapchain->acquired = KGFX_TRUE;
                vulkanSwapchain->currentBackbuffer = &vulkanSwapchain->internalBackbuffers.data[vulkanSwapchain->vk.imageIndex];
            }

            vulkanTexture = (KGFXTexture_Vulkan_t*) vulkanSwapchainTexture->swapchain->currentBackbuffer;
        }

        attachments.data[i] = vulkanTexture->vk.imageView;
        kgfx_vulkan_transitionTexture(vulkanCommandList, vulkanTexture, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    }

    if (vulkanCommandList->bound.depthStencilTarget != NULL) {
        KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) vulkanCommandList->bound.depthStencilTarget;
        attachments.data[attachments.length - 1] = vulkanTexture->vk.imageView;
        
        kgfx_vulkan_transitionTexture(vulkanCommandList, vulkanTexture, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
    }

    VkRenderPassAttachmentBeginInfo attachmentBeginInfo;
    attachmentBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO;
    attachmentBeginInfo.pNext = NULL;
    attachmentBeginInfo.attachmentCount = attachments.length;
    attachmentBeginInfo.pAttachments = attachments.data;

    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = &attachmentBeginInfo;
    beginInfo.renderPass = vulkanCommandList->bound.graphicsPipeline->vk.renderPass;
    beginInfo.framebuffer = vulkanCommandList->bound.graphicsPipeline->vk.framebuffer;
    beginInfo.renderArea.offset.x = 0;
    beginInfo.renderArea.offset.y = 0;
    beginInfo.renderArea.extent.width = vulkanCommandList->bound.graphicsPipeline->framebufferWidth;
    beginInfo.renderArea.extent.height = vulkanCommandList->bound.graphicsPipeline->framebufferHeight;
    beginInfo.clearValueCount = clearValues.length;
    beginInfo.pClearValues = clearValues.data;

    vkCmdBeginRenderPass(vulkanCommandList->vk.commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    KGFX_LIST_FREE(clearValues);
    KGFX_LIST_FREE(attachments);
}

void kgfxCmdEndRendering_vulkan(KGFXCommandList commandList) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    vkCmdEndRenderPass(vulkanCommandList->vk.commandBuffer);
    
    for (uint32_t i = 0; i < vulkanCommandList->bound.graphicsPipeline->renderTargetDescs.length; ++i) {
        KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) vulkanCommandList->bound.renderTargets.data[i];
        if (vulkanTexture->isSwapchainTexture) {
            KGFXSwapchainTexture_Vulkan_t* vulkanSwapchainTexture = (KGFXSwapchainTexture_Vulkan_t*) vulkanTexture;
            vulkanTexture = (KGFXTexture_Vulkan_t*) vulkanSwapchainTexture->swapchain->currentBackbuffer;
        }

        kgfx_vulkan_vkImageLayout(vulkanCommandList->bound.graphicsPipeline->renderTargetDescs.data[i].finalLayout, &vulkanTexture->vk.layout);
    }
    
    if (vulkanCommandList->bound.graphicsPipeline->depthStencilDesc.format != KGFX_FORMAT_UNKNOWN && vulkanCommandList->bound.depthStencilTarget != NULL) {
        KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) vulkanCommandList->bound.depthStencilTarget;
        kgfx_vulkan_vkImageLayout(vulkanCommandList->bound.graphicsPipeline->depthStencilDesc.finalLayout, &vulkanTexture->vk.layout);
    }
}

/* (high) TODO: Vulkan commands */
void kgfxCmdBindUniformBuffer_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    
    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = vulkanBuffer->vk.buffer;
    bufferInfo.offset = (VkDeviceSize) offset;
    bufferInfo.range = (VkDeviceSize) size;
    
    VkWriteDescriptorSet writeSet;
    writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.pNext = NULL;
    writeSet.dstSet = NULL;
    writeSet.dstBinding = binding.bindPoint.bindingIndex.binding;
    writeSet.dstArrayElement = 0;
    writeSet.descriptorCount = 1;
    writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeSet.pImageInfo = NULL;
    writeSet.pBufferInfo = &bufferInfo;
    writeSet.pTexelBufferView = NULL;
    
    VkPipelineLayout vkLayout;
    if (vulkanCommandList->bound.graphicsPipeline != NULL) {
        vkLayout = vulkanCommandList->bound.graphicsPipeline->vk.pipelineLayout;
    } else {
        /* TODO: crash out safely */
        return;
    }
    
    vulkanInstance->vk.functions.vkCmdPushDescriptorSetKHR(vulkanCommandList->vk.commandBuffer, (vulkanCommandList->bound.graphicsPipeline != NULL) ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE, vkLayout, 0, 1, &writeSet);
}

/* (high) TODO: Vulkan resource binding */
void kgfxCmdBindStorageBuffer_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    
    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = vulkanBuffer->vk.buffer;
    bufferInfo.offset = (VkDeviceSize) offset;
    bufferInfo.range = (VkDeviceSize) size;
    
    VkWriteDescriptorSet writeSet;
    writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.pNext = NULL;
    writeSet.dstSet = NULL;
    writeSet.dstBinding = binding.bindPoint.bindingIndex.binding;
    writeSet.dstArrayElement = 0;
    writeSet.descriptorCount = 1;
    writeSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writeSet.pImageInfo = NULL;
    writeSet.pBufferInfo = &bufferInfo;
    writeSet.pTexelBufferView = NULL;
    
    VkPipelineLayout vkLayout;
    if (vulkanCommandList->bound.graphicsPipeline != NULL) {
        vkLayout = vulkanCommandList->bound.graphicsPipeline->vk.pipelineLayout;
    } else {
        /* TODO: crash out safely */
        return;
    }
    
    vulkanInstance->vk.functions.vkCmdPushDescriptorSetKHR(vulkanCommandList->vk.commandBuffer, (vulkanCommandList->bound.graphicsPipeline != NULL) ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE, vkLayout, 0, 1, &writeSet);
}

void kgfxCmdBindUniformTexture_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) texture;
    
    kgfx_vulkan_transitionTexture(vulkanCommandList, vulkanTexture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 0, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
    
    VkDescriptorImageInfo imageInfo;
    imageInfo.sampler = NULL;
    imageInfo.imageView = vulkanTexture->vk.imageView;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    VkWriteDescriptorSet writeSet;
    writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.pNext = NULL;
    writeSet.dstSet = NULL;
    writeSet.dstBinding = binding.bindPoint.bindingIndex.binding;
    writeSet.dstArrayElement = 0;
    writeSet.descriptorCount = 1;
    writeSet.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    writeSet.pImageInfo = &imageInfo;
    writeSet.pBufferInfo = NULL;
    writeSet.pTexelBufferView = NULL;
    
    VkPipelineLayout vkLayout;
    if (vulkanCommandList->bound.graphicsPipeline != NULL) {
        vkLayout = vulkanCommandList->bound.graphicsPipeline->vk.pipelineLayout;
    } else {
        /* TODO: crash out safely */
        return;
    }
    
    vulkanInstance->vk.functions.vkCmdPushDescriptorSetKHR(vulkanCommandList->vk.commandBuffer, (vulkanCommandList->bound.graphicsPipeline != NULL) ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE, vkLayout, 0, 1, &writeSet);
}

void kgfxCmdBindStorageTexture_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    KGFXTexture_Vulkan_t* vulkanTexture = (KGFXTexture_Vulkan_t*) texture;
    
    kgfx_vulkan_transitionTexture(vulkanCommandList, vulkanTexture, VK_IMAGE_LAYOUT_GENERAL, 0, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
    
    VkDescriptorImageInfo imageInfo;
    imageInfo.sampler = NULL;
    imageInfo.imageView = vulkanTexture->vk.imageView;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    
    VkWriteDescriptorSet writeSet;
    writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.pNext = NULL;
    writeSet.dstSet = NULL;
    writeSet.dstBinding = binding.bindPoint.bindingIndex.binding;
    writeSet.dstArrayElement = 0;
    writeSet.descriptorCount = 1;
    writeSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    writeSet.pImageInfo = &imageInfo;
    writeSet.pBufferInfo = NULL;
    writeSet.pTexelBufferView = NULL;
    
    VkPipelineLayout vkLayout;
    if (vulkanCommandList->bound.graphicsPipeline != NULL) {
        vkLayout = vulkanCommandList->bound.graphicsPipeline->vk.pipelineLayout;
    } else {
        /* TODO: crash out safely */
        return;
    }
    
    vulkanInstance->vk.functions.vkCmdPushDescriptorSetKHR(vulkanCommandList->vk.commandBuffer, (vulkanCommandList->bound.graphicsPipeline != NULL) ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE, vkLayout, 0, 1, &writeSet);
}

void kgfxCmdBindSampler_vulkan(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXCommandPool_Vulkan_t* vulkanCommandPool = (KGFXCommandPool_Vulkan_t*) vulkanCommandList->commandPool;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanCommandPool->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    KGFXSampler_Vulkan_t* vulkanSampler = (KGFXSampler_Vulkan_t*) sampler;
    
    VkDescriptorImageInfo imageInfo;
    imageInfo.sampler = vulkanSampler->vk.sampler;
    imageInfo.imageView = NULL;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    VkWriteDescriptorSet writeSet;
    writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSet.pNext = NULL;
    writeSet.dstSet = NULL;
    writeSet.dstBinding = binding.bindPoint.bindingIndex.binding;
    writeSet.dstArrayElement = 0;
    writeSet.descriptorCount = 1;
    writeSet.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    writeSet.pImageInfo = &imageInfo;
    writeSet.pBufferInfo = NULL;
    writeSet.pTexelBufferView = NULL;
    
    VkPipelineLayout vkLayout;
    if (vulkanCommandList->bound.graphicsPipeline != NULL) {
        vkLayout = vulkanCommandList->bound.graphicsPipeline->vk.pipelineLayout;
    } else {
        /* TODO: crash out safely */
        return;
    }
    
    vulkanInstance->vk.functions.vkCmdPushDescriptorSetKHR(vulkanCommandList->vk.commandBuffer, (vulkanCommandList->bound.graphicsPipeline != NULL) ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE, vkLayout, 0, 1, &writeSet);
}

void kgfxCmdBindIndexBuffer_vulkan(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    
    VkIndexType vkIndexType;
    if (!kgfx_vulkan_vkIndexType(indexType, &vkIndexType)) {
        return;
    }

    vkCmdBindIndexBuffer(vulkanCommandList->vk.commandBuffer, vulkanBuffer->vk.buffer, (VkDeviceSize) offset, vkIndexType);
}

void kgfxCmdBindVertexBuffers_vulkan(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFX_LIST(VkBuffer) buffers = { NULL, 0 };
    KGFX_LIST_INIT(buffers, bindingCount, VkBuffer);
    for (uint32_t i = 0; i < bindingCount; ++i) {
        buffers.data[i] = ((KGFXBuffer_Vulkan_t*) pBuffers[i])->vk.buffer;
    }

    vkCmdBindVertexBuffers(vulkanCommandList->vk.commandBuffer, firstBinding, bindingCount, buffers.data, (VkDeviceSize*) pOffsets);
    KGFX_LIST_FREE(buffers);
}

void kgfxCmdDraw_vulkan(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    vkCmdDraw(vulkanCommandList->vk.commandBuffer, vertexCount, instanceCount, firstVertex, firstIndex);
}

void kgfxCmdDrawIndexed_vulkan(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    vkCmdDrawIndexed(vulkanCommandList->vk.commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void kgfxCmdDrawIndirect_vulkan(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    vkCmdDrawIndirect(vulkanCommandList->vk.commandBuffer, vulkanBuffer->vk.buffer, (VkDeviceSize) offset, drawCount, stride);
}

void kgfxCmdDrawIndexedIndirect_vulkan(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXBuffer_Vulkan_t* vulkanBuffer = (KGFXBuffer_Vulkan_t*) buffer;
    vkCmdDrawIndexedIndirect(vulkanCommandList->vk.commandBuffer, vulkanBuffer->vk.buffer, (VkDeviceSize) offset, drawCount, stride);
}

/* (high) TODO: Vulkan texture copy */
void kgfxCmdCopyTexture_vulkan(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc) {
    KGFXCommandList_Vulkan_t* vulkanCommandList = (KGFXCommandList_Vulkan_t*) commandList;
    KGFXTexture_Vulkan_t* vulkanSrcTexture = (KGFXTexture_Vulkan_t*) srcTexture;
    KGFXTexture_Vulkan_t* vulkanDstTexture = (KGFXTexture_Vulkan_t*) dstTexture;

    VkImageCopy region;
    region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.srcSubresource.mipLevel = 0;
    region.srcSubresource.baseArrayLayer = copyDesc->srcFirstLayer;
    region.srcSubresource.layerCount = copyDesc->srcLayerCount;
    region.srcOffset.x = copyDesc->srcX;
    region.srcOffset.y = copyDesc->srcY;
    region.srcOffset.z = copyDesc->srcZ;
    region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.dstSubresource.mipLevel = 0;
    region.dstSubresource.baseArrayLayer = copyDesc->dstFirstLayer;
    region.dstSubresource.layerCount = copyDesc->dstLayerCount;
    region.dstOffset.x = copyDesc->dstX;
    region.dstOffset.y = copyDesc->dstY;
    region.dstOffset.z = copyDesc->dstZ;
    region.extent.width = copyDesc->width;
    region.extent.height = copyDesc->height;
    region.extent.depth = copyDesc->depth;

    if (vulkanSrcTexture->isSwapchainTexture) {
        KGFXSwapchainTexture_Vulkan_t* vulkanSwapchainTexture = (KGFXSwapchainTexture_Vulkan_t*) vulkanSrcTexture;
        KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) vulkanSwapchainTexture->swapchain;
        KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;

        if (!vulkanSwapchain->acquired) {
            VkResult result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                KGFXSwapchain_Vulkan_t* newSwapchain;
                if (!kgfx_vulkan_recreateSwapchain(vulkanDevice, vulkanSwapchain, vulkanSwapchain->vk.surface, &newSwapchain)) {
                    /* TODO: crash out safely */
                    return;
                }

                vulkanSwapchain = newSwapchain;
                if (!newSwapchain->minimized) {
                    result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
                    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                        /* TODO: crash out safely */
                        return;
                    }
                }
            }

            vulkanSwapchain->acquired = KGFX_TRUE;
        }

        vulkanSwapchain->currentBackbuffer = &vulkanSwapchain->internalBackbuffers.data[vulkanSwapchain->vk.imageIndex];
        vulkanSrcTexture = (KGFXTexture_Vulkan_t*) vulkanSwapchain->currentBackbuffer;
    }

    if (vulkanDstTexture->isSwapchainTexture) {
        KGFXSwapchainTexture_Vulkan_t* vulkanSwapchainTexture = (KGFXSwapchainTexture_Vulkan_t*) vulkanDstTexture;
        KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) vulkanSwapchainTexture->swapchain;
        KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;

        if (!vulkanSwapchain->acquired) {
            VkResult result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                KGFXSwapchain_Vulkan_t* newSwapchain;
                if (!kgfx_vulkan_recreateSwapchain(vulkanDevice, vulkanSwapchain, vulkanSwapchain->vk.surface, &newSwapchain)) {
                    /* TODO: crash out safely */
                    return;
                }

                vulkanSwapchain = newSwapchain;
                if (!newSwapchain->minimized) {
                    result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
                    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                        /* TODO: crash out safely */
                        return;
                    }
                }
            }

            vulkanSwapchain->acquired = KGFX_TRUE;
        }

        vulkanSwapchain->currentBackbuffer = &vulkanSwapchain->internalBackbuffers.data[vulkanSwapchain->vk.imageIndex];
        vulkanDstTexture = (KGFXTexture_Vulkan_t*) vulkanSwapchain->currentBackbuffer;
    }

    kgfx_vulkan_transitionTexture(vulkanCommandList, vulkanSrcTexture, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 0, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
    kgfx_vulkan_transitionTexture(vulkanCommandList, vulkanDstTexture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    vkCmdCopyImage(vulkanCommandList->vk.commandBuffer, vulkanSrcTexture->vk.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vulkanDstTexture->vk.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

KGFXResult kgfxPresentSwapchain_vulkan(KGFXSwapchain swapchain) {
    KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) swapchain;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;

    if (!vulkanSwapchain->acquired) {
        VkResult result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            KGFXSwapchain_Vulkan_t* newSwapchain;
            if (!kgfx_vulkan_recreateSwapchain(vulkanDevice, vulkanSwapchain, vulkanSwapchain->vk.surface, &newSwapchain)) {
                /* TODO: crash out safely */
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }

            vulkanSwapchain = newSwapchain;
            if (!newSwapchain->minimized) {
                result = vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
                if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                    /* TODO: crash out safely */
                    return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
                }
            }
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        vulkanSwapchain->currentBackbuffer = &vulkanSwapchain->internalBackbuffers.data[vulkanSwapchain->vk.imageIndex];
    }

    kgfx_vulkan_transitionTexture(NULL, &vulkanSwapchain->currentBackbuffer->base, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 0, VK_ACCESS_MEMORY_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = NULL;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &vulkanSwapchain->vk.imageAvailableSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &vulkanSwapchain->vk.swapchain;
    presentInfo.pImageIndices = &vulkanSwapchain->vk.imageIndex;
    presentInfo.pResults = NULL;
    
    VkResult result = vkQueuePresentKHR(vulkanDevice->vk.genericQueue, &presentInfo);
    vulkanSwapchain->acquired = KGFX_FALSE;

    vkQueueWaitIdle(vulkanDevice->vk.genericQueue);
    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
        KGFXSwapchain_Vulkan_t* newSwapchain;
        if (!kgfx_vulkan_recreateSwapchain(vulkanDevice, vulkanSwapchain, vulkanSwapchain->vk.surface, &newSwapchain)) {
            /* TODO: crash out safely */
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        vulkanSwapchain = newSwapchain;
        if (vulkanSwapchain->minimized) {
            return KGFX_RESULT_SUCCESS;
        }

        vkAcquireNextImageKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, UINT64_MAX, vulkanSwapchain->vk.imageAvailableSemaphore, NULL, &vulkanSwapchain->vk.imageIndex);
        presentInfo.pSwapchains = &vulkanSwapchain->vk.swapchain;
        presentInfo.pWaitSemaphores = &vulkanSwapchain->vk.imageAvailableSemaphore;
        presentInfo.pImageIndices = &vulkanSwapchain->vk.imageIndex;

        vulkanSwapchain->currentBackbuffer = &vulkanSwapchain->internalBackbuffers.data[vulkanSwapchain->vk.imageIndex];
        kgfx_vulkan_transitionTexture(NULL, &vulkanSwapchain->currentBackbuffer->base, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 0, VK_ACCESS_MEMORY_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
        result = vkQueuePresentKHR(vulkanDevice->vk.genericQueue, &presentInfo);
        vkQueueWaitIdle(vulkanDevice->vk.genericQueue);
        if (result == VK_SUCCESS) {
            return KGFX_RESULT_SUCCESS;
        }
    } else if (result == VK_SUCCESS) {
        return KGFX_RESULT_SUCCESS;
    }
    
    return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
}

#define KGFX_MIN(a_, b_) ((a_ < b_) ? a_ : b_)
#define KGFX_MAX(a_, b_) ((a_ > b_) ? a_ : b_)

KGFXResult kgfx_createSwapchain_vulkan(KGFXDevice_Vulkan_t* vulkanDevice, KGFX_Vulkan_Surface_t* surface, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanDevice->vk.physicalDevice, surface->vk.surface, &surfaceCapabilities);

    uint32_t maxImages = surfaceCapabilities.maxImageCount;
    if (maxImages == 0) {
        maxImages = UINT32_MAX;
    }

    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.surface = surface->vk.surface;
    createInfo.minImageCount = KGFX_MAX(KGFX_MIN(pSwapchainDesc->imageCount, maxImages), surfaceCapabilities.minImageCount);
    if (!kgfx_vulkan_vkFormat(pSwapchainDesc->format, &createInfo.imageFormat)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent.width = pSwapchainDesc->width;
    createInfo.imageExtent.height = pSwapchainDesc->height;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    if (!kgfx_vulkan_vkPresentMode(pSwapchainDesc->presentMode, &createInfo.presentMode)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    if (createInfo.presentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = NULL;

    VkSwapchainKHR vkSwapchain;
    VkResult result = vkCreateSwapchainKHR(vulkanDevice->vk.device, &createInfo, NULL, &vkSwapchain);
    if (result != VK_SUCCESS) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SWAPCHAIN_KHR, vkSwapchain, "KGFX Swapchain %u", vulkanDevice->vk.currentSwapchainID++);

    KGFX_LIST(VkImage) images = { NULL, 0 };
    KGFX_LIST_INIT(images, createInfo.minImageCount, VkImage);

    if (vkGetSwapchainImagesKHR(vulkanDevice->vk.device, vkSwapchain, &images.length, NULL) != VK_SUCCESS) {
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    if (vkGetSwapchainImagesKHR(vulkanDevice->vk.device, vkSwapchain, &images.length, images.data) != VK_SUCCESS) {
        KGFX_LIST_FREE(images);
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    KGFX_LIST(VkImageView) imageViews = { NULL, 0 };
    KGFX_LIST_INIT(imageViews, images.length, VkImageView);

    for (uint32_t i = 0; i < images.length; ++i) {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = NULL;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = images.data[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = createInfo.imageFormat;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(vulkanDevice->vk.device, &imageViewCreateInfo, NULL, &imageViews.data[i]) != VK_SUCCESS) {
            for (uint32_t j = 0; j < i; ++j) {
                vkDestroyImageView(vulkanDevice->vk.device, imageViews.data[j], NULL);
            }
            KGFX_LIST_FREE(images);
            KGFX_LIST_FREE(imageViews);
            vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_IMAGE_VIEW, imageViews.data[i], "KGFX Swapchain Image View %u for Swapchain %u", i, vulkanDevice->vk.currentSwapchainID - 1);
        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_IMAGE, images.data[i], "KGFX Swapchain Image %u for Swapchain %u", i, vulkanDevice->vk.currentSwapchainID - 1);
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;

    VkSemaphore vkImageAvailableSemaphore;
    if (vkCreateSemaphore(vulkanDevice->vk.device, &semaphoreCreateInfo, NULL, &vkImageAvailableSemaphore) != VK_SUCCESS) {
        for (uint32_t i = 0; i < images.length; ++i) {
            vkDestroyImageView(vulkanDevice->vk.device, imageViews.data[i], NULL);
        }

        KGFX_LIST_FREE(images);
        KGFX_LIST_FREE(imageViews);
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SEMAPHORE, vkImageAvailableSemaphore, "KGFX Swapchain Image Available Semaphore for Swapchain %u", vulkanDevice->vk.currentSwapchainID - 1);

    KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) malloc(sizeof(KGFXSwapchain_Vulkan_t));
    if (vulkanSwapchain == NULL) {
        vkDestroySemaphore(vulkanDevice->vk.device, vkImageAvailableSemaphore, NULL);
        for (uint32_t i = 0; i < images.length; ++i) {
            vkDestroyImageView(vulkanDevice->vk.device, imageViews.data[i], NULL);
        }
        KGFX_LIST_FREE(images);
        KGFX_LIST_FREE(imageViews);
        vkDestroySwapchainKHR(vulkanDevice->vk.device, vkSwapchain, NULL);
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    memset(vulkanSwapchain, 0, sizeof(KGFXSwapchain_Vulkan_t));

    vulkanSwapchain->obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanSwapchain->obj.instance = vulkanDevice->obj.instance;
    vulkanSwapchain->device = &vulkanDevice->obj;
    vulkanSwapchain->format = pSwapchainDesc->format;
    vulkanSwapchain->presentMode = pSwapchainDesc->presentMode;
    vulkanSwapchain->backbuffer.base.obj.api = KGFX_INSTANCE_API_VULKAN;
    vulkanSwapchain->backbuffer.base.obj.instance = vulkanDevice->obj.instance;
    vulkanSwapchain->backbuffer.base.device = &vulkanDevice->obj;
    vulkanSwapchain->backbuffer.base.format = pSwapchainDesc->format;
    vulkanSwapchain->backbuffer.base.width = pSwapchainDesc->width;
    vulkanSwapchain->backbuffer.base.height = pSwapchainDesc->height;
    vulkanSwapchain->backbuffer.base.layers = 1;
    vulkanSwapchain->backbuffer.base.vk.image = VK_NULL_HANDLE;
    vulkanSwapchain->backbuffer.base.vk.imageView = VK_NULL_HANDLE;
    vulkanSwapchain->backbuffer.base.vk.memory = VK_NULL_HANDLE;
    vulkanSwapchain->backbuffer.base.vk.format = createInfo.imageFormat;
    vulkanSwapchain->backbuffer.base.vk.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    vulkanSwapchain->backbuffer.base.isSwapchainTexture = KGFX_TRUE;
    vulkanSwapchain->backbuffer.swapchain = vulkanSwapchain;
    KGFX_LIST_INIT(vulkanSwapchain->internalBackbuffers, createInfo.minImageCount, KGFXSwapchainTexture_Vulkan_t);
    for (uint32_t i = 0; i < vulkanSwapchain->internalBackbuffers.length; ++i) {
        vulkanSwapchain->internalBackbuffers.data[i].base.obj.api = KGFX_INSTANCE_API_VULKAN;
        vulkanSwapchain->internalBackbuffers.data[i].base.obj.instance = vulkanDevice->obj.instance;
        vulkanSwapchain->internalBackbuffers.data[i].base.device = &vulkanDevice->obj;
        vulkanSwapchain->internalBackbuffers.data[i].base.format = pSwapchainDesc->format;
        vulkanSwapchain->internalBackbuffers.data[i].base.width = pSwapchainDesc->width;
        vulkanSwapchain->internalBackbuffers.data[i].base.height = pSwapchainDesc->height;
        vulkanSwapchain->internalBackbuffers.data[i].base.layers = 1;
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.image = images.data[i];
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.imageView = imageViews.data[i];
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.memory = VK_NULL_HANDLE;
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.format = createInfo.imageFormat;
        vulkanSwapchain->internalBackbuffers.data[i].base.vk.layout = VK_IMAGE_LAYOUT_UNDEFINED;
        vulkanSwapchain->internalBackbuffers.data[i].base.isSwapchainTexture = KGFX_TRUE;
        vulkanSwapchain->internalBackbuffers.data[i].swapchain = vulkanSwapchain;
    }

    vulkanSwapchain->vk.surface = surface->vk.surface;
    vulkanSwapchain->vk.swapchain = vkSwapchain;
    vulkanSwapchain->vk.images.data = images.data;
    vulkanSwapchain->vk.images.length = images.length;
    vulkanSwapchain->vk.imageViews.data = imageViews.data;
    vulkanSwapchain->vk.imageViews.length = imageViews.length;
    vulkanSwapchain->vk.imageAvailableSemaphore = vkImageAvailableSemaphore;
    vulkanSwapchain->vk.imageIndex = 0;

    *pSwapchain = &vulkanSwapchain->obj;
    ++surface->referenceCount;
    return KGFX_RESULT_SUCCESS;
}

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
KGFXResult kgfxCreateSwapchainWin32_vulkan(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    if (pSwapchainDesc->format == KGFX_FORMAT_UNKNOWN) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;

    KGFX_Vulkan_Surface_t* surface = NULL;
    uint32_t surfaceIndex = 0;
    for (uint32_t i = 0; i < vulkanInstance->surfaces.length; ++i) {
        if (hwnd == vulkanInstance->surfaces.data[i].window.win32.hwnd && hinstance == vulkanInstance->surfaces.data[i].window.win32.hinstance) {
            surface = &vulkanInstance->surfaces.data[i];
            surfaceIndex = i;
            break;
        }
    }

    if (surface == NULL) {
        VkWin32SurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.hwnd = (HWND) hwnd;
        createInfo.hinstance = (HINSTANCE) hinstance;

        VkSurfaceKHR vkSurface;
        if (vkCreateWin32SurfaceKHR(vulkanInstance->vk.instance, &createInfo, NULL, &vkSurface) != VK_SUCCESS) {
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SURFACE_KHR, vkSurface, "KGFX Win32 Surface %u", vulkanInstance->surfaces.length);

        KGFX_Vulkan_Surface_t surf;
        surf.vk.surface = vkSurface;
        surf.type = KGFX_INTERNAL_VULKAN_WINDOW_TYPE_WIN32;
        surf.window.win32.hwnd = hwnd;
        surf.window.win32.hinstance = hinstance;
        surf.referenceCount = 0;

        surfaceIndex = vulkanInstance->surfaces.length;
        if (vulkanInstance->surfaces.data == NULL) {
            KGFX_LIST_INIT(vulkanInstance->surfaces, 1, KGFX_Vulkan_Surface_t);
            memcpy(vulkanInstance->surfaces.data, &surf, sizeof(surf));
        } else {
            KGFX_LIST_APPEND(vulkanInstance->surfaces, surf, KGFX_Vulkan_Surface_t);
        }
        surface = &vulkanInstance->surfaces.data[vulkanInstance->surfaces.length - 1];
    }
    
    KGFXResult result = kgfx_createSwapchain_vulkan(vulkanDevice, surface, pSwapchainDesc, pSwapchain);
    if (result != KGFX_RESULT_SUCCESS) {
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        KGFX_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, KGFX_Vulkan_Surface_t);
        return result;
    }
    
    return KGFX_RESULT_SUCCESS;
}
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */

#if defined(__linux__) || defined(__gnu_linux__)
/* (medium) TODO: Vulkan Xlib swapchain */
KGFXResult kgfxCreateSwapchainXlib_vulkan(KGFXDevice device, void* display, unsigned long window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    if (pSwapchainDesc->format == KGFX_FORMAT_UNKNOWN) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;

    KGFX_Vulkan_Surface_t* surface = NULL;
    uint32_t surfaceIndex = 0;
    for (uint32_t i = 0; i < vulkanInstance->surfaces.length; ++i) {
        if (display == vulkanInstance->surfaces.data[i].window.xlib.display && window == vulkanInstance->surfaces.data[i].window.xlib.window) {
            surface = &vulkanInstance->surfaces.data[i];
            surfaceIndex = i;
            break;
        }
    }

    if (surface == NULL) {
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.dpy = (Display*) display;
        createInfo.window = window;

        VkSurfaceKHR vkSurface;
        if (vkCreateXlibSurfaceKHR(vulkanInstance->vk.instance, &createInfo, NULL, &vkSurface) != VK_SUCCESS) {
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SURFACE_KHR, vkSurface, "KGFX Xlib Surface %u", vulkanInstance->surfaces.length);

        KGFX_Vulkan_Surface_t surf;
        surf.vk.surface = vkSurface;
        surf.type = KGFX_INTERNAL_VULKAN_WINDOW_TYPE_XLIB;
        surf.window.xlib.display = display;
        surf.window.xlib.window = window;
        surf.referenceCount = 0;

        surfaceIndex = vulkanInstance->surfaces.length;
        if (vulkanInstance->surfaces.data == NULL) {
            KGFX_LIST_INIT(vulkanInstance->surfaces, 1, KGFX_Vulkan_Surface_t);
            memcpy(vulkanInstance->surfaces.data, &surf, sizeof(surf));
        } else {
            KGFX_LIST_APPEND(vulkanInstance->surfaces, surf, KGFX_Vulkan_Surface_t);
        }
        surface = &vulkanInstance->surfaces.data[vulkanInstance->surfaces.length - 1];
    }
    
    KGFXResult result = kgfx_createSwapchain_vulkan(vulkanDevice, surface, pSwapchainDesc, pSwapchain);
    if (result != KGFX_RESULT_SUCCESS) {
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        KGFX_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, KGFX_Vulkan_Surface_t);
        return result;
    }
    
    return KGFX_RESULT_SUCCESS;
}

/* (low-medium) TODO: Vulkan Xcb swapchain */
KGFXResult kgfxCreateSwapchainXcb_vulkan(KGFXDevice device, void* connection, uint32_t window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (low) TODO: Vulkan Wayland swapchain */
KGFXResult kgfxCreateSwapchainWayland_vulkan(KGFXDevice device, void* display, void* surface, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    if (pSwapchainDesc->format == KGFX_FORMAT_UNKNOWN) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;

    KGFX_Vulkan_Surface_t* vulkanSurface = NULL;
    uint32_t surfaceIndex = 0;
    for (uint32_t i = 0; i < vulkanInstance->surfaces.length; ++i) {
        if (display == vulkanInstance->surfaces.data[i].window.wayland.display && surface == vulkanInstance->surfaces.data[i].window.wayland.surface) {
            vulkanSurface = &vulkanInstance->surfaces.data[i];
            surfaceIndex = i;
            break;
        }
    }

    if (vulkanSurface == NULL) {
        VkWaylandSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.display = (wl_display*) display;
        createInfo.surface = (wl_surface*) surface;

        VkSurfaceKHR vkSurface;
        if (vkCreateWaylandSurfaceKHR(vulkanInstance->vk.instance, &createInfo, NULL, &vkSurface) != VK_SUCCESS) {
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }

        kgfx_vulkan_debugNameObjectPrintf(vulkanDevice, VK_OBJECT_TYPE_SURFACE_KHR, vkSurface, "KGFX Wayland Surface %u", vulkanInstance->surfaces.length);

        KGFX_Vulkan_Surface_t surf;
        surf.vk.surface = vkSurface;
        surf.type = KGFX_INTERNAL_VULKAN_WINDOW_TYPE_WAYLAND;
        surf.window.wayland.display = display;
        surf.window.wayland.surface = surface;
        surf.referenceCount = 0;

        surfaceIndex = vulkanInstance->surfaces.length;
        if (vulkanInstance->surfaces.data == NULL) {
            KGFX_LIST_INIT(vulkanInstance->surfaces, 1, KGFX_Vulkan_Surface_t);
            memcpy(vulkanInstance->surfaces.data, &surf, sizeof(surf));
        } else {
            KGFX_LIST_APPEND(vulkanInstance->surfaces, surf, KGFX_Vulkan_Surface_t);
        }
        vulkanSurface = &vulkanInstance->surfaces.data[vulkanInstance->surfaces.length - 1];
    }
    
    KGFXResult result = kgfx_createSwapchain_vulkan(vulkanDevice, vulkanSurface, pSwapchainDesc, pSwapchain);
    if (result != KGFX_RESULT_SUCCESS) {
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, vulkanSurface->vk.surface, NULL);
        KGFX_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, KGFX_Vulkan_Surface_t);
        return result;
    }
    
    return KGFX_RESULT_SUCCESS;
}
#endif /* #if defined(__linux__) || defined(__gnu_linux__) */

#ifdef __APPLE__
#include <objc/runtime.h>
#include <objc/message.h>

void kgfx_vulkan_releaseLayer(void* pLayer);

KGFXResult kgfx_vulkan_createSurfaceCocoa(VkInstance instance, void* nsWindow, VkSurfaceKHR* pSurface, void** ppLayer);

KGFXResult kgfxCreateSwapchainCocoa_vulkan(KGFXDevice device, void* nsWindow, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    if (pSwapchainDesc->format == KGFX_FORMAT_UNKNOWN) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance;
    
    KGFX_Vulkan_Surface_t* surface = NULL;
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
        KGFXResult result = kgfx_vulkan_createSurfaceCocoa(vulkanInstance->vk.instance, nsWindow, &vkSurface, &layer);
        if (result != KGFX_RESULT_SUCCESS) {
            return result;
        }
        
        KGFX_Vulkan_Surface_t surf;
        surf.vk.surface = vkSurface;
        surf.type = KGFX_INTERNAL_VULKAN_WINDOW_TYPE_COCOA;
        surf.window.cocoa.nsWindow = nsWindow;
        surf.window.cocoa.layer = layer;
        surf.referenceCount = 0;
        
        surfaceIndex = vulkanInstance->surfaces.length;
        if (vulkanInstance->surfaces.data == NULL) {
            KGFX_LIST_INIT(vulkanInstance->surfaces, 1, KGFX_Vulkan_Surface_t);
            memcpy(vulkanInstance->surfaces.data, &surf, sizeof(surf));
        } else {
            KGFX_LIST_APPEND(vulkanInstance->surfaces, surf, KGFX_Vulkan_Surface_t);
        }
        surface = &vulkanInstance->surfaces.data[vulkanInstance->surfaces.length - 1];
    }
    
    KGFXResult result = kgfx_createSwapchain_vulkan(vulkanDevice, surface, pSwapchainDesc, pSwapchain);
    if (result != KGFX_RESULT_SUCCESS) {
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        KGFX_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, KGFX_Vulkan_Surface_t);
        return result;
    }
    
    return KGFX_RESULT_SUCCESS;
}

#endif /* #ifdef __APPLE__ */

KGFXResult kgfxRecreateSwapchain_vulkan(KGFXSwapchain swapchain, uint32_t width, uint32_t height) {
    KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) swapchain;
    if (width == 0 || height == 0) {
        vulkanSwapchain->minimized = KGFX_TRUE;
        return KGFX_RESULT_SUCCESS;
    }

    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;
    KGFX_Vulkan_Surface_t* surface = NULL;
    uint32_t surfaceIndex = 0;
    for (uint32_t i = 0; i < ((KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance)->surfaces.length; ++i) {
        if (((KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance)->surfaces.data[i].vk.surface == vulkanSwapchain->vk.surface) {
            surface = &((KGFXInstance_Vulkan_t*) vulkanDevice->obj.instance)->surfaces.data[i];
            surfaceIndex = i;
            break;
        }
    }

    if (surface == NULL) {
        /* ??? */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    KGFXSwapchain_Vulkan_t* newSwapchain;
    if (!kgfx_vulkan_recreateSwapchain(vulkanDevice, vulkanSwapchain, surface->vk.surface, &newSwapchain)) {
        /* TODO: crash out safely */
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroySwapchain_vulkan(KGFXSwapchain swapchain) {
    KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) swapchain;
    KGFXDevice_Vulkan_t* vulkanDevice = (KGFXDevice_Vulkan_t*) vulkanSwapchain->device;
    KGFXInstance_Vulkan_t* vulkanInstance = (KGFXInstance_Vulkan_t*) vulkanSwapchain->obj.instance;
    KGFX_Vulkan_Surface_t* surface = NULL;
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
    
    vkDeviceWaitIdle(vulkanDevice->vk.device);
    for (uint32_t i = 0; i < vulkanSwapchain->vk.imageViews.length; ++i) {
        vkDestroyImageView(vulkanDevice->vk.device, vulkanSwapchain->vk.imageViews.data[i], NULL);
    }

    KGFX_LIST_FREE(vulkanSwapchain->vk.images);
    KGFX_LIST_FREE(vulkanSwapchain->vk.imageViews);
    vkDestroySwapchainKHR(vulkanDevice->vk.device, vulkanSwapchain->vk.swapchain, NULL);
    vkDestroySemaphore(vulkanDevice->vk.device, vulkanSwapchain->vk.imageAvailableSemaphore, NULL);
    
    --surface->referenceCount;
    if (surface->referenceCount == 0) {
#ifdef __APPLE__
        kgfx_vulkan_releaseLayer(surface->window.cocoa.layer);
#endif /* #ifdef __APPLE__ */
        vkDestroySurfaceKHR(vulkanInstance->vk.instance, surface->vk.surface, NULL);
        KGFX_LIST_REMOVE(vulkanInstance->surfaces, surfaceIndex, KGFX_Vulkan_Surface_t);
    }
}

KGFXTexture kgfxGetSwapchainBackbuffer_vulkan(KGFXSwapchain swapchain) {
    KGFXSwapchain_Vulkan_t* vulkanSwapchain = (KGFXSwapchain_Vulkan_t*) swapchain;
    return &vulkanSwapchain->backbuffer.base.obj;
}

#endif /* #ifdef KGFX_VULKAN_IMPLEMENTATION */

#ifdef KGFX_VULKAN_COCOA_IMPLEMENTATION

#ifndef __OBJC__
#error "Objective-C is required for the Cocoa Vulkan implementation"
#endif /* #ifndef __OBJC__ */

#define VK_USE_PLATFORM_MACOS_MVK
#define VK_USE_PLATFORM_METAL_EXT
#include <vulkan/vulkan.h>

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

void kgfx_vulkan_releaseLayer(void* pLayer) {
    CAMetalLayer* layer = (__bridge CAMetalLayer*) pLayer;
    [layer release];
}

KGFXResult kgfx_vulkan_createSurfaceCocoa(VkInstance instance, void* nsWindow, VkSurfaceKHR* pSurface, void** ppLayer) {
    CAMetalLayer* layer = [CAMetalLayer layer];
    if (layer == nil) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
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
            return KGFX_RESULT_SUCCESS;
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
            return KGFX_RESULT_SUCCESS;
        }
        
        macOSInfo.pView = (__bridge void*) layer;
        if (createMacOSSurface(instance, &macOSInfo, NULL, &surface) == VK_SUCCESS) {
            *pSurface = surface;
            *ppLayer = (__bridge void*) layer;
            return KGFX_RESULT_SUCCESS;
        }
    }
    
    return KGFX_RESULT_ERROR_UNSUPPORTED;
}

#endif /* KGFX_VULKAN_COCOA_IMPLEMENTATION */

#ifdef KGFX_D3D11_IMPLEMENTATION

#ifndef __cplusplus
#error "C++ is required for the D3D11 implementation"
#endif /* #ifndef __cplusplus */

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgidebug.h>

#include <string>
#include <vector>

typedef struct KGFXInstance_D3D11_t {
    KGFXObject obj;

    KGFXInstanceCreateFlagBits flags;
    KGFXDebugCallbackPFN debugCallback;
} KGFXInstance_D3D11_t;

typedef struct KGFXDevice_D3D11_t {
    KGFXObject obj;
    uint32_t adapterID;

    struct {
        ID3D11Device* device;
        ID3D11DeviceContext* context;
    } d3d11;
} KGFXDevice_D3D11_t;

typedef struct KGFXBuffer_D3D11_t {
    KGFXObject obj;
    KGFXDevice device;

    uint64_t size;
    KGFXBufferUsageMask usage;
    KGFXResourceLocation location;

    struct {
        ID3D11Buffer* buffer;
        D3D11_BUFFER_DESC desc;
    } d3d11;
} KGFXBuffer_D3D11_t;

typedef struct KGFXTexture_D3D11_t {
    KGFXObject obj;
    KGFXDevice device;

    uint32_t width;
    uint32_t height;
    uint32_t depth;
    KGFXFormat format;
    KGFXTextureUsageMask usage;
    KGFXResourceLocation location;

    KGFXBool isBackbuffer;

    struct {
        ID3D11Texture2D* texture;
        ID3D11Texture3D* texture3D;

        ID3D11RenderTargetView* rtv;
        ID3D11ShaderResourceView* srv;
        ID3D11UnorderedAccessView* uav;

        D3D11_TEXTURE2D_DESC desc;
    } d3d11;
} KGFXTexture_D3D11_t;

typedef struct KGFXSwapchainTexture_D3D11_t {
    KGFXTexture_D3D11_t base;
    struct KGFXSwapchain_D3D11_t* swapchain;
} KGFXSwapchainTexture_D3D11_t;

typedef struct KGFXSwapchain_D3D11_t {
    KGFXObject obj;
    KGFXDevice device;

    KGFXSwapchainTexture_D3D11_t backbuffer;
    void* hwnd;
    void* hinstance;

    struct {
        IDXGISwapChain* swapchain;
        ID3D11Texture2D* backbuffer;
        ID3D11RenderTargetView* backbufferRTV;
    } d3d11;
} KGFXSwapchain_D3D11_t;

typedef struct KGFXSampler_D3D11_t {
    KGFXObject obj;
    KGFXDevice device;

    struct {
        ID3D11SamplerState* sampler;
    } d3d11;
} KGFXSampler_D3D11_t;

typedef struct KGFXShader_D3D11_t {
    KGFXObject obj;
    KGFXDevice device;
    
    KGFXShaderStage stage;

    struct {
        ID3D11DeviceChild* shader;
        ID3DBlob* vblob;
    } d3d11;
} KGFXShader_D3D11_t;

typedef struct KGFXGraphicsPipeline_D3D11_t {
    KGFXObject obj;
    KGFXDevice device;

    struct {
        ID3D11InputLayout* inputLayout;
        ID3D11BlendState* blendState;
        ID3D11DepthStencilState* depthStencilState;
        ID3D11RasterizerState* rasterizerState;

        struct {
            ID3D11VertexShader* vertexShader;
            ID3D11PixelShader* pixelShader;
            ID3D11GeometryShader* geometryShader;
            ID3D11HullShader* hullShader;
            ID3D11DomainShader* domainShader;
        } shaders;
    } d3d11;
} KGFXGraphicsPipeline_D3D11_t;

KGFXBool kgfx_d3d11_dxgiFormat(KGFXFormat format, DXGI_FORMAT* pFormat) {
    switch (format) {
        case KGFX_FORMAT_R8_UINT:
            *pFormat = DXGI_FORMAT_R8_UINT;
            break;
        case KGFX_FORMAT_R8_SINT:
            *pFormat = DXGI_FORMAT_R8_SINT;
            break;
        case KGFX_FORMAT_R8_UNORM:
            *pFormat = DXGI_FORMAT_R8_UNORM;
            break;
        case KGFX_FORMAT_R8_SNORM:
            *pFormat = DXGI_FORMAT_R8_SNORM;
            break;
        case KGFX_FORMAT_R8G8_UINT:
            *pFormat = DXGI_FORMAT_R8G8_UINT;
            break;
        case KGFX_FORMAT_R8G8_SINT:
            *pFormat = DXGI_FORMAT_R8G8_SINT;
            break;
        case KGFX_FORMAT_R8G8_UNORM:
            *pFormat = DXGI_FORMAT_R8G8_UNORM;
            break;
        case KGFX_FORMAT_R8G8_SNORM:
            *pFormat = DXGI_FORMAT_R8G8_SNORM;
            break;
        case KGFX_FORMAT_R8G8B8A8_UINT:
            *pFormat = DXGI_FORMAT_R8G8B8A8_UINT;
            break;
        case KGFX_FORMAT_R8G8B8A8_SINT:
            *pFormat = DXGI_FORMAT_R8G8B8A8_SINT;
            break;
        case KGFX_FORMAT_R8G8B8A8_UNORM:
            *pFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case KGFX_FORMAT_R8G8B8A8_SNORM:
            *pFormat = DXGI_FORMAT_R8G8B8A8_SNORM;
            break;
        case KGFX_FORMAT_R8G8B8A8_SRGB:
            *pFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            break;
        case KGFX_FORMAT_B8G8R8A8_SRGB:
            *pFormat = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
            break;
        case KGFX_FORMAT_R16_UINT:
            *pFormat = DXGI_FORMAT_R16_UINT;
            break;
        case KGFX_FORMAT_R16_SINT:
            *pFormat = DXGI_FORMAT_R16_SINT;
            break;
        case KGFX_FORMAT_R16_UNORM:
            *pFormat = DXGI_FORMAT_R16_UNORM;
            break;
        case KGFX_FORMAT_R16_SNORM:
            *pFormat = DXGI_FORMAT_R16_SNORM;
            break;
        case KGFX_FORMAT_R16_FLOAT:
            *pFormat = DXGI_FORMAT_R16_FLOAT;
            break;
        case KGFX_FORMAT_R16G16_UINT:
            *pFormat = DXGI_FORMAT_R16G16_UINT;
            break;
        case KGFX_FORMAT_R16G16_SINT:
            *pFormat = DXGI_FORMAT_R16G16_SINT;
            break;
        case KGFX_FORMAT_R16G16_UNORM:
            *pFormat = DXGI_FORMAT_R16G16_UNORM;
            break;
        case KGFX_FORMAT_R16G16_SNORM:
            *pFormat = DXGI_FORMAT_R16G16_SNORM;
            break;
        case KGFX_FORMAT_R16G16_FLOAT:
            *pFormat = DXGI_FORMAT_R16G16_FLOAT;
            break;
        case KGFX_FORMAT_R16G16B16A16_UINT:
            *pFormat = DXGI_FORMAT_R16G16B16A16_UINT;
            break;
        case KGFX_FORMAT_R16G16B16A16_SINT:
            *pFormat = DXGI_FORMAT_R16G16B16A16_SINT;
            break;
        case KGFX_FORMAT_R16G16B16A16_UNORM:
            *pFormat = DXGI_FORMAT_R16G16B16A16_UNORM;
            break;
        case KGFX_FORMAT_R16G16B16A16_SNORM:
            *pFormat = DXGI_FORMAT_R16G16B16A16_SNORM;
            break;
        case KGFX_FORMAT_R16G16B16A16_FLOAT:
            *pFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
            break;
        case KGFX_FORMAT_R32_UINT:
            *pFormat = DXGI_FORMAT_R32_UINT;
            break;
        case KGFX_FORMAT_R32_SINT:
            *pFormat = DXGI_FORMAT_R32_SINT;
            break;
        case KGFX_FORMAT_R32_FLOAT:
            *pFormat = DXGI_FORMAT_R32_FLOAT;
            break;
        case KGFX_FORMAT_R32G32_UINT:
            *pFormat = DXGI_FORMAT_R32G32_UINT;
            break;
        case KGFX_FORMAT_R32G32_SINT:
            *pFormat = DXGI_FORMAT_R32G32_SINT;
            break;
        case KGFX_FORMAT_R32G32_FLOAT:
            *pFormat = DXGI_FORMAT_R32G32_FLOAT;
            break;
        case KGFX_FORMAT_R32G32B32A32_UINT:
            *pFormat = DXGI_FORMAT_R32G32B32A32_UINT;
            break;
        case KGFX_FORMAT_R32G32B32A32_SINT:
            *pFormat = DXGI_FORMAT_R32G32B32A32_SINT;
            break;
        case KGFX_FORMAT_R32G32B32A32_FLOAT:
            *pFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        case KGFX_FORMAT_D32_FLOAT:
            *pFormat = DXGI_FORMAT_D32_FLOAT;
            break;
        case KGFX_FORMAT_D24_UNORM_S8_UINT:
            *pFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
            break;
        case KGFX_FORMAT_D16_UNORM:
            *pFormat = DXGI_FORMAT_D16_UNORM;
            break;
        case KGFX_FORMAT_B8G8R8A8_UNORM:
            *pFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
            break;
        case KGFX_FORMAT_R32G32B32_UINT:
            *pFormat = DXGI_FORMAT_R32G32B32_UINT;
            break;
        case KGFX_FORMAT_R32G32B32_SINT:
            *pFormat = DXGI_FORMAT_R32G32B32_SINT;
            break;
        case KGFX_FORMAT_R32G32B32_FLOAT:
            *pFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case KGFX_FORMAT_UNKNOWN:
            *pFormat = DXGI_FORMAT_UNKNOWN;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_dxgiSwapEffect(KGFXPresentMode mode, DXGI_SWAP_EFFECT* pEffect) {
    switch (mode) {
        case KGFX_PRESENT_MODE_VSYNC_DISCARD_D3D:
            *pEffect = DXGI_SWAP_EFFECT_DISCARD;
            break;
        case KGFX_PRESENT_MODE_VSYNC:
            *pEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
            break;
        case KGFX_PRESENT_MODE_NO_SYNC:
            *pEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_sampleFilter(KGFXTextureFilter magFilter, KGFXTextureFilter minFilter, KGFXTextureFilter mipMapFilter, D3D11_FILTER* pFilter) {
    if (magFilter == KGFX_TEXTURE_FILTER_NEAREST && minFilter == KGFX_TEXTURE_FILTER_NEAREST && mipMapFilter == KGFX_TEXTURE_FILTER_NEAREST) {
        *pFilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    } else if (magFilter == KGFX_TEXTURE_FILTER_LINEAR && minFilter == KGFX_TEXTURE_FILTER_LINEAR && mipMapFilter == KGFX_TEXTURE_FILTER_LINEAR) {
        *pFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    } else if (magFilter == KGFX_TEXTURE_FILTER_LINEAR && minFilter == KGFX_TEXTURE_FILTER_LINEAR && mipMapFilter == KGFX_TEXTURE_FILTER_NEAREST) {
        *pFilter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    } else if (magFilter == KGFX_TEXTURE_FILTER_LINEAR && minFilter == KGFX_TEXTURE_FILTER_NEAREST && mipMapFilter == KGFX_TEXTURE_FILTER_NEAREST) {
        *pFilter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    } else if (magFilter == KGFX_TEXTURE_FILTER_LINEAR && minFilter == KGFX_TEXTURE_FILTER_NEAREST && mipMapFilter == KGFX_TEXTURE_FILTER_LINEAR) {
        *pFilter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
    } else if (magFilter == KGFX_TEXTURE_FILTER_NEAREST && minFilter == KGFX_TEXTURE_FILTER_LINEAR && mipMapFilter == KGFX_TEXTURE_FILTER_NEAREST) {
        *pFilter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
    } else if (magFilter == KGFX_TEXTURE_FILTER_NEAREST && minFilter == KGFX_TEXTURE_FILTER_LINEAR && mipMapFilter == KGFX_TEXTURE_FILTER_LINEAR) {
        *pFilter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
    } else if (magFilter == KGFX_TEXTURE_FILTER_NEAREST && minFilter == KGFX_TEXTURE_FILTER_NEAREST && mipMapFilter == KGFX_TEXTURE_FILTER_LINEAR) {
        *pFilter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    } else {
        return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_textureAddressMode(KGFXSampleMode mode, D3D11_TEXTURE_ADDRESS_MODE* pMode) {
    switch (mode) {
        case KGFX_SAMPLE_MODE_REPEAT:
            *pMode = D3D11_TEXTURE_ADDRESS_WRAP;
            break;
        case KGFX_SAMPLE_MODE_MIRROR:
            *pMode = D3D11_TEXTURE_ADDRESS_MIRROR;
            break;
        case KGFX_SAMPLE_MODE_CLAMP:
            *pMode = D3D11_TEXTURE_ADDRESS_CLAMP;
            break;
        case KGFX_SAMPLE_MODE_CLAMP_BORDER:
            *pMode = D3D11_TEXTURE_ADDRESS_BORDER;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_sampleBorder(KGFXSampleBorder border, FLOAT* pX, FLOAT* pY, FLOAT* pZ, FLOAT* pW) {
    switch (border) {
        case KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_FLOAT:
        case KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_INT:
            *pX = 0.0f;
            *pY = 0.0f;
            *pZ = 0.0f;
            *pW = 0.0f;
            break;
        case KGFX_SAMPLE_BORDER_OPAQUE_BLACK_FLOAT:
        case KGFX_SAMPLE_BORDER_OPAQUE_BLACK_INT:
            *pX = 0.0f;
            *pY = 0.0f;
            *pZ = 0.0f;
            *pW = 1.0f;
        case KGFX_SAMPLE_BORDER_OPAQUE_WHITE_FLOAT:
        case KGFX_SAMPLE_BORDER_OPAQUE_WHITE_INT:
            *pX = 1.0f;
            *pY = 1.0f;
            *pZ = 1.0f;
            *pW = 1.0f;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_inputClass(KGFXVertexInputRate rate, D3D11_INPUT_CLASSIFICATION* pClass) {
    switch (rate) {
        case KGFX_VERTEX_INPUT_RATE_PER_VERTEX:
            *pClass = D3D11_INPUT_PER_VERTEX_DATA;
            break;
        case KGFX_VERTEX_INPUT_RATE_PER_INSTANCE:
            *pClass = D3D11_INPUT_PER_INSTANCE_DATA;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_fillMode(KGFXFillMode mode, D3D11_FILL_MODE* pMode) {
    switch (mode) {
        case KGFX_FILL_MODE_FILL:
            *pMode = D3D11_FILL_SOLID;
            break;
        case KGFX_FILL_MODE_WIRE:
            *pMode = D3D11_FILL_WIREFRAME;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_cullMode(KGFXCullMode mode, D3D11_CULL_MODE* pMode) {
    switch (mode) {
        case KGFX_CULL_MODE_NONE:
            *pMode = D3D11_CULL_NONE;
            break;
        case KGFX_CULL_MODE_FRONT:
            *pMode = D3D11_CULL_FRONT;
            break;
        case KGFX_CULL_MODE_BACK:
            *pMode = D3D11_CULL_BACK;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_comparisonFunc(KGFXCompareOp op, D3D11_COMPARISON_FUNC* pFunc) {
    switch (op) {
        case KGFX_COMPARE_OP_NEVER:
            *pFunc = D3D11_COMPARISON_NEVER;
            break;
        case KGFX_COMPARE_OP_LESS:
            *pFunc = D3D11_COMPARISON_LESS;
            break;
        case KGFX_COMPARE_OP_EQUAL:
            *pFunc = D3D11_COMPARISON_EQUAL;
            break;
        case KGFX_COMPARE_OP_GREATER:
            *pFunc = D3D11_COMPARISON_GREATER;
            break;
        case KGFX_COMPARE_OP_LEQUAL:
            *pFunc = D3D11_COMPARISON_LESS_EQUAL;
            break;
        case KGFX_COMPARE_OP_NEQUAL:
            *pFunc = D3D11_COMPARISON_NOT_EQUAL;
            break;
        case KGFX_COMPARE_OP_GEQUAL:
            *pFunc = D3D11_COMPARISON_GREATER_EQUAL;
            break;
        case KGFX_COMPARE_OP_ALWAYS:
            *pFunc = D3D11_COMPARISON_ALWAYS;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_stencilOp(KGFXStencilOp op, D3D11_STENCIL_OP* pOp) {
    switch (op) {
        case KGFX_STENCIL_OP_KEEP:
            *pOp = D3D11_STENCIL_OP_KEEP;
            break;
        case KGFX_STENCIL_OP_ZERO:
            *pOp = D3D11_STENCIL_OP_ZERO;
            break;
        case KGFX_STENCIL_OP_REPLACE:
            *pOp = D3D11_STENCIL_OP_REPLACE;
            break;
        case KGFX_STENCIL_OP_INCREM_CLAMP:
            *pOp = D3D11_STENCIL_OP_INCR_SAT;
            break;
        case KGFX_STENCIL_OP_DECREM_CLAMP:
            *pOp = D3D11_STENCIL_OP_DECR_SAT;
            break;
        case KGFX_STENCIL_OP_INVERT:
            *pOp = D3D11_STENCIL_OP_INVERT;
            break;
        case KGFX_STENCIL_OP_INCREM:
            *pOp = D3D11_STENCIL_OP_INCR;
            break;
        case KGFX_STENCIL_OP_DECREM:
            *pOp = D3D11_STENCIL_OP_DECR;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_stencilOpDesc(const KGFXStencilOpDesc* pDescKGFX, D3D11_DEPTH_STENCILOP_DESC* pDescD3D11) {
    if (!kgfx_d3d11_stencilOp(pDescKGFX->failOp, &pDescD3D11->StencilFailOp)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_stencilOp(pDescKGFX->passOp, &pDescD3D11->StencilPassOp)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_stencilOp(pDescKGFX->failDepthOp, &pDescD3D11->StencilDepthFailOp)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_comparisonFunc(pDescKGFX->compareOp, &pDescD3D11->StencilFunc)) {
        return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_blend(KGFXBlendFactor factor, D3D11_BLEND* pBlend) {
    switch (factor) {
        case KGFX_BLEND_FACTOR_ZERO:
            *pBlend = D3D11_BLEND_ZERO;
            break;
        case KGFX_BLEND_FACTOR_ONE:
            *pBlend = D3D11_BLEND_ONE;
            break;
        case KGFX_BLEND_FACTOR_SRC_COLOR:
            *pBlend = D3D11_BLEND_SRC_COLOR;
            break;
        case KGFX_BLEND_FACTOR_DST_COLOR:
            *pBlend = D3D11_BLEND_DEST_COLOR;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_SRC_COLOR:
            *pBlend = D3D11_BLEND_INV_SRC_COLOR;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_DST_COLOR:
            *pBlend = D3D11_BLEND_INV_DEST_COLOR;
            break;
        case KGFX_BLEND_FACTOR_SRC_ALPHA:
            *pBlend = D3D11_BLEND_SRC_ALPHA;
            break;
        case KGFX_BLEND_FACTOR_DST_ALPHA:
            *pBlend = D3D11_BLEND_DEST_ALPHA;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_SRC_ALPHA:
            *pBlend = D3D11_BLEND_INV_SRC_ALPHA;
            break;
        case KGFX_BLEND_FACTOR_INVERTED_DST_ALPHA:
            *pBlend = D3D11_BLEND_INV_DEST_ALPHA;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_blendOp(KGFXBlendOp op, D3D11_BLEND_OP* pOp) {
    switch (op) {
        case KGFX_BLEND_OP_ADD:
            *pOp = D3D11_BLEND_OP_ADD;
            break;
        case KGFX_BLEND_OP_SUBTRACT:
            *pOp = D3D11_BLEND_OP_SUBTRACT;
            break;
        case KGFX_BLEND_OP_REVERSE_SUBTRACT:
            *pOp = D3D11_BLEND_OP_REV_SUBTRACT;
            break;
        case KGFX_BLEND_OP_MIN:
            *pOp = D3D11_BLEND_OP_MIN;
            break;
        case KGFX_BLEND_OP_MAX:
            *pOp = D3D11_BLEND_OP_MAX;
            break;
        default:
            return KGFX_FALSE;
    }

    return KGFX_TRUE;
}

KGFXBool kgfx_d3d11_rtBlendDesc(KGFXRenderTargetDesc* pDescKGFX, D3D11_RENDER_TARGET_BLEND_DESC* pDescD3D11) {
    pDescD3D11->BlendEnable = pDescKGFX->enableBlending;
    if (!kgfx_d3d11_blend(pDescKGFX->srcColorBlendFactor, &pDescD3D11->SrcBlend)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_blend(pDescKGFX->dstColorBlendFactor, &pDescD3D11->DestBlend)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_blendOp(pDescKGFX->colorBlendOp, &pDescD3D11->BlendOp)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_blend(pDescKGFX->srcAlphaBlendFactor, &pDescD3D11->SrcBlendAlpha)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_blend(pDescKGFX->dstAlphaBlendFactor, &pDescD3D11->DestBlendAlpha)) {
        return KGFX_FALSE;
    }

    if (!kgfx_d3d11_blendOp(pDescKGFX->alphaBlendOp, &pDescD3D11->BlendOpAlpha)) {
        return KGFX_FALSE;
    }

    pDescD3D11->RenderTargetWriteMask = 0;
    if (pDescKGFX->colorWriteMask & KGFX_COLOR_MASK_FLAG_RED) {
        pDescD3D11->RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
    }

    if (pDescKGFX->colorWriteMask & KGFX_COLOR_MASK_FLAG_GREEN) {
        pDescD3D11->RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
    }

    if (pDescKGFX->colorWriteMask & KGFX_COLOR_MASK_FLAG_BLUE) {
        pDescD3D11->RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
    }

    if (pDescKGFX->colorWriteMask & KGFX_COLOR_MASK_FLAG_ALPHA) {
        pDescD3D11->RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
    }

    return KGFX_TRUE;
}

KGFXResult kgfxCreateInstance_d3d11(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance) {
    KGFXInstance_D3D11_t* d3d11Instance = new KGFXInstance_D3D11_t{};
    d3d11Instance->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Instance->flags = flags;
    *pInstance = &d3d11Instance->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyInstance_d3d11(KGFXInstance instance) {
    KGFXInstance_D3D11_t* d3d11Instance = (KGFXInstance_D3D11_t*) instance;
    delete d3d11Instance;
}

KGFXResult kgfxDebugRegisterCallback_d3d11(KGFXInstance instance, KGFXDebugCallbackPFN callback) {
    KGFXInstance_D3D11_t* d3d11Instance = (KGFXInstance_D3D11_t*) instance;
    d3d11Instance->debugCallback = callback;
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxEnumerateAdapters_d3d11(KGFXInstance instance, uint32_t deviceID, KGFXAdapterDetails* pAdapterDetails) {
    IDXGIFactory* factory;
    if (CreateDXGIFactory(IID_PPV_ARGS(&factory)) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    IDXGIAdapter* adapter;
    if (factory->EnumAdapters(deviceID, &adapter) != S_OK) {
        factory->Release();
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    DXGI_ADAPTER_DESC adapterDesc;
    if (adapter->GetDesc(&adapterDesc) != S_OK) {
        adapter->Release();
        factory->Release();
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    pAdapterDetails->type = adapterDesc.DedicatedVideoMemory > 0 ? KGFX_ADAPTER_TYPE_DEDICATED_GPU : KGFX_ADAPTER_TYPE_INTEGRATED_GPU;

    switch (adapterDesc.VendorId) {
        case 0x10DE:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_NVIDIA;
            break;
        case 0x1002:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_AMD;
            break;
        case 0x8086:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_INTEL;
            break;
        default:
            pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_UNKNOWN;
            break;
    }

    pAdapterDetails->maxTextureDimensions[0] = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    pAdapterDetails->maxTextureDimensions[1] = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    pAdapterDetails->maxTextureDimensions[2] = D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;

    pAdapterDetails->maxFramebufferDimensions[0] = pAdapterDetails->maxTextureDimensions[0];
    pAdapterDetails->maxFramebufferDimensions[1] = pAdapterDetails->maxTextureDimensions[1];
    pAdapterDetails->maxFramebufferDimensions[2] = pAdapterDetails->maxTextureDimensions[2];

    pAdapterDetails->maxViewportDimensions[0] = D3D11_VIEWPORT_BOUNDS_MAX;
    pAdapterDetails->maxViewportDimensions[1] = D3D11_VIEWPORT_BOUNDS_MAX;

    pAdapterDetails->maxViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    pAdapterDetails->maxScissorRects = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

    /* TODO: */

    WideCharToMultiByte(CP_ACP, 0, adapterDesc.Description, -1, (LPSTR) pAdapterDetails->name, sizeof(pAdapterDetails->name), NULL, NULL);
    
    adapter->Release();
    factory->Release();
    return KGFX_RESULT_ENUMERATION_IN_PROGRESS;
}

KGFXResult kgfxCreateDevice_d3d11(KGFXInstance instance, uint32_t deviceID, KGFXDevice* pDevice) {
    IDXGIFactory* factory;
    if (CreateDXGIFactory(IID_PPV_ARGS(&factory)) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    IDXGIAdapter* adapter;
    if (factory->EnumAdapters(deviceID, &adapter) != S_OK) {
        factory->Release();
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    KGFXInstance_D3D11_t* d3d11Instance = (KGFXInstance_D3D11_t*) instance;

    UINT flags = 0;
    if (d3d11Instance->flags & KGFX_INSTANCE_CREATE_FLAG_VALIDATION || d3d11Instance->flags & KGFX_INSTANCE_CREATE_FLAG_DEBUG) {
        flags |= D3D11_CREATE_DEVICE_DEBUG;
    }

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    if (D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, flags, featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &device, NULL, &context) != S_OK) {
        adapter->Release();
        factory->Release();
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    if (d3d11Instance->flags & KGFX_INSTANCE_CREATE_FLAG_VALIDATION || d3d11Instance->flags & KGFX_INSTANCE_CREATE_FLAG_DEBUG) {
        ID3D11InfoQueue* info;
        device->QueryInterface(IID_PPV_ARGS(&info));
        info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
        info->Release();

        HMODULE dxgidebug = LoadLibraryA("dxgidebug.dll");
        if (dxgidebug != NULL) {
            HRESULT (WINAPI *dxgiGetDebugInterface)(REFIID riid, void** ppDebug);
            *((FARPROC*) &dxgiGetDebugInterface) = GetProcAddress(dxgidebug, "DXGIGetDebugInterface");

            IDXGIInfoQueue* dxgiinfo;
            dxgiGetDebugInterface(IID_PPV_ARGS(&dxgiinfo));
            dxgiinfo->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE);
            dxgiinfo->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE);
            dxgiinfo->Release();
        }
    }
    
    KGFXDevice_D3D11_t* d3d11Device = new KGFXDevice_D3D11_t{};
    d3d11Device->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Device->obj.instance = instance;
    d3d11Device->adapterID = deviceID;
    d3d11Device->d3d11.device = device;
    d3d11Device->d3d11.context = context;
    
    *pDevice = &d3d11Device->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyDevice_d3d11(KGFXDevice device) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    d3d11Device->d3d11.context->Release();
    d3d11Device->d3d11.device->Release();
    delete d3d11Device;
}

KGFXResult kgfxCreateBuffer_d3d11(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = (UINT) size;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    if (usage & KGFX_BUFFER_USAGE_VERTEX_BUFFER) {
        desc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;
    }

    if (usage & KGFX_BUFFER_USAGE_INDEX_BUFFER) {
        desc.BindFlags |= D3D11_BIND_INDEX_BUFFER;
    }

    if (usage & KGFX_BUFFER_USAGE_UNIFORM_BUFFER) {
        desc.BindFlags |= D3D11_BIND_CONSTANT_BUFFER;
    }

    if (usage & KGFX_BUFFER_USAGE_STORAGE_BUFFER) {
        desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
    }

    if (usage & KGFX_BUFFER_USAGE_TRANSFER_SRC) {
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
    }

    if (usage & KGFX_BUFFER_USAGE_TRANSFER_DST) {
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
    }

    if (location == KGFX_RESOURCE_LOCATION_DEVICE) {
        desc.Usage = D3D11_USAGE_DEFAULT;
    } else if (location == KGFX_RESOURCE_LOCATION_HOST) {
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
    } else if (location == KGFX_RESOURCE_LOCATION_SHARED) {
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    }

    ID3D11Buffer* buffer;
    if (d3d11Device->d3d11.device->CreateBuffer(&desc, NULL, &buffer) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    KGFXBuffer_D3D11_t* d3d11Buffer = new KGFXBuffer_D3D11_t{};
    d3d11Buffer->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Buffer->device = device;
    d3d11Buffer->size = size;
    d3d11Buffer->usage = usage;
    d3d11Buffer->location = location;
    d3d11Buffer->d3d11.buffer = buffer;
    d3d11Buffer->d3d11.desc = desc;
    
    *pBuffer = &d3d11Buffer->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyBuffer_d3d11(KGFXBuffer buffer) {
    KGFXBuffer_D3D11_t* d3d11Buffer = (KGFXBuffer_D3D11_t*) buffer;
    d3d11Buffer->d3d11.buffer->Release();
    delete d3d11Buffer;
}

KGFXResult kgfxMapBuffer_d3d11(KGFXBuffer buffer, void** ppData) {
    KGFXBuffer_D3D11_t* d3d11Buffer = (KGFXBuffer_D3D11_t*) buffer;
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) d3d11Buffer->device;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (d3d11Device->d3d11.context->Map(d3d11Buffer->d3d11.buffer, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    *ppData = mappedResource.pData;
    return KGFX_RESULT_SUCCESS;
}

void kgfxUnmapBuffer_d3d11(KGFXBuffer buffer) {
    KGFXBuffer_D3D11_t* d3d11Buffer = (KGFXBuffer_D3D11_t*) buffer;
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) d3d11Buffer->device;
    d3d11Device->d3d11.context->Unmap(d3d11Buffer->d3d11.buffer, 0);
}

KGFXResult kgfxUploadBuffer_d3d11(KGFXBuffer buffer, const void* pData, uint64_t size) {
    KGFXBuffer_D3D11_t* d3d11Buffer = (KGFXBuffer_D3D11_t*) buffer;
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) d3d11Buffer->device;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (d3d11Device->d3d11.context->Map(d3d11Buffer->d3d11.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    memcpy(mappedResource.pData, pData, size);
    d3d11Device->d3d11.context->Unmap(d3d11Buffer->d3d11.buffer, 0);
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxDownloadBuffer_d3d11(KGFXBuffer buffer, void* pData, uint64_t size) {
    KGFXBuffer_D3D11_t* d3d11Buffer = (KGFXBuffer_D3D11_t*) buffer;
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) d3d11Buffer->device;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (d3d11Device->d3d11.context->Map(d3d11Buffer->d3d11.buffer, 0, D3D11_MAP_READ, 0, &mappedResource) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    memcpy(pData, mappedResource.pData, size);
    d3d11Device->d3d11.context->Unmap(d3d11Buffer->d3d11.buffer, 0);
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxCreateTexture_d3d11(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    if (pTextureDesc->dimensionType == KGFX_DIMENSION_TYPE_1D) {
        return KGFX_RESULT_ERROR_UNSUPPORTED;
    } else if (pTextureDesc->dimensionType == KGFX_DIMENSION_TYPE_2D) {
        D3D11_TEXTURE2D_DESC desc;
        desc.Width = pTextureDesc->width;
        desc.Height = pTextureDesc->height;
        desc.MipLevels = pTextureDesc->mipMapLevels;
        desc.ArraySize = pTextureDesc->layers;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        if (!kgfx_d3d11_dxgiFormat(pTextureDesc->format, &desc.Format)) {
            return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
        }

        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        
        if (pTextureDesc->usage & KGFX_TEXTURE_USAGE_RENDER_TARGET) {
            desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
        }
        
        if (pTextureDesc->usage & KGFX_TEXTURE_USAGE_DEPTH_STENCIL_TARGET) {
            desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
        }
        
        if (pTextureDesc->usage & KGFX_TEXTURE_USAGE_UNIFORM_TEXTURE) {
            desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
        }
        
        if (pTextureDesc->usage & KGFX_TEXTURE_USAGE_STORAGE_TEXTURE) {
            desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        }
        
        if (pTextureDesc->usage & KGFX_TEXTURE_USAGE_TRANSFER_SRC) {
            desc.Usage = D3D11_USAGE_STAGING;
            desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
        }
        
        if (pTextureDesc->usage & KGFX_TEXTURE_USAGE_TRANSFER_DST) {
            desc.Usage = D3D11_USAGE_STAGING;
            desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
        }
        
        ID3D11Texture2D* texture;
        if (d3d11Device->d3d11.device->CreateTexture2D(&desc, NULL, &texture) != S_OK) {
            return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
        }
        
        KGFXTexture_D3D11_t* d3d11Texture = new KGFXTexture_D3D11_t{};
        d3d11Texture->obj.api = KGFX_INSTANCE_API_D3D11;
        d3d11Texture->device = device;
        d3d11Texture->width = pTextureDesc->width;
        d3d11Texture->height = pTextureDesc->height;
        d3d11Texture->depth = 1;
        d3d11Texture->format = pTextureDesc->format;
        d3d11Texture->usage = pTextureDesc->usage;
        d3d11Texture->location = pTextureDesc->location;
        d3d11Texture->d3d11.texture = texture;
        
        *pTexture = &d3d11Texture->obj;
        return KGFX_RESULT_SUCCESS;
    }

    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyTexture_d3d11(KGFXTexture texture) {
    KGFXTexture_D3D11_t* d3d11Texture = (KGFXTexture_D3D11_t*) texture;
    d3d11Texture->d3d11.texture->Release();
    delete d3d11Texture;
}

KGFXResult kgfxUploadTexture_d3d11(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    KGFXTexture_D3D11_t* d3d11Texture = (KGFXTexture_D3D11_t*) texture;
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) d3d11Texture->device;

    D3D11_BOX box;
    box.left = pTransferDesc->textureX;
    box.top = pTransferDesc->textureY;
    box.front = pTransferDesc->textureZ;
    box.right = pTransferDesc->textureX + pTransferDesc->textureWidth;
    box.bottom = pTransferDesc->textureY + pTransferDesc->textureHeight;
    box.back = pTransferDesc->textureZ + pTransferDesc->textureDepth;
    d3d11Device->d3d11.context->UpdateSubresource(d3d11Texture->d3d11.texture, pTransferDesc->textureFirstLayer, &box, pData, 0, 0);
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxDownloadTexture_d3d11(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    KGFXTexture_D3D11_t* d3d11Texture = (KGFXTexture_D3D11_t*) texture;
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) d3d11Texture->device;

    D3D11_BOX box;
    box.left = pTransferDesc->textureX;
    box.top = pTransferDesc->textureY;
    box.front = pTransferDesc->textureZ;
    box.right = pTransferDesc->textureX + pTransferDesc->textureWidth;
    box.bottom = pTransferDesc->textureY + pTransferDesc->textureHeight;
    box.back = pTransferDesc->textureZ + pTransferDesc->textureDepth;
    d3d11Device->d3d11.context->CopySubresourceRegion(d3d11Texture->d3d11.texture, pTransferDesc->textureFirstLayer, pTransferDesc->textureX, pTransferDesc->textureY, pTransferDesc->textureZ, d3d11Texture->d3d11.texture, pTransferDesc->textureFirstLayer, &box);
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxCreateSampler_d3d11(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    D3D11_SAMPLER_DESC desc;
    if (!kgfx_d3d11_sampleFilter(pSamplerDesc->magFilter, pSamplerDesc->minFilter, pSamplerDesc->mipMapFilter, &desc.Filter)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    if (!kgfx_d3d11_textureAddressMode(pSamplerDesc->sampleModeU, &desc.AddressU)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    if (!kgfx_d3d11_textureAddressMode(pSamplerDesc->sampleModeV, &desc.AddressV)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    if (!kgfx_d3d11_textureAddressMode(pSamplerDesc->sampleModeW, &desc.AddressW)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    desc.MipLODBias = 0.0f;
    desc.MaxAnisotropy = (UINT) pSamplerDesc->anisotropy;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    if (!kgfx_d3d11_sampleBorder(pSamplerDesc->border, &desc.BorderColor[0], &desc.BorderColor[1], &desc.BorderColor[2], &desc.BorderColor[3])) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    desc.MinLOD = pSamplerDesc->minLod;
    desc.MaxLOD = pSamplerDesc->maxLod;

    ID3D11SamplerState* sampler;
    if (d3d11Device->d3d11.device->CreateSamplerState(&desc, &sampler) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    KGFXSampler_D3D11_t* d3d11Sampler = new KGFXSampler_D3D11_t{};
    d3d11Sampler->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Sampler->device = device;
    d3d11Sampler->d3d11.sampler = sampler;

    *pSampler = &d3d11Sampler->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroySampler_d3d11(KGFXSampler sampler) {
    KGFXSampler_D3D11_t* d3d11Sampler = (KGFXSampler_D3D11_t*) sampler;
    d3d11Sampler->d3d11.sampler->Release();
    delete d3d11Sampler;
}

KGFXResult kgfxCreateShaderSPIRV_d3d11(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderDXBC_d3d11(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    ID3D11DeviceChild* shader = NULL;
    switch (stage) {
        case KGFX_SHADER_STAGE_VERTEX:
            if (d3d11Device->d3d11.device->CreateVertexShader(pData, size, NULL, (ID3D11VertexShader**) &shader) != S_OK) {
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }
            break;
        case KGFX_SHADER_STAGE_FRAGMENT:
            if (d3d11Device->d3d11.device->CreatePixelShader(pData, size, NULL, (ID3D11PixelShader**) &shader) != S_OK) {
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }
            break;
        case KGFX_SHADER_STAGE_GEOMETRY:
            if (d3d11Device->d3d11.device->CreateGeometryShader(pData, size, NULL, (ID3D11GeometryShader**) &shader) != S_OK) {
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }
            break;
        case KGFX_SHADER_STAGE_TESS_CONTROL:
            if (d3d11Device->d3d11.device->CreateHullShader(pData, size, NULL, (ID3D11HullShader**) &shader) != S_OK) {
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }
            break;
        case KGFX_SHADER_STAGE_TESS_EVAL:
            if (d3d11Device->d3d11.device->CreateDomainShader(pData, size, NULL, (ID3D11DomainShader**) &shader) != S_OK) {
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }
            break;
        case KGFX_SHADER_STAGE_COMPUTE:
            if (d3d11Device->d3d11.device->CreateComputeShader(pData, size, NULL, (ID3D11ComputeShader**) &shader) != S_OK) {
                return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
            }
            break;
        default:
            return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    KGFXShader_D3D11_t* d3d11Shader = new KGFXShader_D3D11_t{};
    d3d11Shader->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Shader->device = device;
    d3d11Shader->stage = stage;
    d3d11Shader->d3d11.shader = shader;

    *pShader = &d3d11Shader->obj;
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxCreateShaderGLSL_d3d11(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderHLSL_d3d11(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;

    char shaderModel[16];
    char stageChar;
    if (stage == KGFX_SHADER_STAGE_VERTEX) {
        stageChar = 'v';
    } else if (stage == KGFX_SHADER_STAGE_FRAGMENT) {
        stageChar = 'p';
    } else if (stage == KGFX_SHADER_STAGE_GEOMETRY) {
        stageChar = 'g';
    } else if (stage == KGFX_SHADER_STAGE_TESS_CONTROL) {
        stageChar = 'h';
    } else if (stage == KGFX_SHADER_STAGE_TESS_EVAL) {
        stageChar = 'd';
    } else if (stage == KGFX_SHADER_STAGE_COMPUTE) {
        stageChar = 'c';
    } else {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    snprintf(shaderModel, sizeof(shaderModel), "%cs_%u_%u", stageChar, shaderModelMajor, shaderModelMinor);

    D3D_SHADER_MACRO* pMacros = NULL;
    if (macroDefineCount != 0) {
        new D3D_SHADER_MACRO[macroDefineCount + 1];
        for (uint32_t i = 0; i < macroDefineCount; i++) {
            pMacros[i].Name = pMacroDefines[i].macroName;
            pMacros[i].Definition = pMacroDefines[i].value;
        }

        pMacros[macroDefineCount].Name = NULL;
        pMacros[macroDefineCount].Definition = NULL;
    }

    ID3DBlob* blob;
    ID3DBlob* error;
    if (D3DCompile(source, length, NULL, pMacros, NULL, entryName, shaderModel, 0, 0, &blob, &error) != S_OK) {
        if (error != NULL) {
            char* message = new char[error->GetBufferSize() + 1];
            memcpy(message, error->GetBufferPointer(), error->GetBufferSize());
            message[error->GetBufferSize()] = '\0';

            KGFXInstance_D3D11_t* d3d11Instance = (KGFXInstance_D3D11_t*) d3d11Device->obj.instance;
            d3d11Instance->debugCallback(d3d11Device->obj.instance, KGFX_DEBUG_SEVERITY_ERROR, KGFX_DEBUG_SOURCE_UNDERLYING_API, message);
            delete[] message;
            error->Release();
        }

        if (pMacros != NULL) {
            delete[] pMacros;
        }
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    if (pMacros != NULL) {
        delete[] pMacros;
    }

    KGFXResult result = kgfxCreateShaderDXBC_d3d11(device, blob->GetBufferPointer(), (uint32_t) blob->GetBufferSize(), entryName, stage, pShader);
    if (stage != KGFX_SHADER_STAGE_VERTEX) {
        blob->Release();
    } else {
        KGFXShader_D3D11_t* d3d11Shader = (KGFXShader_D3D11_t*) *pShader;
        d3d11Shader->d3d11.vblob = blob;
    }

    return result;
}

void kgfxDestroyShader_d3d11(KGFXShader shader) {
    KGFXShader_D3D11_t* d3d11Shader = (KGFXShader_D3D11_t*) shader;
    if (d3d11Shader->stage == KGFX_SHADER_STAGE_VERTEX) {
        d3d11Shader->d3d11.vblob->Release();
    }

    d3d11Shader->d3d11.shader->Release();
    delete d3d11Shader;
}

KGFXResult kgfxCreateGraphicsPipeline_d3d11(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    if (pPipelineDesc->renderTargetCount > 8) {
        return KGFX_RESULT_ERROR_UNSUPPORTED;
    }

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs(pPipelineDesc->vertexInputDesc.attributeCount);
    const char* defaultSemanticName = "INPUT";
    for (uint32_t i = 0; i < pPipelineDesc->vertexInputDesc.attributeCount; i++) {
        if (pPipelineDesc->vertexInputDesc.pAttributes[i].semanticName == NULL) {
            inputElementDescs[i].SemanticName = defaultSemanticName;
            inputElementDescs[i].SemanticIndex = pPipelineDesc->vertexInputDesc.pAttributes[i].location;
        } else {
            inputElementDescs[i].SemanticName = pPipelineDesc->vertexInputDesc.pAttributes[i].semanticName;
            inputElementDescs[i].SemanticIndex = pPipelineDesc->vertexInputDesc.pAttributes[i].semanticIndex;
        }

        if (!kgfx_d3d11_dxgiFormat(pPipelineDesc->vertexInputDesc.pAttributes[i].format, &inputElementDescs[i].Format)) {
            return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
        }

        inputElementDescs[i].InputSlot = pPipelineDesc->vertexInputDesc.pAttributes[i].binding;
        inputElementDescs[i].AlignedByteOffset = pPipelineDesc->vertexInputDesc.pAttributes[i].offset;
        inputElementDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        if (!kgfx_d3d11_inputClass(pPipelineDesc->vertexInputDesc.pBindings[pPipelineDesc->vertexInputDesc.pAttributes[i].binding].inputRate, &inputElementDescs[i].InputSlotClass)) {
            return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
        }

        inputElementDescs[i].InstanceDataStepRate = 0;
    }

    ID3DBlob* vertexBlob = NULL;
    for (uint32_t i = 0; i < pPipelineDesc->shaderCount; i++) {
        KGFXShader_D3D11_t* d3d11Shader = (KGFXShader_D3D11_t*) pPipelineDesc->pShaders[i];
        if (d3d11Shader->stage == KGFX_SHADER_STAGE_VERTEX) {
            vertexBlob = d3d11Shader->d3d11.vblob;
            break;
        }
    }

    if (vertexBlob == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    ID3D11InputLayout* inputLayout;
    if (d3d11Device->d3d11.device->CreateInputLayout(inputElementDescs.data(), (UINT) inputElementDescs.size(), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &inputLayout) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    D3D11_RASTERIZER_DESC rasterizerDesc;
    if (!kgfx_d3d11_fillMode(pPipelineDesc->fillMode, &rasterizerDesc.FillMode)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    if (!kgfx_d3d11_cullMode(pPipelineDesc->cullMode, &rasterizerDesc.CullMode)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    rasterizerDesc.FrontCounterClockwise = !!pPipelineDesc->counterClockwiseFrontFaceWinding;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;

    ID3D11RasterizerState* rasterizerState;
    if (d3d11Device->d3d11.device->CreateRasterizerState(&rasterizerDesc, &rasterizerState) != S_OK) {
        inputLayout->Release();
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    depthStencilDesc.DepthEnable = !!pPipelineDesc->depthStencilDesc.testDepth;
    depthStencilDesc.DepthWriteMask = pPipelineDesc->depthStencilDesc.writeStencil ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    if (!kgfx_d3d11_comparisonFunc(pPipelineDesc->depthStencilDesc.compareOp, &depthStencilDesc.DepthFunc)) {
        inputLayout->Release();
        rasterizerState->Release();
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    depthStencilDesc.StencilEnable = FALSE;
    depthStencilDesc.StencilReadMask = pPipelineDesc->depthStencilDesc.readMaskStencil;
    depthStencilDesc.StencilWriteMask = pPipelineDesc->depthStencilDesc.writeMaskStencil;
    if (!kgfx_d3d11_stencilOpDesc(&pPipelineDesc->depthStencilDesc.frontStencilOpDesc, &depthStencilDesc.FrontFace)) {
        inputLayout->Release();
        rasterizerState->Release();
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    if (!kgfx_d3d11_stencilOpDesc(&pPipelineDesc->depthStencilDesc.backStencilOpDesc, &depthStencilDesc.BackFace)) {
        inputLayout->Release();
        rasterizerState->Release();
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    ID3D11DepthStencilState* depthStencilState;
    if (d3d11Device->d3d11.device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState) != S_OK) {
        inputLayout->Release();
        rasterizerState->Release();
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    for (uint32_t i = 0; i < pPipelineDesc->renderTargetCount; ++i) {
        if (!kgfx_d3d11_rtBlendDesc(&pPipelineDesc->pRenderTargetDescs[i], &blendDesc.RenderTarget[i])) {
            inputLayout->Release();
            rasterizerState->Release();
            depthStencilState->Release();
            return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
        }
    }

    ID3D11BlendState* blendState;
    if (d3d11Device->d3d11.device->CreateBlendState(&blendDesc, &blendState) != S_OK) {
        inputLayout->Release();
        rasterizerState->Release();
        depthStencilState->Release();
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    KGFXGraphicsPipeline_D3D11_t* d3d11Pipeline = new KGFXGraphicsPipeline_D3D11_t{};
    d3d11Pipeline->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Pipeline->device = device;
    d3d11Pipeline->d3d11.inputLayout = inputLayout;
    d3d11Pipeline->d3d11.rasterizerState = rasterizerState;
    d3d11Pipeline->d3d11.depthStencilState = depthStencilState;
    d3d11Pipeline->d3d11.blendState = blendState;

    for (uint32_t i = 0; i < pPipelineDesc->shaderCount; ++i) {
        KGFXShader_D3D11_t* d3d11Shader = (KGFXShader_D3D11_t*) pPipelineDesc->pShaders[i];
        if (d3d11Shader->stage == KGFX_SHADER_STAGE_VERTEX) {
            d3d11Pipeline->d3d11.shaders.vertexShader = (ID3D11VertexShader*) d3d11Shader->d3d11.shader;
        } else if (d3d11Shader->stage == KGFX_SHADER_STAGE_FRAGMENT) {
            d3d11Pipeline->d3d11.shaders.pixelShader = (ID3D11PixelShader*) d3d11Shader->d3d11.shader;
        } else if (d3d11Shader->stage == KGFX_SHADER_STAGE_GEOMETRY) {
            d3d11Pipeline->d3d11.shaders.geometryShader = (ID3D11GeometryShader*) d3d11Shader->d3d11.shader;
        } else if (d3d11Shader->stage == KGFX_SHADER_STAGE_TESS_CONTROL) {
            d3d11Pipeline->d3d11.shaders.hullShader = (ID3D11HullShader*) d3d11Shader->d3d11.shader;
        } else if (d3d11Shader->stage == KGFX_SHADER_STAGE_TESS_EVAL) {
            d3d11Pipeline->d3d11.shaders.domainShader = (ID3D11DomainShader*) d3d11Shader->d3d11.shader;
        }
    }

    *pPipeline = &d3d11Pipeline->obj;
    return KGFX_RESULT_SUCCESS;
}

KGFXResult kgfxResizeGraphicsPipelineRenderTargets_d3d11(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyGraphicsPipeline_d3d11(KGFXGraphicsPipeline pipeline) {
    KGFXGraphicsPipeline_D3D11_t* d3d11Pipeline = (KGFXGraphicsPipeline_D3D11_t*) pipeline;
    d3d11Pipeline->d3d11.inputLayout->Release();
    d3d11Pipeline->d3d11.rasterizerState->Release();
    d3d11Pipeline->d3d11.depthStencilState->Release();
    d3d11Pipeline->d3d11.blendState->Release();
    delete d3d11Pipeline;
}

KGFXResult kgfxCreateCommandPool_d3d11(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyCommandPool_d3d11(KGFXCommandPool commandPool) {}

KGFXResult kgfxCreateCommandList_d3d11(KGFXCommandPool commandPool, KGFXCommandList* pCommandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyCommandList_d3d11(KGFXCommandList commandList) {}
void kgfxResetCommandList_d3d11(KGFXCommandList commandList) {}

KGFXResult kgfxOpenCommandList_d3d11(KGFXCommandList commandList, KGFXBool isOneTime) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCloseCommandList_d3d11(KGFXCommandList commandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxSubmitCommandList_d3d11(KGFXCommandList commandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxCmdBindGraphicsPipeline_d3d11(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline) {}
void kgfxCmdSetViewportAndScissor_d3d11(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors) {}
void kgfxCmdBindRenderTargets_d3d11(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget) {}
void kgfxCmdBeginRendering_d3d11(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue) {}
void kgfxCmdEndRendering_d3d11(KGFXCommandList commandList) {}

void kgfxCmdBindUniformBuffer_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {}
void kgfxCmdBindStorageBuffer_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {}
void kgfxCmdBindUniformTexture_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {}
void kgfxCmdBindStorageTexture_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {}
void kgfxCmdBindSampler_d3d11(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler) {}

void kgfxCmdBindIndexBuffer_d3d11(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType) {}
void kgfxCmdBindVertexBuffers_d3d11(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets) {}
void kgfxCmdDraw_d3d11(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex) {}
void kgfxCmdDrawIndexed_d3d11(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {}
void kgfxCmdDrawIndirect_d3d11(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {}
void kgfxCmdDrawIndexedIndirect_d3d11(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {}

/* (medium) TODO: D3D11 texture copy */
void kgfxCmdCopyTexture_d3d11(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc) {

}

KGFXResult kgfxPresentSwapchain_d3d11(KGFXSwapchain swapchain) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateSwapchainWin32_d3d11(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    KGFXDevice_D3D11_t* d3d11Device = (KGFXDevice_D3D11_t*) device;
    IDXGIFactory* factory;
    if (CreateDXGIFactory(IID_PPV_ARGS(&factory)) != S_OK) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    IDXGISwapChain* swapchain;
    DXGI_SWAP_CHAIN_DESC desc;
    desc.BufferCount = pSwapchainDesc->imageCount;
    desc.BufferDesc.Width = pSwapchainDesc->width;
    desc.BufferDesc.Height = pSwapchainDesc->height;
    desc.BufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    if (!kgfx_d3d11_dxgiFormat(pSwapchainDesc->format, &desc.BufferDesc.Format)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = (HWND) hwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    if (!kgfx_d3d11_dxgiSwapEffect(pSwapchainDesc->presentMode, &desc.SwapEffect)) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    desc.Flags = 0;

    if (factory->CreateSwapChain(d3d11Device->d3d11.device, &desc, &swapchain) != S_OK) {
        factory->Release();
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }

    KGFXSwapchain_D3D11_t* d3d11Swapchain = new KGFXSwapchain_D3D11_t{};
    d3d11Swapchain->obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Swapchain->device = device;
    d3d11Swapchain->hwnd = hwnd;
    d3d11Swapchain->hinstance = hinstance;
    d3d11Swapchain->backbuffer.base.obj.api = KGFX_INSTANCE_API_D3D11;
    d3d11Swapchain->backbuffer.base.device = device;
    d3d11Swapchain->backbuffer.base.width = pSwapchainDesc->width;
    d3d11Swapchain->backbuffer.base.height = pSwapchainDesc->height;
    d3d11Swapchain->backbuffer.base.depth = 1;
    d3d11Swapchain->backbuffer.base.format = pSwapchainDesc->format;
    d3d11Swapchain->backbuffer.base.usage = KGFX_TEXTURE_USAGE_RENDER_TARGET;
    d3d11Swapchain->backbuffer.base.location = KGFX_RESOURCE_LOCATION_DEVICE;
    d3d11Swapchain->backbuffer.base.isBackbuffer = KGFX_TRUE;
    d3d11Swapchain->backbuffer.base.d3d11.texture = nullptr;
    d3d11Swapchain->backbuffer.swapchain = d3d11Swapchain;
    d3d11Swapchain->d3d11.swapchain = swapchain;
    d3d11Swapchain->d3d11.backbuffer = nullptr;

    swapchain->GetBuffer(0, IID_PPV_ARGS(&d3d11Swapchain->d3d11.backbuffer));
    d3d11Device->d3d11.device->CreateRenderTargetView(d3d11Swapchain->d3d11.backbuffer, NULL, &d3d11Swapchain->d3d11.backbufferRTV);

    *pSwapchain = &d3d11Swapchain->obj;
    return KGFX_RESULT_SUCCESS;
}

/* (medium) TODO: D3D11 swapchain recreation */
KGFXResult kgfxRecreateSwapchain_d3d11(KGFXSwapchain swapchain, uint32_t width, uint32_t height) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroySwapchain_d3d11(KGFXSwapchain swapchain) {
    KGFXSwapchain_D3D11_t* d3d11Swapchain = (KGFXSwapchain_D3D11_t*) swapchain;
    d3d11Swapchain->d3d11.swapchain->Release();
    delete d3d11Swapchain;
}

KGFXTexture kgfxGetSwapchainBackbuffer_d3d11(KGFXSwapchain swapchain) {
    KGFXSwapchain_D3D11_t* d3d11Swapchain = (KGFXSwapchain_D3D11_t*) swapchain;
    return &d3d11Swapchain->backbuffer.base.obj;
}

#endif /* #ifdef KGFX_D3D11_IMPLEMENTATION */

#ifdef KGFX_D3D12_IMPLEMENTATION

#ifndef __cplusplus
#error "C++ is required for the D3D11 implementation"
#endif /* #ifndef __cplusplus */

KGFXResult kgfxCreateInstance_d3d12(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyInstance_d3d12(KGFXInstance instance) {

}

KGFXResult kgfxDebugRegisterCallback_d3d12(KGFXInstance instance, KGFXDebugCallbackPFN callback) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxEnumerateAdapters_d3d12(KGFXInstance instance, uint32_t adapterID, KGFXAdapterDetails* pAdapterDetails) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateDevice_d3d12(KGFXInstance instance, uint32_t adapterID, KGFXDevice* pDevice) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyDevice_d3d12(KGFXDevice device) {

}

/* (medium) TODO: D3D12 buffers */
KGFXResult kgfxCreateBuffer_d3d12(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyBuffer_d3d12(KGFXBuffer buffer) {
    return;
}

KGFXResult kgfxMapBuffer_d3d12(KGFXBuffer buffer, void** ppData) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxUnmapBuffer_d3d12(KGFXBuffer buffer) {
    return;
}

KGFXResult kgfxUploadBuffer_d3d12(KGFXBuffer buffer, const void* pData, uint64_t size) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxDownloadBuffer_d3d12(KGFXBuffer buffer, void* pData, uint64_t size) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (medium) TODO: D3D12 textures */
KGFXResult kgfxCreateTexture_d3d12(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyTexture_d3d12(KGFXTexture texture) {
    return;
}

KGFXResult kgfxUploadTexture_d3d12(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxDownloadTexture_d3d12(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (medium) TODO: D3D12 samplers */
KGFXResult kgfxCreateSampler_d3d12(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroySampler_d3d12(KGFXSampler sampler) {
    return;
}

/* (medium) TODO: D3D12 shaders */
KGFXResult kgfxCreateShaderSPIRV_d3d12(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderDXBC_d3d12(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderGLSL_d3d12(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderHLSL_d3d12(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyShader_d3d12(KGFXShader shader) {
    return;
}

/* (medium) TODO: D3D12 graphics pipeline */
KGFXResult kgfxCreateGraphicsPipeline_d3d12(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxResizeGraphicsPipelineRenderTargets_d3d12(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyGraphicsPipeline_d3d12(KGFXGraphicsPipeline pipeline) {
    return;
}

/* (medium) TODO: D3D12 command utilities */
KGFXResult kgfxCreateCommandPool_d3d12(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyCommandPool_d3d12(KGFXCommandPool commandPool) {
    return;
}

/* (medium) TODO: D3D12 command list cmds */
KGFXResult kgfxCreateCommandList_d3d12(KGFXCommandPool commandPool, KGFXCommandList* pCommandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyCommandList_d3d12(KGFXCommandList commandList) {
    return;
}

void kgfxResetCommandList_d3d12(KGFXCommandList commandList) {
    return;
}

KGFXResult kgfxOpenCommandList_d3d12(KGFXCommandList commandList, KGFXBool isOneTime) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCloseCommandList_d3d12(KGFXCommandList commandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxSubmitCommandList_d3d12(KGFXCommandList commandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxCmdBindGraphicsPipeline_d3d12(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline) {
    return;
}

void kgfxCmdSetViewportAndScissor_d3d12(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors) {
    return;
}

void kgfxCmdBindRenderTargets_d3d12(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget) {
    return;
}

void kgfxCmdBeginRendering_d3d12(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue) {
    return;
}

void kgfxCmdEndRendering_d3d12(KGFXCommandList commandList) {
    return;
}

void kgfxCmdBindUniformBuffer_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    return;
}

/* (high) TODO: D3D12 resource binding */
void kgfxCmdBindStorageBuffer_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    return;
}

void kgfxCmdBindUniformTexture_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    return;
}

void kgfxCmdBindStorageTexture_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    return;
}

void kgfxCmdBindSampler_d3d12(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler) {
    return;
}

void kgfxCmdBindIndexBuffer_d3d12(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType) {
    return;
}

void kgfxCmdBindVertexBuffers_d3d12(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets) {
    return;
}

void kgfxCmdDraw_d3d12(KGFXCommandList _d3d12commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex) {
    return;
}

void kgfxCmdDrawIndexed_d3d12(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    return;
}

void kgfxCmdDrawIndirect_d3d12(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    return;
}

void kgfxCmdDrawIndexedIndirect_d3d12(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    return;
}

/* (medium) TODO: D3D12 texture copy */
void kgfxCmdCopyTexture_d3d12(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc) {

}

KGFXResult kgfxPresentSwapchain_d3d12(KGFXSwapchain swapchain) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (medium) TODO: D3D12 swapchain recreation */
KGFXResult kgfxRecreateSwapchain_d3d12(KGFXSwapchain swapchain, uint32_t width, uint32_t height) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (medium) TODO: D3D12 swapchain */
KGFXResult kgfxCreateSwapchainWin32_d3d12(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroySwapchain_d3d12(KGFXSwapchain swapchain) {
    return;
}

/* (medium) TODO: D3D12 swapchain backbuffer */
KGFXTexture kgfxGetSwapchainBackbuffer_d3d12(KGFXSwapchain swapchain) {
    return NULL;
}

#endif /* #ifdef KGFX_D3D12_IMPLEMENTATION */

#ifdef KGFX_METAL_IMPLEMENTATION

#ifndef __OBJC__
#error "Objective-C is required for KGFX Metal implementation"
#endif /* #ifndef __objc */

#import <Metal/Metal.h>
#import <sys/sysctl.h>

typedef struct KGFXInstance_Metal_t {
    KGFXObject obj;
    
    NSArray<id<MTLDevice>>* devices;
} KGFXInstance_Metal_t;

typedef struct KGFXDevice_Metal_t {
    KGFXObject obj;
    
    id<MTLDevice> device;
} KGFXDevice_Metal_t;

KGFXResult kgfxCreateInstance_metal(KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance) {
    KGFXInstance_Metal_t* instance = (KGFXInstance_Metal_t*) malloc(sizeof(KGFXInstance_Metal_t));
    if (instance == NULL) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    /* (low) TODO: Metal debugging / validation */
    
    instance->obj.api = KGFX_INSTANCE_API_METAL;
    instance->obj.instance = NULL;
    instance->devices = MTLCopyAllDevices();
    
    *pInstance = &instance->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyInstance_metal(KGFXInstance instance) {
    KGFXInstance_Metal_t* mtlInstance = (KGFXInstance_Metal_t*) instance;
    [mtlInstance->devices release];
    free(mtlInstance);
}

/* (low) TODO: Metal debug callback */
KGFXResult kgfxDebugRegisterCallback_metal(KGFXInstance instance, KGFXDebugCallbackPFN callback) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxEnumerateAdapters_metal(KGFXInstance instance, uint32_t deviceID, KGFXAdapterDetails* pAdapterDetails) {
    KGFXInstance_Metal_t* mtlInstance = (KGFXInstance_Metal_t*) instance;
    if (deviceID >= [mtlInstance->devices count]) {
        return KGFX_RESULT_ENUMERATION_DONE;
    }
    
    id<MTLDevice> dev = mtlInstance->devices[deviceID];
    pAdapterDetails->type = (dev.lowPower) ? KGFX_ADAPTER_TYPE_INTEGRATED_GPU : KGFX_ADAPTER_TYPE_DEDICATED_GPU;
    if (dev.hasUnifiedMemory) {
        pAdapterDetails->type = KGFX_ADAPTER_TYPE_UMA_GPU;
    }
    
    NSString* nameLower = [dev.name lowercaseString];
    if ([nameLower containsString:@"apple"]) {
        pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_APPLE;
    } else if ([nameLower containsString:@"amd"]) {
        pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_AMD;
    } else if ([nameLower containsString:@"intel"]) {
        pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_INTEL;
    } else if ([nameLower containsString:@"nvidia"]) {
        pAdapterDetails->vendor = KGFX_ADAPTER_VENDOR_NVIDIA;
    } else {
        pAdapterDetails->vendor = KGFX_ADAPTER_TYPE_UNKNOWN;
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
    
    pAdapterDetails->supportsGraphics = KGFX_TRUE;
    pAdapterDetails->supportsCompute = KGFX_TRUE;
    pAdapterDetails->supportsRayTracing = KGFX_TRUE;
    
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
    
    return KGFX_RESULT_ENUMERATION_IN_PROGRESS;
}

KGFXResult kgfxCreateDevice_metal(KGFXInstance instance, uint32_t deviceID, KGFXDevice* pDevice) {
    KGFXInstance_Metal_t* mtlInstance = (KGFXInstance_Metal_t*) instance;
    
    if (deviceID >= [mtlInstance->devices count]) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    KGFXDevice_Metal_t* device = (KGFXDevice_Metal_t*) malloc(sizeof(KGFXDevice_Metal_t));
    if (device == NULL) {
        return KGFX_RESULT_ERROR_TEMPORARY_ERROR;
    }
    
    device->obj.api = KGFX_INSTANCE_API_METAL;
    device->obj.instance = instance;
    device->device = mtlInstance->devices[deviceID];
    
    *pDevice = &device->obj;
    return KGFX_RESULT_SUCCESS;
}

void kgfxDestroyDevice_metal(KGFXDevice device) {
    free(device);
}

/* (low-medium) TODO: Metal buffers */
KGFXResult kgfxCreateBuffer_metal(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyBuffer_metal(KGFXBuffer buffer) {
    return;
}

KGFXResult kgfxMapBuffer_metal(KGFXBuffer buffer, void** ppData) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxUnmapBuffer_metal(KGFXBuffer buffer) {
    return;
}

KGFXResult kgfxUploadBuffer_metal(KGFXBuffer buffer, const void* pData, uint64_t size) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxDownloadBuffer_metal(KGFXBuffer buffer, void* pData, uint64_t size) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (low-medium) TODO: Metal textures */
KGFXResult kgfxCreateTexture_metal(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyTexture_metal(KGFXTexture texture) {
    return;
}

KGFXResult kgfxUploadTexture_metal(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxDownloadTexture_metal(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (low-medium) TODO: Metal samplers */
KGFXResult kgfxCreateSampler_metal(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroySampler_metal(KGFXSampler sampler) {
    return;
}

/* (low-medium) TODO: Metal shaders */
KGFXResult kgfxCreateShaderSPIRV_metal(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderDXBC_metal(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderGLSL_metal(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCreateShaderHLSL_metal(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyShader_metal(KGFXShader shader) {
    return;
}

/* (low-medium) TODO: Metal graphics pipeline */
KGFXResult kgfxCreateGraphicsPipeline_metal(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxResizeGraphicsPipelineRenderTargets_metal(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyGraphicsPipeline_metal(KGFXGraphicsPipeline pipeline) {
    return;
}

/* (low-medium) TODO: Metal command utilities */
KGFXResult kgfxCreateCommandPool_metal(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyCommandPool_metal(KGFXCommandPool commandPool) {
    return;
}

KGFXResult kgfxCreateCommandList_metal(KGFXCommandPool commandPool, KGFXCommandList* pCommandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroyCommandList_metal(KGFXCommandList commandList) {
    return;
}

/* (low-medium) TODO: Metal command list cmds */
void kgfxResetCommandList_metal(KGFXCommandList commandList) {
    return;
}

KGFXResult kgfxOpenCommandList_metal(KGFXCommandList commandList, KGFXBool isOneTime) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxCloseCommandList_metal(KGFXCommandList commandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

KGFXResult kgfxSubmitCommandList_metal(KGFXCommandList commandList) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxCmdBindGraphicsPipeline_metal(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline) {
    return;
}

void kgfxCmdSetViewportAndScissor_metal(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors) {
    return;
}

void kgfxCmdBindRenderTargets_metal(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget) {
    return;
}

void kgfxCmdBeginRendering_metal(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue) {
    return;
}

void kgfxCmdEndRendering_metal(KGFXCommandList commandList) {
    return;
}

void kgfxCmdBindUniformBuffer_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    return;
}

/* (high) TODO: Metal resource binding */
void kgfxCmdBindStorageBuffer_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    return;
}

void kgfxCmdBindUniformTexture_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    return;
}

void kgfxCmdBindStorageTexture_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    return;
}

void kgfxCmdBindSampler_metal(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler) {
    return;
}

void kgfxCmdBindIndexBuffer_metal(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType) {
    return;
}

void kgfxCmdBindVertexBuffers_metal(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets) {
    return;
}

void kgfxCmdDraw_metal(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex) {
    return;
}

void kgfxCmdDrawIndexed_metal(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    return;
}

void kgfxCmdDrawIndirect_metal(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    return;
}

void kgfxCmdDrawIndexedIndirect_metal(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    return;
}

/* (low-medium) TODO: Metal texture copy */
void kgfxCmdCopyTexture_metal(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc) {

}

KGFXResult kgfxPresentSwapchain_metal(KGFXSwapchain swapchain) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (low-medium) TODO: Metal swapchain */
KGFXResult kgfxCreateSwapchainCocoa_metal(KGFXDevice device, void* nsWindow, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

/* (medium) TODO: Metal swapchain recreation */
KGFXResult kgfxRecreateSwapchain_metal(KGFXSwapchain swapchain, uint32_t width, uint32_t height) {
    return KGFX_RESULT_ERROR_UNIMPLEMENTED;
}

void kgfxDestroySwapchain_metal(KGFXSwapchain swapchain) {
    return;
}

/* (low-medium) TODO: Metal swapchain backbuffer */
KGFXTexture kgfxGetSwapchainBackbuffer_metal(KGFXSwapchain swapchain) {
    return NULL;
}

#endif /* #ifdef KGFX_METAL_IMPLEMENTATION */

#ifdef KGFX_IMPLEMENTATION

KGFXResult kgfxCreateInstance(KGFXInstanceAPI api, KGFXInstanceCreateFlagBits flags, KGFXInstance* pInstance) {
    if (pInstance == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateInstance_vulkan(flags, pInstance);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateInstance_d3d12(flags, pInstance);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateInstance_metal(flags, pInstance);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateInstance_d3d11(flags, pInstance);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFX_API void kgfxDestroyInstance(KGFXInstance instance) {
    if (instance == NULL) {
        return;
    }
    
    switch (instance->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyInstance_vulkan(instance);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyInstance_d3d12(instance);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyInstance_metal(instance);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyInstance_d3d11(instance);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxDebugRegisterCallback(KGFXInstance instance, KGFXDebugCallbackPFN callback) {
    if (instance == NULL || callback == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (instance->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxDebugRegisterCallback_vulkan(instance, callback);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxDebugRegisterCallback_d3d12(instance, callback);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxDebugRegisterCallback_metal(instance, callback);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxDebugRegisterCallback_d3d11(instance, callback);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxEnumerateAdapters(KGFXInstance instance, uint32_t adapterID, KGFXAdapterDetails* pAdapterDetails) {
    if (instance == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (instance->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxEnumerateAdapters_vulkan(instance, adapterID, pAdapterDetails);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxEnumerateAdapters_d3d12(instance, adapterID, pAdapterDetails);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxEnumerateAdapters_metal(instance, adapterID, pAdapterDetails);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxEnumerateAdapters_d3d11(instance, adapterID, pAdapterDetails);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateDevice(KGFXInstance instance, uint32_t adapterID, KGFXDevice* pDevice) {
    if (instance == NULL || pDevice == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (instance->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateDevice_vulkan(instance, adapterID, pDevice);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateDevice_d3d12(instance, adapterID, pDevice);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateDevice_metal(instance, adapterID, pDevice);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateDevice_d3d11(instance, adapterID, pDevice);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroyDevice(KGFXDevice device) {
    if (device == NULL) {
        return;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyDevice_vulkan(device);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyDevice_d3d12(device);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyDevice_metal(device);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyDevice_d3d11(device);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxCreateBuffer(KGFXDevice device, uint64_t size, KGFXBufferUsageMask usage, KGFXResourceLocation location, KGFXBuffer* pBuffer) {
    if (device == NULL || usage == 0 || pBuffer == NULL || size == 0) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateBuffer_vulkan(device, size, usage, location, pBuffer);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateBuffer_d3d12(device, size, usage, location, pBuffer);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateBuffer_metal(device, size, usage, location, pBuffer);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateBuffer_d3d11(device, size, usage, location, pBuffer);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroyBuffer(KGFXBuffer buffer) {
    if (buffer == NULL) {
        return;
    }

    switch (buffer->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyBuffer_vulkan(buffer);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyBuffer_d3d12(buffer);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyBuffer_metal(buffer);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyBuffer_d3d11(buffer);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxMapBuffer(KGFXBuffer buffer, void** ppData) {
    if (buffer == NULL || ppData == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (buffer->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxMapBuffer_vulkan(buffer, ppData);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxMapBuffer_d3d12(buffer, ppData);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxMapBuffer_metal(buffer, ppData);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxMapBuffer_d3d11(buffer, ppData);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxUnmapBuffer(KGFXBuffer buffer) {
    if (buffer == NULL) {
        return;
    }

    switch (buffer->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxUnmapBuffer_vulkan(buffer);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxUnmapBuffer_d3d12(buffer);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxUnmapBuffer_metal(buffer);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxUnmapBuffer_d3d11(buffer);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxUploadBuffer(KGFXBuffer buffer, const void* pData, uint64_t size) {
    if (buffer == NULL || pData == NULL || size == 0) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (buffer->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxUploadBuffer_vulkan(buffer, pData, size);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxUploadBuffer_d3d12(buffer, pData, size);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxUploadBuffer_metal(buffer, pData, size);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxUploadBuffer_d3d11(buffer, pData, size);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxDownloadBuffer(KGFXBuffer buffer, void* pData, uint64_t size) {
    if (buffer == NULL || pData == NULL || size == 0) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (buffer->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxDownloadBuffer_vulkan(buffer, pData, size);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxDownloadBuffer_d3d12(buffer, pData, size);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxDownloadBuffer_metal(buffer, pData, size);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxDownloadBuffer_d3d11(buffer, pData, size);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateTexture(KGFXDevice device, const KGFXTextureDesc* pTextureDesc, KGFXTexture* pTexture) {
    if (device == NULL || pTextureDesc == NULL || pTexture == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateTexture_vulkan(device, pTextureDesc, pTexture);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateTexture_d3d12(device, pTextureDesc, pTexture);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateTexture_metal(device, pTextureDesc, pTexture);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateTexture_d3d11(device, pTextureDesc, pTexture);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroyTexture(KGFXTexture texture) {
    if (texture == NULL) {
        return;
    }

    switch (texture->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyTexture_vulkan(texture);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyTexture_d3d12(texture);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyTexture_metal(texture);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyTexture_d3d11(texture);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxUploadTexture(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    if (texture == NULL || pData == NULL || size == 0 || pTransferDesc == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (texture->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxUploadTexture_vulkan(texture, pData, size, pTransferDesc);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxUploadTexture_d3d12(texture, pData, size, pTransferDesc);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxUploadTexture_metal(texture, pData, size, pTransferDesc);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxUploadTexture_d3d11(texture, pData, size, pTransferDesc);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxDownloadTexture(KGFXTexture texture, void* pData, uint64_t size, const KGFXTextureTransferDesc* pTransferDesc) {
    if (texture == NULL || pData == NULL || size == 0 || pTransferDesc == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (texture->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxDownloadTexture_vulkan(texture, pData, size, pTransferDesc);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxDownloadTexture_d3d12(texture, pData, size, pTransferDesc);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxDownloadTexture_metal(texture, pData, size, pTransferDesc);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxDownloadTexture_d3d11(texture, pData, size, pTransferDesc);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateSampler(KGFXDevice device, const KGFXSamplerDesc* pSamplerDesc, KGFXSampler* pSampler) {
    if (device == NULL || pSamplerDesc == NULL || pSampler == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateSampler_vulkan(device, pSamplerDesc, pSampler);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateSampler_d3d12(device, pSamplerDesc, pSampler);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateSampler_metal(device, pSamplerDesc, pSampler);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateSampler_d3d11(device, pSamplerDesc, pSampler);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroySampler(KGFXSampler sampler) {
    if (sampler == NULL) {
        return;
    }

    switch (sampler->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroySampler_vulkan(sampler);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroySampler_d3d12(sampler);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroySampler_metal(sampler);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroySampler_d3d11(sampler);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxCreateShaderSPIRV(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    if (device == NULL || pData == NULL || pShader == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateShaderSPIRV_vulkan(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateShaderSPIRV_d3d12(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateShaderSPIRV_metal(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateShaderSPIRV_d3d11(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateShaderDXBC(KGFXDevice device, const void* pData, uint32_t size, const char* entryName, KGFXShaderStage stage, KGFXShader* pShader) {
    if (device == NULL || pData == NULL || pShader == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateShaderDXBC_vulkan(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateShaderDXBC_d3d12(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateShaderDXBC_metal(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateShaderDXBC_d3d11(device, pData, size, entryName, stage, pShader);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateShaderGLSL(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t glslVersion, KGFXShader* pShader) {
    if (device == NULL || source == NULL || pShader == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateShaderGLSL_vulkan(device, source, length, entryName, stage, glslVersion, pShader);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateShaderGLSL_d3d12(device, source, length, entryName, stage, glslVersion, pShader);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateShaderGLSL_metal(device, source, length, entryName, stage, glslVersion, pShader);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateShaderGLSL_d3d11(device, source, length, entryName, stage, glslVersion, pShader);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateShaderHLSL(KGFXDevice device, const char* source, uint32_t length, const char* entryName, KGFXShaderStage stage, uint32_t shaderModelMajor, uint32_t shaderModelMinor, uint32_t macroDefineCount, const KGFXMacroDefineHLSL* pMacroDefines, KGFXShader* pShader) {
    if (device == NULL || source == NULL || pShader == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateShaderHLSL_vulkan(device, source, length, entryName, stage, shaderModelMajor, shaderModelMinor, macroDefineCount, pMacroDefines, pShader);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateShaderHLSL_d3d12(device, source, length, entryName, stage, shaderModelMajor, shaderModelMinor, macroDefineCount, pMacroDefines, pShader);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateShaderHLSL_metal(device, source, length, entryName, stage, shaderModelMajor, shaderModelMinor, macroDefineCount, pMacroDefines, pShader);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateShaderHLSL_d3d11(device, source, length, entryName, stage, shaderModelMajor, shaderModelMinor, macroDefineCount, pMacroDefines, pShader);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroyShader(KGFXShader shader) {
    if (shader == NULL) {
        return;
    }
    
    switch (shader->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyShader_vulkan(shader);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyShader_d3d12(shader);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyShader_metal(shader);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyShader_d3d11(shader);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxCreateGraphicsPipeline(KGFXDevice device, const KGFXGraphicsPipelineDesc* pPipelineDesc, KGFXGraphicsPipeline* pPipeline) {
    if (device == NULL || pPipelineDesc == NULL || pPipeline == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateGraphicsPipeline_vulkan(device, pPipelineDesc, pPipeline);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateGraphicsPipeline_d3d12(device, pPipelineDesc, pPipeline);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateGraphicsPipeline_metal(device, pPipelineDesc, pPipeline);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateGraphicsPipeline_d3d11(device, pPipelineDesc, pPipeline);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxResizeGraphicsPipelineRenderTargets(KGFXGraphicsPipeline pipeline, uint32_t width, uint32_t height) {
    if (pipeline == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (pipeline->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxResizeGraphicsPipelineRenderTargets_vulkan(pipeline, width, height);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxResizeGraphicsPipelineRenderTargets_d3d12(pipeline, width, height);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxResizeGraphicsPipelineRenderTargets_metal(pipeline, width, height);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxResizeGraphicsPipelineRenderTargets_d3d11(pipeline, width, height);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroyGraphicsPipeline(KGFXGraphicsPipeline pipeline) {
    if (pipeline == NULL) {
        return;
    }
    
    switch (pipeline->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyGraphicsPipeline_vulkan(pipeline);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyGraphicsPipeline_d3d12(pipeline);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyGraphicsPipeline_metal(pipeline);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyGraphicsPipeline_d3d11(pipeline);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxCreateCommandPool(KGFXDevice device, uint32_t maxCommandLists, KGFXQueueType queueType, KGFXCommandPool* pCommandPool) {
    if (device == NULL || maxCommandLists == 0 || pCommandPool == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateCommandPool_vulkan(device, maxCommandLists, queueType, pCommandPool);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateCommandPool_d3d12(device, maxCommandLists, queueType, pCommandPool);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateCommandPool_metal(device, maxCommandLists, queueType, pCommandPool);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateCommandPool_d3d11(device, maxCommandLists, queueType, pCommandPool);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNIMPLEMENTED;
    }
}

void kgfxDestroyCommandPool(KGFXCommandPool commandPool) {
    if (commandPool == NULL) {
        return;
    }
    
    switch (commandPool->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyCommandPool_vulkan(commandPool);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyCommandPool_d3d12(commandPool);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyCommandPool_metal(commandPool);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyCommandPool_d3d11(commandPool);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxCreateCommandList(KGFXCommandPool commandPool, KGFXCommandList* pCommandList) {
    if (commandPool == NULL || pCommandList == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }
    
    switch (commandPool->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateCommandList_vulkan(commandPool, pCommandList);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateCommandList_d3d12(commandPool, pCommandList);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateCommandList_metal(commandPool, pCommandList);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateCommandList_d3d11(commandPool, pCommandList);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNIMPLEMENTED;
    }
    
}

void kgfxDestroyCommandList(KGFXCommandList commandList) {
    if (commandList == NULL) {
        return;
    }
    
    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroyCommandList_vulkan(commandList);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroyCommandList_d3d12(commandList);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroyCommandList_metal(commandList);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroyCommandList_d3d11(commandList);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxResetCommandList(KGFXCommandList commandList) {
    if (commandList == NULL) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxResetCommandList_vulkan(commandList);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxResetCommandList_d3d12(commandList);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxResetCommandList_metal(commandList);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxResetCommandList_d3d11(commandList);
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxOpenCommandList(KGFXCommandList commandList, KGFXBool isOneTime) {
    if (commandList == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxOpenCommandList_vulkan(commandList, isOneTime);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxOpenCommandList_d3d12(commandList, isOneTime);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxOpenCommandList_metal(commandList, isOneTime);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxOpenCommandList_d3d11(commandList, isOneTime);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCloseCommandList(KGFXCommandList commandList) {
    if (commandList == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCloseCommandList_vulkan(commandList);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCloseCommandList_d3d12(commandList);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCloseCommandList_metal(commandList);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCloseCommandList_d3d11(commandList);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxSubmitCommandList(KGFXCommandList commandList) {
    if (commandList == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxSubmitCommandList_vulkan(commandList);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxSubmitCommandList_d3d12(commandList);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxSubmitCommandList_metal(commandList);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxSubmitCommandList_d3d11(commandList);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxCmdBindGraphicsPipeline(KGFXCommandList commandList, KGFXGraphicsPipeline pipeline) {
    if (commandList == NULL) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindGraphicsPipeline_vulkan(commandList, pipeline);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindGraphicsPipeline_d3d12(commandList, pipeline);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindGraphicsPipeline_metal(commandList, pipeline);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindGraphicsPipeline_d3d11(commandList, pipeline);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdSetViewportAndScissor(KGFXCommandList commandList, uint32_t viewportAndScissorCount, KGFXViewport* pViewports, KGFXScissor* pScissors) {
    if (commandList == NULL || (viewportAndScissorCount != 0 && (pViewports == NULL || pScissors == NULL))) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdSetViewportAndScissor_vulkan(commandList, viewportAndScissorCount, pViewports, pScissors);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdSetViewportAndScissor_d3d12(commandList, viewportAndScissorCount, pViewports, pScissors);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdSetViewportAndScissor_metal(commandList, viewportAndScissorCount, pViewports, pScissors);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdSetViewportAndScissor_d3d11(commandList, viewportAndScissorCount, pViewports, pScissors);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindRenderTargets(KGFXCommandList commandList, uint32_t renderTargetCount, KGFXTexture* pRenderTargets, KGFXTexture depthStencilTarget) {
    if (commandList == NULL || (renderTargetCount != 0 && pRenderTargets == NULL)) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindRenderTargets_vulkan(commandList, renderTargetCount, pRenderTargets, depthStencilTarget);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindRenderTargets_d3d12(commandList, renderTargetCount, pRenderTargets, depthStencilTarget);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindRenderTargets_metal(commandList, renderTargetCount, pRenderTargets, depthStencilTarget);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindRenderTargets_d3d11(commandList, renderTargetCount, pRenderTargets, depthStencilTarget);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBeginRendering(KGFXCommandList commandList, uint32_t renderTargetClearValueCount, KGFXClearValue* pRenderTargetClearValues, KGFXClearValue* pDepthStencilClearValue) {
    if (commandList == NULL || (renderTargetClearValueCount != 0 && pRenderTargetClearValues == NULL)) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBeginRendering_vulkan(commandList, renderTargetClearValueCount, pRenderTargetClearValues, pDepthStencilClearValue);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBeginRendering_d3d12(commandList, renderTargetClearValueCount, pRenderTargetClearValues, pDepthStencilClearValue);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBeginRendering_metal(commandList, renderTargetClearValueCount, pRenderTargetClearValues, pDepthStencilClearValue);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBeginRendering_d3d11(commandList, renderTargetClearValueCount, pRenderTargetClearValues, pDepthStencilClearValue);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdEndRendering(KGFXCommandList commandList) {
    if (commandList == NULL) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdEndRendering_vulkan(commandList);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdEndRendering_d3d12(commandList);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdEndRendering_metal(commandList);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdEndRendering_d3d11(commandList);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindUniformBuffer(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    if (commandList == NULL || buffer == NULL || size == 0) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindUniformBuffer_vulkan(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindUniformBuffer_d3d12(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindUniformBuffer_metal(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindUniformBuffer_d3d11(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindStorageBuffer(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXBuffer buffer, uint64_t offset, uint64_t size) {
    if (commandList == NULL || buffer == NULL || size == 0) {
        return;
    }
    
    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindStorageBuffer_vulkan(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindStorageBuffer_d3d12(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindStorageBuffer_metal(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindStorageBuffer_d3d11(commandList, binding, buffer, offset, size);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindUniformTexture(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    if (commandList == NULL || texture == NULL) {
        return;
    }
    
    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindUniformTexture_vulkan(commandList, binding, texture);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindUniformTexture_d3d12(commandList, binding, texture);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindUniformTexture_metal(commandList, binding, texture);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindUniformTexture_d3d11(commandList, binding, texture);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindStorageTexture(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXTexture texture) {
    if (commandList == NULL || texture == NULL) {
        return;
    }
    
    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindStorageTexture_vulkan(commandList, binding, texture);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindStorageTexture_d3d12(commandList, binding, texture);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindStorageTexture_metal(commandList, binding, texture);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindStorageTexture_d3d11(commandList, binding, texture);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindSampler(KGFXCommandList commandList, KGFXUniformBinding binding, KGFXSampler sampler) {
    if (commandList == NULL || sampler == NULL) {
        return;
    }
    
    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindSampler_vulkan(commandList, binding, sampler);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindSampler_d3d12(commandList, binding, sampler);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindSampler_metal(commandList, binding, sampler);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindSampler_d3d11(commandList, binding, sampler);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindIndexBuffer(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, KGFXIndexType indexType) {
    if (commandList == NULL || buffer == NULL) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindIndexBuffer_vulkan(commandList, buffer, offset, indexType);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindIndexBuffer_d3d12(commandList, buffer, offset, indexType);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindIndexBuffer_metal(commandList, buffer, offset, indexType);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindIndexBuffer_d3d11(commandList, buffer, offset, indexType);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdBindVertexBuffers(KGFXCommandList commandList, uint32_t firstBinding, uint32_t bindingCount, KGFXBuffer* pBuffers, uint64_t* pOffsets) {
    if (commandList == NULL || bindingCount == 0 || pBuffers == NULL || pOffsets == NULL) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdBindVertexBuffers_vulkan(commandList, firstBinding, bindingCount, pBuffers, pOffsets);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdBindVertexBuffers_d3d12(commandList, firstBinding, bindingCount, pBuffers, pOffsets);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdBindVertexBuffers_metal(commandList, firstBinding, bindingCount, pBuffers, pOffsets);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdBindVertexBuffers_d3d11(commandList, firstBinding, bindingCount, pBuffers, pOffsets);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdDraw(KGFXCommandList commandList, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstIndex) {
    if (commandList == NULL || vertexCount == 0 || instanceCount == 0) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdDraw_vulkan(commandList, vertexCount, instanceCount, firstVertex, firstIndex);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdDraw_d3d12(commandList, vertexCount, instanceCount, firstVertex, firstIndex);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdDraw_metal(commandList, vertexCount, instanceCount, firstVertex, firstIndex);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdDraw_d3d11(commandList, vertexCount, instanceCount, firstVertex, firstIndex);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdDrawIndexed(KGFXCommandList commandList, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    if (commandList == NULL || indexCount == 0 || instanceCount == 0) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdDrawIndexed_vulkan(commandList, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdDrawIndexed_d3d12(commandList, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdDrawIndexed_metal(commandList, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdDrawIndexed_d3d11(commandList, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdDrawIndirect(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    if (commandList == NULL || buffer == NULL || drawCount == 0 || stride == 0) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdDrawIndirect_vulkan(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdDrawIndirect_d3d12(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdDrawIndirect_metal(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdDrawIndirect_d3d11(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdDrawIndexedIndirect(KGFXCommandList commandList, KGFXBuffer buffer, uint64_t offset, uint32_t drawCount, uint32_t stride) {
    if (commandList == NULL || drawCount == 0 || stride == 0) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdDrawIndexedIndirect_vulkan(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdDrawIndexedIndirect_d3d12(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdDrawIndexedIndirect_metal(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdDrawIndexedIndirect_d3d11(commandList, buffer, offset, drawCount, stride);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

void kgfxCmdCopyTexture(KGFXCommandList commandList, KGFXTexture srcTexture, KGFXTexture dstTexture, KGFXTextureCopyDesc* copyDesc) {
    if (commandList == NULL || srcTexture == NULL || dstTexture == NULL || copyDesc == NULL) {
        return;
    }

    switch (commandList->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxCmdCopyTexture_vulkan(commandList, srcTexture, dstTexture, copyDesc);
            break;
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxCmdCopyTexture_d3d12(commandList, srcTexture, dstTexture, copyDesc);
            break;
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxCmdCopyTexture_metal(commandList, srcTexture, dstTexture, copyDesc);
            break;
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxCmdCopyTexture_d3d11(commandList, srcTexture, dstTexture, copyDesc);
            break;
#endif /* KGFX_D3D11 */
        default:
            break;
    }
}

KGFXResult kgfxPresentSwapchain(KGFXSwapchain swapchain) {
    if (swapchain == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (swapchain->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxPresentSwapchain_vulkan(swapchain);
#endif /* KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxPresentSwapchain_d3d12(swapchain);
#endif /* KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxPresentSwapchain_metal(swapchain);
#endif /* KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxPresentSwapchain_d3d11(swapchain);
#endif /* KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

KGFXResult kgfxCreateSwapchainWin32(KGFXDevice device, void* hwnd, void* hinstance, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
#if !(defined(_WIN32) || defined(_WIN64) || defined(__WIN32__))
    return KGFX_RESULT_ERROR_UNSUPPORTED;
#else

    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateSwapchainWin32_vulkan(device, hwnd, hinstance, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxCreateSwapchainWin32_d3d12(device, hwnd, hinstance, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_D3D12 */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxCreateSwapchainWin32_d3d11(device, hwnd, hinstance, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !(defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)) */
}

KGFXResult kgfxCreateSwapchainXlib(KGFXDevice device, void* display, unsigned long window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
#if !defined(__linux__) && !defined(__gnu_linux__)
    return KGFX_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateSwapchainXlib_vulkan(device, display, window, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_VULKAN */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !defined(__linux__) && !defined(__gnu_linux__) */
}

KGFXResult kgfxCreateSwapchainXcb(KGFXDevice device, void* connection, uint32_t window, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
#if !defined(__linux__) && !defined(__gnu_linux__)
    return KGFX_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateSwapchainXcb_vulkan(device, connection, window, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_VULKAN */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !defined(__linux__) && !defined(__gnu_linux__) */
}

KGFXResult kgfxCreateSwapchainWayland(KGFXDevice device, void* display, void* surface, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
#if !defined(__linux__) && !defined(__gnu_linux__)
    return KGFX_RESULT_ERROR_UNSUPPORTED;
#else
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateSwapchainWayland_vulkan(device, display, surface, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_VULKAN */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #if !defined(__linux__) && !defined(__gnu_linux__) */
}

KGFXResult kgfxCreateSwapchainCocoa(KGFXDevice device, void* nsWindow, const KGFXSwapchainDesc* pSwapchainDesc, KGFXSwapchain* pSwapchain) {
#ifndef __APPLE__
    return KGFX_RESULT_ERROR_UNSUPPORTED;
#else
    
    switch (device->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxCreateSwapchainCocoa_vulkan(device, nsWindow, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_VULKAN */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxCreateSwapchainCocoa_metal(device, nsWindow, pSwapchainDesc, pSwapchain);
#endif /* #ifdef KGFX_VULKAN */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
#endif /* #ifndef __APPLE__ */
}

KGFXResult kgfxRecreateSwapchain(KGFXSwapchain swapchain, uint32_t width, uint32_t height) {
    if (swapchain == NULL) {
        return KGFX_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (swapchain->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxRecreateSwapchain_vulkan(swapchain, width, height);
#endif /* #ifdef KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxRecreateSwapchain_d3d12(swapchain, width, height);
#endif /* #ifdef KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxRecreateSwapchain_metal(swapchain, width, height);
#endif /* #ifdef KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxRecreateSwapchain_d3d11(swapchain, width, height);
#endif /* #ifdef KGFX_D3D11 */
        default:
            return KGFX_RESULT_ERROR_UNSUPPORTED;
    }
}

void kgfxDestroySwapchain(KGFXSwapchain swapchain) {
    switch (swapchain->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            kgfxDestroySwapchain_vulkan(swapchain);
            break;
#endif /* #ifdef KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            kgfxDestroySwapchain_d3d12(swapchain);
            break;
#endif /* #ifdef KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            kgfxDestroySwapchain_metal(swapchain);
            break;
#endif /* #ifdef KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            kgfxDestroySwapchain_d3d11(swapchain);
            break;
#endif /* #ifdef KGFX_D3D11 */
        default:
            break;
    }
}

KGFXTexture kgfxGetSwapchainBackbuffer(KGFXSwapchain swapchain) {
    if (swapchain == NULL) {
        return NULL;
    }

    switch (swapchain->api) {
#ifdef KGFX_VULKAN
        case KGFX_INSTANCE_API_VULKAN:
            return kgfxGetSwapchainBackbuffer_vulkan(swapchain);
#endif /* #ifdef KGFX_VULKAN */
#ifdef KGFX_D3D12
        case KGFX_INSTANCE_API_D3D12:
            return kgfxGetSwapchainBackbuffer_d3d12(swapchain);
#endif /* #ifdef KGFX_D3D12 */
#ifdef KGFX_METAL
        case KGFX_INSTANCE_API_METAL:
            return kgfxGetSwapchainBackbuffer_metal(swapchain);
#endif /* #ifdef KGFX_METAL */
#ifdef KGFX_D3D11
        case KGFX_INSTANCE_API_D3D11:
            return kgfxGetSwapchainBackbuffer_d3d11(swapchain);
#endif /* #ifdef KGFX_D3D11 */
        default:
            return NULL;
    }
}

#endif /* KGFX_IMPLEMENTATION */

#ifdef KGFX_PLUSPLUS
} /* namespace ckgfx */

} /* namespace kgfx */

#include <exception>
#include <functional>
#include <string>
#include <cstring>
#include <algorithm>

namespace kgfx {

enum class Result {
    ErrorUnimplemented = ckgfx::KGFX_RESULT_ERROR_UNIMPLEMENTED,
    ErrorTemporaryError = ckgfx::KGFX_RESULT_ERROR_TEMPORARY_ERROR,
    Success = ckgfx::KGFX_RESULT_SUCCESS,
    EnumerationDone = ckgfx::KGFX_RESULT_ENUMERATION_DONE,
    EnumerationInProgress = ckgfx::KGFX_RESULT_ENUMERATION_IN_PROGRESS,
    ErrorUnknown = ckgfx::KGFX_RESULT_ERROR_UNKNOWN,
    ErrorInvalidArgument = ckgfx::KGFX_RESULT_ERROR_INVALID_ARGUMENT,
    ErrorUnsupported = ckgfx::KGFX_RESULT_ERROR_UNSUPPORTED,
};

enum class InstanceAPI {
    Vulkan = ckgfx::KGFX_INSTANCE_API_VULKAN,
    Metal = ckgfx::KGFX_INSTANCE_API_METAL,
    D3D12 = ckgfx::KGFX_INSTANCE_API_D3D12,
};

enum class InstanceCreateFlags {
    None = ckgfx::KGFX_INSTANCE_CREATE_FLAG_NONE,
    Debug = ckgfx::KGFX_INSTANCE_CREATE_FLAG_DEBUG,
    Validation = ckgfx::KGFX_INSTANCE_CREATE_FLAG_VALIDATION,
    Graphical = ckgfx::KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL,
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
    Verbose = ckgfx::KGFX_DEBUG_SEVERITY_VERBOSE,
    Info = ckgfx::KGFX_DEBUG_SEVERITY_INFO,
    Warning = ckgfx::KGFX_DEBUG_SEVERITY_WARNING,
    Error = ckgfx::KGFX_DEBUG_SEVERITY_ERROR,
};

enum class DebugSource {
    KGFX = ckgfx::KGFX_DEBUG_SOURCE_KGFX,
    UnderlyingAPI = ckgfx::KGFX_DEBUG_SOURCE_UNDERLYING_API,
};

enum class AdapterType {
    Unknown = ckgfx::KGFX_ADAPTER_TYPE_UNKNOWN,
    CPU = ckgfx::KGFX_ADAPTER_TYPE_CPU,
    DedicatedGPU = ckgfx::KGFX_ADAPTER_TYPE_DEDICATED_GPU,
    IntegratedGPU = ckgfx::KGFX_ADAPTER_TYPE_INTEGRATED_GPU,
    UmaGPU = ckgfx::KGFX_ADAPTER_TYPE_UMA_GPU,
    Virtual = ckgfx::KGFX_ADAPTER_TYPE_VIRTUAL,
};

enum class AdapterVendor {
    Unknown = ckgfx::KGFX_ADAPTER_VENDOR_UNKNOWN,
    AMD = ckgfx::KGFX_ADAPTER_VENDOR_AMD,
    Intel = ckgfx::KGFX_ADAPTER_VENDOR_INTEL,
    Nvidia = ckgfx::KGFX_ADAPTER_VENDOR_NVIDIA,
    ARM = ckgfx::KGFX_ADAPTER_VENDOR_ARM,
    Qualcomm = ckgfx::KGFX_ADAPTER_VENDOR_QUALCOMM,
    ImgTec = ckgfx::KGFX_ADAPTER_VENDOR_IMGTEC,
    Apple = ckgfx::KGFX_ADAPTER_VENDOR_APPLE,
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
    
    operator ckgfx::KGFXAdapterDetails() {
        return {};
    }
    
    AdapterDetails operator=(ckgfx::KGFXAdapterDetails const& details) {
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
    
    AdapterDetails(ckgfx::KGFXAdapterDetails const& details) :
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
    Error(ckgfx::KGFXResult result) : std::runtime_error(detail::resultToString(static_cast<Result>(result))), result(static_cast<Result>(result)) {}
    
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

/* (low) TODO: KGFX++ implementation */
class Instance {
public:
    Instance(InstanceAPI api, InstanceCreateFlags flags) {
        ckgfx::KGFXResult r = ckgfx::kgfxCreateInstance(static_cast<ckgfx::KGFXInstanceAPI>(api), static_cast<ckgfx::KGFXInstanceCreateFlagBits>(flags), &handle);
        if (r != ckgfx::KGFX_RESULT_SUCCESS) {
            throw Error(r);
        }
    }
    
    ~Instance() {
        ckgfx::kgfxDestroyInstance(handle);
    }
    
    Result debugRegisterCallback(DebugCallbackFunction callback) {
        debugCallbacks[handle] = DebugCallbackPair(this, callback);
        return static_cast<Result>(ckgfx::kgfxDebugRegisterCallback(handle, debugCallback));
    }
    
    Result enumerateAdapters(uint32_t deviceID, AdapterDetails& adapterDetails) {
        ckgfx::KGFXAdapterDetails cAdapterDetails = {};
        Result result = static_cast<Result>(ckgfx::kgfxEnumerateAdapters(handle, deviceID, &cAdapterDetails));
        adapterDetails = cAdapterDetails;
    }
    
private:
    static void debugCallback(ckgfx::KGFXInstance instance, ckgfx::KGFXDebugSeverity severity, ckgfx::KGFXDebugSource source, const char* message) {
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
    
    static std::unordered_map<ckgfx::KGFXInstance, DebugCallbackPair> debugCallbacks;
    ckgfx::KGFXInstance handle;
};

} /* namespace kgfx */
#endif /* #ifdef KGFX_PLUSPLUS */

#endif /* KGFX_H */
