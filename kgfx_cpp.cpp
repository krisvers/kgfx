#include "common.h"

#ifdef TEST_KGFX_CPP
#define KGFX_VULKAN
#define KGFX_VULKAN_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define KGFX_WIN32
#define KGFX_D3D11
#define KGFX_D3D11_IMPLEMENTATION
#define KGFX_D3D12
#define KGFX_D3D12_IMPLEMENTATION
#elif defined(__APPLE__)
#define KGFX_COCOA
#define KGFX_METAL
#else
#define KGFX_XLIB
#define KGFX_XCB
#define KGFX_WAYLAND
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */

#define KGFX_IMPLEMENTATION
#include "kgfx.h"

#endif /* #ifdef TEST_KGFX_CPP */
