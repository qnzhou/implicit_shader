if (TARGET dawn::dawn)
    return()
endif()

message(STATUS "Third-party (external): creating target dawn::dawn")

include(CPM)
CPMAddPackage(
    NAME dawn
    GIT_REPOSITORY https://dawn.googlesource.com/dawn
    GIT_TAG 06acb665e31338b0050a0385f398896dcfc5670d
)

add_library(dawn::dawn ALIAS dawn)
