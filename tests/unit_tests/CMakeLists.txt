find_package(Catch2 3 REQUIRED)

add_executable(unit_tests "")
target_sources(unit_tests PRIVATE
  event_listeners.cpp
  test_curs_addch.cpp
  test_curs_addchstr.cpp
  test_curs_addstr.cpp
  test_curs_attr.cpp
  test_curs_bkgd.cpp
  test_curs_color.cpp
  test_curs_deleteln.cpp
  test_curs_getch.cpp
  test_curs_getyx.cpp
  test_curs_inch.cpp
  test_curs_inchstr.cpp
  test_curs_initscr.cpp
  test_curs_inopts.cpp
  test_curs_insch.cpp
  test_curs_insstr.cpp
  test_curs_instr.cpp
  test_curs_mouse.cpp
  test_curs_move.cpp
  test_curs_opaque.cpp
  test_curs_outopts.cpp
  test_curs_overlay.cpp
  test_curs_scroll.cpp
  test_curs_touch.cpp
  test_curs_window.cpp
  test_type_attr.cpp
  test_type_chtype.cpp
  test_type_color.cpp
  test_type_key.cpp
  test_type_mouse_mask.cpp
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
