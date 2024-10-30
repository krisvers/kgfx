# CMakeList.txt : CMake project for prism, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("prism")

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Add source to this project's executable.
add_executable(prism "main.c" "prism.h" "prism.c" "prism.cpp" "prism.m" "common.h")

include_directories(${Vulkan_INCLUDE_DIRS})
target_link_directories(prism PRIVATE ${Vulkan_LIBRARY})
target_link_libraries(prism PRIVATE ${Vulkan_LIBRARIES})

message(STATUS ${glfw3_INCLUDE_DIR} ${glfw3_DIR})
target_include_directories(prism PUBLIC ${glfw3_INCLUDE_DIRS})
target_link_libraries(prism PRIVATE ${GLFW_LIBRARIES})

if (WIN32)
    target_link_libraries(prism PRIVATE d3d12.lib dxgi.lib d3dcompiler.lib)
elseif (APPLE)
    target_link_libraries(prism PRIVATE "-framework Metal")
    target_link_libraries(prism PRIVATE "-framework Cocoa")
    target_link_libraries(prism PRIVATE "-framework QuartzCore")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET prism PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
