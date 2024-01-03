find_package(Doxygen REQUIRED)

set(DOXYGEN_PROJECT_NAME "CursesCpp")
set(DOXYGEN_QUIET Yes)
set(DOXYGEN_EXTRACT_ALL Yes)

doxygen_add_docs(CursesCpp_Documentation
  main.dox
  ${CursesCpp_SOURCE_DIR}/src/curses_cpp
  ALL
)

include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR})
