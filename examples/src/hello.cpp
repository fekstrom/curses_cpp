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
#include <string>

int main()
{
    const auto auto_endwin = curses::Initscr();
    curses::Noecho();
    curses::StartColor();

    const auto green = 1;
    curses::InitPair(green, {curses::Color::Green, curses::Color::Black});

    auto window = curses::Window{{0, std::min(79, curses::Cols())}};
    window.Box();

    const auto [window_h, window_w] = window.Getmaxyx();

    auto hello = window.Subwin({2, 5}, {window_h / 7, (window_w - 5) / 2});
    hello.Keypad(); // Make Getch recognize arrow keys
    hello.Attron(curses::Attr::Bold);
    hello.Addstr({0, 0}, "Hello");
    hello.Addstr({1, 0}, "World");
    hello.Attrset(curses::Attr::Normal);

    auto instr = window.Subwin({3, 0}, {window_h - 6, 2});
    instr.Addstr({0, 0}, "C : Toggle color");
    instr.Addstr({1, 0}, "U : Toggle underline");
    instr.Addstr({2, 0}, "Q : Quit");

    auto debug = window.Subwin({3, 0}, {window_h - 3, 0});
    debug.Box();

    hello.Move({0, 0});

    window.Refresh();

    for (bool quit = false; !quit; )
    {
        const auto [h, w] = hello.Getmaxyx();
        auto yx = hello.Getyx();
        const auto ch = hello.Inch();
        const auto key = hello.Getch();
        switch (key)
        {
        case 'c':
        case 'C':
            hello.Addch(ch ^ curses::ColorPair(green));
            break;
        case 'u':
        case 'U':
            hello.Addch(ch ^ curses::Attr::Underline);
            break;
        case 'q':
        case 'Q':
            quit = true;
            break;
        case curses::Key::Up:
            yx.y = std::max(0, yx.y - 1);
            break;
        case curses::Key::Down:
            yx.y = std::min(h - 1, yx.y + 1);
            break;
        case curses::Key::Left:
            yx.x = std::max(0, yx.x - 1);
            break;
        case curses::Key::Right:
            yx.x = std::min(w - 1, yx.x + 1);
            break;
        }
        hello.Move(yx);

        debug.Clear();
        debug.Box();
        debug.Addstr({1, 1}, "Key: " + curses::Keyname(key));
        debug.Refresh();
    }

    return 0;
}
