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

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace curses;

Window CenteredSubwindow(Window& window, SizeLinesCols size)
{
    const auto [win_h, win_w] = window.Getmaxyx();
    const auto [sub_h, sub_w] = size;
    const auto top_left = PosYx{(win_h - sub_h) / 2, (win_w - sub_w) / 2};
    return window.Derwin(size, top_left);
}

std::string ToStringIfNonneg(int n)
{
    return n >= 0 ? std::to_string(n) : "";
}

struct TrackMousePosition // NOLINT: Missing copy and move operations
{
    // https://gist.github.com/sylt/93d3f7b77e7f3a881603
    TrackMousePosition()  { printf("\033[?1003h\n"); } // Make the terminal report mouse movement
    ~TrackMousePosition() { printf("\033[?1003l\n"); } // Make it stop
};

class App
{
public:
    App() :
        auto_endwin_{Initscr()},
        main_window_{{}, {}},
        mouse_stats_{main_window_.Subwin({3, 20}, {0, 0})},
        quit_button_{main_window_.Subwin({3, 8}, {main_window_.Getmaxyx().y - 3, 0})},
        pick_window_{CenteredSubwindow(main_window_, {5, 15})}
    {
        Mousemask(Mmask::Button1Pressed | Mmask::ReportMousePosition);
        if (!HasMouse()) throw std::runtime_error("Mouse not available");
        Mouseinterval(0); // Get rid of delay by disabling click detection
        Raw();
        CursSet(CursorVisibility::Invisible);
        main_window_.Keypad();
    }

    void Run()
    {
        const auto _ = TrackMousePosition{};
        pick_window_pos_ = pick_window_.Getbegyx();
        UpdateUi();
        while (ProcessOneEvent())
        {
            UpdateUi();
        }
    }

private:
    void UpdateUi()
    {
        if (pick_window_.Getbegyx() != pick_window_pos_)
        {
            pick_window_.Erase();
            pick_window_.Noutrefresh();
            pick_window_.Mvwin(pick_window_pos_);
        }

        mouse_stats_.Erase();
        mouse_stats_.Addstr({0, 0}, "Mouse events: " + std::to_string(num_mouse_events_));
        mouse_stats_.Addstr({1, 0}, "Mouse y     : " + ToStringIfNonneg(mouse_pos_.y));
        mouse_stats_.Addstr({2, 0}, "Mouse x     : " + ToStringIfNonneg(mouse_pos_.x));
        mouse_stats_.Noutrefresh();

        quit_button_.Erase();
        quit_button_.Box();
        quit_button_.Addstr({1, 1}, " Quit ");
        quit_button_.Noutrefresh();

        pick_window_.Addstr({2, 1}, picked_up_ ? " Put me down " : " Pick me up! ");
        pick_window_.Box();
        pick_window_.Noutrefresh();

        Doupdate();
    }

    // Return false to quit
    bool ProcessOneEvent()
    {
        const auto key = main_window_.Getch();
        if (key == 'q' || key == 'Q') return false;
        // After a mouse event that wasn't enabled with Mousemask,
        // Getch returns Key::Mouse but Getmouse returns null. So
        // it's not enough to check for Key::Mouse here.
        const auto event = Getmouse();
        if (!event) return true;
        assert(event->bstate == Mmask::Button1Pressed ||
                event->bstate == Mmask::ReportMousePosition);
        ++num_mouse_events_;

        const auto prv_mouse_pos = std::exchange(mouse_pos_, event->Getyx());

        if (picked_up_)
        {
            const auto [ h,  w] = main_window_.Getmaxyx();
            const auto [hh, ww] = pick_window_.Getmaxyx();
            const auto [y0, x0] = pick_window_pos_;
            const auto dy = mouse_pos_.y - prv_mouse_pos.y;
            const auto dx = mouse_pos_.x - prv_mouse_pos.x;
            pick_window_pos_.y = std::clamp(y0 + dy, 0, h - hh);
            pick_window_pos_.x = std::clamp(x0 + dx, 0, w - ww);
            picked_up_ = event->bstate != Mmask::Button1Pressed;
            return true;
        }
        if (pick_window_.Enclose(mouse_pos_))
        {
            picked_up_ = event->bstate == Mmask::Button1Pressed;
            return true;
        }
        if (quit_button_.Enclose(mouse_pos_))
        {
            return event->bstate != Mmask::Button1Pressed;
        }
        return true;
    }

    AutoEndwin auto_endwin_;
    Window main_window_;
    Window mouse_stats_;
    Window quit_button_;
    Window pick_window_;

    int num_mouse_events_ = 0;
    PosYx mouse_pos_ = PosYx{-1, -1};
    bool picked_up_ = false;
    PosYx pick_window_pos_ = PosYx{-1, -1};
};

int main()
{
    try
    {
        auto app = App{};
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        return 1;
    }
    return 0;
}
