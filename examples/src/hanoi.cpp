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

#include <array>
#include <cassert>
#include <cstdlib>
#include <limits>
#include <string>
#include <variant>
#include <vector>

using namespace curses;

template<typename Sized>
static int ISize(Sized&& sized)
{
    assert(sized.size() <= std::numeric_limits<int>::max());
    return static_cast<int>(sized.size());
}

class Game
{
public:
    static constexpr int MaxNumDisks = 9;

    explicit Game(int num_disks)
    {
        num_disks = std::max(num_disks, 0);
        num_disks = std::min(num_disks, MaxNumDisks);
        for (int d = num_disks; d > 0; --d)
        {
            stacks_.at(0).push_back(d);
        }
    }

    const std::vector<int>& GetStack(int i) const
    {
        return stacks_.at(i - 1);
    }

    bool IsDone() const
    {
        return stacks_.at(0).empty() && stacks_.at(1).empty();
    }

    bool CanMove(int src, int dst) const
    {
        if (stacks_.at(src - 1).empty()) return false;
        if (stacks_.at(dst - 1).empty()) return true;
        return stacks_.at(src - 1).back() < stacks_.at(dst - 1).back();
    }

    bool TryMove(int src, int dst)
    {
        if (!CanMove(src, dst)) return false;
        stacks_.at(dst - 1).push_back(stacks_.at(src - 1).back());
        stacks_.at(src - 1).pop_back();
        return true;
    }

private:
    std::array<std::vector<int>, 3> stacks_{};
};

Window CenteredSubwindow(Window& window, SizeLinesCols size)
{
    const auto [win_h, win_w] = window.Getmaxyx();
    const auto [sub_h, sub_w] = size;
    const auto top_left = PosYx{(win_h - sub_h) / 2, (win_w - sub_w) / 2};
    return window.Derwin(size, top_left);
}

void DrawRect(Window& window, int pair_number, SizeLinesCols size, PosYx top_left)
{
    auto str = std::basic_string<Chtype>{};
    str.resize(size.cols, ' ' | Attr::Reverse | ColorPair(pair_number));
    for (int dy = 0; dy < size.lines; ++dy)
    {
        const auto [y, x] = top_left;
        window.Addchstr({y + dy, x}, str);
    }
}

void ShowGame(Window& window, const Game& game)
{
    const auto pair_white = 1;
    const auto pair_red = 2;
    InitPair(pair_white, {Color::White, Color::White});
    InitPair(pair_red, {Color::Red, Color::Red});

    const auto max_disk = Game::MaxNumDisks;
    const auto max_diam = (2 * max_disk + 1);
    const auto xpadding = 2;
    const auto draw_h = 2 + max_disk + 1;
    const auto draw_w = 3 * max_diam + 4 * xpadding;

    auto draw_window = CenteredSubwindow(window, {draw_h, draw_w});
    draw_window.Clear();

    DrawRect(draw_window, pair_white, {1, draw_w}, {draw_h - 2, 0});
    for (int i = 1; i <= 3; ++i)
    {
        const auto y = 0;
        const auto x = i * xpadding + (i - 1) * max_diam + max_disk;
        DrawRect(draw_window, pair_white, {draw_h - 1, 1}, {y, x});
        draw_window.Addch({draw_h - 1, x}, static_cast<char>('0' + i) | Attr::Bold);
    }

    for (int i = 1; i <= 3; ++i)
    {
        const auto& stack = game.GetStack(i);
        for (int j = 0; j < stack.size(); ++j)
        {
            const auto y = draw_h - j - 3;
            const auto x = i * xpadding + (i - 1) * max_diam + max_disk - stack.at(j);
            const auto diam = 2 * stack.at(j) + 1;
            DrawRect(draw_window, pair_red, {1, diam}, {y, x});
        }
    }

    draw_window.Refresh();
}

void ShowStatus(Window& window, std::string status, bool wait_for_key = false)
{
    if (wait_for_key) status += " (Press any key)";
    window.Clear();
    auto draw_window = CenteredSubwindow(window, {1, ISize(status)});
    draw_window.Addstr(status);
    window.Refresh();
    if (wait_for_key) draw_window.Getch();
}

void ShowTitle(Window& window, const std::string& title)
{
    window.Clear();
    auto draw_window = CenteredSubwindow(window, {1, ISize(title)});
    draw_window.Attron(Attr::Bold);
    draw_window.Addstr(title);
    window.Refresh();
}

void ShowHelp(Window& window)
{
    window.Clear();
    window.Box();
    const auto [h, w] = window.Getmaxyx();
    auto draw_window = CenteredSubwindow(window, {5, w - 2});
    draw_window.Addstr({0, 0}, "1-3      : Enter move");
    draw_window.Addstr({1, 0}, "U        : Undo previous move");
    draw_window.Addstr({2, 0}, "Q        : Quit");
    draw_window.Addstr({3, 0}, "Backspace: Cancel");
    draw_window.Addstr({4, 0}, "Enter    : Confirm");
    window.Refresh();
}

struct Move
{
    int src = -1;
    int dst = -1;
};
struct UndoMove {};
struct Quit{};

using Command = std::variant<Move, UndoMove, Quit>;

std::string to_string(const Move& move) // NOLINT: lower_case
{
    auto ret = std::string{};
    if (move.src == -1) return ret;
    ret.append(std::to_string(move.src) + "->");
    if (move.dst == -1) return ret;
    ret.append(std::to_string(move.dst));
    return ret;
}

std::string to_string(const UndoMove&) // NOLINT: lower_case
{
    return "Undo";
}

std::string to_string(const Quit&) // NOLINT: lower_case
{
    return "Quit";
}

std::string to_string(const Command& command) // NOLINT: lower_case
{
    return std::visit([] (auto&& val) { return to_string(val); }, command);
}

Command GetInput(Window& window)
{
    window.Clear();
    window.Box();
    window.Refresh();

    const auto [h, w] = window.Getmaxyx();
    auto draw_window = CenteredSubwindow(window, {1, w - 2});
    draw_window.Keypad();

    auto move = Move{};
    auto ret = Command{Move{}};
    auto done = false;
    while (true)
    {
        const auto key = draw_window.Getch({0, 0});
        draw_window.Clear();
        switch (key)
        {
        case '1':
        case '2':
        case '3':
            (move.src == -1 ? move.src : move.dst) = key - '0';
            ret = move;
            done = move.dst != -1;
            break;
        case 'u':
        case 'U':
            move = {};
            ret = UndoMove{};
            done = true;
            break;
        case 'q':
        case 'Q':
            move = {};
            ret = Quit{};
            done = true;
            break;
        case 263: // Backspace
            (move.dst == -1 ? move.src : move.dst) = -1;
            ret = move;
            done = false;
            break;
        case 10: // Enter
            if (done) return ret;
            break;
        }
        draw_window.Addstr({0, 0}, to_string(ret));
    }
    assert(!"unreachable");
    return Quit{};
}

Window WindowBelow(Window& window, int nlines)
{
    const auto size = SizeLinesCols{nlines, 0};
    const auto top_left = PosYx{window.Getbegyx().y + window.Getmaxyx().y, 0};
    auto* parent = window.GetParent();
    return parent ? parent->Derwin(size, top_left) : Window{size, top_left};
}

template<typename... Ts>
struct Overloaded : Ts... { using Ts::operator()...; };

template<typename... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

int main(int argc, const char** argv)
{
    const auto _ = Initscr();
    CursSet(CursorVisibility::Invisible);
    Noecho();
    StartColor();

    const auto min_lines = 40;
    const auto min_cols = 80;

    auto status_window = Window{{1, std::min(min_cols, Cols())}, {Lines() - 1, 0}};

    if (!(Lines() >= min_lines && Cols() >= min_cols))
    {
        ShowStatus(status_window, "Screen too small", true);
        return 1;
    }

    const auto num_disks = (argc == 2)
            ? std::atoi(argv[1])    // Returns 0 on failure
            : 7;                    // Default
    if (!(1 <= num_disks && num_disks <= Game::MaxNumDisks))
    {
        const auto status = "Num disks should be in 1.." + std::to_string(Game::MaxNumDisks);
        ShowStatus(status_window, status, true);
        return 1;
    }

    auto main_window = Window{{0, min_cols}, {}};
    auto title_window = main_window.Derwin({3, 0}, {0, 0});
    auto game_window = WindowBelow(title_window, 22);
    auto help_window = WindowBelow(game_window, 7);
    auto input_window = WindowBelow(help_window, 3);
    ShowTitle(title_window, "Towers of Hanoi");
    ShowHelp(help_window);

    auto game = Game{num_disks};
    auto moves = std::vector<Move>{};
    auto quit = false;
    const auto handlers = Overloaded
    {
        [&] (const Move& move) -> std::string
        {
            const auto res = game.TryMove(move.src, move.dst);
            if (!res) return "Invalid move: " + to_string(move);
            moves.push_back(move);
            return "";
        },
        [&] (const UndoMove&) -> std::string
        {
            if (moves.empty()) return "No move to undo";
            const auto res = game.TryMove(moves.back().dst, moves.back().src);
            assert(res);
            const auto move = moves.back();
            moves.pop_back();
            return "Undid move: " + to_string(move);
        },
        [&] (const Quit&) -> std::string
        {
            quit = true;
            return "";
        }
    };

    ShowGame(game_window, game);
    while (!(quit || game.IsDone()))
    {
        const auto command = GetInput(input_window);
        const auto status = std::visit(handlers, command);
        ShowGame(game_window, game);
        ShowStatus(status_window, status);
    }
    if (game.IsDone())
    {
        ShowStatus(status_window, "Success!", true);
    }

    return 0;
}
