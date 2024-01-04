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

TEST_CASE("curs_deleteln")
{
    const auto _ = Initscr();
    auto window = Window({}, {});
    auto [h, w] = window.Getmaxyx();
    REQUIRE(h >= 3);
    REQUIRE(w >= 3);

    window.Addstr({0, 0}, "ABC");
    window.Addstr({1, 0}, "DEF");

    window.Move({1, 0});
    window.Insertln();
    REQUIRE(window.Instr({0, 0}, 3) == "ABC");
    REQUIRE(window.Instr({1, 0}, 3) == "   ");
    REQUIRE(window.Instr({2, 0}, 3) == "DEF");

    window.Move({1, 0});
    window.Deleteln();
    REQUIRE(window.Instr({0, 0}, 3) == "ABC");
    REQUIRE(window.Instr({1, 0}, 3) == "DEF");
    REQUIRE(window.Instr({2, 0}, 3) == "   ");

    window.Move({1, 0});
    window.Insdelln(1);
    REQUIRE(window.Instr({0, 0}, 3) == "ABC");
    REQUIRE(window.Instr({1, 0}, 3) == "   ");
    REQUIRE(window.Instr({2, 0}, 3) == "DEF");

    window.Move({1, 0});
    window.Insdelln(-1);
    REQUIRE(window.Instr({0, 0}, 3) == "ABC");
    REQUIRE(window.Instr({1, 0}, 3) == "DEF");
    REQUIRE(window.Instr({2, 0}, 3) == "   ");
}
