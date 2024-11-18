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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

typedef struct Vertex {
    float x, y, z;
    float r, g, b;
    float u, v;
} Vertex;

typedef struct UniformData {
    mat4x4 mvp;
} UniformData;

typedef struct TestKGFXState {
    KGFXBool isValid;
    
    const char* apiString;
    uint32_t width, height;
    uint32_t triangleCount;
    UniformData* pUniformData;
    KGFXTexture backbuffer;
    
    KGFXInstance instance;
    KGFXDevice device;
    KGFXSwapchain swapchain;
    KGFXShader vertexShader;
    KGFXShader fragmentShader;
    KGFXGraphicsPipeline graphicsPipeline;
    KGFXCommandPool commandPool;
    KGFXCommandList commandList;
    KGFXBuffer vertexBuffer;
    KGFXBuffer uniformBuffer;
    KGFXTexture logoTexture;
    KGFXTexture depthTexture;
    KGFXSampler nearestSampler;
} TestKGFXState;

void destroyTestState(TestKGFXState* pState) {
    if (!pState->isValid) {
        return;
    }
    
    pState->isValid = KGFX_FALSE;
    if (pState->nearestSampler != NULL) { kgfxDestroySampler(pState->nearestSampler); }
    if (pState->depthTexture != NULL) { kgfxDestroyTexture(pState->depthTexture); }
    if (pState->logoTexture != NULL) { kgfxDestroyTexture(pState->logoTexture); }
    if (pState->uniformBuffer != NULL) { kgfxDestroyBuffer(pState->uniformBuffer); }
    if (pState->vertexBuffer != NULL) { kgfxDestroyBuffer(pState->vertexBuffer); }
    if (pState->commandList != NULL) { kgfxDestroyCommandList(pState->commandList); }
    if (pState->commandPool != NULL) { kgfxDestroyCommandPool(pState->commandPool); }
    if (pState->graphicsPipeline != NULL) { kgfxDestroyGraphicsPipeline(pState->graphicsPipeline); }
    if (pState->fragmentShader != NULL) { kgfxDestroyShader(pState->fragmentShader); }
    if (pState->vertexShader != NULL) { kgfxDestroyShader(pState->vertexShader); }
    if (pState->swapchain != NULL) { kgfxDestroySwapchain(pState->swapchain); }
    if (pState->device != NULL) { kgfxDestroyDevice(pState->device); }
    if (pState->instance != NULL) { kgfxDestroyInstance(pState->instance); }
}

void debugCallbackKGFX(KGFXInstance instance, KGFXDebugSeverity severity, KGFXDebugSource source, const char* message) {
    printf("[KGFX] %s\n", message);
}

static const char* apiStrings[] = {  "Unknown API", "Vulkan", "D3D12", "Metal", "D3D11" };

const char* getApiString(KGFXInstanceAPI api) {
    if (api == KGFX_INSTANCE_API_VULKAN) {
        return apiStrings[1];
    } else if (api == KGFX_INSTANCE_API_D3D12) {
        return apiStrings[2];
    } else if (api == KGFX_INSTANCE_API_METAL) {
        return apiStrings[3];
    } else if (api == KGFX_INSTANCE_API_D3D11) {
        return apiStrings[4];
    }
    
    return apiStrings[0];
}

KGFXBool loadFile(const char* filename, char** ppData, size_t* pSize) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return KGFX_FALSE;
    }

    fseek(fp, 0, SEEK_END);
    *pSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *ppData = (char*) malloc(*pSize);
    if (*ppData == NULL) {
        fclose(fp);
        return KGFX_FALSE;
    }

    fread(*ppData, 1, *pSize, fp);
    fclose(fp);

    return KGFX_TRUE;
}

KGFXResult createShaderHLSL(TestKGFXState* pState) {
    char* source;
    size_t length;
    if (!loadFile("shaders/src/shader_hlsl.hlsl", &source, &length)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    KGFXShader shader;
    KGFXResult result = kgfxCreateShaderHLSL(pState->device, source, (uint32_t) length, "vs_main", KGFX_SHADER_STAGE_VERTEX, 5, 0, 0, NULL, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(source);
        return result;
    }

    pState->vertexShader = shader;

    result = kgfxCreateShaderHLSL(pState->device, source, (uint32_t) length, "ps_main", KGFX_SHADER_STAGE_FRAGMENT, 5, 0, 0, NULL, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(source);
        return result;
    }

    free(source);
    pState->fragmentShader = shader;

    return KGFX_RESULT_SUCCESS;
}

KGFXResult createShaderDXBC(TestKGFXState* pState) {
    char* vsource;
    size_t vlength;
    if (!loadFile("shaders/bin/vshader.dxbc", &vsource, &vlength)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    KGFXShader shader;
    KGFXResult result = kgfxCreateShaderDXBC(pState->device, vsource, (uint32_t) vlength, "vs_main", KGFX_SHADER_STAGE_VERTEX, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(vsource);
        return result;
    }

    free(vsource);
    pState->vertexShader = shader;

    char* psource;
    size_t plength;
    if (!loadFile("shaders/bin/pshader.dxbc", &psource, &plength)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    result = kgfxCreateShaderDXBC(pState->device, psource, (uint32_t) plength, "ps_main", KGFX_SHADER_STAGE_FRAGMENT, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(psource);
        return result;
    }

    free(psource);
    pState->fragmentShader = shader;

    return KGFX_RESULT_SUCCESS;
}

KGFXResult createShaderGLSL(TestKGFXState* pState) {
    char* vsource;
    size_t vlength;
    if (!loadFile("shaders/src/shader.vert.glsl", &vsource, &vlength)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    KGFXShader shader;
    KGFXResult result = kgfxCreateShaderGLSL(pState->device, vsource, (uint32_t) vlength, "main", KGFX_SHADER_STAGE_VERTEX, 450, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(vsource);
        return result;
    }

    free(vsource);
    pState->vertexShader = shader;

    char* fsource;
    size_t flength;
    if (!loadFile("shaders/src/shader.frag.glsl", &fsource, &flength)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    result = kgfxCreateShaderGLSL(pState->device, fsource, (uint32_t) flength, "main", KGFX_SHADER_STAGE_FRAGMENT, 450, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(fsource);
        return result;
    }

    free(fsource);
    pState->fragmentShader = shader;

    return KGFX_RESULT_SUCCESS;
}

KGFXResult createShaderSPIRV(TestKGFXState* pState) {
    char* vsource;
    size_t vlength;
    if (!loadFile("shaders/bin/vshader.spv", &vsource, &vlength)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    KGFXShader shader;
    KGFXResult result = kgfxCreateShaderSPIRV(pState->device, vsource, (uint32_t) vlength, "main", KGFX_SHADER_STAGE_VERTEX, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(vsource);
        return result;
    }

    free(vsource);
    pState->vertexShader = shader;

    char* fsource;
    size_t flength;
    if (!loadFile("shaders/bin/fshader.spv", &fsource, &flength)) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }

    result = kgfxCreateShaderSPIRV(pState->device, fsource, (uint32_t) flength, "main", KGFX_SHADER_STAGE_FRAGMENT, &shader);
    if (result != KGFX_RESULT_SUCCESS) {
        free(fsource);
        return result;
    }

    free(fsource);
    pState->fragmentShader = shader;

    return KGFX_RESULT_SUCCESS;
}

KGFXResult createShaders(TestKGFXState* pState) {
    KGFXResult result = createShaderHLSL(pState);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create HLSL shader(s) on %s falling back to GLSL\n", pState->apiString);
    } else {
        return result;
    }

    result = createShaderGLSL(pState);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create GLSL shader(s) on %s falling back to DXBC\n", pState->apiString);
    } else {
        return result;
    }

    result = createShaderDXBC(pState);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create DXBC shader(s) on %s falling back to SPIRV\n", pState->apiString);
    } else {
        return result;
    }

    result = createShaderSPIRV(pState);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create SPIRV shader(s) on %s\n", pState->apiString);
    }

    return result;
}

KGFXResult testSetup(TestKGFXState* pState, GLFWwindow* window, KGFXInstanceAPI api) {
    pState->isValid = KGFX_TRUE;
    pState->apiString = getApiString(api);
    
    KGFXResult result = kgfxCreateInstance(api, KGFX_INSTANCE_CREATE_FLAG_DEBUG | KGFX_INSTANCE_CREATE_FLAG_VALIDATION | KGFX_INSTANCE_CREATE_FLAG_GRAPHICAL, &pState->instance);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create instance on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    kgfxDebugRegisterCallback(pState->instance, debugCallbackKGFX);

    KGFXAdapterDetails adapterDetails = { 0 };
    const char memSuffixLookup[] = { 'B', 'K', 'M', 'G', 'T', 'P' };
    for (uint32_t i = 0; kgfxEnumerateAdapters(pState->instance, i, &adapterDetails) == KGFX_RESULT_ENUMERATION_IN_PROGRESS; i++) {
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

    result = kgfxCreateDevice(pState->instance, 0, &pState->device);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create device on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    KGFXSwapchainDesc swapchainDesc;
    swapchainDesc.width = pState->width;
    swapchainDesc.height = pState->height;
    swapchainDesc.format = KGFX_FORMAT_B8G8R8A8_SRGB;
    swapchainDesc.imageCount = 2;
    swapchainDesc.presentMode = KGFX_PRESENT_MODE_VSYNC;
    
#ifdef KGFX_WIN32
    result = kgfxCreateSwapchainWin32(pState->device, glfwGetWin32Window(window), GetModuleHandle(NULL), &swapchainDesc, &pState->swapchain);
#elif defined(KGFX_XLIB) || defined(KGFX_WAYLAND)

#ifdef TEST_WAYLAND
    result = kgfxCreateSwapchainWayland(pState->device, glfwGetWaylandDisplay(), glfwGetWaylandWindow(window), &swapchainDesc, &pState->swapchain);
#else
    result = kgfxCreateSwapchainXlib(pState->device, glfwGetX11Display(), glfwGetX11Window(window), &swapchainDesc, &pState->swapchain);
#endif /* #ifdef TEST_WAYLAND */

#elif defined(KGFX_COCOA)
    result = kgfxCreateSwapchainCocoa(pState->device, glfwGetCocoaWindow(window), &swapchainDesc, &pState->swapchain);
#endif /* #ifdef KGFX_WIN32 */
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create swapchain on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }

    pState->backbuffer = kgfxGetSwapchainBackbuffer(pState->swapchain);
    
    result = createShaders(pState);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create shader(s) on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    KGFXShader shaders[2] = { pState->vertexShader, pState->fragmentShader };
    
    KGFXRenderTargetDesc renderTargetDescs[1];
    renderTargetDescs[0].format = swapchainDesc.format;
    renderTargetDescs[0].width = pState->width;
    renderTargetDescs[0].height = pState->height;
    renderTargetDescs[0].layers = 1;
    renderTargetDescs[0].enableBlending = KGFX_TRUE;
    renderTargetDescs[0].colorBlendOp = KGFX_BLEND_OP_ADD;
    renderTargetDescs[0].srcColorBlendFactor = KGFX_BLEND_FACTOR_SRC_ALPHA;
    renderTargetDescs[0].dstColorBlendFactor = KGFX_BLEND_FACTOR_INVERTED_SRC_ALPHA;
    renderTargetDescs[0].alphaBlendOp = KGFX_BLEND_OP_ADD;
    renderTargetDescs[0].srcAlphaBlendFactor = KGFX_BLEND_FACTOR_ONE;
    renderTargetDescs[0].dstAlphaBlendFactor = KGFX_BLEND_FACTOR_ZERO;
    renderTargetDescs[0].colorWriteMask = KGFX_COLOR_MASK_ALL;
    renderTargetDescs[0].loadOp = KGFX_RENDER_TARGET_OP_CLEAR;
    renderTargetDescs[0].storeOp = KGFX_RENDER_TARGET_OP_STORE;
    renderTargetDescs[0].finalLayout = KGFX_TEXTURE_LAYOUT_PRESENT;
    
    KGFXVertexBindingDesc bindingDesc;
    bindingDesc.binding = 0;
    bindingDesc.stride = sizeof(Vertex);
    bindingDesc.inputRate = KGFX_VERTEX_INPUT_RATE_PER_VERTEX;
    
    KGFXVertexAttributeDesc attributeDescs[3];
    attributeDescs[0].binding = 0;
    attributeDescs[0].location = 0;
    attributeDescs[0].offset = offsetof(Vertex, x);
    attributeDescs[0].format = KGFX_FORMAT_R32G32B32_FLOAT;
    attributeDescs[1].binding = 0;
    attributeDescs[1].location = 1;
    attributeDescs[1].offset = offsetof(Vertex, r);
    attributeDescs[1].format = KGFX_FORMAT_R32G32B32_FLOAT;
    attributeDescs[2].binding = 0;
    attributeDescs[2].location = 2;
    attributeDescs[2].offset = offsetof(Vertex, u);
    attributeDescs[2].format = KGFX_FORMAT_R32G32_FLOAT;
    
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
    pipelineDesc.depthStencilDesc.width = pState->width;
    pipelineDesc.depthStencilDesc.height = pState->height;
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
    
    KGFXUniformDesc uniformDescs[3];
    uniformDescs[0].resourceType = KGFX_RESOURCE_TYPE_UNIFORM_BUFFER;
    uniformDescs[0].binding.type = KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX;
    uniformDescs[0].binding.bindPoint.bindingIndex.binding = 0;
    uniformDescs[0].stages = KGFX_SHADER_STAGE_VERTEX;
    uniformDescs[0].arrayLength = 1;
    uniformDescs[1].resourceType = KGFX_RESOURCE_TYPE_UNIFORM_TEXTURE;
    uniformDescs[1].binding.type = KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX;
    uniformDescs[1].binding.bindPoint.bindingIndex.binding = 1;
    uniformDescs[1].stages = KGFX_SHADER_STAGE_FRAGMENT;
    uniformDescs[1].arrayLength = 1;
    uniformDescs[2].resourceType = KGFX_RESOURCE_TYPE_SAMPLER;
    uniformDescs[2].binding.type = KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX;
    uniformDescs[2].binding.bindPoint.bindingIndex.binding = 2;
    uniformDescs[2].stages = KGFX_SHADER_STAGE_FRAGMENT;
    uniformDescs[2].arrayLength = 1;
    
    pipelineDesc.uniformSignatureDesc.uniformCount = sizeof(uniformDescs) / sizeof(uniformDescs[0]);
    pipelineDesc.uniformSignatureDesc.pUniforms = uniformDescs;
    
    result = kgfxCreateGraphicsPipeline(pState->device, &pipelineDesc, &pState->graphicsPipeline);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create graphics pipeline on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    result = kgfxCreateCommandPool(pState->device, 1, KGFX_QUEUE_TYPE_GENERIC, &pState->commandPool);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create command pool on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    result = kgfxCreateCommandList(pState->commandPool, &pState->commandList);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create command list on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }

    Vertex vertices[] = {
        { -2.0f,  1.0f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f },
        {  2.0f,  1.0f, -0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
        { -2.0f, -1.0f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
        {  2.0f, -1.0f, -0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f },
        {  2.0f,  1.0f, -0.5f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f },
        { -2.0f, -1.0f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f },
    };
    
    pState->triangleCount = sizeof(vertices) / sizeof(vertices[0]);

    result = kgfxCreateBuffer(pState->device, sizeof(vertices), KGFX_BUFFER_USAGE_VERTEX_BUFFER | KGFX_BUFFER_USAGE_TRANSFER_DST, KGFX_RESOURCE_LOCATION_DEVICE, &pState->vertexBuffer);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create vertex buffer on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }

    result = kgfxUploadBuffer(pState->vertexBuffer, vertices, sizeof(vertices));
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to upload to vertex buffer on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    result = kgfxCreateBuffer(pState->device, sizeof(UniformData), KGFX_BUFFER_USAGE_UNIFORM_BUFFER, KGFX_RESOURCE_LOCATION_HOST, &pState->uniformBuffer);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create uniform buffer on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    result = kgfxMapBuffer(pState->uniformBuffer, (void*) &pState->pUniformData);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to map uniform buffer on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    int w, h, components;
    uint8_t* pixels = stbi_load("logo.png", &w, &h, &components, STBI_rgb_alpha);
    if (pixels == NULL) {
        printf("Failed to load logo.png on %s\n", pState->apiString);
        destroyTestState(pState);
        return KGFX_RESULT_ERROR_UNKNOWN;
    }
    
    KGFXTextureDesc textureDesc;
    textureDesc.dimensionType = KGFX_DIMENSION_TYPE_2D;
    textureDesc.width = w;
    textureDesc.height = h;
    textureDesc.depth = 1;
    textureDesc.layers = 1;
    textureDesc.mipMapLevels = 1;
    textureDesc.format = KGFX_FORMAT_R8G8B8A8_SRGB;
    textureDesc.usage = KGFX_TEXTURE_USAGE_UNIFORM_TEXTURE | KGFX_TEXTURE_USAGE_TRANSFER_DST;
    textureDesc.location = KGFX_RESOURCE_LOCATION_DEVICE;
    
    result = kgfxCreateTexture(pState->device, &textureDesc, &pState->logoTexture);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create logo texture on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    KGFXTextureTransferDesc transferDesc;
    transferDesc.textureX = 0;
    transferDesc.textureY = 0;
    transferDesc.textureZ = 0;
    transferDesc.textureWidth = w;
    transferDesc.textureHeight = h;
    transferDesc.textureDepth = 1;
    transferDesc.textureFirstLayer = 0;
    transferDesc.textureLayerCount = 1;
    
    result = kgfxUploadTexture(pState->logoTexture, pixels, w * h * components, &transferDesc);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to upload logo texture on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    stbi_image_free(pixels);

    textureDesc.width = pState->width;
    textureDesc.height = pState->height;
    textureDesc.format = KGFX_FORMAT_D32_FLOAT;
    textureDesc.usage = KGFX_TEXTURE_USAGE_DEPTH_STENCIL_TARGET;
    textureDesc.location = KGFX_RESOURCE_LOCATION_DEVICE;
    
    result = kgfxCreateTexture(pState->device, &textureDesc, &pState->depthTexture);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create depth texture on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
    
    KGFXSamplerDesc samplerDesc;
    samplerDesc.magFilter = KGFX_TEXTURE_FILTER_NEAREST;
    samplerDesc.minFilter = KGFX_TEXTURE_FILTER_NEAREST;
    samplerDesc.mipMapFilter = KGFX_TEXTURE_FILTER_NEAREST;
    samplerDesc.sampleModeU = KGFX_SAMPLE_MODE_CLAMP;
    samplerDesc.sampleModeV = KGFX_SAMPLE_MODE_CLAMP;
    samplerDesc.sampleModeW = KGFX_SAMPLE_MODE_CLAMP;
    samplerDesc.border = KGFX_SAMPLE_BORDER_TRANSPARENT_BLACK_INT;
    samplerDesc.anisotropy = 1.0f;
    samplerDesc.maxLod = 1.0f;
    samplerDesc.minLod = 0.0f;
    
    result = kgfxCreateSampler(pState->device, &samplerDesc, &pState->nearestSampler);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to create nearest sampler on %s\n", pState->apiString);
        destroyTestState(pState);
        return result;
    }
                         
    return KGFX_RESULT_SUCCESS;
}

KGFXResult testLoop(TestKGFXState* pState) {
    if (!pState->isValid) {
        return KGFX_RESULT_ERROR_UNKNOWN;
    }
    
    mat4x4_identity(pState->pUniformData->mvp);
    mat4x4_rotate_Z(pState->pUniformData->mvp, pState->pUniformData->mvp, glfwGetTime());
    
    mat4x4 proj;
    mat4x4_perspective(proj, 60, 4.0f / 3.0f, 0.0f, 100.0f);
    
    mat4x4_mul(pState->pUniformData->mvp, proj, pState->pUniformData->mvp);
    
    kgfxResetCommandList(pState->commandList);
    
    KGFXResult result = kgfxOpenCommandList(pState->commandList, KGFX_FALSE);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to open command list on %s\n", pState->apiString);
        return result;
    }

    kgfxCmdBindGraphicsPipeline(pState->commandList, pState->graphicsPipeline);

    KGFXUniformBinding binding;
    binding.bindPoint.bindingIndex.binding = 0;
    binding.type = KGFX_UNIFORM_BIND_POINT_TYPE_BINDING_INDEX;
    kgfxCmdBindUniformBuffer(pState->commandList, binding, pState->uniformBuffer, 0, sizeof(UniformData));
    
    binding.bindPoint.bindingIndex.binding = 1;
    kgfxCmdBindUniformTexture(pState->commandList, binding, pState->logoTexture);
    
    binding.bindPoint.bindingIndex.binding = 2;
    kgfxCmdBindSampler(pState->commandList, binding, pState->nearestSampler);

    uint64_t offset = 0;
    kgfxCmdBindVertexBuffers(pState->commandList, 0, 1, &pState->vertexBuffer, &offset);

    KGFXViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) pState->width;
    viewport.height = (float) pState->height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    KGFXScissor scissor;
    scissor.x = 0;
    scissor.y = 0;
    scissor.width = pState->width;
    scissor.height = pState->height;

    kgfxCmdSetViewportAndScissor(pState->commandList, 1, &viewport, &scissor);
    
    KGFXTexture renderTargets[1] = { pState->backbuffer };
    kgfxCmdBindRenderTargets(pState->commandList, sizeof(renderTargets) / sizeof(renderTargets[0]), renderTargets, pState->depthTexture);
    
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

    kgfxCmdBeginRendering(pState->commandList, 1, &clearValue, &depthClearValue);

    kgfxCmdDraw(pState->commandList, pState->triangleCount, 1, 0, 0);

    kgfxCmdEndRendering(pState->commandList);
    result = kgfxCloseCommandList(pState->commandList);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to close command list on %s\n", pState->apiString);
        return result;
    }

    result = kgfxSubmitCommandList(pState->commandList);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to submit command list on %s\n", pState->apiString);
        return result;
    }

    result = kgfxPresentSwapchain(pState->swapchain);
    if (result != KGFX_RESULT_SUCCESS) {
        printf("Failed to present swapchain on %s\n", pState->apiString);
        return result;
    }

    glfwPollEvents();
#ifdef KGFX_WIN32
    Sleep(2);
#else
    usleep(2000);
#endif /* #ifdef KGFX_WIN32 */
    return KGFX_RESULT_SUCCESS;
}

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define API_COUNT 4

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return 1;
    }
    
    uint32_t current = 0;
    KGFXInstanceAPI apis[API_COUNT] = { KGFX_INSTANCE_API_VULKAN, KGFX_INSTANCE_API_D3D12, KGFX_INSTANCE_API_METAL, KGFX_INSTANCE_API_D3D11 };
    TestKGFXState states[API_COUNT];
    GLFWwindow* windows[API_COUNT];
    memset(&states, 0, sizeof(states));
    
    uint32_t totalRunning = API_COUNT;
    for (uint32_t i = 0; i < API_COUNT; ++i) {
        states[i].width = WINDOW_WIDTH;
        states[i].height = WINDOW_HEIGHT;
        
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "KGFX %s Test", getApiString(apis[i]));
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        windows[i] = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, buffer, NULL, NULL);
        if (windows[i] == NULL) {
            --totalRunning;
            printf("Failed to create GLFW window\n");
            continue;
        }
        
        KGFXResult result = testSetup(&states[i], windows[i], apis[i]);
        if (result != KGFX_RESULT_SUCCESS) {
            --totalRunning;
            printf("Failed to setup test for KGFX on %s\n", getApiString(apis[i]));
            glfwDestroyWindow(windows[i]);
        }
    }
    
    while (totalRunning > 0) {
        glfwPollEvents();
        
        for (uint32_t i = 0; i < API_COUNT; ++i) {
            if (!states[i].isValid) {
                continue;
            }
            
            if (testLoop(&states[i]) != KGFX_RESULT_SUCCESS || glfwWindowShouldClose(windows[i])) {
                destroyTestState(&states[i]);
                glfwDestroyWindow(windows[i]);
                --totalRunning;
            }
        }
        
#ifdef KGFX_WIN32
        Sleep(10);
#else
        usleep(10000);
#endif /* #ifdef KGFX_WIN32 */
    }
    
    glfwTerminate();
    
    return 0;
}
