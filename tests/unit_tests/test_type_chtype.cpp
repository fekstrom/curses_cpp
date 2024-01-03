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
#include <utility>

using namespace curses;

static_assert(std::is_same_v<decltype(std::declval<Chtype>().Get()), chtype>);

static_assert(Chtype{'a'}.GetChar() == 'a');
static_assert(Chtype{'K'}.GetChar() == 'K');
static_assert(Chtype{'3'}.GetChar() == '3');

static_assert(Chtype{ 59U}.Get() ==  59U);
static_assert(Chtype{  1U}.Get() ==   1U);
static_assert(Chtype{999U}.Get() == 999U);

static constexpr auto ch0 = Chtype{'A'};
static constexpr auto ch1 = ch0 | Attr::Blink;
static constexpr auto ch2 = ch1 | ColorPair(1);

static_assert(ch0.GetChar() == 'A');
static_assert(ch0.GetAttr() == Attr::Normal);
static_assert(ch0.GetAttrRemoveColor() == Attr::Normal);
static_assert(ch0.GetColorPair() == Attr::Normal);
static_assert(ch0.GetPairNumber() == 0);

static_assert(ch1.GetChar() == 'A');
static_assert(ch1.GetAttr() == Attr::Blink);
static_assert(ch1.GetAttrRemoveColor() == Attr::Blink);
static_assert(ch1.GetColorPair() == Attr::Normal);
static_assert(ch1.GetPairNumber() == 0);

static_assert(ch2.GetChar() == 'A');
static_assert(ch2.GetAttr() == (Attr::Blink | ColorPair(1)));
static_assert(ch2.GetAttrRemoveColor() == Attr::Blink);
static_assert(ch2.GetColorPair() == ColorPair(1));
static_assert(ch2.GetPairNumber() == 1);

static constexpr auto ch3 = 'S' | Attr::Reverse | ColorPair(4);

static_assert(std::is_same_v<std::remove_cv_t<decltype(ch3)>, Chtype>);
static_assert(ch3.GetChar() == 'S');
static_assert(ch3.GetAttr() == (Attr::Reverse | ColorPair(4)));
static_assert(ch3.GetAttrRemoveColor() == Attr::Reverse);
static_assert(ch3.GetColorPair() == ColorPair(4));
static_assert(ch3.GetPairNumber() == 4);
