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
#define KGFX_MAKE_VERSION(major, minor, patch) ((u32) (((major << 24) & 0xFF) | ((minor << 12) & 0xFFF) | (patch & 0xFFF)))
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
	KGFX_NULL_OUTPUT_ARGUMENT = 3,
	KGFX_INVALID_CONTEXT = 4,
	KGFX_NOT_IMPLEMENTED = 5,
	KGFX_INVALID_ARGUMENT = 6,
} KGFXresult;

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
} KGFXdatatype;

typedef enum {
	KGFX_VERTEX_INPUT_RATE_VERTEX = 0,
	KGFX_VERTEX_INPUT_RATE_INSTANCE = 1,
} KGFXinputrate;

typedef enum {
	KGFX_BINDPOINT_VERTEX = 0,
	KGFX_BINDPOINT_FRAGMENT = 1,
	KGFX_BINDPOINT_GEOMETRY = 2,
	KGFX_BINDPOINT_COMPUTE = 3,
	KGFX_BINDPOINT_COUNT,
	KGFX_BINDPOINT_MAX = KGFX_BINDPOINT_COMPUTE,
	KGFX_BINDPOINT_MIN = KGFX_BINDPOINT_VERTEX,
} KGFXbindpoint;

typedef enum {
	KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER = 0,
	KGFX_DESCRIPTOR_USAGE_STORAGE_BUFFER = 1,
	KGFX_DESCRIPTOR_USAGE_COUNT,
	KGFX_DESCRIPTOR_USAGE_MAX = KGFX_DESCRIPTOR_USAGE_COUNT - 1,
	KGFX_DESCRIPTOR_USAGE_MIN = KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER,
} KGFXdescriptorusage;

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
	KGFX_BUFFER_USAGE_COUNT,
	KGFX_BUFFER_USAGE_MAX = KGFX_BUFFER_USAGE_COUNT - 1,
	KGFX_BUFFER_USAGE_MIN = KGFX_BUFFER_USAGE_VERTEX_BUFFER,
} KGFXbufferusageflags;

typedef enum {
	KGFX_MESH_BUFFER_BINDPOINT_VERTEX = 0,
	KGFX_MESH_BUFFER_BINDPOINT_INDEX = 1,
	KGFX_MESH_BUFFER_BINDPOINT_COUNT,
	KGFX_MESH_BUFFER_BINDPOINT_MAX = KGFX_MESH_BUFFER_BINDPOINT_COUNT - 1,
	KGFX_MESH_BUFFER_BINDPOINT_MIN = KGFX_MESH_BUFFER_BINDPOINT_VERTEX,
} KGFXmeshbufferbindpoint;

typedef enum {
	KGFX_MESH_TOPOLOGY_POINTS = 0,
	KGFX_MESH_TOPOLOGY_LINES = 1,
	KGFX_MESH_TOPOLOGY_LINE_STRIP = 2,
	KGFX_MESH_TOPOLOGY_TRIANGLES = 3,
	KGFX_MESH_TOPOLOGY_TRIANGLE_STRIP = 4,
	KGFX_MESH_TOPOLOGY_COUNT,
	KGFX_MESH_TOPOLOGY_MAX = KGFX_MESH_TOPOLOGY_COUNT - 1,
	KGFX_MESH_TOPOLOGY_MIN = KGFX_MESH_TOPOLOGY_POINTS,
} KGFXmeshtopology;

#define KGFX_IS_SUCCESSFUL(result) ((result) == KGFX_SUCCESS)

KGFX_DEFINE_HANDLE(KGFXcontext);

/* pipeline related handles */
KGFX_DEFINE_HANDLE(KGFXpipeline);
KGFX_DEFINE_HANDLE(KGFXrenderpass);
KGFX_DEFINE_HANDLE(KGFXshader);

/* buffer related handles */
KGFX_DEFINE_HANDLE(KGFXbuffer);
KGFX_DEFINE_HANDLE(KGFXmesh);
KGFX_DEFINE_HANDLE(KGFXpipelinemesh);
KGFX_DEFINE_HANDLE(KGFXuniformbuffer);

/* pipeline related descriptors */
typedef struct {
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
	KGFXdatatype type;
	KGFXdescriptorusage usage;
} KGFXdescriptorset;

typedef struct {
	KGFXpipelinebinding* pBindings;
	u32 bindingCount;
	KGFXdescriptorset* pDescriptorSets;
	u32 descriptorSetCount;
} KGFXpipelinelayout;

typedef struct {
	KGFXshader* pShaders;
	u32 shaderCount;
	KGFXpipelinelayout layout;
} KGFXpipelinedesc;

/* buffer related structures */
typedef struct {
	KGFXbufferlocation location;
	KGFXbufferusageflags usage;
	u32 size;
	void* pData;
} KGFXbufferdesc;

typedef struct {
	KGFXbuffer buffer;
	KGFXmeshbufferbindpoint bindpoint;
	u32 offset;
} KGFXmeshbuffer;

typedef struct {
	KGFXmeshbuffer* pBuffers;
	u32 bufferCount;
} KGFXmeshdesc;

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
KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context);

/* destroys a kgfx context */
void kgfxDestroyContext(KGFXcontext ctx);

KGFXshader kgfxCreateShader(KGFXcontext ctx, const void* data, u32 size, KGFXshadertype type, KGFXshadermedium medium);

void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader);

KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc);

KGFXpipelinemesh kgfxPipelineAddMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXmesh mesh, u32 binding);

void kgfxPipelineRemoveMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinemesh pipelineMesh);

KGFXuniformbuffer kgfxPipelineBindUniformBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding);

void kgfxPipelineUnbindUniformBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer);

void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline);

KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc);

KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data);

void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer);

KGFXmesh kgfxCreateMesh(KGFXcontext ctx, KGFXmeshdesc meshDesc);

void kgfxDestroyMesh(KGFXcontext ctx, KGFXmesh mesh);

/* returns implementation version */
u32 kgfxGetImplementationVersion();

void kgfxRender(KGFXcontext ctx, KGFXpipeline pipeline);

#ifdef __cplusplus
}
#endif

#endif
