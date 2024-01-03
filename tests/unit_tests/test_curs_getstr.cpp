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

#include <algorithm>
#include <string>

using namespace curses;

TEST_CASE("curs_getstr")
{
    const auto _ = Initscr();
    Cbreak();
    auto window = Window({}, {});

    const auto user_input = std::string{"0123456"};
    const auto user = [&] ()
    {
        Ungetch('\n');
        std::for_each(
                user_input.rbegin(),
                user_input.rend(),
                [] (char c) { Ungetch(c); });
    };

    user();
    REQUIRE(window.Getstr() == "0123456");

    user();
    REQUIRE(window.Getstr({2, 3}) == "0123456");

    user();
    REQUIRE(window.Getstr(4) == "0123");

    user();
    REQUIRE(window.Getstr({2, 3}, 4) == "0123");
}
