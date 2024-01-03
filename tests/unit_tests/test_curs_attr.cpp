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

TEST_CASE("Attron, Attroff, Attrset, Colorset, Attrget")
{
    const auto _ = Initscr();
    auto window = Window({}, {});

//  TODO: Need StartColor
//    REQUIRE(StartColor() == Result::Ok);

    REQUIRE(window.Attrget() == Attr::Normal);
    REQUIRE(window.Attron(Attr::Blink) == Result::Ok);
    REQUIRE(window.Attrget() == Attr::Blink);
    REQUIRE(window.Attron(Attr::Bold | Attr::Reverse) == Result::Ok);
    REQUIRE(window.Attrget() == (Attr::Blink | Attr::Bold | Attr::Reverse));
    REQUIRE(window.Attroff(Attr::Bold | Attr::Dim) == Result::Ok);
    REQUIRE(window.Attrget() == (Attr::Blink | Attr::Reverse));
    REQUIRE(window.Attron(ColorPair(3)) == Result::Ok);
    REQUIRE(window.Attrget() == (Attr::Blink | Attr::Reverse | ColorPair(3)));
    REQUIRE(window.Attroff(ColorPair(1)) == Result::Ok);
    REQUIRE(window.Attrget() == (Attr::Blink | Attr::Reverse));

    REQUIRE(window.Attrset(Attr::Reverse | ColorPair(5)) == Result::Ok);
    REQUIRE(window.Attrget() == (Attr::Reverse | ColorPair(5)));

//  TODO: Need StartColor
//    REQUIRE(window.Colorset(2) == Result::Ok);
//    REQUIRE(window.Attrget() == (Attr::Reverse | ColorPair(2)));
}

TEST_CASE("Chgat")
{
    const auto _ = Initscr();
    auto window = Window({}, {});

    // TODO: Need Addstr
//    window.Addstr({0, 0}, "AAAAAA");
//    window.Addstr({1, 0}, "BBBBBB");
//    REQUIRE(window.Chgat({0, 0}, Attr::Reverse) == Result::Ok);
//    REQUIRE(window.Chgat(Attr::Bold) == Result::Ok);
//    REQUIRE(window.Chgat({1, 0}, 3, Attr::Dim) == Result::Ok);
//    REQUIRE(window.Chgat(2, Attr::Underline) == Result::Ok);
}
