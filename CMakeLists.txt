cmake_minimum_required(VERSION 3.10)
project(WoWDX9Hook)

# Force 32-bit build
set(CMAKE_GENERATOR_PLATFORM Win32)
set(CMAKE_VS_PLATFORM_NAME "Win32")
set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x86")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add source files
set(SOURCE_FILES
    dllmain.cpp
    hook.cpp
)

# ImGui source files
set(IMGUI_FILES
    ImGui/imgui.cpp
    ImGui/imgui_demo.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp
    ImGui/backends/imgui_impl_dx9.cpp
    ImGui/backends/imgui_impl_win32.cpp
)

# Create shared library
add_library(WoWDX9Hook SHARED ${SOURCE_FILES} ${IMGUI_FILES})

# MinHook
add_library(minhook STATIC
    MinHook/src/buffer.c
    MinHook/src/hook.c
    MinHook/src/trampoline.c
    MinHook/src/hde/hde32.c
)

target_include_directories(minhook PUBLIC MinHook/include)

# Include directories
target_include_directories(WoWDX9Hook PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ImGui
    ImGui/backends
    MinHook/include
    "$ENV{DXSDK_DIR}/Include"
)

# Link directories
target_link_directories(WoWDX9Hook PRIVATE
    "$ENV{DXSDK_DIR}/Lib/x86"
)

# Link libraries
target_link_libraries(WoWDX9Hook PRIVATE
    minhook
    d3d9
    d3dx9
) 