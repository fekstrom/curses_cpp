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

TEST_CASE("curs_color: InitPair, PairContent")
{
    const auto _ = Initscr();
    if (!HasColors()) return;

    const auto pair_number = 1;
    const auto pair = ColorPairFgBg{Color::Red, Color::Green};
    REQUIRE(Result::Ok == StartColor());
    REQUIRE(Result::Ok == InitPair(pair_number, pair));
    REQUIRE(PairContent(pair_number) == pair);
}

TEST_CASE("curs_color: InitColor, ColorContent")
{
    const auto _ = Initscr();
    if (!(HasColors() && CanChangeColor())) return;

    const auto color = static_cast<Color>(8);
    const auto rgb = ColorRgb{0, 499, 1000};
    REQUIRE(Result::Ok == StartColor());
    REQUIRE(Result::Ok == InitColor(color, rgb));
    REQUIRE(ColorContent(color) == rgb);
}
