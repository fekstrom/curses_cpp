option(CURSES_CPP_FORCE_ENABLE_ASSERTS OFF "Enable asserts, overriding CMake's default")

if(NOT CURSES_CPP_FORCE_ENABLE_ASSERTS)
  return()
endif()

macro(remove_dndebug flags)
  string(REPLACE "-DNDEBUG" "" ${flags} "${${flags}}")
  string(REPLACE "/DNDEBUG" "" ${flags} "${${flags}}")
endmacro()

remove_dndebug(CMAKE_CXX_FLAGS)
remove_dndebug(CMAKE_CXX_FLAGS_DEBUG)
remove_dndebug(CMAKE_CXX_FLAGS_RELEASE)
remove_dndebug(CMAKE_CXX_FLAGS_RELWITHDEBINFO)
remove_dndebug(CMAKE_CXX_FLAGS_MINSIZEREL)
