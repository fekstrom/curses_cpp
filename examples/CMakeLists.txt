cmake_minimum_required(VERSION 3.21)

project(CursesCppExamples VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(PROJECT_IS_TOP_LEVEL)
  include (GNUInstallDirs)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CursesCppExamples_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
  endif()
  find_package(CursesCpp)
endif()

add_subdirectory(src)
