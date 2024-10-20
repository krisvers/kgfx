#ifndef PRISM_H
#define PRISM_H

#include <stdint.h>

#define PRISM_DEFINE_HANDLE(object) typedef struct PrismObject* object

#ifndef PRISM_API
#define PRISM_API
#endif /* PRISM_API */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int8_t PrismBool;

#define PRISM_TRUE 1
#define PRISM_FALSE 0

PRISM_DEFINE_HANDLE(PrismInstance);
PRISM_DEFINE_HANDLE(PrismDevice);

typedef enum PrismResult {
    PRISM_RESULT_ENUMERATION_IN_PROGRESS = -1,
    PRISM_RESULT_SUCCESS = 0,
    PRISM_RESULT_ENUMERATION_DONE = PRISM_RESULT_SUCCESS,
    PRISM_RESULT_ERROR_UNKNOWN = 1,
    PRISM_RESULT_ERROR_INVALID_ARGUMENT = 2,
    PRISM_RESULT_ERROR_UNSUPPORTED = 3,
    PRISM_RESULT_ERROR_UNIMPLEMENTED = 4,
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
} PrismInstanceCreateFlags;

typedef uint32_t PrismInstanceCreateFlagsBits;

typedef enum PrismAdapterType {
    PRISM_ADAPTER_TYPE_UNKNOWN = 0,
    PRISM_ADAPTER_TYPE_CPU = 1,
    PRISM_ADAPTER_TYPE_DEDICATED_GPU = 2,
    PRISM_ADAPTER_TYPE_INTEGRATED_GPU = 3,
    PRISM_ADAPTER_TYPE_UMA_GPU = 4,
    PRISM_ADAPTER_TYPE_VIRTUAL = 5,
    PRISM_ADAPTER_TYPE_COUNT,
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
    PRISM_ADAPTER_VENDOR_COUNT,
} PrismAdapterVendor;

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
    PrismBool supportsTransfer;
    PrismBool supportsCompute;
    PrismBool supportsRayTracing;

    uint64_t dedicatedMemoryGPU;
    uint64_t dedicatedMemoryCPU;
    uint64_t sharedMemory;
    uint64_t maxMemory;

    const char name[64];
} PrismAdapterDetails;

PRISM_API PrismResult prismCreateInstance(PrismInstanceAPI api, PrismInstanceCreateFlagsBits flags, PrismInstance* pInstance);
PRISM_API void prismDestroyInstance(PrismInstance instance);

PRISM_API PrismResult prismEnumerateAdapters(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails);
PRISM_API PrismResult prismCreateDevice(PrismInstance instance, uint32_t adapterID, PrismBool isGraphical, PrismDevice* pDevice);
PRISM_API void prismDestroyDevice(PrismDevice device);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef PRISM_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>

typedef struct PrismObject {
    PrismInstanceAPI api;
    PrismInstance instance;
} PrismObject;

#define PRISM_LIST(T_) struct { T_* data; uint32_t length; }
#define PRISM_LIST_INIT(list_, length_, T_) list_.data = (T_*) malloc(sizeof(T_) * length_); if (list_.data == NULL) { abort(); } list_.length = length_
#define PRISM_LIST_FREE(list_) free(list_.data)

#ifdef PRISM_VULKAN

#include <vulkan/vulkan.h>

typedef struct PrismInstance_Vulkan_t {
    PrismObject obj;

    PrismInstanceCreateFlagsBits flags;
    VkInstance instance;

    PRISM_LIST(VkPhysicalDevice) physicalDevices;
} PrismInstance_Vulkan_t;

typedef struct PrismDevice_Vulkan_t {
    PrismObject obj;

    VkDevice device;
} PrismDevice_Vulkan_t;

static PrismResult prismCreateInstance_vulkan(PrismInstanceCreateFlagsBits flags, PrismInstance* pInstance) {
    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Prism";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Prism";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    PrismInstance_Vulkan_t* instance = (PrismInstance_Vulkan_t*) malloc(sizeof(PrismInstance_Vulkan_t));
    if (instance == NULL) {
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    instance->obj.api = PRISM_INSTANCE_API_VULKAN;
    instance->flags = flags;
    VkResult result = vkCreateInstance(&createInfo, NULL, &instance->instance);
    if (result != VK_SUCCESS) {
        free(instance);
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    uint32_t adapterCount = 0;
    vkEnumeratePhysicalDevices(instance->instance, &adapterCount, NULL);

    PRISM_LIST_INIT(instance->physicalDevices, adapterCount, VkPhysicalDevice);
    vkEnumeratePhysicalDevices(instance->instance, &adapterCount, instance->physicalDevices.data);

    *pInstance = &instance->obj;
    return PRISM_RESULT_SUCCESS;
}

static void prismDestroyInstance_vulkan(PrismInstance instance) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    vkDestroyInstance(vulkanInstance->instance, NULL);
    PRISM_LIST_FREE(vulkanInstance->physicalDevices);
    free(vulkanInstance);
}

static PrismResult prismEnumerateAdapters_vulkan(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    if (adapterID >= vulkanInstance->physicalDevices.length) {
        return PRISM_RESULT_ENUMERATION_DONE;
    }

    VkPhysicalDevice physicalDevice = vulkanInstance->physicalDevices.data[adapterID];
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

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
    pAdapterDetails->supportsTransfer = properties.limits.maxComputeWorkGroupCount[0] > 0;
    pAdapterDetails->supportsCompute = properties.limits.maxComputeWorkGroupCount[0] > 0;
    pAdapterDetails->supportsRayTracing = VK_VERSION_MAJOR(properties.apiVersion) >= 1 && VK_VERSION_MINOR(properties.apiVersion) >= 2;

    /* detect memory */
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

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

static PrismResult prismCreateDevice_vulkan(PrismInstance instance, uint32_t adapterID, PrismBool isGraphical, PrismDevice* pDevice) {
    PrismInstance_Vulkan_t* vulkanInstance = (PrismInstance_Vulkan_t*) instance;
    if (adapterID >= vulkanInstance->physicalDevices.length) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }

    VkPhysicalDevice physicalDevice = vulkanInstance->physicalDevices.data[adapterID];
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);

    VkDeviceQueueCreateInfo queueCreateInfo = { 0 };
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &features;

    PrismDevice_Vulkan_t* device = (PrismDevice_Vulkan_t*) malloc(sizeof(PrismDevice_Vulkan_t));
    VkResult result = vkCreateDevice(physicalDevice, &createInfo, NULL, &device->device);
    if (result != VK_SUCCESS) {
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    device->obj.api = PRISM_INSTANCE_API_VULKAN;
    device->obj.instance = instance;
    *pDevice = &device->obj;
    return PRISM_RESULT_SUCCESS;
}

static void prismDestroyDevice_vulkan(PrismDevice device) {
    PrismDevice_Vulkan_t* vulkanDevice = (PrismDevice_Vulkan_t*) device;
    vkDestroyDevice(vulkanDevice->device, NULL);
    free(vulkanDevice);
}

#endif /* PRISM_VULKAN */

#ifdef PRISM_D3D12
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

    PrismInstanceCreateFlagsBits flags;
    IDXGIFactory4* factory;
} PrismInstance_D3D12_t;

typedef struct PrismDevice_D3D12 {
    PrismObject obj;

    ID3D12Device* device;
} PrismDevice_D3D12_t;

static PrismResult prismCreateInstance_d3d12(PrismInstanceCreateFlagsBits flags, PrismInstance* pInstance) {
    IDXGIFactory4* factory;
    HRESULT result = CreateDXGIFactory2(flags & PRISM_INSTANCE_CREATE_FLAG_DEBUG ? DXGI_CREATE_FACTORY_DEBUG : 0, PRISM_D3D12_IID(IDXGIFactory4, factory));
    if (FAILED(result)) {
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    PrismInstance_D3D12_t* instance = (PrismInstance_D3D12_t*) malloc(sizeof(PrismInstance_D3D12_t));
    if (instance == NULL) {
        PRISM_D3D12_CALL(factory, Release);
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    instance->obj.api = PRISM_INSTANCE_API_D3D12;
    instance->flags = flags;
    instance->factory = factory;

    *pInstance = &instance->obj;
    return PRISM_RESULT_SUCCESS;
}

static void prismDestroyInstance_d3d12(PrismInstance instance) {
    PrismInstance_D3D12_t* d3d12Instance = (PrismInstance_D3D12_t*) instance;
    PRISM_D3D12_CALL(d3d12Instance->factory, Release);
    free(d3d12Instance);
}

static PrismResult prismEnumerateAdapters_d3d12(PrismInstance instance, uint32_t adapterID, PrismAdapterDetails* pAdapterDetails) {
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

    pAdapterDetails->supportsGraphics = TRUE;
    pAdapterDetails->supportsTransfer = TRUE;
    pAdapterDetails->supportsCompute = TRUE;
    pAdapterDetails->supportsRayTracing = featuresSupport.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;

    pAdapterDetails->dedicatedMemoryGPU = desc.DedicatedVideoMemory;
    pAdapterDetails->dedicatedMemoryCPU = desc.DedicatedSystemMemory;
    pAdapterDetails->sharedMemory = desc.SharedSystemMemory;
    pAdapterDetails->maxMemory = desc.DedicatedVideoMemory + desc.DedicatedSystemMemory + desc.SharedSystemMemory;

    snprintf((char*) pAdapterDetails->name, 64, "%ls", desc.Description);
    PRISM_D3D12_CALL(adapter, Release);
    return PRISM_RESULT_ENUMERATION_IN_PROGRESS;
}

static PrismResult prismCreateDevice_d3d12(PrismInstance instance, uint32_t adapterID, PrismBool isGraphical, PrismDevice* pDevice) {
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
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    PrismDevice_D3D12_t* d3d12Device = (PrismDevice_D3D12_t*) malloc(sizeof(PrismDevice_D3D12_t));
    if (d3d12Device == NULL) {
        PRISM_D3D12_CALL(device, Release);
        PRISM_D3D12_CALL(adapter, Release);
        return PRISM_RESULT_ERROR_UNKNOWN;
    }

    d3d12Device->obj.api = PRISM_INSTANCE_API_D3D12;
    d3d12Device->obj.instance = instance;
    d3d12Device->device = device;

    *pDevice = &d3d12Device->obj;
    PRISM_D3D12_CALL(adapter, Release);
    return PRISM_RESULT_SUCCESS;
}

static void prismDestroyDevice_d3d12(PrismDevice device) {
    PrismDevice_D3D12_t* d3d12Device = (PrismDevice_D3D12_t*) device;
    PRISM_D3D12_CALL(d3d12Device->device, Release);
    free(d3d12Device);
}
#endif /* PRISM_D3D12 */

#ifdef PRISM_METAL
#error "Not implemented yet"
#endif /* PRISM_METAL */

PRISM_API PrismResult prismCreateInstance(PrismInstanceAPI api, PrismInstanceCreateFlagsBits flags, PrismInstance* pInstance) {
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

PrismResult prismCreateDevice(PrismInstance instance, uint32_t adapterID, PrismBool isGraphical, PrismDevice* pDevice) {
    if (instance == NULL || pDevice == NULL) {
        return PRISM_RESULT_ERROR_INVALID_ARGUMENT;
    }

    switch (instance->api) {
#ifdef PRISM_VULKAN
        case PRISM_INSTANCE_API_VULKAN:
            return prismCreateDevice_vulkan(instance, adapterID, isGraphical, pDevice);
#endif /* PRISM_VULKAN */
#ifdef PRISM_D3D12
        case PRISM_INSTANCE_API_D3D12:
            return prismCreateDevice_d3d12(instance, adapterID, isGraphical, pDevice);
#endif /* PRISM_D3D12 */
#ifdef PRISM_METAL
        case PRISM_INSTANCE_API_METAL:
            return prismCreateDevice_metal(instance, adapterID, isGraphical, pDevice);
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

#endif /* PRISM_IMPLEMENTATION */

#endif /* PRISM_H */