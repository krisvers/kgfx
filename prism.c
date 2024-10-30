#include "common.h"

#ifdef TEST_PRISM_C
#define PRISM_VULKAN
#define PRISM_VULKAN_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define PRISM_D3D12
#define PRISM_D3D12_IMPLEMENTATION
#elif defined(__APPLE__)
#define PRISM_METAL
#endif /* #if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) */

#define PRISM_IMPLEMENTATION
#include "prism.h"

#endif /* #ifdef TEST_PRISM_C */
