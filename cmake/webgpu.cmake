if (TARGET webgpu::webgpu)
    return()
endif()

message(STATUS "Third-party (external): creating target webgpu::webgpu")

include(CPM)
CPMAddPackage(
    NAME webgpu-dist
    GITHUB_REPOSITORY eliemichel/WebGPU-distribution
    GIT_TAG dawn
)

add_library(webgpu::webgpu ALIAS webgpu)
