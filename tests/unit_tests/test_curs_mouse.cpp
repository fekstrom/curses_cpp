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

TEST_CASE("curs_mouse: Free functions")
{
    const auto _ = Initscr();
    Raw();
    Mousemask(Mmask::AllMouseEvents);
    REQUIRE(HasMouse());

    Mouseinterval(150);
    REQUIRE(Mouseinterval(-1) == 150);
    REQUIRE(Mouseinterval(0) == 150);
    REQUIRE(Mouseinterval(150) == 0);

    auto event = Mevent{};
    event.id = 0;
    event.x = 10;
    event.y = 20;
    event.z = 0;
    event.bstate = Mmask::Button1Clicked;

    REQUIRE(Result::Ok == Ungetmouse(event));

    const auto received_event = Getmouse();
    REQUIRE(received_event);
    REQUIRE(received_event->id == event.id);
    REQUIRE(received_event->x == event.x);
    REQUIRE(received_event->y == event.y);
    REQUIRE(received_event->z == event.z);
    REQUIRE(received_event->bstate == event.bstate);

    REQUIRE(Getmouse() == std::nullopt);
}

TEST_CASE("curs_mouse: Window members")
{
    const auto _ = Initscr();
    REQUIRE(Lines() >= 3);
    REQUIRE(Cols() >= 3);

    Raw();
    Mousemask(Mmask::AllMouseEvents);
    REQUIRE(HasMouse());

    auto window = Window{{}, {1, 1}};

    auto event = Mevent{};
    event.id = 0;
    event.x = 2;
    event.y = 2;
    event.z = 0;
    event.bstate = Mmask::Button1Pressed;

    REQUIRE(Result::Ok == Ungetmouse(event));

    REQUIRE(window.Getch() == Key::Mouse);

    const auto received_event = Getmouse();
    REQUIRE(received_event);

    const auto pos_on_screen = PosYx{received_event->y, received_event->x};
    REQUIRE(window.Enclose(pos_on_screen));
    REQUIRE(window.TransformToWindow(pos_on_screen) == PosYx{1, 1});
    REQUIRE(window.TransformToScreen(window.TransformToWindow(pos_on_screen)) == pos_on_screen);
}
