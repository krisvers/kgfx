#ifndef KRISVERS_KGFX_H
#define KRISVERS_KGFX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kgfx_types.h"
#include "kgfx_platform.h"
	
/*
	KGFX versions (32 bit)

	2 nibbles for major
	3 nibbles for minor
	3 nibbles for patch
	1.2.3 = 0x01002003
*/
#define KGFX_MAKE_VERSION(major, minor, patch) ((u32) (((major & 0xFF) << 24) | ((minor & 0xFFF) << 12) | (patch & 0xFFF)))
#define KGFX_HEADER_VERSION 0x01000000
#define KGFX_ANY_MAJOR 0xFF
#define KGFX_ANY_MINOR 0xFFF
#define KGFX_ANY_PATCH 0xFFF
#define KGFX_MAJOR(version) ((version >> 24) & 0xFF)
#define KGFX_MINOR(version) ((version >> 12) & 0xFFF)
#define KGFX_PATCH(version) (version & 0xFFF)
#define KGFX_ANY_VERSION KGFX_MAKE_VERSION(KGFX_ANY_MAJOR, KGFX_ANY_MINOR, KGFX_ANY_PATCH)
#define KGFX_DEFINE_HANDLE(structure) typedef struct structure##_t* structure;
#define KGFX_MAGIC 0x8BF7E6A8
#define KGFX_HANDLE_NULL (0)

#ifndef KGFX_API
#define KGFX_API
#endif

/* platform specifics */
#ifdef KGFX_WINDOWS
typedef struct {
	void* hwnd;
} KGFXwindowWIN;

typedef KGFXwindowWIN KGFXwindow;
#elif KGFX_LINUX
typedef struct {
	void* display;
	unsigned long window;
} KGFXwindowXlib;

typedef KGFXwindowXlib KGFXwindow;
#elif KGFX_MACOS
typedef struct {
	void* window;
	void* contentView;
	void* layer;
} KGFXwindowCocoa;

typedef KGFXwindowCocoa KGFXwindow;
#endif

/* result codes */
typedef enum {
	KGFX_SUCCESS = 0,
	KGFX_GENERIC_ERROR = 1,
	KGFX_VERSION_NOT_SUPPORTED = 2,
	KGFX_VERSION_MAJOR_MISMATCH = 3,
	KGFX_NULL_OUTPUT_ARGUMENT = 4,
	KGFX_INVALID_CONTEXT = 5,
	KGFX_NOT_IMPLEMENTED = 6,
	KGFX_INVALID_ARGUMENT = 7,
	KGFX_SIZE_TOO_LARGE = 8,
} KGFXresult;

typedef enum {
	KGFX_BACKEND_UNKNOWN = 0,
	KGFX_BACKEND_GENERIC = KGFX_BACKEND_UNKNOWN,
	KGFX_BACKEND_VULKAN = 1,
	KGFX_BACKEND_D3D12 = 2,
	KGFX_BACKEND_METAL = 3,
	KGFX_BACKEND_OPENGL = 4,
	KGFX_BACKEND_DUMMY = 255,
	KGFX_BACKEND_COUNT,
	KGFX_BACKEND_MAX = KGFX_BACKEND_COUNT - 1,
	KGFX_BACKEND_MIN = KGFX_BACKEND_UNKNOWN,
} KGFXbackend;

/* pipeline related enumerations */
typedef enum {
	KGFX_SHADERTYPE_VERTEX = 0,
	KGFX_SHADERTYPE_FRAGMENT = 1,
	KGFX_SHADERTYPE_GEOMETRY = 2,
	KGFX_SHADERTYPE_COMPUTE = 3,
	KGFX_SHADERTYPE_COUNT,
	KGFX_SHADERTYPE_MAX = KGFX_SHADERTYPE_COMPUTE,
	KGFX_SHADERTYPE_MIN = KGFX_SHADERTYPE_VERTEX,
} KGFXshadertype;

typedef enum {
	KGFX_MEDIUM_SPIRV = 0,
	KGFX_MEDIUM_GLSL = 1,
	KGFX_MEDIUM_HLSL = 2,
	KGFX_MEDIUM_MSL = 3,
} KGFXshadermedium;

typedef enum {
	KGFX_DATATYPE_FLOAT = 1,
	KGFX_DATATYPE_FLOAT2,
	KGFX_DATATYPE_FLOAT3,
	KGFX_DATATYPE_FLOAT4,
	KGFX_DATATYPE_INT,
	KGFX_DATATYPE_INT2,
	KGFX_DATATYPE_INT3,
	KGFX_DATATYPE_INT4,
	KGFX_DATATYPE_UINT,
	KGFX_DATATYPE_UINT2,
	KGFX_DATATYPE_UINT3,
	KGFX_DATATYPE_UINT4,
	KGFX_DATATYPE_MAT2,
	KGFX_DATATYPE_MAT3,
	KGFX_DATATYPE_MAT4,
	KGFX_DATATYPE_TEXTURE_SAMPLER,
	KGFX_DATATYPE_COUNT,
	KGFX_DATATYPE_MAX = KGFX_DATATYPE_COUNT - 1,
	KGFX_DATATYPE_MIN = KGFX_DATATYPE_FLOAT,
} KGFXdatatype;

typedef enum {
	KGFX_VERTEX_INPUT_RATE_VERTEX = 0,
	KGFX_VERTEX_INPUT_RATE_INSTANCE = 1,
} KGFXinputrate;

typedef enum {
	KGFX_BINDPOINT_VERTEX = 1,
	KGFX_BINDPOINT_FRAGMENT = 2,
	KGFX_BINDPOINT_GEOMETRY = 4,
	KGFX_BINDPOINT_COMPUTE = 8,
	KGFX_BINDPOINT_COUNT,
	KGFX_BINDPOINT_MAX = KGFX_BINDPOINT_COMPUTE,
	KGFX_BINDPOINT_MIN = KGFX_BINDPOINT_VERTEX,
} KGFXbindpoint;

typedef enum {
	KGFX_DESCRIPTOR_USAGE_INVALID = 0,
	KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER = 1,
	KGFX_DESCRIPTOR_USAGE_STORAGE_BUFFER = 2,
	KGFX_DESCRIPTOR_USAGE_TEXTURE = 3,
	KGFX_DESCRIPTOR_USAGE_STORAGE_TEXTURE = 4,
	KGFX_DESCRIPTOR_USAGE_COUNT,
	KGFX_DESCRIPTOR_USAGE_MAX = KGFX_DESCRIPTOR_USAGE_COUNT - 1,
	KGFX_DESCRIPTOR_USAGE_MIN = KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER,
} KGFXdescriptorusage;

typedef enum {
	KGFX_CULLMODE_NONE = 0,
	KGFX_CULLMODE_FRONT = 1,
	KGFX_CULLMODE_BACK = 2,
	KGFX_CULLMODE_COUNT,
	KGFX_CULLMODE_MAX = KGFX_CULLMODE_COUNT - 1,
	KGFX_CULLMODE_MIN = KGFX_CULLMODE_NONE,
} KGFXcullmode;

typedef enum {
	KGFX_FRONTFACE_CCW = 0,
	KGFX_FRONTFACE_CW = 1,
	KGFX_FRONTFACE_COUNT,
	KGFX_FRONTFACE_MAX = KGFX_FRONTFACE_COUNT - 1,
	KGFX_FRONTFACE_MIN = KGFX_FRONTFACE_CCW,
} KGFXfrontface;

typedef enum {
	KGFX_FILLMODE_SOLID = 0,
	KGFX_FILLMODE_LINES = 1,
	KGFX_FILLMODE_COUNT,
	KGFX_FILLMODE_MAX = KGFX_FILLMODE_COUNT - 1,
	KGFX_FILLMODE_MIN = KGFX_FILLMODE_SOLID,
} KGFXfillmode;

typedef enum {
	KGFX_TOPOLOGY_POINTS = 0,
	KGFX_TOPOLOGY_LINES = 1,
	KGFX_TOPOLOGY_TRIANGLES = 2,
	KGFX_TOPOLOGY_COUNT,
	KGFX_TOPOLOGY_MAX = KGFX_TOPOLOGY_COUNT - 1,
	KGFX_TOPOLOGY_MIN = KGFX_TOPOLOGY_POINTS,
} KGFXtopology;

/* buffer related enumerations */
typedef enum {
	KGFX_BUFFER_LOCATION_CPU = 0,
	KGFX_BUFFER_LOCATION_GPU = 1,
} KGFXbufferlocation;

typedef enum {
	KGFX_BUFFER_USAGE_VERTEX_BUFFER = 1,
	KGFX_BUFFER_USAGE_INDEX_BUFFER = 2,
	KGFX_BUFFER_USAGE_UNIFORM_BUFFER = 4,
	KGFX_BUFFER_USAGE_STORAGE_BUFFER = 8,
	KGFX_BUFFER_USAGE_TEXTURE_SRC = 16,
	KGFX_BUFFER_USAGE_COUNT,
	KGFX_BUFFER_USAGE_MAX = KGFX_BUFFER_USAGE_COUNT - 1,
	KGFX_BUFFER_USAGE_MIN = KGFX_BUFFER_USAGE_VERTEX_BUFFER,
} KGFXbufferusageflags;

/* texture related enumerations */
typedef enum {
	KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM = 0,
	KGFX_TEXTURE_FORMAT_R8G8B8A8_SRGB = 1,
	KGFX_TEXTURE_FORMAT_R32G32B32A32_SFLOAT = 2,
	KGFX_TEXTURE_FORMAT_R32G32B32_SFLOAT = 3,
	KGFX_TEXTURE_FORMAT_R32G32_SFLOAT = 4,
	KGFX_TEXTURE_FORMAT_R32_SFLOAT = 5,
	KGFX_TEXTURE_FORMAT_DEPTH = 6,
	KGFX_TEXTURE_FORMAT_COUNT,
	KGFX_TEXTURE_FORMAT_MAX = KGFX_TEXTURE_FORMAT_COUNT - 1,
	KGFX_TEXTURE_FORMAT_MIN = KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM,
} KGFXtextureformat;

typedef enum {
	KGFX_SAMPLER_FILTER_NEAREST = 0,
	KGFX_SAMPLER_FILTER_LINEAR = 1,
	KGFX_SAMPLER_FILTER_COUNT,
	KGFX_SAMPLER_FILTER_MAX = KGFX_SAMPLER_FILTER_COUNT - 1,
	KGFX_SAMPLER_FILTER_MIN = KGFX_SAMPLER_FILTER_NEAREST,
} KGFXsamplerfilter;

typedef enum {
	KGFX_SAMPLER_ADDRESS_MODE_REPEAT = 0,
	KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT = 1,
	KGFX_SAMPLER_ADDRESS_MODE_CLAMP = 2,
	KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_CLAMP = 3,
	KGFX_SAMPLER_ADDRESS_MODE_COUNT,
	KGFX_SAMPLER_ADDRESS_MODE_MAX = KGFX_SAMPLER_ADDRESS_MODE_COUNT - 1,
	KGFX_SAMPLER_ADDRESS_MODE_MIN = KGFX_SAMPLER_ADDRESS_MODE_REPEAT,
} KGFXsampleraddressmode;

#define KGFX_IS_SUCCESSFUL(result) ((result) == KGFX_SUCCESS)

KGFX_DEFINE_HANDLE(KGFXcontext)

/* pipeline related handles */
KGFX_DEFINE_HANDLE(KGFXpipeline)
KGFX_DEFINE_HANDLE(KGFXrenderpass)
KGFX_DEFINE_HANDLE(KGFXshader)

/* command list */
KGFX_DEFINE_HANDLE(KGFXcommandlist)

/* buffer related handles */
KGFX_DEFINE_HANDLE(KGFXbuffer)
KGFX_DEFINE_HANDLE(KGFXtexture)
KGFX_DEFINE_HANDLE(KGFXsampler)

/* pipeline related descriptors */
typedef struct {
	const char* entryName;
	const void* pData;
	u32 size;
	KGFXshadertype type;
	KGFXshadermedium medium;
} KGFXshaderdesc;

typedef struct {
	const char* semanticName;
	KGFXdatatype type;
	u32 location;
} KGFXpipelineattribute;

typedef struct {
	KGFXinputrate inputRate;
	KGFXpipelineattribute* pAttributes;
	u32 attributeCount;
	KGFXbindpoint bindpoint;
	u32 binding;
} KGFXpipelinebinding;

typedef struct {
	KGFXbindpoint bindpoint;
	u32 binding;
	KGFXdescriptorusage usage;
	u32 size;
} KGFXdescriptorsetdesc;

typedef struct {
	KGFXpipelinebinding* pBindings;
	u32 bindingCount;
	KGFXdescriptorsetdesc* pDescriptorSets;
	u32 descriptorSetCount;
} KGFXpipelinelayout;

typedef struct {
	KGFXshader* pShaders;
	u32 shaderCount;
	KGFXpipelinelayout layout;
	KGFXcullmode cullMode;
	KGFXfrontface frontFace;
	KGFXfillmode fillMode;
	KGFXtopology topology;
} KGFXpipelinedesc;

/* buffer related structures */
typedef struct {
	KGFXbufferlocation location;
	KGFXbufferusageflags usage;
	u32 size;
	void* pData;
} KGFXbufferdesc;

typedef struct {
	KGFXtextureformat format;
	u32 width;
	u32 height;
	u32 depth;
} KGFXtexturedesc;

typedef struct {
	KGFXsamplerfilter magFilter;
	KGFXsamplerfilter minFilter;
	KGFXsamplerfilter mipmapMode;
	KGFXsampleraddressmode addressModeU;
	KGFXsampleraddressmode addressModeV;
	KGFXsampleraddressmode addressModeW;
} KGFXsamplerdesc;

/*
	initializes a kgfx context with requested version
	any non KGFX_SUCCESS result is a failure

	the requested version is the oldest version the application requests
	future versions are compatible with the previous versions when used properly

	if the requested version is more recent than the supported version, this function will return a KGFX_VERSION_NOT_SUPPORTED error
	if the requested version is older than the supported version, this function will return a context with the implementation version
		the version is not guaranteed to be the requested version, but it will be equal or higher

	if the requested version has any of the major, minor or patch set to KGFX_ANY_MAJOR, KGFX_ANY_MINOR or KGFX_ANY_PATCH, the version that is equal or higher will be returned
*/
KGFX_API KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context);

/* destroys a kgfx context */
KGFX_API void kgfxDestroyContext(KGFXcontext ctx);

KGFX_API KGFXshader kgfxCreateShader(KGFXcontext ctx, KGFXshaderdesc shaderDesc);

KGFX_API void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader);

KGFX_API KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc);

KGFX_API void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline);

KGFX_API KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc);

KGFX_API KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data);

KGFX_API void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer);

KGFX_API void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer);

KGFX_API KGFXresult kgfxBufferCopy(KGFXcontext ctx, KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset);

KGFX_API u32 kgfxBufferSize(KGFXcontext ctx, KGFXbuffer buffer);

KGFX_API void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer);

KGFX_API KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc);

KGFX_API KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset);

KGFX_API void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture);

KGFX_API KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc);

KGFX_API void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler);

KGFX_API KGFXcommandlist kgfxCreateCommandList(KGFXcontext ctx);

KGFX_API void kgfxCommandReset(KGFXcontext ctx, KGFXcommandlist commandList);

KGFX_API void kgfxCommandBindPipeline(KGFXcontext ctx, KGFXcommandlist commandList, KGFXpipeline pipeline);

KGFX_API void kgfxCommandBindVertexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding);

KGFX_API void kgfxCommandBindIndexBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding);

KGFX_API void kgfxCommandBindDescriptorSetBuffer(KGFXcontext ctx, KGFXcommandlist commandList, KGFXbuffer buffer, u32 binding);

KGFX_API void kgfxCommandBindDescriptorSetTexture(KGFXcontext ctx, KGFXcommandlist commandList, KGFXtexture texture, KGFXsampler sampler, u32 binding);

KGFX_API void kgfxCommandDraw(KGFXcontext ctx, KGFXcommandlist commandList, u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);

KGFX_API void kgfxCommandDrawIndexed(KGFXcontext ctx, KGFXcommandlist commandList, u32 indexCount, u32 instanceCount, u32 firstIndex, s32 vertexOffset, u32 firstInstance);

KGFX_API void kgfxCommandPresent(KGFXcontext ctx, KGFXcommandlist commandList);

KGFX_API void kgfxCommandListSubmit(KGFXcontext ctx, KGFXcommandlist commandList);

KGFX_API void kgfxDestroyCommandList(KGFXcontext ctx, KGFXcommandlist commandList);

/* returns implementation version */
KGFX_API u32 kgfxGetImplementationVersion(void);

/* if pBackends is null, returns the number of supported backends. otherwise it returns the number written to pBackends (less than or equal to count) */
KGFX_API u32 kgfxGetBackends(KGFXbackend* pBackends, u32 count);

#ifdef __cplusplus
}
#endif

#endif
