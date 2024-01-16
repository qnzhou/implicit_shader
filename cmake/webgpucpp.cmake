if (TARGET webgpucpp::webgpucpp)
    return()
endif()

message(STATUS "Third-party (external): creating target webgpucpp::webgpucpp")

include(CPM)
CPMAddPackage(
    NAME dawn
    URL https://raw.githubusercontent.com/eliemichel/WebGPU-Cpp/90c7c5ca1ed0450a78ee9591bf2a2602f6236b24/dawn/webgpu.hpp
    DOWNLOAD_NO_EXTRACT 1
    DOWNLOAD_ONLY ON
)

include(dawn)
add_library(webgpucpp INTERFACE)
target_include_directories(webgpucpp INTERFACE ${dawn_SOURCE_DIR})
target_link_libraries(webgpucpp INTERFACE dawn::dawn)

add_library(webgpucpp::webgpucpp ALIAS webgpucpp)
