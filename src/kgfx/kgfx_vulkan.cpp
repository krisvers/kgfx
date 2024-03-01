#define NOMINMAX

#include <kgfx/kgfx.h>

#ifndef NDEBUG
#define KGFX_DEBUG
#define KGFX_VALIDATION
#endif

#ifdef KGFX_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#define SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif KGFX_LINUX
#define VK_USE_PLATFORM_XLIB_KHR
#define SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#elif KGFX_MACOS
#define VK_USE_PLATFORM_MACOS_MVK
#define SURFACE_EXTENSION_NAME VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cstring>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(0, 1, 0)
#define KGFX_VK_TARGET_FRAMES 2

#ifdef KGFX_DEBUG
#define DEBUG_OUT(msg) debugFunc(msg)
#define DEBUG_OUTF(fmt, ...) debugFunc(fmt, __VA_ARGS__)
#else
#define DEBUG_OUT(msg)
#define DEBUG_OUTF(fmt, ...)
#endif

struct Vulkan {
	VkInstance instance;
	VkSurfaceKHR surface;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties physicalDeviceProperties;
	VkDevice device;

	u32 swapchainImageCount = 0;
	u32 swapchainImageCurrent = 0;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	VkSwapchainKHR swapchain;
	VkPresentModeKHR presentMode;
	VkSurfaceFormatKHR swapchainFormat;

	std::vector<KGFXshader> shaders;
	std::vector<KGFXpipeline> pipelines;
	std::vector<KGFXrenderpass> renderPasses;

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	VkExtent2D extent;
	VkViewport viewport;
	VkRect2D scissor;

	u32 graphicsQueueIndex;
	u32 presentQueueIndex;
	u32 transferQueueIndex;
	u32 computeQueueIndex;

	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkQueue transferQueue;
	VkQueue computeQueue;

	KGFXcontext ctx;

	VkResult init();
	void destroy();

	void render(KGFXpipeline pipeline);

	VkResult createSurface(std::function<u32(const VkSurfaceFormatKHR* formats, u32 formatCount)> selectFormatFunc, std::function<u32(const VkPresentModeKHR* modes, u32 modeCount)> selectModeFunc);
	VkResult findPhysicalDevice(std::function<u32(const VkPhysicalDevice* device, u32 deviceCount)> selectFunc);
	VkResult selectQueueIndices();
	VkResult createDevice();
	VkResult createSwapchain();
	KGFXrenderpass createRenderPass();
	VkResult createCommandUtilities();
	VkResult createSyncUtilities();

	VkExtent2D getWindowExtent();
	VkResult recreateSwapchain();

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

	u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
};

struct KGFXcontext_t {
	u32 version;

	KGFXwindow window;
	Vulkan vulkan;
};

struct KGFXshader_t {
	u32 id;
	VkShaderModule module;
	KGFXshadertype type;
};

struct KGFXpipeline_t {
	u32 id;
	VkPipeline pipeline;
	VkPipelineLayout layout;
	KGFXrenderpass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	u32 vertexStride;
	std::vector<KGFXpipelinemesh> meshes;

	std::vector<struct KGFXdescriptorset_t> descriptorSets;
	bool allDescriptorSetsBound;
};

struct KGFXrenderpass_t {
	VkRenderingInfo renderingInfo;
};

struct KGFXbuffer_t {
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkDeviceSize size;
};

struct KGFXmesh_t {
	KGFXbuffer vertexBuffer;
	u32 vertexOffset;
	KGFXbuffer indexBuffer;
	u32 indexOffset;
};

struct KGFXpipelinemesh_t {
	u32 id;
	u32 binding;
	KGFXmesh mesh;
};

struct KGFXuniformbuffer_t {
	u32 id;
	u32 binding;
	u32 offset;
	KGFXbuffer buffer;
};

struct KGFXdescriptorset_t {
	u32 binding;
	KGFXdescriptorusage usage;
	void* desc;
};

struct KGFXtexture_t {
	VkImage image;
	VkImageView view;
	VkFormat format;
	VkExtent3D extent;
	VkDeviceMemory memory;
};

struct KGFXsampler_t {
	VkSampler sampler;
};

struct KGFXpipelinetexture_t {
	u32 id;
	VkImageView imageView;
	VkSampler sampler;
};

static void debugFuncConcat(std::stringstream& stream, std::string& format);

template<typename T, typename... Types>
static void debugFuncConcat(std::stringstream& stream, std::string& format, T& arg, Types&... args);

static void debugFunc(const char* format);
template<typename... Types>
static void debugFunc(const char* format, Types... types);

constexpr u32 datatypeSize(KGFXdatatype datatype) {
	switch (datatype) {
	case KGFX_DATATYPE_FLOAT:
		return sizeof(f32);
	case KGFX_DATATYPE_FLOAT2:
		return sizeof(f32) * 2;
	case KGFX_DATATYPE_FLOAT3:
		return sizeof(f32) * 3;
	case KGFX_DATATYPE_FLOAT4:
		return sizeof(f32) * 4;
	case KGFX_DATATYPE_INT:
		return sizeof(s32);
	case KGFX_DATATYPE_INT2:
		return sizeof(s32) * 2;
	case KGFX_DATATYPE_INT3:
		return sizeof(s32) * 3;
	case KGFX_DATATYPE_INT4:
		return sizeof(s32) * 4;
	case KGFX_DATATYPE_UINT:
		return sizeof(u32);
	case KGFX_DATATYPE_UINT2:
		return sizeof(u32) * 2;
	case KGFX_DATATYPE_UINT3:
		return sizeof(u32) * 3;
	case KGFX_DATATYPE_UINT4:
		return sizeof(u32) * 4;
	case KGFX_DATATYPE_MAT2:
		return sizeof(f32) * 2 * 2;
	case KGFX_DATATYPE_MAT3:
		return sizeof(f32) * 3 * 3;
	case KGFX_DATATYPE_MAT4:
		return sizeof(f32) * 4 * 4;
	}

	return 0;
}

constexpr VkFormat datatypeVkFormat(KGFXdatatype datatype) {
	switch (datatype) {
	case KGFX_DATATYPE_FLOAT:
		return VK_FORMAT_R32_SFLOAT;
	case KGFX_DATATYPE_FLOAT2:
		return VK_FORMAT_R32G32_SFLOAT;
	case KGFX_DATATYPE_FLOAT3:
		return VK_FORMAT_R32G32B32_SFLOAT;
	case KGFX_DATATYPE_FLOAT4:
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case KGFX_DATATYPE_INT:
		return VK_FORMAT_R32_SINT;
	case KGFX_DATATYPE_INT2:
		return VK_FORMAT_R32G32_SINT;
	case KGFX_DATATYPE_INT3:
		return VK_FORMAT_R32G32B32_SINT;
	case KGFX_DATATYPE_INT4:
		return VK_FORMAT_R32G32B32A32_SINT;
	case KGFX_DATATYPE_UINT:
		return VK_FORMAT_R32_UINT;
	case KGFX_DATATYPE_UINT2:
		return VK_FORMAT_R32G32_UINT;
	case KGFX_DATATYPE_UINT3:
		return VK_FORMAT_R32G32B32_UINT;
	case KGFX_DATATYPE_UINT4:
		return VK_FORMAT_R32G32B32A32_UINT;
	case KGFX_DATATYPE_MAT2:
		return VK_FORMAT_UNDEFINED;
	case KGFX_DATATYPE_MAT3:
		return VK_FORMAT_UNDEFINED;
	case KGFX_DATATYPE_MAT4:
		return VK_FORMAT_UNDEFINED;
	}

	return VK_FORMAT_UNDEFINED;
}

constexpr VkFormat textureFormatVkFormat(KGFXtextureformat format) {
	switch (format) {
	case KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case KGFX_TEXTURE_FORMAT_R8G8B8A8_SRGB:
		return VK_FORMAT_R8G8B8A8_SRGB;
	case KGFX_TEXTURE_FORMAT_R32G32B32A32_SFLOAT:
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case KGFX_TEXTURE_FORMAT_R32G32B32_SFLOAT:
		return VK_FORMAT_R32G32B32_SFLOAT;
	case KGFX_TEXTURE_FORMAT_R32G32_SFLOAT:
		return VK_FORMAT_R32G32_SFLOAT;
	case KGFX_TEXTURE_FORMAT_R32_SFLOAT:
		return VK_FORMAT_R32_SFLOAT;
	case KGFX_TEXTURE_FORMAT_DEPTH:
		return VK_FORMAT_D32_SFLOAT;
	}

	return VK_FORMAT_UNDEFINED;
}

constexpr VkBufferUsageFlags bufferUsageVkFlags(KGFXbufferusageflags usage) {
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

	if (usage & KGFX_BUFFER_USAGE_TEXTURE_SRC) {
		flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	}

	return flags;
}

constexpr VkDescriptorType descriptorUsageVkUsage(KGFXdescriptorusage usage) {
	switch (usage) {
	case KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER:
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	case KGFX_DESCRIPTOR_USAGE_STORAGE_BUFFER:
		return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	case KGFX_DESCRIPTOR_USAGE_TEXTURE:
		return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	case KGFX_DESCRIPTOR_USAGE_STORAGE_TEXTURE:
		return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	}

	return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

const VkShaderStageFlags bindpointVkShaderStageFlags(KGFXbindpoint bindpoint) {
	VkShaderStageFlags flags = 0;
	if (bindpoint & KGFX_BINDPOINT_VERTEX) {
		flags |= VK_SHADER_STAGE_VERTEX_BIT;
	}
	if (bindpoint & KGFX_BINDPOINT_FRAGMENT) {
		flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
	}
	if (bindpoint & KGFX_BINDPOINT_GEOMETRY) {
		flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
	}
	if (bindpoint & KGFX_BINDPOINT_COMPUTE) {
		flags |= VK_SHADER_STAGE_COMPUTE_BIT;
	}

	return flags;
}

constexpr VkMemoryPropertyFlags bufferLocationVkMemoryPropertyFlags(KGFXbufferlocation location) {
	switch (location) {
	case KGFX_BUFFER_LOCATION_CPU:
		return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	case KGFX_BUFFER_LOCATION_GPU:
		return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}

	return VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
}

constexpr VkFilter samplerFilterVkFilter(KGFXsamplerfilter filter) {
	switch (filter) {
	case KGFX_SAMPLER_FILTER_NEAREST:
		return VK_FILTER_NEAREST;
	case KGFX_SAMPLER_FILTER_LINEAR:
		return VK_FILTER_LINEAR;
	}

	return VK_FILTER_MAX_ENUM;
}

constexpr VkSamplerMipmapMode samplerFilterVkMipmapMode(KGFXsamplerfilter filter) {
	switch (filter) {
	case KGFX_SAMPLER_FILTER_NEAREST:
		return VK_SAMPLER_MIPMAP_MODE_NEAREST;
	case KGFX_SAMPLER_FILTER_LINEAR:
		return VK_SAMPLER_MIPMAP_MODE_LINEAR;
	}

	return VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
}

constexpr VkSamplerAddressMode samplerAddressModeVkSamplerAddressMode(KGFXsampleraddressmode samplerAddressMode) {
	switch (samplerAddressMode) {
	case KGFX_SAMPLER_ADDRESS_MODE_REPEAT:
		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	case KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	case KGFX_SAMPLER_ADDRESS_MODE_CLAMP:
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	case KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_CLAMP:
		return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
	}

	return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;

}

KGFXresult kgfxCreateContext(u32 version, KGFXwindow window, KGFXcontext* context) {
	if (context == nullptr) {
		DEBUG_OUT("Invalid KGFXcontext out pointer");
		return KGFX_NULL_OUTPUT_ARGUMENT;
	}

	if ((KGFX_MAJOR(version) > KGFX_MAJOR(KGFX_IMPL_VER) && KGFX_MAJOR(version) != KGFX_ANY_MAJOR) || (KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VER) && KGFX_MINOR(version) != KGFX_ANY_MINOR) || (KGFX_PATCH(version) > KGFX_PATCH(KGFX_IMPL_VER) && KGFX_PATCH(version) != KGFX_ANY_PATCH)) {
		DEBUG_OUT("Requested version is not supported");
		return KGFX_VERSION_NOT_SUPPORTED;
	}

	KGFXcontext ctx = new KGFXcontext_t;
	ctx->version = (version == KGFX_ANY_VERSION) ? KGFX_IMPL_VER : version;
	ctx->vulkan.ctx = ctx;
	ctx->window = window;

	VkResult result = ctx->vulkan.init();
	if (result != VK_SUCCESS) {
		delete ctx;
		DEBUG_OUT("Failed to initialize vulkan");
		return KGFX_GENERIC_ERROR;
	}

	*context = ctx;
	DEBUG_OUT("Successfully initialized vulkan and KGFX");
	return KGFX_SUCCESS;
}

void kgfxDestroyContext(KGFXcontext ctx) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->vulkan.destroy();
	delete ctx;
}

u32 kgfxGetImplementationVersion() {
	return KGFX_IMPL_VER;
}

void kgfxRender(KGFXcontext ctx, KGFXpipeline pipeline) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	ctx->vulkan.render(pipeline);
}

KGFXshader kgfxCreateShader(KGFXcontext ctx, KGFXshaderdesc shaderDesc) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.createShader(shaderDesc);
}

void kgfxDestroyShader(KGFXcontext ctx, KGFXshader shader) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (shader == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXshader");
		return;
	}

	ctx->vulkan.destroyShader(shader);
}

KGFXpipeline kgfxCreatePipeline(KGFXcontext ctx, KGFXpipelinedesc pipelineDesc) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.createPipeline(pipelineDesc);
}

void kgfxDestroyPipeline(KGFXcontext ctx, KGFXpipeline pipeline) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	ctx->vulkan.destroyPipeline(pipeline);
}

KGFXpipelinemesh kgfxPipelineAddMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXmesh mesh, u32 binding) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return KGFX_HANDLE_NULL;
	}

	if (mesh == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXmesh");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.pipelineAddMesh(pipeline, mesh, binding);
}

void kgfxPipelineRemoveMesh(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinemesh pipelineMesh) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (pipelineMesh == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipelinemesh");
		return;
	}

	ctx->vulkan.pipelineRemoveMesh(pipeline, pipelineMesh);
}


KGFXuniformbuffer kgfxPipelineBindDescriptorSetBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding, u32 offset) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return KGFX_HANDLE_NULL;
	}

	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.pipelineBindDescriptorSetBuffer(pipeline, buffer, binding, offset);
}

void kgfxPipelineUnbindDescriptorSetBuffer(KGFXcontext ctx, KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (uniformBuffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXuniformbuffer");
		return;
	}

	ctx->vulkan.pipelineUnbindDescriptorSetBuffer(pipeline, uniformBuffer);
}

KGFXpipelinetexture kgfxPipelineBindDescriptorSetTexture(KGFXcontext ctx, KGFXpipeline pipeline, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return KGFX_HANDLE_NULL;
	}

	if (texture == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXtexture");
		return KGFX_HANDLE_NULL;
	}

	if (sampler == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXsampler");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.pipelineBindDescriptorSetTexture(pipeline, texture, sampler, binding);
}

void kgfxPipelineUnbindDescriptorSetTexture(KGFXcontext ctx, KGFXpipeline pipeline, KGFXpipelinetexture pipelineTexture) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (pipeline == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipeline");
		return;
	}

	if (pipelineTexture == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXpipelinetexture");
		return;
	}

	ctx->vulkan.pipelineUnbindDescriptorSetTexture(pipeline, pipelineTexture);
}

KGFXbuffer kgfxCreateBuffer(KGFXcontext ctx, KGFXbufferdesc bufferDesc) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.createBuffer(bufferDesc);
}

KGFXresult kgfxBufferUpload(KGFXcontext ctx, KGFXbuffer buffer, u32 size, void* data) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_INVALID_CONTEXT;
	}

	return ctx->vulkan.uploadBuffer(buffer, size, data);
}

void* kgfxBufferMap(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return nullptr;
	}

	return ctx->vulkan.mapBuffer(buffer);
}

void kgfxBufferUnmap(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	ctx->vulkan.unmapBuffer(buffer);
}

KGFXresult kgfxBufferCopy(KGFXcontext ctx, KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_INVALID_CONTEXT;
	}

	return ctx->vulkan.copyBuffer(dstBuffer, srcBuffer, size, dstOffset, srcOffset);
}

void kgfxDestroyBuffer(KGFXcontext ctx, KGFXbuffer buffer) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (buffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return;
	}

	ctx->vulkan.destroyBuffer(buffer);
}

KGFXmesh kgfxCreateMesh(KGFXcontext ctx, KGFXmeshdesc meshDesc) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.createMesh(meshDesc);
}

void kgfxDestroyMesh(KGFXcontext ctx, KGFXmesh mesh) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (mesh == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXmesh");
		return;
	}

	ctx->vulkan.destroyMesh(mesh);
}

KGFXtexture kgfxCreateTexture(KGFXcontext ctx, KGFXtexturedesc textureDesc) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.createTexture(textureDesc);
}

KGFXresult kgfxCopyBufferToTexture(KGFXcontext ctx, KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_INVALID_CONTEXT;
	}

	if (dstTexture == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXtexture");
		return KGFX_INVALID_ARGUMENT;
	}

	if (srcBuffer == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXbuffer");
		return KGFX_INVALID_ARGUMENT;
	}

	return ctx->vulkan.copyBufferToTexture(dstTexture, srcBuffer, srcOffset);
}

void kgfxDestroyTexture(KGFXcontext ctx, KGFXtexture texture) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (texture == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXtexture");
		return;
	}

	ctx->vulkan.destroyTexture(texture);
}

KGFXsampler kgfxCreateSampler(KGFXcontext ctx, KGFXsamplerdesc samplerDesc) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return KGFX_HANDLE_NULL;
	}

	return ctx->vulkan.createSampler(samplerDesc);
}

void kgfxDestroySampler(KGFXcontext ctx, KGFXsampler sampler) {
	if (ctx == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXcontext");
		return;
	}

	if (sampler == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Invalid KGFXsampler");
		return;
	}

	ctx->vulkan.destroySampler(sampler);
}

VkResult Vulkan::init() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "kgfx";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "kgfx";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	std::vector<const char*> extensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		SURFACE_EXTENSION_NAME,
	};

	std::vector<const char*> layers = {
		#ifdef KGFX_VALIDATION
		"VK_LAYER_KHRONOS_validation",
		#endif
	};

	u32 availableExtensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

	for (u32 i = 0; i < extensions.size(); ++i) {
		const char*& ext = extensions[i];
		bool found = false;
		for (VkExtensionProperties& avail : availableExtensions) {
			if (strcmp(avail.extensionName, ext) != 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			DEBUG_OUTF("Extension {} not found, removing from requested extensions", ext);
			extensions.erase(extensions.begin() + i);
		}
	}

	u32 availablelayerCount;
	vkEnumerateInstanceLayerProperties(&availablelayerCount, nullptr);

	std::vector<VkLayerProperties> availablelayers(availablelayerCount);
	vkEnumerateInstanceLayerProperties(&availablelayerCount, availablelayers.data());

	for (u32 i = 0; i < layers.size(); ++i) {
		const char*& ext = layers[i];
		bool found = false;
		for (VkLayerProperties& avail : availablelayers) {
			if (strcmp(avail.layerName, ext) != 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			DEBUG_OUTF("Layer {} not found, removing from requested layers", ext);
			layers.erase(layers.begin() + i);
		}
	}

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = static_cast<u32>(layers.size());
	createInfo.ppEnabledLayerNames = layers.data();

	VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkInstance");
		return res;
	}

	res = findPhysicalDevice([](const VkPhysicalDevice* device, u32 deviceCount) -> u32 {
		u32 best = std::numeric_limits<u32>::max();
		s32 bestScore = -1;
		VkPhysicalDeviceProperties bestProps;

		for (u32 i = 0; i < deviceCount; ++i) {
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(device[i], &props);

			VkPhysicalDeviceFeatures feats;
			vkGetPhysicalDeviceFeatures(device[i], &feats);

			s32 score = 0;
			if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 5000;
			}

			if (feats.geometryShader) {
				score += 1000;
			}

			score += props.limits.maxImageDimension2D;

			if (score > bestScore) {
				bestScore = score;
				best = i;
				bestProps = props;
			}
		}

		DEBUG_OUTF("VkPhysicalDevice selector: best device is {} ({})", bestProps.deviceName, bestScore);
		return best;
		});

	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to select VkPhysicalDevice");
		return res;
	}

	res = createSurface([](const VkSurfaceFormatKHR* formats, u32 formatCount) -> u32 {
		u32 best = std::numeric_limits<u32>::max();
		s32 bestScore = -1;
		for (u32 i = 0; i < formatCount; ++i) {
			if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				best = i;
				break;
			}
		}

		return best;
	}, [](const VkPresentModeKHR* modes, u32 modeCount) -> u32 {
		return 0;
	});

	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkSurfaceKHR");
		return res;
	}

	res = selectQueueIndices();
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to select queue indices");
		return res;
	}

	res = createDevice();
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkDevice");
		return res;
	}

	res = createSwapchain();
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkSwapchainKHR");
		return res;
	}

	KGFXrenderpass renderPass = createRenderPass();
	if (renderPass == KGFX_HANDLE_NULL) {
		DEBUG_OUT("Failed to create default VkRenderPass");
		return res;
	}

	res = createCommandUtilities();
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create command utilities");
		return res;
	}

	res = createSyncUtilities();
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create sync utilities");
		return res;
	}

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<f32>(extent.width);
	viewport.height = static_cast<f32>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	return VK_SUCCESS;
}

void Vulkan::destroy() {
	vkDeviceWaitIdle(device);

	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroyFence(device, inFlightFence, nullptr);
	vkDestroyCommandPool(device, commandPool, nullptr);
	for (KGFXrenderpass& renderPass : renderPasses) {
		delete renderPass;
	}
	for (u32 i = 0; i < swapchainImageCount; ++i) {
		vkDestroyImageView(device, swapchainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void Vulkan::render(KGFXpipeline pipeline) {
	if (extent.width == 0 || extent.height == 0 || extent.width < surfaceCapabilities.minImageExtent.width || extent.height < surfaceCapabilities.minImageExtent.height || extent.width > surfaceCapabilities.maxImageExtent.width || extent.height > surfaceCapabilities.maxImageExtent.height) {
		extent = getWindowExtent();
		return;
	}
	vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, std::numeric_limits<u64>::max());

	u32 imageIndex;
	VkResult res = vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<u64>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
	if (res == VK_SUBOPTIMAL_KHR || res == VK_ERROR_OUT_OF_DATE_KHR) {
		extent = getWindowExtent();
		if (extent.width == 0 || extent.height == 0 || extent.width < surfaceCapabilities.minImageExtent.width || extent.height < surfaceCapabilities.minImageExtent.height || extent.width > surfaceCapabilities.maxImageExtent.width || extent.height > surfaceCapabilities.maxImageExtent.height) {
			return;
		}
		res = recreateSwapchain();
		if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
			DEBUG_OUT("Failed to recreate swapchain");
		}
		return;
	} else if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to acquire next image");
		return;
	}
	
	vkResetFences(device, 1, &inFlightFence);
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	
	if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS) {
		DEBUG_OUT("Failed to begin command buffer");
		return;
	}

	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcAccessMask = 0;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	imageMemoryBarrier.srcQueueFamilyIndex = 0;
	imageMemoryBarrier.dstQueueFamilyIndex = 0;
	imageMemoryBarrier.image = swapchainImages[imageIndex];
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	
	VkRenderingAttachmentInfo renderingAttachmentInfo = {};
	renderingAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	renderingAttachmentInfo.imageView = swapchainImageViews[imageIndex];
	renderingAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	renderingAttachmentInfo.clearValue = { { { 0, 0, 0, 1 } } };
	renderingAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	renderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	pipeline->renderPass->renderingInfo.colorAttachmentCount = 1;
	pipeline->renderPass->renderingInfo.pColorAttachments = &renderingAttachmentInfo;
	pipeline->renderPass->renderingInfo.renderArea.extent = extent;
	pipeline->renderPass->renderingInfo.renderArea.offset = { 0, 0 };
	vkCmdBeginRendering(commandBuffer, &pipeline->renderPass->renderingInfo);

	viewport.width = static_cast<f32>(extent.width);
	viewport.height = static_cast<f32>(extent.height);

	scissor.extent = extent;

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 1, &pipeline->descriptorSet, 0, nullptr);

	VkDeviceSize offset = 0;
	for (KGFXpipelinemesh& pipelineMesh : pipeline->meshes) {
		if (pipelineMesh->mesh->vertexBuffer != KGFX_HANDLE_NULL) {
			offset = pipelineMesh->mesh->vertexOffset;
			vkCmdBindVertexBuffers(commandBuffer, pipelineMesh->binding, 1, &pipelineMesh->mesh->vertexBuffer->buffer, &offset);
		}
		if (pipelineMesh->mesh->indexBuffer != KGFX_HANDLE_NULL) {
			offset = pipelineMesh->mesh->indexOffset;
			vkCmdBindIndexBuffer(commandBuffer, pipelineMesh->mesh->indexBuffer->buffer, offset, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffer, static_cast<u32>(pipelineMesh->mesh->indexBuffer->size / sizeof(u32)), 1, 0, pipelineMesh->mesh->vertexOffset, 0);
		} else {
			if (pipelineMesh->mesh->vertexBuffer == KGFX_HANDLE_NULL) {
				DEBUG_OUT("[warning]: No vertex or index buffer provided for pipeline mesh");
				continue;
			}
			vkCmdDraw(commandBuffer, static_cast<u32>(pipelineMesh->mesh->vertexBuffer->size / pipeline->vertexStride), 1, pipelineMesh->mesh->vertexOffset, 0);
		}
	}

	vkCmdEndRendering(commandBuffer);

	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = 0;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	imageMemoryBarrier.srcQueueFamilyIndex = 0;
	imageMemoryBarrier.dstQueueFamilyIndex = 0;
	imageMemoryBarrier.image = swapchainImages[imageIndex];
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		DEBUG_OUT("Failed to end command buffer");
		return;
	}

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore;
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
		DEBUG_OUT("Failed to submit draw commands");
		return;
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;

	res = vkQueuePresentKHR(presentQueue, &presentInfo);
	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
		extent = getWindowExtent();
		if (extent.width == 0 || extent.height == 0 || extent.width < surfaceCapabilities.minImageExtent.width || extent.height < surfaceCapabilities.minImageExtent.height || extent.width > surfaceCapabilities.maxImageExtent.width || extent.height > surfaceCapabilities.maxImageExtent.height) {
			return;
		}
		res = recreateSwapchain();
		if (res != VK_SUCCESS || res != VK_SUBOPTIMAL_KHR) {
			DEBUG_OUT("Failed to recreate swapchain");
		}
		return;
	} else if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to present image");
	}

	swapchainImageCurrent = (swapchainImageCurrent + 1) % swapchainImageCount;
}

VkResult Vulkan::createSurface(std::function<u32(const VkSurfaceFormatKHR* formats, u32 formatCount)> selectFormatFunc, std::function<u32(const VkPresentModeKHR* modes, u32 modeCount)> selectModeFunc) {
	VkResult res = VK_SUCCESS;
#ifdef KGFX_WINDOWS
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hinstance = GetModuleHandle(nullptr);
	createInfo.hwnd = reinterpret_cast<HWND>(ctx->window.hwnd);

	res = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
#elif KGFX_LINUX
	VkXlibSurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	createInfo.dpy = reinterpret_cast<Display*>(ctx->window.display);
	createInfo.window = reinterpret_cast<Window>(ctx->window.window);

	res = vkCreateXlibSurfaceKHR(instance, &createInfo, nullptr, &surface);
#elif KGFX_MACOS
	VkMacOSSurfaceCreateInfoMVK createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pView = ctx->window.layer;

	res = vkCreateMacOSSurfaceMVK(instance, &createInfo, nullptr, &surface);
#endif
	if (res != VK_SUCCESS) {
		return res;
	}

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

	u32 formatIndex = selectFormatFunc(formats.data(), formatCount);
	if (formatIndex == std::numeric_limits<u32>::max() || formatIndex >= formats.size()) {
		DEBUG_OUT("Failed to select surface format");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	u32 presentModeIndex = selectModeFunc(presentModes.data(), presentModeCount);
	if (presentModeIndex == std::numeric_limits<u32>::max() || presentModeIndex >= presentModes.size()) {
		DEBUG_OUT("Failed to select present mode");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	swapchainFormat = formats[formatIndex];
	presentMode = presentModes[presentModeIndex];

	extent = surfaceCapabilities.currentExtent;
	scissor = { { 0, 0 }, extent };

	return VK_SUCCESS;
}

VkResult Vulkan::findPhysicalDevice(std::function<u32(const VkPhysicalDevice* device, u32 deviceCount)> selectFunc) {
	u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		DEBUG_OUT("No VkPhysicalDevices found");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	u32 index = selectFunc(devices.data(), static_cast<u32>(devices.size()));
	if (index == std::numeric_limits<u32>::max() || index >= devices.size()) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	physicalDevice = devices[index];
	if (physicalDevice == VK_NULL_HANDLE) {
		DEBUG_OUT("Invalid VkPhysicalDevice selected");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	DEBUG_OUTF("Selected VkPhysicalDevice: \"{}\"", physicalDeviceProperties.deviceName);
	DEBUG_OUTF("  API version: {}.{}.{}", VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion), VK_VERSION_MINOR(physicalDeviceProperties.apiVersion), VK_VERSION_PATCH(physicalDeviceProperties.apiVersion));
	DEBUG_OUTF("  Driver version: {}.{}.{}", VK_VERSION_MAJOR(physicalDeviceProperties.driverVersion), VK_VERSION_MINOR(physicalDeviceProperties.driverVersion), VK_VERSION_PATCH(physicalDeviceProperties.driverVersion));
	DEBUG_OUTF("  Vendor ID: {}", physicalDeviceProperties.vendorID);
	DEBUG_OUTF("  Device ID: {}", physicalDeviceProperties.deviceID);
#ifdef KGFX_DEBUG
	{
		const char* type = (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) ? "software" : (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) ? "discrete" : (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) ? "integrated" : (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) ? "virtual" : (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER) ? "other" : "unknown";
		DEBUG_OUTF("  Device type: {}", type);
	}
	#endif

	return VK_SUCCESS;
}

VkResult Vulkan::selectQueueIndices() {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	graphicsQueueIndex = std::numeric_limits<u32>::max();
	presentQueueIndex = std::numeric_limits<u32>::max();
	transferQueueIndex = std::numeric_limits<u32>::max();
	computeQueueIndex = std::numeric_limits<u32>::max();

	for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsQueueIndex = i;
		}

		if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			transferQueueIndex = i;
		}

		if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			computeQueueIndex = i;
		}

		VkBool32 present = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &present);

		if (present) {
			presentQueueIndex = i;
		}

		if (graphicsQueueIndex != std::numeric_limits<u32>::max() && presentQueueIndex != std::numeric_limits<u32>::max() && transferQueueIndex != std::numeric_limits<u32>::max() && computeQueueIndex != std::numeric_limits<u32>::max()) {
			break;
		}
	}

	if (graphicsQueueIndex == std::numeric_limits<u32>::max() || presentQueueIndex == std::numeric_limits<u32>::max() || transferQueueIndex == std::numeric_limits<u32>::max() || computeQueueIndex == std::numeric_limits<u32>::max()) {
		DEBUG_OUT("Failed to select queue indices");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	return VK_SUCCESS;
}

VkResult Vulkan::createDevice() {
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	
	f32 priority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = {
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr, 0,
			graphicsQueueIndex, 1,
			&priority,
		},
	};

	if (graphicsQueueIndex != presentQueueIndex) {
		queueCreateInfos.push_back({
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr, 0,
			presentQueueIndex, 1,
			&priority,
		});
	}

	if (graphicsQueueIndex != transferQueueIndex && presentQueueIndex != transferQueueIndex) {
		queueCreateInfos.push_back({
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr, 0,
			transferQueueIndex, 1,
			&priority,
		});
	}

	if (graphicsQueueIndex != computeQueueIndex && presentQueueIndex != computeQueueIndex && transferQueueIndex != computeQueueIndex) {
		queueCreateInfos.push_back({
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr, 0,
			computeQueueIndex, 1,
			&priority,
		});
	}

	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
	createInfo.enabledExtensionCount = 1;
	const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	createInfo.ppEnabledExtensionNames = extensions;
	
	VkPhysicalDeviceFeatures2 features = {};
	features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	features.features.samplerAnisotropy = VK_TRUE;

	VkPhysicalDeviceVulkan13Features vk13features = {};
	vk13features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	vk13features.dynamicRendering = VK_TRUE;
	features.pNext = &vk13features;
	createInfo.pNext = &features;

	VkResult res = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkDevice");
		return res;
	}

	vkGetDeviceQueue(device, graphicsQueueIndex, 0, &graphicsQueue);
	vkGetDeviceQueue(device, presentQueueIndex, 0, &presentQueue);
	vkGetDeviceQueue(device, transferQueueIndex, 0, &transferQueue);
	vkGetDeviceQueue(device, computeQueueIndex, 0, &computeQueue);
	return res;
}

VkResult Vulkan::createSwapchain() {
	extent = getWindowExtent();

	if (surfaceCapabilities.maxImageCount == 0) {
		swapchainImageCount = std::max(static_cast<u32>(KGFX_VK_TARGET_FRAMES), surfaceCapabilities.minImageCount);
	} else {
		swapchainImageCount = std::max(std::min(static_cast<u32>(KGFX_VK_TARGET_FRAMES), surfaceCapabilities.maxImageCount), surfaceCapabilities.minImageCount);
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = swapchainImageCount;
	createInfo.imageFormat = swapchainFormat.format;
	createInfo.imageColorSpace = swapchainFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.presentMode = presentMode;
	createInfo.preTransform = surfaceCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.clipped = VK_TRUE;

	VkResult res = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain);
	if (res != VK_SUCCESS) {
		return res;
	}

	swapchainImages.resize(swapchainImageCount);
	swapchainImageViews.resize(swapchainImageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());
	for (u32 i = 0; i < swapchainImageCount; ++i) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapchainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapchainFormat.format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		res = vkCreateImageView(device, &createInfo, nullptr, &swapchainImageViews[i]);
		if (res != VK_SUCCESS) {
			DEBUG_OUT("Failed to create swapchain VkImageView");
			return res;
		}
	}

	return res;
}

KGFXrenderpass Vulkan::createRenderPass() {
	KGFXrenderpass renderPass = new KGFXrenderpass_t;
	renderPass->renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
	renderPass->renderingInfo.pNext = nullptr;
	renderPass->renderingInfo.flags = 0;
	renderPass->renderingInfo.renderArea = { { 0, 0 }, extent };
	renderPass->renderingInfo.layerCount = 1;
	renderPass->renderingInfo.viewMask = 0;
	renderPass->renderingInfo.colorAttachmentCount = 0;
	renderPass->renderingInfo.pColorAttachments = nullptr;
	renderPass->renderingInfo.pDepthAttachment = nullptr;
	renderPass->renderingInfo.pStencilAttachment = nullptr;
	renderPasses.push_back(renderPass);

	return renderPass;
}

KGFXshader Vulkan::createShader(KGFXshaderdesc shaderDesc) {
	if (shaderDesc.pData == nullptr) {
		DEBUG_OUT("Invalid shader data/source");
		return KGFX_HANDLE_NULL;
	}

	if (shaderDesc.type < KGFX_SHADERTYPE_MIN || shaderDesc.type > KGFX_SHADERTYPE_MAX) {
		DEBUG_OUT("Invalid KGFXshadertype");
		return KGFX_HANDLE_NULL;
	}

	KGFXshader shader = new KGFXshader_t;
	shader->type = shaderDesc.type;

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	if (shaderDesc.medium == KGFX_MEDIUM_SPIRV) {
		createInfo.codeSize = shaderDesc.size;
		createInfo.pCode = reinterpret_cast<const u32*>(shaderDesc.pData);
	} else {
		DEBUG_OUT("Unsupported shader medium");
		return KGFX_HANDLE_NULL;
	}

	VkResult res = vkCreateShaderModule(device, &createInfo, nullptr, &shader->module);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkShaderModule");
		delete shader;
		return KGFX_HANDLE_NULL;
	}

	shader->id = static_cast<u32>(shaders.size());
	shaders.push_back(shader);
	return shader;
}

KGFXpipeline Vulkan::createPipeline(KGFXpipelinedesc pipelineDesc) {
	vkDeviceWaitIdle(device);
	if (pipelineDesc.pShaders == nullptr || pipelineDesc.shaderCount == 0) {
		DEBUG_OUT("Invalid KGFXpipelinedesc shader list");
		return KGFX_HANDLE_NULL;
	}

	KGFXpipeline pipeline = new KGFXpipeline_t;
	pipeline->renderPass = renderPasses[0];
	pipeline->allDescriptorSetsBound = false;

	VkShaderStageFlagBits stageFlags[] = { VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT, VK_SHADER_STAGE_GEOMETRY_BIT, VK_SHADER_STAGE_COMPUTE_BIT };
	VkDescriptorType descriptorTypes[] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER };

	VkDescriptorSetLayoutBinding* descriptorSetLayoutBindings = new VkDescriptorSetLayoutBinding[pipelineDesc.layout.descriptorSetCount];
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = pipelineDesc.layout.descriptorSetCount;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings;

	for (u32 i = 0; i < pipelineDesc.layout.descriptorSetCount; ++i) {
		descriptorSetLayoutBindings[i].binding = pipelineDesc.layout.pDescriptorSets[i].binding;
		descriptorSetLayoutBindings[i].descriptorType = descriptorUsageVkUsage(pipelineDesc.layout.pDescriptorSets[i].usage);
		descriptorSetLayoutBindings[i].descriptorCount = 1;
		descriptorSetLayoutBindings[i].stageFlags = bindpointVkShaderStageFlags(pipelineDesc.layout.pDescriptorSets[i].bindpoint);
		descriptorSetLayoutBindings[i].pImmutableSamplers = nullptr;
	}

	VkResult res = vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &pipeline->descriptorSetLayout);
	delete[] descriptorSetLayoutBindings;
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkDescriptorSetLayout");
		delete pipeline;
		return KGFX_HANDLE_NULL;
	}

	VkDescriptorPoolSize poolSizes[2];
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 1;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.poolSizeCount = 2;
	descriptorPoolCreateInfo.pPoolSizes = poolSizes;

	res = vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &pipeline->descriptorPool);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkDescriptorPool");
		vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
		delete pipeline;
		return KGFX_HANDLE_NULL;
	}

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = pipeline->descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &pipeline->descriptorSetLayout;

	res = vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &pipeline->descriptorSet);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkDescriptorSets");
		vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(device, pipeline->descriptorPool, nullptr);
		delete pipeline;
		return KGFX_HANDLE_NULL;
	}

	KGFXdescriptorset_t defaultDescSet = {};
	defaultDescSet.binding = std::numeric_limits<u32>::max();
	defaultDescSet.desc = nullptr;
	defaultDescSet.usage = KGFX_DESCRIPTOR_USAGE_INVALID;
	pipeline->descriptorSets.resize(pipelineDesc.layout.descriptorSetCount, defaultDescSet);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &pipeline->descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	res = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipeline->layout);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkPipelineLayout");
		vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(device, pipeline->descriptorPool, nullptr);
		delete pipeline;
		return KGFX_HANDLE_NULL;
	}

	if (pipelineDesc.pShaders == nullptr) {
		DEBUG_OUTF("Shader count of {}, but pShaders is NULL", pipelineDesc.shaderCount);
		vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(device, pipeline->descriptorPool, nullptr);
		return KGFX_HANDLE_NULL;
	}

	VkPipelineShaderStageCreateInfo* stages = new VkPipelineShaderStageCreateInfo[pipelineDesc.shaderCount];
	for (u32 i = 0; i < pipelineDesc.shaderCount; ++i) {
		stages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stages[i].flags = 0;
		stages[i].pNext = nullptr;
		if (pipelineDesc.pShaders[i]->type > KGFX_SHADERTYPE_MAX || pipelineDesc.pShaders[i]->type < KGFX_SHADERTYPE_MIN) {
			DEBUG_OUTF("Invalid shader type {}", pipelineDesc.pShaders[i]->type);
			delete[] stages;
			vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
			vkDestroyDescriptorPool(device, pipeline->descriptorPool, nullptr);
			return KGFX_HANDLE_NULL;
		}
		stages[i].stage = stageFlags[pipelineDesc.pShaders[i]->type];
		stages[i].module = pipelineDesc.pShaders[i]->module;
		stages[i].pName = "main";
		stages[i].pSpecializationInfo = nullptr;
	}

	VkVertexInputBindingDescription* bindings = nullptr;
	u32 vertexBindCount = 0;
	VkVertexInputAttributeDescription* attributes = nullptr;
	u32 vertexAttributeCount = 0;

	VkVertexInputRate inputRates[] = { VK_VERTEX_INPUT_RATE_VERTEX, VK_VERTEX_INPUT_RATE_INSTANCE };

	if (pipelineDesc.layout.bindingCount != 0) {
		u32 vertexInputStride = 0;
		std::vector<KGFXpipelinebinding> kgfxVertexBindings;
		for (u32 i = 0; i < pipelineDesc.layout.bindingCount; ++i) {
			if (pipelineDesc.layout.pBindings[i].bindpoint == KGFX_BINDPOINT_VERTEX) {
				vertexInputStride = 0;
				for (u32 j = 0; j < pipelineDesc.layout.pBindings[i].attributeCount; ++j) {
					vertexInputStride += datatypeSize(pipelineDesc.layout.pBindings[i].pAttributes[j].type);
				}

				vertexAttributeCount += pipelineDesc.layout.pBindings[i].attributeCount;
				kgfxVertexBindings.push_back(pipelineDesc.layout.pBindings[i]);
				++vertexBindCount;

				pipeline->vertexStride = vertexInputStride;
			}
		}

		if (vertexBindCount != 0) {
			bindings = new VkVertexInputBindingDescription[vertexBindCount];
			attributes = new VkVertexInputAttributeDescription[vertexAttributeCount];

			for (u32 i = 0; i < vertexBindCount; ++i) {
				bindings[i].binding = kgfxVertexBindings[i].binding;
				bindings[i].stride = vertexInputStride;
				bindings[i].inputRate = inputRates[kgfxVertexBindings[i].inputRate];

				u32 offset = 0;
				for (u32 j = 0; j < kgfxVertexBindings[i].attributeCount; ++j) {
					attributes[i * kgfxVertexBindings[i].attributeCount + j].location = kgfxVertexBindings[i].pAttributes[j].location;
					attributes[i * kgfxVertexBindings[i].attributeCount + j].binding = kgfxVertexBindings[i].binding;
					attributes[i * kgfxVertexBindings[i].attributeCount + j].format = datatypeVkFormat(kgfxVertexBindings[i].pAttributes[j].type);
					attributes[i * kgfxVertexBindings[i].attributeCount + j].offset = offset;

					offset += datatypeSize(kgfxVertexBindings[i].pAttributes[j].type);
				}
			}
		}
	}

	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};
	pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexBindCount;
	pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = bindings;
	pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeCount;
	pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = attributes;

	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
	pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
	pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipelineViewportStateCreateInfo.viewportCount = 1;
	pipelineViewportStateCreateInfo.pViewports = &viewport;
	pipelineViewportStateCreateInfo.scissorCount = 1;
	pipelineViewportStateCreateInfo.pScissors = &scissor;

	VkCullModeFlags cullModes[] = { VK_CULL_MODE_NONE, VK_CULL_MODE_FRONT_BIT, VK_CULL_MODE_BACK_BIT, VK_CULL_MODE_FRONT_AND_BACK };
	VkFrontFace frontFaces[] = { VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FRONT_FACE_CLOCKWISE };

	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
	pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	pipelineRasterizationStateCreateInfo.cullMode = cullModes[pipelineDesc.cullMode];
	pipelineRasterizationStateCreateInfo.frontFace = frontFaces[pipelineDesc.frontFace];
	pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
	pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
	pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
	pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};
	pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
	pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
	pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
	pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
	pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
	pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	pipelineColorBlendStateCreateInfo.attachmentCount = 1;
	pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
	pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {};
	pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	pipelineDynamicStateCreateInfo.dynamicStateCount = 2;
	pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates;

	VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {};
	pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
	pipelineRenderingCreateInfo.colorAttachmentCount = 1;
	pipelineRenderingCreateInfo.pColorAttachmentFormats = &swapchainFormat.format;
	pipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
	pipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
	pipelineRenderingCreateInfo.viewMask = 0;

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = &pipelineRenderingCreateInfo;
	pipelineCreateInfo.stageCount = pipelineDesc.shaderCount;
	pipelineCreateInfo.pStages = stages;
	pipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
	pipelineCreateInfo.pTessellationState = nullptr;
	pipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
	pipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
	pipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
	pipelineCreateInfo.pDepthStencilState = nullptr;
	pipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	pipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
	pipelineCreateInfo.layout = pipeline->layout;
	pipelineCreateInfo.renderPass = nullptr;
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;

	res = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline->pipeline);
	delete[] bindings;
	delete[] attributes;
	delete[] stages;
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkPipeline");
		vkDestroyPipelineLayout(device, pipeline->layout, nullptr);
		vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(device, pipeline->descriptorPool, nullptr);
		delete pipeline;
		return KGFX_HANDLE_NULL;
	}

	pipeline->id = static_cast<u32>(pipelines.size());
	pipelines.push_back(pipeline);
	return pipeline;
}

VkResult Vulkan::createCommandUtilities() {
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = graphicsQueueIndex;

	VkResult res = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkCommandPool");
		return res;
	}

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	res = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkCommandBuffer");
		return res;
	}

	return res;
}

VkResult Vulkan::createSyncUtilities() {
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkResult res = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkSemaphore");
		return res;
	}

	res = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphore);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkSemaphore");
		return res;
	}

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	res = vkCreateFence(device, &fenceCreateInfo, nullptr, &inFlightFence);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkFence");
		return res;
	}

	return res;
}

VkExtent2D Vulkan::getWindowExtent() {
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
	return surfaceCapabilities.currentExtent;
}

VkResult Vulkan::recreateSwapchain() {
	VkExtent2D extent = getWindowExtent();
	if (extent.width == 0 || extent.height == 0) {
		return VK_SUBOPTIMAL_KHR;
	}

	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkResult res = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkSemaphore");
		return res;
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);
	for (u32 i = 0; i < swapchainImageCount; ++i) {
		vkDestroyImageView(device, swapchainImageViews[i], nullptr);
	}

	res = createSwapchain();
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to recreate swapchain");
		return res;
	}

	return res;
}

void Vulkan::destroyShader(KGFXshader shader) {
	vkDestroyShaderModule(device, shader->module, nullptr);
	u32 id = shader->id;
	shaders.erase(shaders.begin() + id);
	for (u32 i = id; i < shaders.size(); ++i) {
		--shaders[i]->id;
	}

	delete shader;
}

void Vulkan::destroyPipeline(KGFXpipeline pipeline) {
	vkDeviceWaitIdle(device);
	vkDestroyPipeline(device, pipeline->pipeline, nullptr);
	vkDestroyPipelineLayout(device, pipeline->layout, nullptr);
	vkDestroyDescriptorSetLayout(device, pipeline->descriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(device, pipeline->descriptorPool, nullptr);

	u32 id = pipeline->id;
	pipelines.erase(pipelines.begin() + id);
	for (u32 i = id; i < pipelines.size(); ++i) {
		--pipelines[i]->id;
	}
	delete pipeline;
}

u32 Vulkan::findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties props;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &props);

	for (u32 i = 0; i < props.memoryTypeCount; ++i) {
		if ((typeFilter & (1 << i)) && (props.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	DEBUG_OUT("Failed to find suitable memory type");
	return std::numeric_limits<u32>::max();
}

KGFXbuffer Vulkan::createBuffer(KGFXbufferdesc bufferDesc) {
	KGFXbuffer buffer = new KGFXbuffer_t;
	if (bufferDesc.size == 0) {
		buffer->buffer = VK_NULL_HANDLE;
		buffer->memory = VK_NULL_HANDLE;
		buffer->size = 0;
		return buffer;
	}

	buffer->size = bufferDesc.size;

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = bufferDesc.size;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsageVkFlags(bufferDesc.usage);
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult res = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer->buffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkBuffer");
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer->buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, bufferLocationVkMemoryPropertyFlags(bufferDesc.location));

	res = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &buffer->memory);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkDeviceMemory");
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	res = vkBindBufferMemory(device, buffer->buffer, buffer->memory, 0);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to bind VkBuffer to VkDeviceMemory");
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	if (bufferDesc.pData == nullptr) {
		return buffer;
	}

	VkBufferCreateInfo stagingBufferCreateInfo = {};
	stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferCreateInfo.size = bufferDesc.size;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer stagingBuffer;
	res = vkCreateBuffer(device, &stagingBufferCreateInfo, nullptr, &stagingBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create staging VkBuffer");
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	VkMemoryRequirements stagingMemoryRequirements;
	vkGetBufferMemoryRequirements(device, stagingBuffer, &stagingMemoryRequirements);

	VkMemoryAllocateInfo stagingMemoryAllocateInfo = {};
	stagingMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	stagingMemoryAllocateInfo.allocationSize = stagingMemoryRequirements.size;
	stagingMemoryAllocateInfo.memoryTypeIndex = findMemoryType(stagingMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkDeviceMemory stagingMemory;
	res = vkAllocateMemory(device, &stagingMemoryAllocateInfo, nullptr, &stagingMemory);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate staging VkDeviceMemory");
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	res = vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to bind staging VkBuffer to staging VkDeviceMemory");
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	void* data;
	res = vkMapMemory(device, stagingMemory, 0, bufferDesc.size, 0, &data);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to map staging VkDeviceMemory");
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	memcpy(data, bufferDesc.pData, bufferDesc.size);
	vkUnmapMemory(device, stagingMemory);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;
	
	VkCommandBuffer commandBuffer;
	res = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkCommandBuffer");
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	res = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to begin recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	VkBufferCopy bufferCopy = {};
	bufferCopy.size = bufferDesc.size;
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	vkCmdCopyBuffer(commandBuffer, stagingBuffer, buffer->buffer, 1, &bufferCopy);

	res = vkEndCommandBuffer(commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to end recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	
	res = vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to submit VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	res = vkQueueWaitIdle(transferQueue);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to wait for VkQueue");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_HANDLE_NULL;
	}

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);

	buffer->size = bufferDesc.size;
	return buffer;
}

KGFXresult Vulkan::uploadBuffer(KGFXbuffer buffer, u32 size, void* data) {
	if (size > buffer->size) {
		DEBUG_OUT("Invalid upload size");
		return KGFX_INVALID_ARGUMENT;
	}

	VkBufferCreateInfo stagingBufferCreateInfo = {};
	stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferCreateInfo.size = size;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer stagingBuffer;
	VkResult res = vkCreateBuffer(device, &stagingBufferCreateInfo, nullptr, &stagingBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create staging VkBuffer");
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	VkMemoryRequirements stagingMemoryRequirements;
	vkGetBufferMemoryRequirements(device, stagingBuffer, &stagingMemoryRequirements);

	VkMemoryAllocateInfo stagingMemoryAllocateInfo = {};
	stagingMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	stagingMemoryAllocateInfo.allocationSize = stagingMemoryRequirements.size;
	stagingMemoryAllocateInfo.memoryTypeIndex = findMemoryType(stagingMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkDeviceMemory stagingMemory;
	res = vkAllocateMemory(device, &stagingMemoryAllocateInfo, nullptr, &stagingMemory);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate staging VkDeviceMemory");
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	res = vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to bind staging VkBuffer to staging VkDeviceMemory");
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	void* mapped;
	res = vkMapMemory(device, stagingMemory, 0, size, 0, &mapped);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to map staging VkDeviceMemory");
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	memcpy(mapped, data, size);
	vkUnmapMemory(device, stagingMemory);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	res = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkCommandBuffer");
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	res = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to begin recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	VkBufferCopy bufferCopy = {};
	bufferCopy.size = size;
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	vkCmdCopyBuffer(commandBuffer, stagingBuffer, buffer->buffer, 1, &bufferCopy);

	res = vkEndCommandBuffer(commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to end recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	res = vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to submit VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	res = vkQueueWaitIdle(transferQueue);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to wait for VkQueue");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkFreeMemory(device, stagingMemory, nullptr);
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		delete buffer;
		return KGFX_GENERIC_ERROR;
	}

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	return KGFX_SUCCESS;
}

void* Vulkan::mapBuffer(KGFXbuffer buffer) {
	void* mapped = nullptr;
	VkResult res = vkMapMemory(device, buffer->memory, 0, buffer->size, 0, &mapped);
	if (res != VK_SUCCESS) {
		return nullptr;
	}

	return mapped;
}

void Vulkan::unmapBuffer(KGFXbuffer buffer) {
	vkUnmapMemory(device, buffer->memory);
}

KGFXresult Vulkan::copyBuffer(KGFXbuffer dstBuffer, KGFXbuffer srcBuffer, u32 size, u32 dstOffset, u32 srcOffset) {
	if (size > dstBuffer->size || size > srcBuffer->size) {
		DEBUG_OUT("Invalid copy size");
		return KGFX_SIZE_TOO_LARGE;
	}

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VkResult res = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkCommandBuffer");
		return KGFX_GENERIC_ERROR;
	}

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	res = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to begin recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		return KGFX_GENERIC_ERROR;
	}

	VkBufferCopy bufferCopy = {};
	bufferCopy.size = size;
	bufferCopy.srcOffset = srcOffset;
	bufferCopy.dstOffset = dstOffset;
	vkCmdCopyBuffer(commandBuffer, srcBuffer->buffer, dstBuffer->buffer, 1, &bufferCopy);

	res = vkEndCommandBuffer(commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to end recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		return KGFX_GENERIC_ERROR;
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	res = vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to submit VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		return KGFX_GENERIC_ERROR;
	}

	return KGFX_SUCCESS;
}

void Vulkan::destroyBuffer(KGFXbuffer buffer) {
	vkDeviceWaitIdle(device);
	if (buffer->memory != VK_NULL_HANDLE) {
		vkDestroyBuffer(device, buffer->buffer, nullptr);
		vkFreeMemory(device, buffer->memory, nullptr);
	}
	delete buffer;
}

KGFXmesh Vulkan::createMesh(KGFXmeshdesc meshDesc) {
	KGFXmesh mesh = new KGFXmesh_t;
	mesh->vertexBuffer = KGFX_HANDLE_NULL;
	mesh->vertexOffset = 0;
	mesh->indexBuffer = KGFX_HANDLE_NULL;
	mesh->indexOffset = 0;

	for (u32 i = 0; i < meshDesc.bufferCount; ++i) {
		if (meshDesc.pBuffers[i].bindpoint == KGFX_MESH_BUFFER_BINDPOINT_VERTEX) {
			if (mesh->vertexBuffer != KGFX_HANDLE_NULL) {
				DEBUG_OUT("Multiple vertex buffers in KGFXmeshdesc");
				delete mesh;
				return KGFX_HANDLE_NULL;
			}
			mesh->vertexBuffer = meshDesc.pBuffers[i].buffer;
			mesh->vertexOffset = meshDesc.pBuffers[i].offset;
		} else if (meshDesc.pBuffers[i].bindpoint == KGFX_MESH_BUFFER_BINDPOINT_INDEX) {
			if (mesh->indexBuffer != KGFX_HANDLE_NULL) {
				DEBUG_OUT("Multiple index buffers in KGFXmeshdesc");
				delete mesh;
				return KGFX_HANDLE_NULL;
			}
			mesh->indexBuffer = meshDesc.pBuffers[i].buffer;
			mesh->indexOffset = meshDesc.pBuffers[i].offset;
		}
	}

	return mesh;
}

void Vulkan::destroyMesh(KGFXmesh mesh) {
	delete mesh;
}

KGFXpipelinemesh Vulkan::pipelineAddMesh(KGFXpipeline pipeline, KGFXmesh mesh, u32 binding) {
	KGFXpipelinemesh pipelineMesh = new KGFXpipelinemesh_t;
	pipelineMesh->mesh = mesh;
	pipelineMesh->id = static_cast<u32>(pipeline->meshes.size());
	pipelineMesh->binding = binding;
	pipeline->meshes.push_back(pipelineMesh);

	return pipelineMesh;
}

void Vulkan::pipelineRemoveMesh(KGFXpipeline pipeline, KGFXpipelinemesh pipelineMesh) {
	u32 id = pipelineMesh->id;
	if (id >= pipeline->meshes.size()) {
		DEBUG_OUT("Invalid KGFXpipelinemesh");
		return;
	}

	pipeline->meshes.erase(pipeline->meshes.begin() + id);
	for (u32 i = id; i < pipeline->meshes.size(); ++i) {
		--pipeline->meshes[i]->id;
	}
	delete pipelineMesh;
}

KGFXresult Vulkan::pipelineUpdateDescriptorSets(KGFXpipeline pipeline) {
	struct desc_image_with_binding_t {
		VkDescriptorImageInfo imageInfo;
		u32 binding;
	};
	struct desc_buffer_with_binding_t {
		VkDescriptorBufferInfo bufferInfo;
		u32 binding;
	};
	std::vector<VkWriteDescriptorSet> writeDescriptorSets(pipeline->descriptorSets.size());
	std::vector<desc_image_with_binding_t> imageInfos;
	std::vector<desc_buffer_with_binding_t> bufferInfos;

	for (u32 i = 0; i < pipeline->descriptorSets.size(); ++i) {
		KGFXdescriptorusage usage = pipeline->descriptorSets[i].usage;
		if (usage == KGFX_DESCRIPTOR_USAGE_INVALID) {
			return KGFX_SUCCESS;
		} else if (usage == KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER) {
			KGFXuniformbuffer ubuf = reinterpret_cast<KGFXuniformbuffer>(pipeline->descriptorSets[i].desc);
			if (ubuf == nullptr) {
				return KGFX_SUCCESS;
			}

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = ubuf->buffer->buffer;
			bufferInfo.offset = static_cast<VkDeviceSize>(ubuf->offset);
			bufferInfo.range = static_cast<VkDeviceSize>(ubuf->buffer->size);

			desc_buffer_with_binding_t desc = {};
			desc.bufferInfo = bufferInfo;
			desc.binding = pipeline->descriptorSets[i].binding;
			bufferInfos.push_back(desc);
		} else if (usage == KGFX_DESCRIPTOR_USAGE_TEXTURE) {
			KGFXpipelinetexture ptexture = reinterpret_cast<KGFXpipelinetexture>(pipeline->descriptorSets[i].desc);
			if (ptexture == nullptr) {
				return KGFX_SUCCESS;
			}

			VkDescriptorImageInfo imageInfo;
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = ptexture->imageView;
			imageInfo.sampler = ptexture->sampler;

			desc_image_with_binding_t desc = {};
			desc.imageInfo = imageInfo;
			desc.binding = pipeline->descriptorSets[i].binding;
			imageInfos.push_back(desc);
		} else {
			return KGFX_SUCCESS;
		}
	}
	
	u32 descIndex = 0;
	for (desc_image_with_binding_t& info : imageInfos) {
		writeDescriptorSets[descIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[descIndex].pNext = nullptr;
		writeDescriptorSets[descIndex].dstSet = pipeline->descriptorSet;
		writeDescriptorSets[descIndex].dstBinding = info.binding;
		writeDescriptorSets[descIndex].dstArrayElement = 0;
		writeDescriptorSets[descIndex].descriptorCount = 1;
		writeDescriptorSets[descIndex].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSets[descIndex].pBufferInfo = nullptr;
		writeDescriptorSets[descIndex].pImageInfo = &info.imageInfo;
		writeDescriptorSets[descIndex].pTexelBufferView = nullptr;
		++descIndex;
	}

	for (desc_buffer_with_binding_t& info : bufferInfos) {
		writeDescriptorSets[descIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[descIndex].pNext = nullptr;
		writeDescriptorSets[descIndex].dstSet = pipeline->descriptorSet;
		writeDescriptorSets[descIndex].dstBinding = info.binding;
		writeDescriptorSets[descIndex].dstArrayElement = 0;
		writeDescriptorSets[descIndex].descriptorCount = 1;
		writeDescriptorSets[descIndex].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSets[descIndex].pBufferInfo = &info.bufferInfo;
		writeDescriptorSets[descIndex].pImageInfo = nullptr;
		writeDescriptorSets[descIndex].pTexelBufferView = nullptr;
		++descIndex;
	}

	vkUpdateDescriptorSets(device, static_cast<u32>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	pipeline->allDescriptorSetsBound = true;
	return KGFX_SUCCESS;
}

void Vulkan::pipelineUnbindDescriptorSet(KGFXpipeline pipeline, KGFXdescriptorset_t& set) {
	switch (set.usage) {
	case KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER:
		pipelineUnbindDescriptorSetBuffer(pipeline, reinterpret_cast<KGFXuniformbuffer>(set.desc));
		break;
	case KGFX_DESCRIPTOR_USAGE_TEXTURE:
		pipelineUnbindDescriptorSetTexture(pipeline, reinterpret_cast<KGFXpipelinetexture>(set.desc));
		break;
	}
	
	set.desc = nullptr;
}

KGFXuniformbuffer Vulkan::pipelineBindDescriptorSetBuffer(KGFXpipeline pipeline, KGFXbuffer buffer, u32 binding, u32 offset) {
	if (pipeline->descriptorSets.size() < binding) {
		DEBUG_OUT("Invalid binding");
		return KGFX_HANDLE_NULL;
	}

	if (pipeline->descriptorSets[binding].desc != nullptr) {
		pipelineUnbindDescriptorSet(pipeline, pipeline->descriptorSets[binding]);
	}

	KGFXdescriptorset_t descriptorSet = {};
	KGFXuniformbuffer uniformBuffer = new KGFXuniformbuffer_t;
	uniformBuffer->id = binding;
	uniformBuffer->buffer = buffer;
	uniformBuffer->binding = binding;
	uniformBuffer->offset = offset;

	descriptorSet.binding = binding;
	descriptorSet.desc = reinterpret_cast<void*>(uniformBuffer);
	descriptorSet.usage = KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER;
	pipeline->descriptorSets[binding] = descriptorSet;

	if (pipelineUpdateDescriptorSets(pipeline) != KGFX_SUCCESS) {
		delete uniformBuffer;
		return KGFX_HANDLE_NULL;
	}
	return uniformBuffer;
}

void Vulkan::pipelineUnbindDescriptorSetBuffer(KGFXpipeline pipeline, KGFXuniformbuffer uniformBuffer) {
	u32 id = uniformBuffer->id;
	if (id >= pipeline->descriptorSets.size()) {
		DEBUG_OUT("Invalid KGFXdescriptorset");
		return;
	}

	pipeline->descriptorSets[id].desc = nullptr;
	delete uniformBuffer;
}

KGFXpipelinetexture Vulkan::pipelineBindDescriptorSetTexture(KGFXpipeline pipeline, KGFXtexture texture, KGFXsampler sampler, u32 binding) {
	if (pipeline->descriptorSets.size() <= binding) {
		DEBUG_OUT("Invalid binding");
		return KGFX_HANDLE_NULL;
	}

	if (pipeline->descriptorSets[binding].desc != nullptr) {
		pipelineUnbindDescriptorSet(pipeline, pipeline->descriptorSets[binding]);
	}

	KGFXpipelinetexture pipelineTexture = new KGFXpipelinetexture_t;
	pipelineTexture->id = binding;
	pipelineTexture->imageView = texture->view;
	pipelineTexture->sampler = sampler->sampler;

	KGFXdescriptorset_t descriptorSet = {};
	descriptorSet.binding = binding;
	descriptorSet.desc = reinterpret_cast<void*>(pipelineTexture);
	descriptorSet.usage = KGFX_DESCRIPTOR_USAGE_TEXTURE;
	pipeline->descriptorSets[binding] = descriptorSet;

	if (pipelineUpdateDescriptorSets(pipeline) != KGFX_SUCCESS) {
		delete pipelineTexture;
		return KGFX_HANDLE_NULL;
	}
	return pipelineTexture;
}

void Vulkan::pipelineUnbindDescriptorSetTexture(KGFXpipeline pipeline, KGFXpipelinetexture pipelineTexture) {
	u32 id = pipelineTexture->id;
	if (id >= pipeline->descriptorSets.size()) {
		DEBUG_OUT("Invalid KGFXdescriptorset");
		return;
	}

	pipeline->descriptorSets[id].desc = nullptr;
	delete pipelineTexture;
}

KGFXtexture Vulkan::createTexture(KGFXtexturedesc textureDesc) {
	KGFXtexture texture = new KGFXtexture_t;
	texture->image = VK_NULL_HANDLE;
	texture->view = VK_NULL_HANDLE;
	texture->extent = { textureDesc.width, textureDesc.height, textureDesc.depth };
	texture->format = textureFormatVkFormat(textureDesc.format);
	texture->memory = VK_NULL_HANDLE;
	
	VkExtent3D extent = { textureDesc.width, (textureDesc.height == 0) ? 1 : textureDesc.height, (textureDesc.depth == 0) ? 1 : textureDesc.depth };
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = (texture->extent.depth != 0) ? VK_IMAGE_TYPE_3D : (texture->extent.height != 0) ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_1D;
	imageCreateInfo.extent = extent;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = texture->format;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	VkResult res = vkCreateImage(device, &imageCreateInfo, nullptr, &texture->image);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkImage");
		delete texture;
		return KGFX_HANDLE_NULL;
	}

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(device, texture->image, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	res = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &texture->memory);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkDeviceMemory");
		vkDestroyImage(device, texture->image, nullptr);
		delete texture;
		return KGFX_HANDLE_NULL;
	}

	res = vkBindImageMemory(device, texture->image, texture->memory, 0);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to bind VkImage to VkDeviceMemory");
		vkFreeMemory(device, texture->memory, nullptr);
		vkDestroyImage(device, texture->image, nullptr);
		delete texture;
		return KGFX_HANDLE_NULL;
	}

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = texture->image;
	imageViewCreateInfo.viewType = (texture->extent.depth != 0) ? VK_IMAGE_VIEW_TYPE_3D : (texture->extent.height != 0) ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_1D;
	imageViewCreateInfo.format = texture->format;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	res = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &texture->view);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkImageView");
		vkFreeMemory(device, texture->memory, nullptr);
		vkDestroyImage(device, texture->image, nullptr);
		delete texture;
		return KGFX_HANDLE_NULL;
	}

	return texture;
}

KGFXresult Vulkan::copyBufferToTexture(KGFXtexture dstTexture, KGFXbuffer srcBuffer, u32 srcOffset) {
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VkResult res = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to allocate VkCommandBuffer");
		return KGFX_GENERIC_ERROR;
	}

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	res = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to begin recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		return KGFX_GENERIC_ERROR;
	}

	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcAccessMask = 0;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.image = dstTexture->image;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	
	VkExtent3D extent = { dstTexture->extent.width, (dstTexture->extent.height == 0) ? 1 : dstTexture->extent.height, (dstTexture->extent.depth == 0) ? 1 : dstTexture->extent.depth };
	VkBufferImageCopy bufferImageCopy = {};
	bufferImageCopy.bufferOffset = srcOffset;
	bufferImageCopy.bufferRowLength = 0;
	bufferImageCopy.bufferImageHeight = 0;
	bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	bufferImageCopy.imageSubresource.mipLevel = 0;
	bufferImageCopy.imageSubresource.baseArrayLayer = 0;
	bufferImageCopy.imageSubresource.layerCount = 1;
	bufferImageCopy.imageOffset = { 0, 0, 0 };
	bufferImageCopy.imageExtent = extent;

	vkCmdCopyBufferToImage(commandBuffer, srcBuffer->buffer, dstTexture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.image = dstTexture->image;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	res = vkEndCommandBuffer(commandBuffer);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to end recording VkCommandBuffer");
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		return KGFX_GENERIC_ERROR;
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(transferQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	return KGFX_SUCCESS;
}

void Vulkan::destroyTexture(KGFXtexture texture) {
	vkDeviceWaitIdle(device);
	vkDestroyImageView(device, texture->view, nullptr);
	vkDestroyImage(device, texture->image, nullptr);
	vkFreeMemory(device, texture->memory, nullptr);
	delete texture;
}

KGFXsampler Vulkan::createSampler(KGFXsamplerdesc samplerDesc) {
	KGFXsampler sampler = new KGFXsampler_t;

	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = samplerFilterVkFilter(samplerDesc.magFilter);
	samplerCreateInfo.minFilter = samplerFilterVkFilter(samplerDesc.minFilter);
	samplerCreateInfo.mipmapMode = samplerFilterVkMipmapMode(samplerDesc.mipmapMode);
	samplerCreateInfo.addressModeU = samplerAddressModeVkSamplerAddressMode(samplerDesc.addressModeU);
	samplerCreateInfo.addressModeV = samplerAddressModeVkSamplerAddressMode(samplerDesc.addressModeV);
	samplerCreateInfo.addressModeW = samplerAddressModeVkSamplerAddressMode(samplerDesc.addressModeW);
	samplerCreateInfo.mipLodBias = 1.0f;
	samplerCreateInfo.anisotropyEnable = VK_TRUE;
	samplerCreateInfo.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy;
	samplerCreateInfo.compareEnable = VK_TRUE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 0.0f;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	VkResult res = vkCreateSampler(device, &samplerCreateInfo, nullptr, &sampler->sampler);
	if (res != VK_SUCCESS) {
		DEBUG_OUT("Failed to create VkSampler");
		delete sampler;
		return KGFX_HANDLE_NULL;
	}

	return sampler;
}

void Vulkan::destroySampler(KGFXsampler sampler) {
	vkDeviceWaitIdle(device);
	vkDestroySampler(device, sampler->sampler, nullptr);
	delete sampler;
}

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
