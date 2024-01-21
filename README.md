# CursesCpp

CursesCpp is a C++ interface to the `ncurses` library. It uses C++
idioms such as RAII, function overloading, and strong types, but
otherwise aims to be transparent.

There's currently no documentation, but it should be relatively easy
to translate from the [ncurses man pages](https://invisible-island.net/ncurses/man/ncurses.3x.html)
to CursesCpp. Some reading of the CursesCpp [code](src/) will likely
be needed. An IDE with good code navigation and code completion should help.

You may want to consider using the C++ interface that is included in
[ncurses](https://github.com/mirror/ncurses/tree/master/c%2B%2B).

## Examples

Here's a small program that uses CursesCpp.

```
#include "curses_cpp/curses.hpp"

int main()
{
    const auto auto_endwin = curses::Initscr();
    auto window = curses::Window{{}, {}};
    window.Box();
    const auto [h, w] = window.Getmaxyx();
    window.Addstr({h / 2, (w - 21) / 2}, "Press any key to exit");
    window.Getch();
    return 0;
}
```

More examples can be found in the [examples](examples) directory.

## Dependencies

CursesCpp depends on ncurses and requires C++ 17. The unit tests
(optional) use Catch2.

## CMake

CursesCpp can be consumed by a CMake project using find_package or
add_subdirectory.

### find_package

Build and install:

```
$ cd curses_cpp
$ cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
$ cmake --install build --prefix <path-to-install-dir>
```

Configure consuming project:

```
$ cmake ... -D CMAKE_PREFIX_PATH=<path-to-install-dir>
```

CMakeLists.txt of consuming project:

```
find_package(CursesCpp REQUIRED)
target_link_libraries(<target> ... CursesCpp::CursesCpp)
```

### add_subdirectory

CMakeLists.txt of consuming project:

```
add_subdirectory(curses_cpp)
target_link_libraries(<target> ... CursesCpp::CursesCpp)
```

## Missing pieces

The following parts of ncurses are not exposed in CursesCpp. (The names refer
to the [ncurses man pages](https://invisible-island.net/ncurses/man/ncurses.3x.html).)

### Non-wide characters

- curs_legacy
- curs_memleaks
- curs_pad
- curs_print
- curs_printw
- curs_scanw
- curs_scr_dump
- curs_slk
- curs_termcap
- curs_terminfo
- curs_trace

### Wide characters

- curs_add_wch
- curs_add_wchstr
- curs_addwstr
- curs_bkgrnd
- curs_border_set
- curs_getcchar
- curs_get_wch
- curs_get_wstr
- curs_ins_wch
- curs_ins_wstr
- curs_in_wch
- curs_in_wchstr
- curs_inwstr

### Extensions

- default_colors
- define_key
- keybound
- key_defined
- keyok
- legacy_coding
- new_pair
- resizeterm
- wresize
