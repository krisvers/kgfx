#include "common.h"

#ifdef TEST_KGFX_CPP
#define KGFX_VULKAN
#define KGFX_VULKAN_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define KGFX_D3D12
#define KGFX_D3D12_IMPLEMENTATION
#elif defined(__APPLE__)
#define KGFX_METAL
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */

#define KGFX_IMPLEMENTATION
#include "kgfx.h"

#endif /* #ifdef TEST_KGFX_CPP */
