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
#include "linmath.h"
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

typedef struct Vertex {
    float x, y, z;
    float r, g, b;
} Vertex;

typedef struct UniformData {
    mat4x4 mvp;
} UniformData;

void debugCallbackKGFX(KGFXInstance instance, KGFXDebugSeverity severity, KGFXDebugSource source, const char* message) {
    printf("[KGFX] %s\n", message);
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
    swapchainDesc.presentMode = KGFX_PRESENT_MODE_VSYNC;
    
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
    
    #include "shader.vert.spv.h"
    #include "shader.frag.spv.h"
    
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
    
    KGFXRenderTargetDesc renderTargetDescs[1];
    renderTargetDescs[0].format = swapchainDesc.format;
    renderTargetDescs[0].width = swapchainDesc.width;
    renderTargetDescs[0].height = swapchainDesc.height;
    renderTargetDescs[0].layers = 1;
    renderTargetDescs[0].enableBlending = KGFX_FALSE;
    renderTargetDescs[0].colorWriteMask = KGFX_COLOR_MASK_ALL;
    renderTargetDescs[0].loadOp = KGFX_RENDER_TARGET_OP_CLEAR;
    renderTargetDescs[0].storeOp = KGFX_RENDER_TARGET_OP_STORE;
    renderTargetDescs[0].finalLayout = KGFX_TEXTURE_LAYOUT_PRESENT;
    
    KGFXVertexBindingDesc bindingDesc;
    bindingDesc.binding = 0;
    bindingDesc.stride = sizeof(Vertex);
    bindingDesc.inputRate = KGFX_VERTEX_INPUT_RATE_PER_VERTEX;
    
    KGFXVertexAttributeDesc attributeDescs[2];
    attributeDescs[0].binding = 0;
    attributeDescs[0].location = 0;
    attributeDescs[0].offset = 0;
    attributeDescs[0].format = KGFX_FORMAT_R32G32B32_FLOAT;
    attributeDescs[1].binding = 0;
    attributeDescs[1].location = 1;
    attributeDescs[1].offset = offsetof(Vertex, r);
    attributeDescs[1].format = KGFX_FORMAT_R32G32B32_FLOAT;
    
    KGFXGraphicsPipelineDesc pipelineDesc;
    pipelineDesc.cullMode = KGFX_CULL_MODE_NONE;
    pipelineDesc.counterClockwiseFrontFaceWinding = KGFX_TRUE;
    pipelineDesc.fillMode = KGFX_FILL_MODE_FILL;
    pipelineDesc.topology = KGFX_TOPOLOGY_TRIANGLES;
    pipelineDesc.viewportAndScissorCount = 1;
    pipelineDesc.vertexInputDesc.bindingCount = 1;
    pipelineDesc.vertexInputDesc.pBindings = &bindingDesc;
    pipelineDesc.vertexInputDesc.attributeCount = sizeof(attributeDescs) / sizeof(attributeDescs[0]);
    pipelineDesc.vertexInputDesc.pAttributes = attributeDescs;
    pipelineDesc.shaderCount = 2;
    pipelineDesc.pShaders = shaders;
    pipelineDesc.blendLogicOp = KGFX_LOGIC_OP_DISABLED;
    pipelineDesc.framebufferWidth = swapchainDesc.width;
    pipelineDesc.framebufferHeight = swapchainDesc.height;
    pipelineDesc.framebufferLayers = 1;
    pipelineDesc.renderTargetCount = sizeof(renderTargetDescs) / sizeof(renderTargetDescs[0]);
    pipelineDesc.pRenderTargetDescs = renderTargetDescs;
    
    pipelineDesc.depthStencilDesc.format = KGFX_FORMAT_D32_FLOAT;
    pipelineDesc.depthStencilDesc.width = swapchainDesc.width;
    pipelineDesc.depthStencilDesc.height = swapchainDesc.height;
    pipelineDesc.depthStencilDesc.layers = 1;
    pipelineDesc.depthStencilDesc.compareOp = KGFX_COMPARE_OP_LEQUAL;
    pipelineDesc.depthStencilDesc.writeDepth = KGFX_TRUE;
    pipelineDesc.depthStencilDesc.testDepth = KGFX_TRUE;
    pipelineDesc.depthStencilDesc.writeStencil = KGFX_FALSE;
    pipelineDesc.depthStencilDesc.testStencil = KGFX_FALSE;
    pipelineDesc.depthStencilDesc.readMaskStencil = 0;
    pipelineDesc.depthStencilDesc.writeMaskStencil = 0;
    pipelineDesc.depthStencilDesc.frontStencilOpDesc.failOp = KGFX_STENCIL_OP_KEEP;
    pipelineDesc.depthStencilDesc.frontStencilOpDesc.failDepthOp = KGFX_STENCIL_OP_KEEP;
    pipelineDesc.depthStencilDesc.frontStencilOpDesc.passOp = KGFX_STENCIL_OP_REPLACE;
    pipelineDesc.depthStencilDesc.frontStencilOpDesc.compareOp = KGFX_COMPARE_OP_LESS;
    pipelineDesc.depthStencilDesc.frontStencilOpDesc.reference = 0;
    pipelineDesc.depthStencilDesc.backStencilOpDesc.failOp = KGFX_STENCIL_OP_KEEP;
    pipelineDesc.depthStencilDesc.backStencilOpDesc.failDepthOp = KGFX_STENCIL_OP_KEEP;
    pipelineDesc.depthStencilDesc.backStencilOpDesc.passOp = KGFX_STENCIL_OP_KEEP;
    pipelineDesc.depthStencilDesc.backStencilOpDesc.compareOp = KGFX_COMPARE_OP_LESS;
    pipelineDesc.depthStencilDesc.backStencilOpDesc.reference = 0;
    pipelineDesc.depthStencilDesc.maximumDepth = 1.0f;
    pipelineDesc.depthStencilDesc.minimumDepth = 0.0f;
    pipelineDesc.depthStencilDesc.loadOp = KGFX_RENDER_TARGET_OP_CLEAR;
    pipelineDesc.depthStencilDesc.storeOp = KGFX_RENDER_TARGET_OP_STORE;
    pipelineDesc.depthStencilDesc.stencilLoadOp = KGFX_RENDER_TARGET_OP_CLEAR;
    pipelineDesc.depthStencilDesc.stencilStoreOp = KGFX_RENDER_TARGET_OP_STORE;
    pipelineDesc.depthStencilDesc.finalLayout = KGFX_TEXTURE_LAYOUT_DEPTH_READ;
    
    KGFXUniformDesc uniformDesc;
    uniformDesc.resourceType = KGFX_RESOURCE_TYPE_UNIFORM_BUFFER;
    uniformDesc.binding.type = KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX;
    uniformDesc.binding.bindPoint.bindingIndex.binding = 0;
    uniformDesc.stages = KGFX_SHADER_STAGE_VERTEX;
    uniformDesc.arrayLength = 1;
    
    pipelineDesc.uniformSignatureDesc.uniformCount = 1;
    pipelineDesc.uniformSignatureDesc.pUniforms = &uniformDesc;
    
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

    Vertex vertices[] = {
        { 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
        { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.5f, 0.2f, 0.0f, 0.0f },
        { 1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 0.2f },
        { -1.0f, -1.0f, 0.5f, 0.0f, 0.2f, 0.0f },
    };

    KGFXBuffer vertexBuffer;
    result = kgfxCreateBuffer(device, sizeof(vertices), KGFX_BUFFER_USAGE_VERTEX_BUFFER | KGFX_BUFFER_USAGE_TRANSFER_DST, KGFX_RESOURCE_LOCATION_DEVICE, &vertexBuffer);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create vertex buffer\n");
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

    result = kgfxUploadBuffer(vertexBuffer, vertices, sizeof(vertices));
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to upload to vertex buffer\n");
        kgfxDestroyBuffer(vertexBuffer);
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
    
    KGFXBuffer uniformBuffer;
    result = kgfxCreateBuffer(device, sizeof(UniformData), KGFX_BUFFER_USAGE_UNIFORM_BUFFER, KGFX_RESOURCE_LOCATION_HOST, &uniformBuffer);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create uniform buffer\n");
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
    
    UniformData* pUniformData;
    result = kgfxMapBuffer(uniformBuffer, &pUniformData);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to map uniform buffer\n");
        kgfxDestroyBuffer(uniformBuffer);
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
    
    KGFXTextureDesc textureDesc;
    textureDesc.dimensionType = KGFX_DIMENSION_TYPE_2D;
    textureDesc.width = swapchainDesc.width;
    textureDesc.height = swapchainDesc.height;
    textureDesc.depth = 1;
    textureDesc.layers = 1;
    textureDesc.mipMapLevels = 1;
    textureDesc.format = KGFX_FORMAT_D32_FLOAT;
    textureDesc.usage = KGFX_TEXTURE_USAGE_DEPTH_STENCIL_TARGET;
    textureDesc.location = KGFX_RESOURCE_LOCATION_DEVICE;
    
    KGFXTexture depthTexture;
    result = kgfxCreateTexture(device, &textureDesc, &depthTexture);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create depth texture\n");
        kgfxUnmapBuffer(uniformBuffer);
        kgfxDestroyBuffer(uniformBuffer);
        kgfxDestroyBuffer(vertexBuffer);
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
    
    while (!glfwWindowShouldClose(window)) {
        mat4x4_identity(pUniformData->mvp);
        mat4x4_rotate_Z(pUniformData->mvp, pUniformData->mvp, glfwGetTime());
        
        kgfxResetCommandList(commandList);
        
        result = kgfxOpenCommandList(commandList, KGFX_FALSE);
        if (result != KGFX_RESULT_SUCCESS) {
            printf("Failed to open command list\n");
            kgfxDestroyTexture(depthTexture);
            kgfxUnmapBuffer(uniformBuffer);
            kgfxDestroyBuffer(uniformBuffer);
            kgfxDestroyBuffer(vertexBuffer);
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

        KGFXUniformBinding binding;
        binding.bindPoint.bindingIndex.binding = 0;
        binding.type = KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX;
        kgfxCmdBindUniformBuffer(commandList, binding, uniformBuffer, 0, sizeof(UniformData));

        uint64_t offset = 0;
        kgfxCmdBindVertexBuffers(commandList, 0, 1, &vertexBuffer, &offset);

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
        
        KGFXTexture renderTargets[1] = { backbuffer };
        kgfxCmdBindRenderTargets(commandList, sizeof(renderTargets) / sizeof(renderTargets[0]), renderTargets, depthTexture);
        
        KGFXClearValue clearValue;
        clearValue.type = KGFX_CLEAR_VALUE_TYPE_F32;
        clearValue.value.f32[0] = 0.01f;
        clearValue.value.f32[1] = 0.0f;
        clearValue.value.f32[2] = 0.02f;
        clearValue.value.f32[3] = 1.0f;
        
        KGFXClearValue depthClearValue;
        depthClearValue.type = KGFX_CLEAR_VALUE_TYPE_DEPTH_STENCIL;
        depthClearValue.value.depthStencil.depth = 1.0f;
        depthClearValue.value.depthStencil.stencil = 0;

        kgfxCmdBeginRendering(commandList, 1, &clearValue, &depthClearValue);

        kgfxCmdDraw(commandList, sizeof(vertices) / sizeof(Vertex), 1, 0, 0);

        kgfxCmdEndRendering(commandList);
        result = kgfxCloseCommandList(commandList);
        if (result != KGFX_RESULT_SUCCESS) {
            printf("Failed to close command list\n");
            kgfxDestroyTexture(depthTexture);
            kgfxUnmapBuffer(uniformBuffer);
            kgfxDestroyBuffer(uniformBuffer);
            kgfxDestroyBuffer(vertexBuffer);
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
            kgfxDestroyTexture(depthTexture);
            kgfxUnmapBuffer(uniformBuffer);
            kgfxDestroyBuffer(uniformBuffer);
            kgfxDestroyBuffer(vertexBuffer);
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
            kgfxDestroyTexture(depthTexture);
            kgfxUnmapBuffer(uniformBuffer);
            kgfxDestroyBuffer(uniformBuffer);
            kgfxDestroyBuffer(vertexBuffer);
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
    
    kgfxDestroyTexture(depthTexture);
    kgfxUnmapBuffer(uniformBuffer);
    kgfxDestroyBuffer(uniformBuffer);
    kgfxDestroyBuffer(vertexBuffer);
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
