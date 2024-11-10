# CMakeList.txt : CMake project for kgfx, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("kgfx")
set(CMAKE_BUILD_TYPE Debug)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

set(SOURCES "main.c" "kgfx.h" "kgfx_c.c" "kgfx_cpp.cpp" "common.h")
if (APPLE)
	list(APPEND SOURCES "kgfx_objc.m")
endif()

# Add source to this project's executable.
add_executable(kgfx ${SOURCES})

include_directories(${Vulkan_INCLUDE_DIRS})
target_link_directories(kgfx PRIVATE ${Vulkan_LIBRARY})
target_link_libraries(kgfx PRIVATE ${Vulkan_LIBRARIES})

target_link_libraries(kgfx PRIVATE glfw)

if (WIN32)
    target_link_libraries(kgfx PRIVATE d3d12.lib dxgi.lib d3dcompiler.lib)
elseif (APPLE)
    target_link_libraries(kgfx PRIVATE "-framework Metal")
    target_link_libraries(kgfx PRIVATE "-framework Cocoa")
    target_link_libraries(kgfx PRIVATE "-framework QuartzCore")
else()
    if (DEFINED ENV{WAYLAND_DISPLAY})
        target_link_libraries(kgfx PRIVATE wayland-client)
        target_compile_definitions(kgfx PRIVATE TEST_WAYLAND)
	message(STATUS "teatet")
    endif()
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET kgfx PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
