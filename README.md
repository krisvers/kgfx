# KGFX

A modern single-header graphics library designed to implement a shared low-level interface across Vulkan, D3D12, and Metal 

## Goal

This project is a redesign of previous KGFX iteration which I abandoned after finding flaws with them. With the most recent iteration, the build solution was clunky to use with other projects and I kept encountering frustrations with dynamic linking. The sokol graphics library is a less influential source for this project, but the idea for single-header originated from it.

Overall, this is meant to serve as a personal library to use lower level graphics programming concepts with less initialization-heavy code (*ahem* Vulkan). I also plan to avoid many design decisions that would lead to this becoming OpenGL and sharing its interesting dualism between a fixed-function state machine and a dynamic pipeline with separable contexts. Multi-threading is not a focus of this project currently.

While I don't see there being many opportunities for this to be useful, you can technically use multiple APIs under the hood in separate instances of KGFX concurrently. It just seems like a cool bit of freedom to support.

## Roadmap

1. Draw a triangle purely using KGFX interface across Vulkan, D3D12, and Metal
    - (✅) Device acquisition (All)
    - (🔨) Swapchains (Only on macOS and Win32 currently 🌋)
    - (🔨) Command utilities (Basic commands and creation 🌋)
    - (🔨) Graphics pipelines (Basic parts 🌋)
	- (🔨) Resource creation and usage (Buffers work, textures are being worked on 🌋)

## Priorities

The default C interface will be much higher priority than a C++ wrapper, though it is planned. Other language bindings might have higher precedence than the C++ wrapper, but this is further in the roadmap than is currently ideal.

Vulkan will likely be the first to have all features implemented as it is at the highest priority. D3D12 is second highest priority and Metal is at the lowest currently.

Often, I will add macOS features earlier than Windows and Linux features due to my main development platform being on a MacBook.

A native/underlying API exposure layer might be added to extend the use of this library, but is not planned currently. It is technically possible to use the library like this if you do this inside the file with an API implementation include as this exposes the internals to the translation unit that includes it.

Implementing KGFX with higher-level APIs (OpenGL, D3D11, ...) is unlikely to happen especially when targetting lower-level, explicit features as they have no methods to perform certain operations without costly and unnecessary emulation.

Other lower-level APIs like GNM or NVN are likely outside of the scope of this project due to the closed-source nature of them. Supporting WebGPU does not make much sense as this project is similar in nature being an abstraction to provide a uniform interface across the newer low-level APIs.

A software rasterization implementation could be planned, but is highly unlikely and wouldn't be very useful.

Overall, a list of priorities would be:

- 1. Vulkan implementation
- 2. D3D12 implementation
- 3. Cross-API HLSL Support
- 4. Cross-API SPIR-V Support
- 5. Cross-API GLSL Support
- 6. Language bindings (Java, C#, Rust, ...)
- 7. C++ wrapper
- 8. Metal implementation
- 9. Other shader language support (Slang, Shady-based compilers, ...)
- 10. API exposure interface
- 11. Emulating low-level with a higher level underlying API
- 12. Low-level proprietary APIs (very unlikely)

## Targets

For the Vulkan implementation it will require version 1.2.xxx with certain common extensions (`VK_KHR_swapchain`, `VK_KHR_surface`, `VK_EXT_descriptor_indexing`, `VK_KHR_maintenance1`)

The lowest officially supported hardware will be (with most up-to-date drivers): NVIDIA GTX 660 Ti, Intel HD 630, R9 200 Series, and M1 MacBook Air GPU. Support is not explicitly planned for any older hardware or mobile device GPUs, but KGFX might still function on some certain devices.