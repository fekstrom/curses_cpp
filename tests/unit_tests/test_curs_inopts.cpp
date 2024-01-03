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

TEST_CASE("curs_inopts: Free functions")
{
    const auto _ = Initscr();
    REQUIRE(Result::Ok == Cbreak());
    REQUIRE(Result::Ok == Nocbreak());
    REQUIRE(Result::Ok == Echo());
    REQUIRE(Result::Ok == Noecho());
    REQUIRE(Result::Ok == Nl());
    REQUIRE(Result::Ok == Nonl());
    REQUIRE(Result::Ok == Raw());
    REQUIRE(Result::Ok == Noraw());
    Qiflush();
    Noqiflush();
    REQUIRE(Result::Ok == Halfdelay(1));
    REQUIRE(Result::Err == Halfdelay(-1));
    REQUIRE(Result::Ok == Typeahead(-1));
    REQUIRE(Result::Ok == Meta());
}

TEST_CASE("curs_inopts: Window methods")
{
    const auto _ = Initscr();
    auto window = Window({}, {});
    REQUIRE(Result::Ok == window.Keypad(true));
    REQUIRE(Result::Ok == window.Keypad(false));
    REQUIRE(Result::Ok == window.Nodelay(true));
    REQUIRE(Result::Ok == window.Nodelay(false));
    REQUIRE(Result::Ok == window.Notimeout(true));
    REQUIRE(Result::Ok == window.Notimeout(false));
    window.Timeout( 1);
    window.Timeout( 0);
    window.Timeout(-1);
}
