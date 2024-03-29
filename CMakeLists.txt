cmake_minimum_required(VERSION 3.21)

project(CursesCpp VERSION 0.0.1)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type specified. Defaulting to CMAKE_BUILD_TYPE=Release.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

include(CursesCppCompilerWarnings)
include(CursesCppForceEnableAsserts)
include(CursesCppOutputDirectories)

option(CURSES_CPP_BUILD_DOCUMENTATION "Build documentation" OFF)
option(CURSES_CPP_BUILD_EXAMPLES "Build examples as part of main build" ON)
option(CURSES_CPP_BUILD_UNIT_TESTS "Build unit tests" OFF)

if(CURSES_CPP_BUILD_DOCUMENTATION)
  add_subdirectory(docs)
endif()

if(CURSES_CPP_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

set(CURSES_NEED_NCURSES ON)
find_package(Curses REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
