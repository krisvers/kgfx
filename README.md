# kgfx #
a medium-level graphics library

## project structure ##
- [src](src/.): top level example directory
- [src/kgfx](src/kgfx/.): kgfx implementations source code (Vulkan, Metal, ...)
- [src/kgfx_gh](src/kgfx_gh/.): kgfx GLFW helper library source code
  - although technically part of this repository, it functions more as a glue library separate from kgfx
- [include](include/kgfx/.): kgfx headers
- [include](include/kgfx_gh/.): kgfx GLFW helper library headers
- [lib/include](lib/include/.): vendor library headers
- [lib](lib/.): platform directories for library files
  - [lib/vs](lib/vs/.): visual studio 2022 library files
    - [lib/vs/x86](lib/vs/x86/.): x86 specific library files
    - [lib/vs/x64](lib/vs/x64/.): x64 specific library files
  - [lib/mac](lib/mac/.): macOS library files
- [assets](assets/.): assets directory for kgfx examples

## API documentation ##
