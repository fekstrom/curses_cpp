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

static_assert(std::is_same_v<std::underlying_type_t<Attr>, attr_t>);

static constexpr bool AreEqual(Attr a, attr_t b)
{
    const auto c = static_cast<unsigned>(a);
    return c == b;
}

static_assert(AreEqual(Attr::Normal    , A_NORMAL    ));
static_assert(AreEqual(Attr::Standout  , A_STANDOUT  ));
static_assert(AreEqual(Attr::Underline , A_UNDERLINE ));
static_assert(AreEqual(Attr::Reverse   , A_REVERSE   ));
static_assert(AreEqual(Attr::Blink     , A_BLINK     ));
static_assert(AreEqual(Attr::Dim       , A_DIM       ));
static_assert(AreEqual(Attr::Bold      , A_BOLD      ));
static_assert(AreEqual(Attr::Altcharset, A_ALTCHARSET));
static_assert(AreEqual(Attr::Invis     , A_INVIS     ));
static_assert(AreEqual(Attr::Protect   , A_PROTECT   ));
static_assert(AreEqual(Attr::Horizontal, A_HORIZONTAL));
static_assert(AreEqual(Attr::Left      , A_LEFT      ));
static_assert(AreEqual(Attr::Low       , A_LOW       ));
static_assert(AreEqual(Attr::Right     , A_RIGHT     ));
static_assert(AreEqual(Attr::Top       , A_TOP       ));
static_assert(AreEqual(Attr::Vertical  , A_VERTICAL  ));

static_assert(AreEqual(Attr::Normal    , WA_NORMAL    ));
static_assert(AreEqual(Attr::Standout  , WA_STANDOUT  ));
static_assert(AreEqual(Attr::Underline , WA_UNDERLINE ));
static_assert(AreEqual(Attr::Reverse   , WA_REVERSE   ));
static_assert(AreEqual(Attr::Blink     , WA_BLINK     ));
static_assert(AreEqual(Attr::Dim       , WA_DIM       ));
static_assert(AreEqual(Attr::Bold      , WA_BOLD      ));
static_assert(AreEqual(Attr::Altcharset, WA_ALTCHARSET));
static_assert(AreEqual(Attr::Invis     , WA_INVIS     ));
static_assert(AreEqual(Attr::Protect   , WA_PROTECT   ));
static_assert(AreEqual(Attr::Horizontal, WA_HORIZONTAL));
static_assert(AreEqual(Attr::Left      , WA_LEFT      ));
static_assert(AreEqual(Attr::Low       , WA_LOW       ));
static_assert(AreEqual(Attr::Right     , WA_RIGHT     ));
static_assert(AreEqual(Attr::Top       , WA_TOP       ));
static_assert(AreEqual(Attr::Vertical  , WA_VERTICAL  ));

static constexpr auto attr = Attr::Bold | Attr::Reverse | ColorPair(5);
static_assert(RemoveColor(attr) == (Attr::Bold | Attr::Reverse));
static_assert(PairNumber(attr) == 5);
