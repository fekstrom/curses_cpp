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

#include <stdexcept>

#define CHECK_GET [&] { auto* ret = Get(); assert(ret); return ret; }

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

Window::Window(SizeLinesCols lines_cols, PosYx top_left) :
    window_{newwin(lines_cols.lines, lines_cols.cols, top_left.y, top_left.x)}
{
    if (!window_)
    {
        throw std::runtime_error{"newwin failed"};
    }
}

Window::Window(const Window& other) :
    window_{dupwin(static_cast<WINDOW*>(other.window_))},
    parent_{other.parent_}
{
    if ((window_ == nullptr) != (other.window_ == nullptr))
    {
        throw std::runtime_error{"dupwin failed"};
    }
}

Window& Window::operator=(const Window& other)
{
    auto tmp = other;
    swap(*this, tmp);
    return *this;
}

Window::~Window()
{
    if (window_ == nullptr) return;
    assert(window_ != curscr);
    assert(window_ != newscr);
    assert(window_ != stdscr);
    const auto res = delwin(window_);
    // delwin returns ERR if this is the parent of another window, see
    // https://invisible-island.net/ncurses/man/curs_window.3x.html
    assert((res != ERR) && "delwin error (possibly because a subwindow is still alive)");
}

Window Window::Subwin(SizeLinesCols lines_cols, PosYx top_left_on_screen)
{
    return SubwinImpl(lines_cols, top_left_on_screen, "subwin");
}

Window Window::Derwin(SizeLinesCols lines_cols, PosYx top_left_in_parent)
{
    return SubwinImpl(lines_cols, top_left_in_parent, "derwin");
}

Result Window::Mvwin(PosYx top_left)
{
    return static_cast<Result>(mvwin(CHECK_GET(), top_left.y, top_left.x));
}

Result Window::Mvderwin(PosYx viewed_top_left)
{
    return static_cast<Result>(mvderwin(CHECK_GET(), viewed_top_left.y, viewed_top_left.x));
}

Result Window::Syncok(bool enable)
{
    return static_cast<Result>(syncok(CHECK_GET(), enable));
}

void Window::Syncup()
{
    wsyncup(CHECK_GET());
}

void Window::Cursyncup()
{
    wcursyncup(CHECK_GET());
}

void Window::Syncdown()
{
    wsyncdown(CHECK_GET());
}

Window Window::SubwinImpl(
        SizeLinesCols lines_cols,
        PosYx top_left,
        const std::string& method)
{
    const auto [l, c] = lines_cols;
    const auto [y, x] = top_left;
    auto* window = static_cast<WINDOW*>(nullptr);
    if (method == "subwin") window = subwin(CHECK_GET(), l, c, y, x);
    if (method == "derwin") window = derwin(CHECK_GET(), l, c, y, x);
    if (method == "subpad") window = subpad(CHECK_GET(), l, c, y, x);
    if (!window) throw std::runtime_error{method + " failed"};
    assert(wgetparent(window) == window_);
    auto ret = Window{};
    ret.window_ = window;
    ret.parent_ = this;
    return ret;
}

} // namespace curses
