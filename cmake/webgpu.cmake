if (TARGET webgpu::webgpu)
    return()
endif()

message(STATUS "Third-party (external): creating target webgpu::webgpu")

include(CPM)
CPMAddPackage(
    NAME webgpu-dist
    GITHUB_REPOSITORY eliemichel/WebGPU-distribution
    GIT_TAG f46c4344b85f6bc538f08720c369472bcfd62ade
)

add_library(webgpu::webgpu ALIAS webgpu)
