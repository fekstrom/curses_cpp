// MIT License
//
// Copyright (c) 2024 fekstrom
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "curses_cpp/curses.hpp"

#include <curses.h>

namespace curses
{

AutoEndwin::AutoEndwin(AutoEndwin&& other) noexcept
    : released_{other.released_}
{
    other.released_ = true;
}

AutoEndwin& AutoEndwin::operator=(AutoEndwin&& other) noexcept
{
    released_ = other.released_;
    other.released_ = true;
    return *this;
}

AutoEndwin::~AutoEndwin()
{
    if (released_) return;
    const auto res = endwin();
    assert(res != ERR);
}

AutoEndwin Initscr()
{
    auto* window = initscr();
    // If initscr fails the ncurses exits the program, see
    // https://invisible-island.net/ncurses/man/curs_initscr.3x.html
    assert(window);
    // When keypad is first enabled, ncurses loads the key
    // definitions needed for HasKey, see
    // https://invisible-island.net/ncurses/man/curs_inopts.3x.html
    keypad(window, true);
    keypad(window, false);
    return AutoEndwin{};
}

Result Endwin()
{
    return static_cast<Result>(endwin());
}

bool Isendwin()
{
    return isendwin();
}

int Lines()
{
    return LINES;
}

int Cols()
{
    return COLS;
}

} // namespace curses
