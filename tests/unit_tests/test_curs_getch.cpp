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

TEST_CASE("HasKey")
{
    Initscr().NoAutoEndwin();   // Endwin called at (*)
    auto window = Window({}, {});

    CHECK(HasKey(Key::Enter));
    CHECK(HasKey(Key::F1));
    window.Keypad();
    CHECK(HasKey(Key::Enter));
    CHECK(HasKey(Key::F1));
    window.Keypad(false);
    CHECK(HasKey(Key::Enter));
    CHECK(HasKey(Key::F1));
    Endwin();                   // (*)
    CHECK(HasKey(Key::Enter));
    CHECK(HasKey(Key::F1));
}

TEST_CASE("Ungetch, Getch")
{
    const auto _ = Initscr();
    auto window = Window({}, {});

    REQUIRE(Result::Ok == Ungetch('A'));
    REQUIRE(Result::Ok == Ungetch('B'));
    REQUIRE('B' == window.Getch());
    REQUIRE('A' == window.Getch({0, 0}));
    window.Keypad();
    REQUIRE(Result::Ok == Ungetch(Key::F1));
    REQUIRE(Key::F1 == window.Getch());
}
