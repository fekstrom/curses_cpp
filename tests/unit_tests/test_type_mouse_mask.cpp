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

static_assert(std::is_same_v<std::underlying_type_t<Mmask>, mmask_t>);

static_assert(static_cast<mmask_t>(Mmask::Button1Pressed)       == BUTTON1_PRESSED);
static_assert(static_cast<mmask_t>(Mmask::Button1Released)      == BUTTON1_RELEASED);
static_assert(static_cast<mmask_t>(Mmask::Button1Clicked)       == BUTTON1_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button1DoubleClicked) == BUTTON1_DOUBLE_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button1TripleClicked) == BUTTON1_TRIPLE_CLICKED);

static_assert(static_cast<mmask_t>(Mmask::Button2Pressed)       == BUTTON2_PRESSED);
static_assert(static_cast<mmask_t>(Mmask::Button2Released)      == BUTTON2_RELEASED);
static_assert(static_cast<mmask_t>(Mmask::Button2Clicked)       == BUTTON2_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button2DoubleClicked) == BUTTON2_DOUBLE_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button2TripleClicked) == BUTTON2_TRIPLE_CLICKED);

static_assert(static_cast<mmask_t>(Mmask::Button3Pressed)       == BUTTON3_PRESSED);
static_assert(static_cast<mmask_t>(Mmask::Button3Released)      == BUTTON3_RELEASED);
static_assert(static_cast<mmask_t>(Mmask::Button3Clicked)       == BUTTON3_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button3DoubleClicked) == BUTTON3_DOUBLE_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button3TripleClicked) == BUTTON3_TRIPLE_CLICKED);

static_assert(static_cast<mmask_t>(Mmask::Button4Pressed)       == BUTTON4_PRESSED);
static_assert(static_cast<mmask_t>(Mmask::Button4Released)      == BUTTON4_RELEASED);
static_assert(static_cast<mmask_t>(Mmask::Button4Clicked)       == BUTTON4_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button4DoubleClicked) == BUTTON4_DOUBLE_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button4TripleClicked) == BUTTON4_TRIPLE_CLICKED);

static_assert(static_cast<mmask_t>(Mmask::Button5Pressed)       == BUTTON5_PRESSED);
static_assert(static_cast<mmask_t>(Mmask::Button5Released)      == BUTTON5_RELEASED);
static_assert(static_cast<mmask_t>(Mmask::Button5Clicked)       == BUTTON5_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button5DoubleClicked) == BUTTON5_DOUBLE_CLICKED);
static_assert(static_cast<mmask_t>(Mmask::Button5TripleClicked) == BUTTON5_TRIPLE_CLICKED);

static_assert(static_cast<mmask_t>(Mmask::ButtonShift)          == BUTTON_SHIFT);
static_assert(static_cast<mmask_t>(Mmask::ButtonCtrl)           == BUTTON_CTRL);
static_assert(static_cast<mmask_t>(Mmask::ButtonAlt)            == BUTTON_ALT);
static_assert(static_cast<mmask_t>(Mmask::ReportMousePosition)  == REPORT_MOUSE_POSITION);
static_assert(static_cast<mmask_t>(Mmask::AllMouseEvents)       == ALL_MOUSE_EVENTS);
