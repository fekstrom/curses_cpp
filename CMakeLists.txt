cmake_minimum_required(VERSION 3.20)

project(CursesCpp VERSION 0.0.1)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  enable_testing()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CursesCppCompilerWarnings)
include(CursesCppForceEnableAsserts)
include(CursesCppOutputDirectories)

option(CURSES_CPP_BUILD_DOCUMENTATION "Build documentation" OFF)
option(CURSES_CPP_BUILD_UNIT_TESTS "Build unit tests" OFF)

if(CURSES_CPP_BUILD_DOCUMENTATION)
  add_subdirectory(docs)
endif()

set(CURSES_NEED_NCURSES ON)
find_package(Curses REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
