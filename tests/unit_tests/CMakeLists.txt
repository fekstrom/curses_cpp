find_package(Catch2 3 REQUIRED)

add_executable(unit_tests "")
target_sources(unit_tests PRIVATE
  test_curs_addch.cpp
  test_curs_addchstr.cpp
  test_curs_addstr.cpp
  test_curs_attr.cpp
  test_curs_color.cpp
  test_curs_getch.cpp
  test_curs_getyx.cpp
  test_curs_initscr.cpp
  test_curs_inopts.cpp
  test_curs_move.cpp
  test_curs_opaque.cpp
  test_curs_outopts.cpp
  test_curs_window.cpp
  test_type_attr.cpp
  test_type_chtype.cpp
  test_type_color.cpp
  test_type_key.cpp
  test_type_result.cpp
  test_type_window.cpp
)
target_link_libraries(unit_tests PRIVATE
  CursesCpp::CompilerWarnings
  CursesCpp::Curses
  CursesCpp::CursesCpp
  Catch2::Catch2WithMain
)

add_test(NAME "Unit tests" COMMAND unit_tests)
