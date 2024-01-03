find_package(Catch2 3 REQUIRED)

add_executable(unit_tests "")
target_sources(unit_tests PRIVATE
  dummy.cpp
  test_type_color.cpp
  test_type_result.cpp
)
target_link_libraries(unit_tests PRIVATE
  CursesCpp::CompilerWarnings
  CursesCpp::Curses
  CursesCpp::CursesCpp
  Catch2::Catch2WithMain
)

add_test(NAME "Unit tests" COMMAND unit_tests)
