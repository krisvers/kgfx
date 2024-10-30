#ifdef WIN32
#define PRISM_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__) || defined(__gnu_linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define PRISM_XLIB
#define PRISM_XCB
#define PRISM_WAYLAND
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define PRISM_COCOA
#endif /* #ifdef WIN32 */

#include "prism.h"
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

void debugCallbackPrism(PrismInstance instance, PrismDebugSeverity severity, PrismDebugSource source, const char* message) {
    printf("Prism: %s\n", message);
}

PrismResult test(GLFWwindow* window, PrismInstanceAPI api) {
    PrismInstance instance;
    PrismResult result = prismCreateInstance(api, PRISM_INSTANCE_CREATE_FLAG_DEBUG | PRISM_INSTANCE_CREATE_FLAG_VALIDATION | PRISM_INSTANCE_CREATE_FLAG_GRAPHICAL, &instance);
    if (result != PRISM_RESULT_SUCCESS) {
        return result;
    }
    
    prismDebugRegisterCallback(instance, debugCallbackPrism);

    PrismAdapterDetails adapterDetails = { 0 };
    const char memSuffixLookup[] = { 'B', 'K', 'M', 'G', 'T', 'P' };
    for (uint32_t i = 0; prismEnumerateAdapters(instance, i, &adapterDetails) == PRISM_RESULT_ENUMERATION_IN_PROGRESS; i++) {
        printf("Adapter %u: %s\n", i, adapterDetails.name);
        printf("    Device Type: %u\n", adapterDetails.type);
        printf("    Vendor: %u\n", adapterDetails.vendor);
        printf("    Max Texture Dimensions: %ux%ux%u\n", adapterDetails.maxTextureDimensions[0], adapterDetails.maxTextureDimensions[1], adapterDetails.maxTextureDimensions[2]);
        printf("    Max Framebuffer Dimensions: %ux%ux%u\n", adapterDetails.maxFramebufferDimensions[0], adapterDetails.maxFramebufferDimensions[1], adapterDetails.maxFramebufferDimensions[2]);
        printf("    Max Viewport Dimensions: %ux%u\n", adapterDetails.maxViewportDimensions[0], adapterDetails.maxViewportDimensions[1]);

        printf("    Max Viewports: %u\n", adapterDetails.maxViewports);
        printf("    Max Scissor Rects: %u\n", adapterDetails.maxScissorRects);
        printf("    Max Vertex Input Attributes: %u\n", adapterDetails.maxVertexInputAttributes);
        printf("    Max Vertex Input Bindings: %u\n", adapterDetails.maxVertexInputBindings);
        printf("    Max Vertex Input Attribute Offset: %u\n", adapterDetails.maxVertexInputAttributeOffset);
        printf("    Max Vertex Input Binding Stride: %u\n", adapterDetails.maxVertexInputBindingStride);
        printf("    Max Vertex Output Components: %u\n", adapterDetails.maxVertexOutputComponents);
        printf("    Max Color Attachments: %u\n", adapterDetails.maxColorAttachments);
        printf("    Max Anisotropy: %f\n", adapterDetails.maxAnisotropy);

        printf("    Supports Graphics: %s\n", adapterDetails.supportsGraphics ? "true" : "false");
        printf("    Supports Compute: %s\n", adapterDetails.supportsCompute ? "true" : "false");
        printf("    Supports RayTracing: %s\n", adapterDetails.supportsRayTracing ? "true" : "false");

        float mem = (float) adapterDetails.dedicatedMemoryGPU;
        uint32_t memSuffix = 0;
        while (mem > 1024.0f) {
            mem /= 1024.0f;
            ++memSuffix;
        }

        printf("    Dedicated GPU Memory: %.02f%c\n", mem, memSuffixLookup[memSuffix]);

        mem = (float) adapterDetails.dedicatedMemoryCPU;
        memSuffix = 0;
        while (mem > 1024.0f) {
            mem /= 1024.0f;
            ++memSuffix;
        }

        printf("    Dedicated CPU Memory: %.02f%c\n", mem, memSuffixLookup[memSuffix]);

        mem = (float) adapterDetails.sharedMemory;
        memSuffix = 0;
        while (mem > 1024.0f) {
            mem /= 1024.0f;
            ++memSuffix;
        }

        printf("    Shared Memory: %.02f%c\n", mem, memSuffixLookup[memSuffix]);

        mem = (float) adapterDetails.maxMemory;
        memSuffix = 0;
        while (mem > 1024.0f) {
            mem /= 1024.0f;
            ++memSuffix;
        }

        printf("    Max Memory: %.02f%c\n", mem, memSuffixLookup[memSuffix]);
    }

    PrismDevice device;
    result = prismCreateDevice(instance, 0, &device);
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to create Prism device\n");
        return result;
    }
    
    PrismSwapchainDesc swapchainDesc;
    swapchainDesc.width = 800;
    swapchainDesc.height = 600;
    swapchainDesc.format = PRISM_FORMAT_B8G8R8A8_SRGB;
    swapchainDesc.imageCount = 2;
    swapchainDesc.presentMode = PRISM_PRESENT_MODE_NO_SYNC;
    
    PrismSwapchain swapchain;
#ifdef PRISM_WIN32
    result = prismCreateSwapchainWin32(device, glfwGetWin32Window(window), GetModuleHandle(NULL), &swapchainDesc, &swapchain);
#elif defined(PRISM_XLIB)
    result = prismCreateSwapchainXlib(device, glfwGetX11Display(window), glfwGetX11Window(window), &swapchainDesc, &swapchain);
#elif defined(PRISM_COCOA)
    result = prismCreateSwapchainCocoa(device, glfwGetCocoaWindow(window), &swapchainDesc, &swapchain);
#endif
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to create Prism swapchain\n");
        return result;
    }
    
    PrismCommandPool commandPool;
    result = prismCreateCommandPool(device, 1, PRISM_QUEUE_TYPE_GENERIC, &commandPool);
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to create command pool\n");
        return result;
    }
    
    PrismCommandList commandList;
    result = prismCreateCommandList(commandPool, &commandList);
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to create command list\n");
        return result;
    }
    
    prismDestroyCommandList(commandList);
    prismDestroyCommandPool(commandPool);
    prismDestroySwapchain(swapchain);
    prismDestroyDevice(device);
    prismDestroyInstance(instance);
    return PRISM_RESULT_SUCCESS;
}

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return 1;
    }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        return 1;
    }
    
    PrismResult result = test(window, PRISM_INSTANCE_API_VULKAN);
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to test Vulkan Prism\n");
    }

    result = test(window, PRISM_INSTANCE_API_D3D12);
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to test D3D12 Prism\n");
    }

    result = test(window, PRISM_INSTANCE_API_METAL);
    if (result != PRISM_RESULT_SUCCESS) {
        printf("Failed to test Metal Prism\n");
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
