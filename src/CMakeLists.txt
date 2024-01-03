# Target CursesCpp::Curses
add_library(CursesCpp_Curses INTERFACE)
add_library(CursesCpp::Curses ALIAS CursesCpp_Curses)
set_target_properties(CursesCpp_Curses PROPERTIES EXPORT_NAME Curses)
target_include_directories(CursesCpp_Curses INTERFACE ${CURSES_INCLUDE_DIR})
target_link_libraries(CursesCpp_Curses INTERFACE ${CURSES_LIBRARIES})

# Target CursesCpp::CursesCpp
add_library(CursesCpp_CursesCpp "")
add_library(CursesCpp::CursesCpp ALIAS CursesCpp_CursesCpp)
set_target_properties(CursesCpp_CursesCpp PROPERTIES EXPORT_NAME CursesCpp)
target_sources(CursesCpp_CursesCpp PRIVATE
  curses_cpp/curses.cpp
  curses_cpp/curses.hpp
  curses_cpp/version.hpp
)
target_include_directories(CursesCpp_CursesCpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(CursesCpp_CursesCpp
PRIVATE
  $<BUILD_INTERFACE:CursesCpp::CompilerWarnings>
  CursesCpp::Curses
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/curses_cpp/CursesCppConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CursesCppConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CursesCpp
)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CursesCppConfigVersion.cmake
  VERSION ${CursesCpp_VERSION}
  COMPATIBILITY ExactVersion # No promises about compatibility while major version is 0.
)

install(TARGETS CursesCpp_CursesCpp CursesCpp_Curses
  EXPORT CursesCppExport
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT CursesCppExport
  NAMESPACE CursesCpp::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CursesCpp
)
install(DIRECTORY curses_cpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN build_internal* EXCLUDE
  PATTERN *.hpp
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CursesCppConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CursesCppConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CursesCpp
)
