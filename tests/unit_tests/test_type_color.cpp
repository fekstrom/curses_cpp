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

#include <type_traits>

using namespace curses;

static_assert(std::is_same_v<std::underlying_type_t<Color>, decltype(COLOR_BLACK)>);

static_assert(static_cast<int>(Color::Black)   == COLOR_BLACK);
static_assert(static_cast<int>(Color::Red)     == COLOR_RED);
static_assert(static_cast<int>(Color::Green)   == COLOR_GREEN);
static_assert(static_cast<int>(Color::Yellow)  == COLOR_YELLOW);
static_assert(static_cast<int>(Color::Blue)    == COLOR_BLUE);
static_assert(static_cast<int>(Color::Magenta) == COLOR_MAGENTA);
static_assert(static_cast<int>(Color::Cyan)    == COLOR_CYAN);
static_assert(static_cast<int>(Color::White)   == COLOR_WHITE);
