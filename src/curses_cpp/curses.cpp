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

#include <stdexcept>

#define CHECK_GET [&] { auto* ret = Get(); assert(ret); return ret; }
#define RETURN_RESULT(expr) return static_cast<Result>(expr)

namespace curses
{

AutoEndwin::AutoEndwin(AutoEndwin&& other) noexcept
    : released_{other.released_}
{
    other.released_ = true;
}

AutoEndwin& AutoEndwin::operator=(AutoEndwin&& other) noexcept
{
    released_ = other.released_;
    other.released_ = true;
    return *this;
}

AutoEndwin::~AutoEndwin()
{
    if (released_) return;
    const auto res = endwin();
    assert(res != ERR);
}

AutoEndwin Initscr()
{
    auto* window = initscr();
    // If initscr fails the ncurses exits the program, see
    // https://invisible-island.net/ncurses/man/curs_initscr.3x.html
    assert(window);
    // When keypad is first enabled, ncurses loads the key
    // definitions needed for HasKey, see
    // https://invisible-island.net/ncurses/man/curs_inopts.3x.html
    keypad(window, true);
    keypad(window, false);
    return AutoEndwin{};
}

Result Endwin() { RETURN_RESULT(endwin()); }
bool Isendwin() { return isendwin(); }
int Lines() { return LINES; }
int Cols() { return COLS; }

Result Cbreak(bool enable) { RETURN_RESULT(enable ? cbreak() : nocbreak()); }
Result Nocbreak() { RETURN_RESULT(nocbreak()); }
Result Echo(bool enable) { RETURN_RESULT(enable ? echo() : noecho()); }
Result Noecho() { RETURN_RESULT(noecho()); }
Result Nl(bool enable) { RETURN_RESULT(enable ? nl() : nonl()); }
Result Nonl() { RETURN_RESULT(nonl()); }
Result Raw(bool enable) { RETURN_RESULT(enable ? raw() : noraw()); }
Result Noraw() { RETURN_RESULT(noraw()); }
void Qiflush(bool enable) { enable ? qiflush() : noqiflush(); }
void Noqiflush() { noqiflush(); }
Result Halfdelay(int tenths) { RETURN_RESULT(halfdelay(tenths)); }
Result Typeahead(int fd) { RETURN_RESULT(typeahead(fd)); }
Result Meta(bool enable)
{
    // The pointer argument to meta is always ignored, see
    // https://invisible-island.net/ncurses/man/curs_inopts.3x.html
    RETURN_RESULT(meta(nullptr, enable));
}

bool HasColors() { return has_colors(); }
bool CanChangeColor() { return can_change_color(); }
Result StartColor() { RETURN_RESULT(start_color()); }
int Colors() { return COLORS; }
int ColorPairs() { return COLOR_PAIRS; }

Result InitPair(int pair_number, ColorPairFgBg fg_bg)
{
    const auto res = init_pair(
            static_cast<short>(pair_number),
            static_cast<short>(fg_bg.fg),
            static_cast<short>(fg_bg.bg));
    RETURN_RESULT(res);
}

ColorPairFgBg PairContent(int pair_number)
{
    short fg = 0;
    short bg = 0;
    pair_content(static_cast<short>(pair_number), &fg, &bg);
    return {static_cast<Color>(fg), static_cast<Color>(bg)};
}

Result InitColor(Color color, ColorRgb rgb)
{
    const auto res = init_color(
            static_cast<short>(color),
            static_cast<short>(rgb.r),
            static_cast<short>(rgb.g),
            static_cast<short>(rgb.b));
    RETURN_RESULT(res);
}

ColorRgb ColorContent(Color color)
{
    short r = 0;
    short g = 0;
    short b = 0;
    color_content(static_cast<short>(color), &r, &g, &b);
    return {r, g, b};
}

Result Doupdate() { RETURN_RESULT(doupdate()); }

Window::Window(SizeLinesCols lines_cols, PosYx top_left) :
    window_{newwin(lines_cols.lines, lines_cols.cols, top_left.y, top_left.x)}
{
    if (!window_)
    {
        throw std::runtime_error{"newwin failed"};
    }
}

Window::Window(const Window& other) :
    window_{dupwin(static_cast<WINDOW*>(other.window_))},
    parent_{other.parent_}
{
    if ((window_ == nullptr) != (other.window_ == nullptr))
    {
        throw std::runtime_error{"dupwin failed"};
    }
}

Window& Window::operator=(const Window& other)
{
    auto tmp = other;
    swap(*this, tmp);
    return *this;
}

Window::~Window()
{
    if (window_ == nullptr) return;
    assert(window_ != curscr);
    assert(window_ != newscr);
    assert(window_ != stdscr);
    const auto res = delwin(window_);
    // delwin returns ERR if this is the parent of another window, see
    // https://invisible-island.net/ncurses/man/curs_window.3x.html
    assert((res != ERR) && "delwin error (possibly because a subwindow is still alive)");
}

Window Window::Subwin(SizeLinesCols lines_cols, PosYx top_left_on_screen)
{
    return SubwinImpl(lines_cols, top_left_on_screen, "subwin");
}

Window Window::Derwin(SizeLinesCols lines_cols, PosYx top_left_in_parent)
{
    return SubwinImpl(lines_cols, top_left_in_parent, "derwin");
}

Result Window::Mvwin(PosYx top_left)
{
    RETURN_RESULT(mvwin(CHECK_GET(), top_left.y, top_left.x));
}

Result Window::Mvderwin(PosYx viewed_top_left)
{
    RETURN_RESULT(mvderwin(CHECK_GET(), viewed_top_left.y, viewed_top_left.x));
}

Result Window::Syncok(bool enable) { RETURN_RESULT(syncok(CHECK_GET(), enable)); }
void Window::Syncup() { wsyncup(CHECK_GET()); }
void Window::Cursyncup() { wcursyncup(CHECK_GET()); }
void Window::Syncdown() { wsyncdown(CHECK_GET()); }

bool Window::IsCleared() const { return is_cleared(CHECK_GET()); }
bool Window::IsIdcok() const { return is_idcok(CHECK_GET()); }
bool Window::IsIdlok() const { return is_idlok(CHECK_GET()); }
bool Window::IsImmedok() const { return is_immedok(CHECK_GET()); }
bool Window::IsKeypad() const { return is_keypad(CHECK_GET()); }
bool Window::IsLeaveok() const { return is_leaveok(CHECK_GET()); }
bool Window::IsNodelay() const { return is_nodelay(CHECK_GET()); }
bool Window::IsNotimeout() const { return is_notimeout(CHECK_GET()); }
bool Window::IsPad() const { return is_pad(CHECK_GET()); }
bool Window::IsScrollok() const { return is_scrollok(CHECK_GET()); }
bool Window::IsSubwin() const { return is_subwin(CHECK_GET()); }
bool Window::IsSyncok() const { return is_syncok(CHECK_GET()); }
int Window::Getdelay() const { return wgetdelay(CHECK_GET()); }

Result Window::Keypad(bool enable) { RETURN_RESULT(keypad(CHECK_GET(), enable)); }
Result Window::Nodelay(bool enable) { RETURN_RESULT(nodelay(CHECK_GET(), enable)); }
Result Window::Notimeout(bool enable) { RETURN_RESULT(notimeout(CHECK_GET(), enable)); }
void Window::Timeout(int delay_ms) { wtimeout(CHECK_GET(), delay_ms); }

Result Window::Clearok(bool enable) { RETURN_RESULT(clearok(CHECK_GET(), enable)); }
Result Window::Idlok(bool enable) { RETURN_RESULT(idlok(CHECK_GET(), enable)); }
void Window::Idcok(bool enable) { idcok(CHECK_GET(), enable); }
void Window::Immedok(bool enable) { immedok(CHECK_GET(), enable); }
Result Window::Leaveok(bool enable) { RETURN_RESULT(leaveok(CHECK_GET(), enable)); }
Result Window::Scrollok(bool enable) { RETURN_RESULT(scrollok(CHECK_GET(), enable)); }
Result Window::Setscrreg(int top, int bot) { RETURN_RESULT(wsetscrreg(CHECK_GET(), top, bot)); }

Result Window::Attron(Attr attr) { RETURN_RESULT(wattron(CHECK_GET(), static_cast<int>(attr))); }
Result Window::Attroff(Attr attr) { RETURN_RESULT(wattroff(CHECK_GET(), static_cast<int>(attr))); }
Result Window::Attrset(Attr attr) { RETURN_RESULT(wattrset(CHECK_GET(), static_cast<int>(attr))); }
Result Window::Colorset(int pair_number) { RETURN_RESULT(wcolor_set(CHECK_GET(), static_cast<short>(pair_number), nullptr)); }
Attr Window::Attrget() { return static_cast<Attr>(getattrs(CHECK_GET())); }

Result Window::Chgat(Attr attr) { return Chgat(-1, attr); }
Result Window::Chgat(int n, Attr attr)
{
    const auto a = static_cast<attr_t>(RemoveColor(attr));
    const auto c = static_cast<short>(PairNumber(attr));
    RETURN_RESULT(wchgat(CHECK_GET(), n, a, c, nullptr));
}

Result Window::Chgat(PosYx yx, Attr attr) { return Chgat(yx, -1, attr); }
Result Window::Chgat(PosYx yx, int n, Attr attr)
{
    const auto a = static_cast<attr_t>(RemoveColor(attr));
    const auto c = static_cast<short>(PairNumber(attr));
    RETURN_RESULT(mvwchgat(CHECK_GET(), yx.y, yx.x, n, a, c, nullptr));
}

Result Window::Erase() { RETURN_RESULT(werase(CHECK_GET())); }
Result Window::Clear() { RETURN_RESULT(wclear(CHECK_GET())); }
Result Window::Clrtobot() { RETURN_RESULT(wclrtobot(CHECK_GET())); }
Result Window::Clrtoeol() { RETURN_RESULT(wclrtoeol(CHECK_GET())); }

Result Window::Refresh() { RETURN_RESULT(wrefresh(CHECK_GET())); }
Result Window::Noutrefresh() { RETURN_RESULT(wnoutrefresh(CHECK_GET())); }
Result Window::Redrawwin() { RETURN_RESULT(redrawwin(CHECK_GET())); }
Result Window::Redrawln(int beg_line, int num_lines) { RETURN_RESULT(wredrawln(CHECK_GET(), beg_line, num_lines)); }

Result Window::Border(const BorderSides& sides, const BorderCorners& corners)
{
    const auto res = wborder(CHECK_GET(),
            sides.l.Get(), sides.r.Get(), sides.t.Get(), sides.b.Get(),
            corners.tl.Get(), corners.tr.Get(), corners.bl.Get(), corners.br.Get());
    RETURN_RESULT(res);
}

Result Window::Hline(Chtype ch, int n) { RETURN_RESULT(whline(CHECK_GET(), ch.Get(), n)); }
Result Window::Hline(PosYx yx, Chtype ch, int n) { RETURN_RESULT(mvwhline(CHECK_GET(), yx.y, yx.x, ch.Get(), n)); }
Result Window::Vline(Chtype ch, int n) { RETURN_RESULT(wvline(CHECK_GET(), ch.Get(), n)); }
Result Window::Vline(PosYx yx, Chtype ch, int n) { RETURN_RESULT(mvwvline(CHECK_GET(), yx.y, yx.x, ch.Get(), n)); }

Result Window::Move(PosYx yx) { RETURN_RESULT(wmove(CHECK_GET(), yx.y, yx.x)); }

Window Window::SubwinImpl(
        SizeLinesCols lines_cols,
        PosYx top_left,
        const std::string& method)
{
    const auto [l, c] = lines_cols;
    const auto [y, x] = top_left;
    auto* window = static_cast<WINDOW*>(nullptr);
    if (method == "subwin") window = subwin(CHECK_GET(), l, c, y, x);
    if (method == "derwin") window = derwin(CHECK_GET(), l, c, y, x);
    if (method == "subpad") window = subpad(CHECK_GET(), l, c, y, x);
    if (!window) throw std::runtime_error{method + " failed"};
    assert(wgetparent(window) == window_);
    auto ret = Window{};
    ret.window_ = window;
    ret.parent_ = this;
    return ret;
}

} // namespace curses
