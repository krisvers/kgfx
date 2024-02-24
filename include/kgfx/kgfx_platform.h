#ifndef KRISVERS_KGFX_PLATFORM_H
#define KRISVERS_KGFX_PLATFORM_H

#if defined(_WIN32)
#define KGFX_WINDOWS 1
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#define KGFX_LINUX 1
#elif defined(__APPLE__)
#define KGFX_MACOS 1
#endif

#endif