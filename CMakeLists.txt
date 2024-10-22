cmake_minimum_required(VERSION 3.0...3.25)
project(implicit_shader)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(webgpu)
include(mshio)

file(GLOB INC_FILES ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_library(imshader STATIC ${SRC_FILES} ${INC_FILES})
target_link_libraries(imshader webgpu::webgpu)
set_target_properties(imshader PROPERTIES
    CXX_STANDARD 20
    COMPILE_WARNING_AS_ERROR ON
)
target_include_directories(imshader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_definitions(imshader PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_LIST_DIR}/shaders/")
add_library(implicit_shader::imshader ALIAS imshader)
add_library(implicit_shader::implicit_shader ALIAS imshader)

option(IMPLICIT_SHADER_BUILD_EXAMPLE "Build example" OFF)
if (IMPLICIT_SHADER_BUILD_EXAMPLE)
    add_executable(implicit_shader ${CMAKE_CURRENT_LIST_DIR}/app/main.cpp)
    target_link_libraries(implicit_shader webgpu::webgpu implicit_shader::imshader mshio::mshio)
    set_target_properties(implicit_shader PROPERTIES
        CXX_STANDARD 20
        COMPILE_WARNING_AS_ERROR ON
    )
    target_copy_webgpu_binaries(implicit_shader)
endif()

option(IMPLICIT_SHADER_BUILD_TESTS "Build tests" OFF)
if (IMPLICIT_SHADER_BUILD_TESTS)
    include(CTest)
    enable_testing()
    include(catch2)
    include(implicit_functions)

    file(GLOB TEST_FILES "${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp")
    add_executable(implicit_shader_tests ${TEST_FILES})
    target_link_libraries(implicit_shader_tests PRIVATE
        implicit_shader::imshader
        implicit_functions::implicit_functions
        Catch2::Catch2WithMain)
    set_target_properties(implicit_shader_tests PROPERTIES
        CXX_STANDARD 20
        COMPILE_WARNING_AS_ERROR ON
    )
endif()
