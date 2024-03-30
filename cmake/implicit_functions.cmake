if (TARGET implicit_functions::implicit_functions)
    return()
endif()

message(STATUS "Third-party (external): creating target 'implicit_functions::implicit_functions'")

include(CPM)
CPMAddPackage(
  NAME implicit_functions
  GITHUB_REPOSITORY qnzhou/implicit_functions
  GIT_TAG main
)

set_target_properties(implicit_functions PROPERTIES FOLDER third_party)
