#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define KGFX_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__) || defined(__gnu_linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define KGFX_XLIB
#define KGFX_XCB
#define KGFX_WAYLAND
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define KGFX_COCOA
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */

#ifdef KGFX_WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif /* #ifdef KGFX_WIN32 */

#include "kgfx.h"
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

typedef struct Vertex {
    float x, y, z;
} Vertex;

void debugCallbackKGFX(KGFXInstance instance, KGFXDebugSeverity severity, KGFXDebugSource source, const char* message) {
    // TODO: printf("[KGFX] %s\n", message);
}

KGFXResult test(GLFWwindow* window, KGFXInstanceAPI api) {
    KGFXInstance instance;
    KGFXResult result = kgfxCreateInstance(api, KGFX_INSTANCE_CREATE_FLAG_DEBUG | KGFX_INSTANCE_CREATE_FLAG_VALIDATION | KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL, &instance);
    if (result != KGFX_RESULT_SUCCESS) {
        return result;
    }
    
    kgfxDebugRegisterCallback(instance, debugCallbackKGFX);

    KGFXAdapterDetails adapterDetails = { 0 };
    const char memSuffixLookup[] = { 'B', 'K', 'M', 'G', 'T', 'P' };
    for (uint32_t i = 0; kgfxEnumerateAdapters(instance, i, &adapterDetails) == KGFX_RESULT_ENUMERATION_IN_PROGRESS; i++) {
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

    KGFXDevice device;
    result = kgfxCreateDevice(instance, 0, &device);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create KGFX device\n");
        kgfxDestroyInstance(instance);
        return result;
    }
    
    KGFXSwapchainDesc swapchainDesc;
    swapchainDesc.width = 800;
    swapchainDesc.height = 600;
    swapchainDesc.format = KGFX_FORMAT_B8G8R8A8_SRGB;
    swapchainDesc.imageCount = 2;
    swapchainDesc.presentMode = KGFX_PRESENT_MODE_NO_SYNC;
    
    KGFXSwapchain swapchain;
#ifdef KGFX_WIN32
    result = kgfxCreateSwapchainWin32(device, glfwGetWin32Window(window), GetModuleHandle(NULL), &swapchainDesc, &swapchain);
#elif defined(KGFX_XLIB)
    result = kgfxCreateSwapchainXlib(device, glfwGetX11Display(window), glfwGetX11Window(window), &swapchainDesc, &swapchain);
#elif defined(KGFX_COCOA)
    result = kgfxCreateSwapchainCocoa(device, glfwGetCocoaWindow(window), &swapchainDesc, &swapchain);
#endif
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create KGFX swapchain\n");
        kgfxDestroyDevice(device);
        kgfxDestroyInstance(instance);
        return result;
    }

    KGFXTexture backbuffer = kgfxGetSwapchainBackbuffer(swapchain);
    
    const uint32_t vertexShaderSPIRV[] = {
        0x07230203,0x00010000,0x0008000b,0x0000001b,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0007000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000d,0x00000012,0x00030003,
        0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,0x00000000,0x00060005,0x0000000b,
        0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x0000000b,0x00000000,0x505f6c67,
        0x7469736f,0x006e6f69,0x00070006,0x0000000b,0x00000001,0x505f6c67,0x746e696f,0x657a6953,
        0x00000000,0x00070006,0x0000000b,0x00000002,0x435f6c67,0x4470696c,0x61747369,0x0065636e,
        0x00070006,0x0000000b,0x00000003,0x435f6c67,0x446c6c75,0x61747369,0x0065636e,0x00030005,
        0x0000000d,0x00000000,0x00040005,0x00000012,0x6f506e69,0x00000073,0x00050048,0x0000000b,
        0x00000000,0x0000000b,0x00000000,0x00050048,0x0000000b,0x00000001,0x0000000b,0x00000001,
        0x00050048,0x0000000b,0x00000002,0x0000000b,0x00000003,0x00050048,0x0000000b,0x00000003,
        0x0000000b,0x00000004,0x00030047,0x0000000b,0x00000002,0x00040047,0x00000012,0x0000001e,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040015,0x00000008,0x00000020,
        0x00000000,0x0004002b,0x00000008,0x00000009,0x00000001,0x0004001c,0x0000000a,0x00000006,
        0x00000009,0x0006001e,0x0000000b,0x00000007,0x00000006,0x0000000a,0x0000000a,0x00040020,
        0x0000000c,0x00000003,0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000003,0x00040015,
        0x0000000e,0x00000020,0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040017,
        0x00000010,0x00000006,0x00000003,0x00040020,0x00000011,0x00000001,0x00000010,0x0004003b,
        0x00000011,0x00000012,0x00000001,0x0004002b,0x00000006,0x00000014,0x3f800000,0x00040020,
        0x00000019,0x00000003,0x00000007,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,
        0x000200f8,0x00000005,0x0004003d,0x00000010,0x00000013,0x00000012,0x00050051,0x00000006,
        0x00000015,0x00000013,0x00000000,0x00050051,0x00000006,0x00000016,0x00000013,0x00000001,
        0x00050051,0x00000006,0x00000017,0x00000013,0x00000002,0x00070050,0x00000007,0x00000018,
        0x00000015,0x00000016,0x00000017,0x00000014,0x00050041,0x00000019,0x0000001a,0x0000000d,
        0x0000000f,0x0003003e,0x0000001a,0x00000018,0x000100fd,0x00010038
    };
    
    const uint32_t fragmentShaderSPIRV[] = {
        0x07230203,0x00010000,0x0008000b,0x0000000d,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0006000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x00030010,0x00000004,
        0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,0x00000000,
        0x00050005,0x00000009,0x4374756f,0x726f6c6f,0x00000000,0x00040047,0x00000009,0x0000001e,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x0004002b,0x00000006,0x0000000a,
        0x3f800000,0x0004002b,0x00000006,0x0000000b,0x00000000,0x0007002c,0x00000007,0x0000000c,
        0x0000000a,0x0000000b,0x0000000b,0x0000000a,0x00050036,0x00000002,0x00000004,0x00000000,
        0x00000003,0x000200f8,0x00000005,0x0003003e,0x00000009,0x0000000c,0x000100fd,0x00010038
    };
    
    KGFXShader vertexShader;
    result = kgfxCreateShaderSPIRV(device, vertexShaderSPIRV, sizeof(vertexShaderSPIRV), "main", KGFX_SHADER_STAGE_VERTEX, &vertexShader);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create shader from SPIR-V\n");
        kgfxDestroySwapchain(swapchain);
        kgfxDestroyDevice(device);
        kgfxDestroyInstance(instance);
        return result;
    }
    
    KGFXShader fragmentShader;
    result = kgfxCreateShaderSPIRV(device, fragmentShaderSPIRV, sizeof(fragmentShaderSPIRV), "main", KGFX_SHADER_STAGE_FRAGMENT, &fragmentShader);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create shader from SPIR-V\n");
        kgfxDestroyShader(vertexShader);
        kgfxDestroySwapchain(swapchain);
        kgfxDestroyDevice(device);
        kgfxDestroyInstance(instance);
        return result;
    }
    
    KGFXShader shaders[2] = { vertexShader, fragmentShader };
    
    KGFXRenderTargetDesc renderTargetDesc;
    renderTargetDesc.format = swapchainDesc.format;
    renderTargetDesc.width = swapchainDesc.width;
    renderTargetDesc.height = swapchainDesc.height;
    renderTargetDesc.layers = 1;
    renderTargetDesc.enableBlending = KGFX_FALSE;
    renderTargetDesc.colorWriteMask = KGFX_COLOR_MASK_ALL;
    renderTargetDesc.loadOp = KGFX_RENDER_TARGET_OP_CLEAR;
    renderTargetDesc.storeOp = KGFX_RENDER_TARGET_OP_STORE;
    renderTargetDesc.finalLayout = KGFX_TEXTURE_LAYOUT_PRESENT;
    
    KGFXVertexBindingDesc bindingDesc;
    bindingDesc.binding = 0;
    bindingDesc.stride = sizeof(Vertex);
    bindingDesc.inputRate = KGFX_VERTEX_INPUT_RATE_PER_VERTEX;
    
    KGFXVertexAttributeDesc attributeDesc;
    attributeDesc.binding = 0;
    attributeDesc.location = 0;
    attributeDesc.offset = 0;
    attributeDesc.format = KGFX_FORMAT_R32G32B32_FLOAT;
    
    KGFXGraphicsPipelineDesc pipelineDesc;
    pipelineDesc.cullMode = KGFX_CULL_MODE_NONE;
    pipelineDesc.counterClockwiseFrontFaceWinding = KGFX_TRUE;
    pipelineDesc.fillMode = KGFX_FILL_MODE_FILL;
    pipelineDesc.topology = KGFX_TOPOLOGY_TRIANGLES;
    pipelineDesc.viewportAndScissorCount = 1;
    pipelineDesc.vertexInputDesc.bindingCount = 1;
    pipelineDesc.vertexInputDesc.pBindings = &bindingDesc;
    pipelineDesc.vertexInputDesc.attributeCount = 1;
    pipelineDesc.vertexInputDesc.pAttributes = &attributeDesc;
    pipelineDesc.shaderCount = 2;
    pipelineDesc.pShaders = shaders;
    pipelineDesc.blendLogicOp = KGFX_LOGIC_OP_DISABLED;
    pipelineDesc.framebufferWidth = swapchainDesc.width;
    pipelineDesc.framebufferHeight = swapchainDesc.height;
    pipelineDesc.framebufferLayers = 1;
    pipelineDesc.renderTargetCount = 1;
    pipelineDesc.pRenderTargetDescs = &renderTargetDesc;
    pipelineDesc.depthStencilDesc.format = KGFX_FORMAT_UNKNOWN;
    pipelineDesc.uniformSignatureDesc.uniformCount = 0;
    pipelineDesc.uniformSignatureDesc.pUniforms = NULL;
    
    KGFXGraphicsPipeline pipeline;
    result = kgfxCreateGraphicsPipeline(device, &pipelineDesc, &pipeline);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create graphics pipeline\n");
        kgfxDestroyShader(fragmentShader);
        kgfxDestroyShader(vertexShader);
        kgfxDestroySwapchain(swapchain);
        kgfxDestroyDevice(device);
        kgfxDestroyInstance(instance);
        return result;
    }
    
    /* TODO: pipeline destruction */
    
    KGFXCommandPool commandPool;
    result = kgfxCreateCommandPool(device, 1, KGFX_QUEUE_TYPE_GENERIC, &commandPool);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create command pool\n");
        kgfxDestroyGraphicsPipeline(pipeline);
        kgfxDestroyShader(fragmentShader);
        kgfxDestroyShader(vertexShader);
        kgfxDestroySwapchain(swapchain);
        kgfxDestroyDevice(device);
        kgfxDestroyInstance(instance);
        return result;
    }
    
    KGFXCommandList commandList;
    result = kgfxCreateCommandList(commandPool, &commandList);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create command list\n");
        kgfxDestroyCommandPool(commandPool);
        kgfxDestroyGraphicsPipeline(pipeline);
        kgfxDestroyShader(fragmentShader);
        kgfxDestroyShader(vertexShader);
        kgfxDestroySwapchain(swapchain);
        kgfxDestroyDevice(device);
        kgfxDestroyInstance(instance);
        return result;
    }
    
    while (!glfwWindowShouldClose(window)) {
        result = kgfxOpenCommandList(commandList, KGFX_FALSE);
        if (result != KGFX_RESULT_SUCCESS) {
            printf("Failed to open command list\n");
            kgfxDestroyCommandList(commandList);
            kgfxDestroyCommandPool(commandPool);
            kgfxDestroyGraphicsPipeline(pipeline);
            kgfxDestroyShader(fragmentShader);
            kgfxDestroyShader(vertexShader);
            kgfxDestroySwapchain(swapchain);
            kgfxDestroyDevice(device);
            kgfxDestroyInstance(instance);
            return result;
        }

        kgfxCmdBindGraphicsPipeline(commandList, pipeline);

        KGFXViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapchainDesc.width;
        viewport.height = (float) swapchainDesc.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        KGFXScissor scissor;
        scissor.x = 0;
        scissor.y = 0;
        scissor.width = swapchainDesc.width;
        scissor.height = swapchainDesc.height;

        kgfxCmdSetViewportAndScissor(commandList, 1, &viewport, &scissor);
        kgfxCmdBindRenderTargets(commandList, 1, &backbuffer, NULL);

        KGFXClearValue clearValue;
        clearValue.type = KGFX_CLEAR_VALUE_TYPE_F32;
        clearValue.value.f32[0] = 1.0f;
        clearValue.value.f32[1] = 0.0f;
        clearValue.value.f32[2] = 1.0f;
        clearValue.value.f32[3] = 1.0f;

        kgfxCmdBeginRendering(commandList, 1, &clearValue, NULL);

        kgfxCmdDraw(commandList, 3, 1, 0, 0);

        kgfxCmdEndRendering(commandList);
        result = kgfxCloseCommandList(commandList);
        if (result != KGFX_RESULT_SUCCESS) {
            printf("Failed to close command list\n");
            kgfxDestroyCommandList(commandList);
            kgfxDestroyCommandPool(commandPool);
            kgfxDestroyGraphicsPipeline(pipeline);
            kgfxDestroyShader(fragmentShader);
            kgfxDestroyShader(vertexShader);
            kgfxDestroySwapchain(swapchain);
            kgfxDestroyDevice(device);
            kgfxDestroyInstance(instance);
            return result;
        }

        result = kgfxSubmitCommandList(commandList);
        if (result != KGFX_RESULT_SUCCESS) {
            printf("Failed to submit command list\n");
            kgfxDestroyCommandList(commandList);
            kgfxDestroyCommandPool(commandPool);
            kgfxDestroyGraphicsPipeline(pipeline);
            kgfxDestroyShader(fragmentShader);
            kgfxDestroyShader(vertexShader);
            kgfxDestroySwapchain(swapchain);
            kgfxDestroyDevice(device);
            kgfxDestroyInstance(instance);
            return result;
        }

        result = kgfxPresentSwapchain(swapchain);
        if (result != KGFX_RESULT_SUCCESS) {
            printf("Failed to present swapchain\n");
            kgfxDestroyCommandList(commandList);
            kgfxDestroyCommandPool(commandPool);
            kgfxDestroyGraphicsPipeline(pipeline);
            kgfxDestroyShader(fragmentShader);
            kgfxDestroyShader(vertexShader);
            kgfxDestroySwapchain(swapchain);
            kgfxDestroyDevice(device);
            kgfxDestroyInstance(instance);
            return result;
        }
        
        glfwPollEvents();
#ifdef KGFX_WIN32
        Sleep(2);
#else
        usleep(2000);
#endif /* #ifdef KGFX_WIN32 */
    }
    
    kgfxDestroyCommandList(commandList);
    kgfxDestroyCommandPool(commandPool);
    kgfxDestroyGraphicsPipeline(pipeline);
    kgfxDestroyShader(fragmentShader);
    kgfxDestroyShader(vertexShader);
    kgfxDestroySwapchain(swapchain);
    kgfxDestroyDevice(device);
    kgfxDestroyInstance(instance);
    return KGFX_RESULT_SUCCESS;
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
    
    KGFXResult result = test(window, KGFX_INSTANCE_API_VULKAN);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to test Vulkan KGFX\n");
    }

    result = test(window, KGFX_INSTANCE_API_D3D12);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to test D3D12 KGFX\n");
    }

    result = test(window, KGFX_INSTANCE_API_METAL);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to test Metal KGFX\n");
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
