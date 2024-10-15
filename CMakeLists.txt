cmake_minimum_required(VERSION 3.16)
project(
    WGF3
    VERSION 0.0.1 # any version number
    LANGUAGES CXX C # programming languages used by the project
)

add_library(WGF3
    "WGFinstance/Framework.h"
    "WGFinstance/Framework.cpp"
    "WGFinstance/Adapter.h"
    "WGFinstance/Adapter.cpp"
    "WGFinstance/Device.h"
    "WGFinstance/Device.cpp"
    "Utilities/Logging.h"
    "Utilities/Logging.cpp"
    "Utilities/Timer.h"
    "WGFinstance/DeviceLimits.h"
    "WGFinstance/WindowParameters.h"
    "WGFinstance/AppWindow.cpp" "WGFinstance/AppWindow.h"
    "WGFinstance/Events.h"
    "Pipeline/Pipeline.h"
    "Pipeline/RenderPipeline.h"
    "GraphicObjects/Buffer/Buffer.h"
    "GraphicObjects/Buffer/BaseBuffer.h"
    "GraphicObjects/Buffer/BufferLayout.h"
    "GraphicObjects/Shader/Shader.h"
    "GraphicObjects/Shader/Shader.cpp"
 "Pipeline/RenderPipeline.cpp" "Pipeline/RenderPipelineBuilder.h" "GraphicObjects/BindGroup/BindGroup.h" "Utilities/Object3D.cpp" "Utilities/Object3D.h" "Utilities/Cameras/Camera.h" "Utilities/Cameras/TurnTableCamera.h" "GraphicObjects/Textures/DepthTexture.h" "GraphicObjects/Textures/Texture2D.h" "GraphicObjects/Textures/RAIITexture.h" "GraphicObjects/Textures/Texture2D.cpp" "GraphicObjects/Textures/BaseTexture.h" "GraphicObjects/Textures/RAIITextureView.h" "Utilities/Cameras/FPVCamera.h" "GraphicObjects/Textures/Texture2DArray.h" "GraphicObjects/Textures/Image.h" "GraphicObjects/Textures/Image.cpp" "RenderPass/RenderPass.h" "RenderPass/RenderPass.cpp" "RenderPass/RenderPassFactory.h"  "GraphicObjects/Textures/Texture3D.h" "Utilities/Differences.h" "Pipeline/ComputePipeline.h")

set_target_properties(WGF3 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)

target_include_directories(WGF3 PUBLIC WGFinstance)
target_include_directories(WGF3 PUBLIC GraphicObjects)
target_include_directories(WGF3 PUBLIC Utilities)
target_include_directories(WGF3 PUBLIC Pipeline)
target_include_directories(WGF3 PUBLIC RenderPass)
target_include_directories(WGF3 PUBLIC vendor/headerOnly)

add_subdirectory(vendor/glfw3webgpu)
add_subdirectory(vendor/glm)

if (NOT EMSCRIPTEN)
    # Do not include this with emscripten, it provides its own version.
    add_subdirectory(vendor/glfw)
endif()



if (MSVC)
    target_compile_options(WGF3 PRIVATE /W4)
else()
    target_compile_options(WGF3 PRIVATE -Wall -Wextra -pedantic)
endif()

if (XCODE)
    set_target_properties(WGF3 PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal"
    )
endif()

include(FetchContent)

if(EMSCRIPTEN)
    FetchContent_Declare(
    webgpu
    GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
    GIT_TAG        emscripten
    )
else()
    FetchContent_Declare(
    webgpu
    GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
    GIT_TAG        wgpu
    )
endif()


FetchContent_MakeAvailable(webgpu)
target_link_libraries(WGF3 PRIVATE webgpu)

if (EMSCRIPTEN)
    add_library(glfw INTERFACE)
    target_link_options(glfw INTERFACE -sUSE_GLFW=3)

    target_link_options(WGF3 PRIVATE
        -sUSE_GLFW=3 # Use Emscripten-provided GLFW
        -sUSE_WEBGPU # Handle WebGPU symbols
        -sASYNCIFY # Required by WebGPU-C++
    )
    set_target_properties(WGF3 PROPERTIES SUFFIX ".html")
endif()

target_link_libraries(WGF3 PUBLIC glfw glfw3webgpu glm)
target_copy_webgpu_binaries(WGF3)