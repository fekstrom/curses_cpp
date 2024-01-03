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

#include <catch2/catch_test_macros.hpp>

using namespace curses;

TEST_CASE("curs_window: Window::Subwin")
{
    const auto _ = Initscr();

    auto window = Window{{}, {1, 1}};
    auto subwindow = window.Subwin({5, 5}, {1, 1});
    REQUIRE(subwindow.GetParent() == &window);
    subwindow.Mvderwin({2, 2});
}

TEST_CASE("curs_window: Window::Derwin")
{
    const auto _ = Initscr();

    auto window = Window{{}, {1, 1}};
    auto derwindow = window.Derwin({5, 5}, {1, 1});
    REQUIRE(derwindow.GetParent() == &window);
    derwindow.Mvderwin({2, 2});
}
