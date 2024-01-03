add_library(CursesCpp_CompilerWarnings INTERFACE)
add_library(CursesCpp::CompilerWarnings ALIAS CursesCpp_CompilerWarnings)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(CursesCpp_CompilerWarnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wno-sign-conversion
  )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(CursesCpp_CompilerWarnings INTERFACE
    /W4
  )
endif()
