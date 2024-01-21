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

#include <algorithm>
#include <array>
#include <deque>
#include <limits>
#include <stdexcept>

#define CHECK_GET [&] { auto* ret = Get(); assert(ret); return ret; }
#define RETURN_RESULT(expr) return static_cast<Result>(expr)

namespace curses
{

template<typename Sized>
static int ISize(Sized&& sized)
{
    assert(sized.size() <= std::numeric_limits<int>::max());
    return static_cast<int>(sized.size());
}

namespace
{

template<int StackBufStrCap, typename CharType = char>
class StringBuffer
{
    static_assert(std::is_same_v<CharType, char> || std::is_same_v<CharType, Chtype>);
public:
    // Create a buffer with capacity for str_cap characters + trailing null
    explicit StringBuffer(int str_cap) :
        data_{buf_.data()} // NOLINT: buf_ is uninitialized
    {
        if (str_cap >= ISize(buf_))
        {
            str_.resize(str_cap + 1);
            data_ = str_.data();
        }
    }

    // Return a pointer to the buffer
    CharType* Data() { return data_; }

    // After a null-terminated string has been written to Data(), extract it.
    std::basic_string<CharType> Str() &&
    {
        const auto using_buf = (data_ == buf_.data());
        if (using_buf)
        {
            auto* e = std::find(buf_.begin(), buf_.end(), CharType{'\0'});
            assert(e != buf_.end());
            str_.assign(buf_.begin(), e);
        }
        else
        {
            auto e = std::find(str_.begin(), str_.end(), CharType{'\0'});
            assert(e != str_.end());
            str_.resize(e - str_.begin());
        }
        return std::move(str_);
    }

private:
    CharType* data_;
    std::basic_string<CharType> str_;
    std::array<CharType, StackBufStrCap + 1> buf_;  // + 1 for trailing null
};

} // namespace

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
    [[maybe_unused]]
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

std::string Termname()
{
    auto* ret = termname();
    return ret == nullptr ? "" : std::string{ret};
}

std::string Longname()
{
    auto* ret = longname();
    return ret == nullptr ? "" : std::string{ret};
}

Attr Termattrs() { return static_cast<Attr>(termattrs()); }
char Erasechar() { return erasechar(); }
char Killchar() { return killchar(); }

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

void Filter() { filter(); }
void Nofilter() { nofilter(); }
void UseEnv(bool bf) { use_env(bf); }
void UseTioctl(bool bf) { use_tioctl(bf); }
Result DelayOutput(int ms) { RETURN_RESULT(delay_output(ms)); }
Result Flushinp() { RETURN_RESULT(flushinp()); }

std::string Unctrl(Chtype ch)
{
    const auto* ret = unctrl(ch.Get());
    return std::string{ret ? ret : "null"};
}

std::string Keyname(int key)
{
    const auto* ret = keyname(key);
    return std::string{ret ? ret : "null"};
}

Result Putwin(Window& win, FILE* file)
{
    RETURN_RESULT(putwin(win.Get(), file));
}

std::optional<Window> Getwin(FILE* file)
{
    auto* window = getwin(file);
    if (window == nullptr) return std::nullopt;
    auto ret = Window{};
    ret.window_ = window;
    return ret;
}

Result DefProgMode() { RETURN_RESULT(def_prog_mode()); }
Result DefShellMode() { RETURN_RESULT(def_shell_mode()); }
Result ResetProgMode() { RETURN_RESULT(reset_prog_mode()); }
Result ResetShellMode() { RETURN_RESULT(reset_shell_mode()); }
Result Savetty() { RETURN_RESULT(savetty()); }
Result Resetty() { RETURN_RESULT(resetty()); }

PosYx Getsyx()
{
    auto ret = PosYx{};
    getsyx(ret.y, ret.x);
    return ret;
}

void Setsyx(PosYx yx)
{
    setsyx(yx.y, yx.x);
}

CursorVisibility CursSet(CursorVisibility vis)
{
    return static_cast<CursorVisibility>(curs_set(static_cast<int>(vis)));
}

Result Napms(int ms) { RETURN_RESULT(napms(ms)); }

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

Result Ungetch(int ch) { RETURN_RESULT(ungetch(ch)); }
bool HasKey(int ch) { return static_cast<bool>(has_key(ch)); }

bool HasMouse() { return has_mouse(); }

Mmask Mousemask(Mmask newmask)
{
    return static_cast<Mmask>(mousemask(static_cast<mmask_t>(newmask), nullptr));
}

Mmask Mousemask(Mmask newmask, Mmask oldmask)
{
    auto oldmask_mmask = static_cast<mmask_t>(oldmask);
    return static_cast<Mmask>(mousemask(static_cast<mmask_t>(newmask), &oldmask_mmask));
}

int Mouseinterval(int interval_ms) { return mouseinterval(interval_ms); }

std::optional<Mevent> Getmouse()
{
    auto out = MEVENT{};
    const auto res = getmouse(&out);
    if (res == ERR) return std::nullopt;
    return Mevent{out.id, out.x, out.y, out.z, static_cast<Mmask>(out.bstate)};
}

Result Ungetmouse(const Mevent& event)
{
    auto evt = MEVENT{event.id, event.x, event.y, event.z, static_cast<mmask_t>(event.bstate)};
    RETURN_RESULT(ungetmouse(&evt));
}

Result Beep() { RETURN_RESULT(beep()); }
Result Flash() { RETURN_RESULT(flash()); }

Window::Window(SizeLinesCols lines_cols, PosYx top_left) :
    window_{newwin(lines_cols.lines, lines_cols.cols, top_left.y, top_left.x)}
{
    if (!window_)
    {
        throw std::runtime_error{"newwin failed"};
    }
}

std::string CursesVersion()
{
    return std::string{curses_version()};
}

bool UseExtendendNames(bool enable)
{
    return use_extended_names(enable);
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
    [[maybe_unused]]
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
ScrregTopBot Window::Getscrreg() const
{
    auto ret = ScrregTopBot{};
    wgetscrreg(CHECK_GET(), &ret.top, &ret.bot);
    return ret;
}

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
Result Window::Setscrreg(ScrregTopBot top_bot) { RETURN_RESULT(wsetscrreg(CHECK_GET(), top_bot.top, top_bot.bot)); }

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

Result Window::Bkgd(Chtype ch) { RETURN_RESULT(wbkgd(CHECK_GET(), ch.Get())); }
void Window::Bkgdset(Chtype ch) { wbkgdset(CHECK_GET(), ch.Get()); }
Chtype Window::Getbkgd() { return Chtype{getbkgd(CHECK_GET())}; }

Result Window::Erase() { RETURN_RESULT(werase(CHECK_GET())); }
Result Window::Clear() { RETURN_RESULT(wclear(CHECK_GET())); }
Result Window::Clrtobot() { RETURN_RESULT(wclrtobot(CHECK_GET())); }
Result Window::Clrtoeol() { RETURN_RESULT(wclrtoeol(CHECK_GET())); }

Result Window::Refresh() { RETURN_RESULT(wrefresh(CHECK_GET())); }
Result Window::Noutrefresh() { RETURN_RESULT(wnoutrefresh(CHECK_GET())); }
Result Window::Redrawwin() { RETURN_RESULT(redrawwin(CHECK_GET())); }
Result Window::Redrawln(int beg_line, int num_lines) { RETURN_RESULT(wredrawln(CHECK_GET(), beg_line, num_lines)); }

Result Window::Touchwin(bool changed)
{
    if (changed) RETURN_RESULT(touchwin(CHECK_GET()));
    RETURN_RESULT(untouchwin(CHECK_GET()));
}
Result Window::Untouchwin() { RETURN_RESULT(untouchwin(CHECK_GET())); }
bool Window::IsWintouched() { return is_wintouched(CHECK_GET()); }

Result Window::Touchline(int start, int count, bool changed)
{
    RETURN_RESULT(wtouchln(CHECK_GET(), start, count, changed));
}
Result Window::Untouchline(int start, int count) { RETURN_RESULT(wtouchln(CHECK_GET(), start, count, false)); }
bool Window::IsLinetouched(int line) { return is_linetouched(CHECK_GET(), line); }

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

Result Window::Overlay(const Window& src)
{
    RETURN_RESULT(overlay(src.Get(), CHECK_GET()));
}

Result Window::Overlay(const Window& src, PosYx src_min, PosYx dst_min, PosYx dst_max)
{
    const auto res = copywin(
            src.Get(), CHECK_GET(),
            src_min.y, src_min.x,
            dst_min.y, dst_min.x,
            dst_max.y, dst_max.x, true);
    RETURN_RESULT(res);
}

Result Window::Overwrite(const Window& src)
{
    RETURN_RESULT(overwrite(src.Get(), CHECK_GET()));
}

Result Window::Overwrite(const Window& src, PosYx src_min, PosYx dst_min, PosYx dst_max)
{
    const auto res = copywin(
            src.Get(), CHECK_GET(),
            src_min.y, src_min.x,
            dst_min.y, dst_min.x,
            dst_max.y, dst_max.x, false);
    RETURN_RESULT(res);
}

Result Window::Move(PosYx yx) { RETURN_RESULT(wmove(CHECK_GET(), yx.y, yx.x)); }

PosYx Window::Getyx()
{
    auto ret = PosYx{};
    getyx(CHECK_GET(), ret.y, ret.x);
    return ret;
}

PosYx Window::Getparyx()
{
    auto ret = PosYx{};
    getparyx(CHECK_GET(), ret.y, ret.x);
    return ret;
}

PosYx Window::Getbegyx()
{
    auto ret = PosYx{};
    getbegyx(CHECK_GET(), ret.y, ret.x);
    return ret;
}

PosYx Window::Getmaxyx()
{
    auto ret = PosYx{};
    getmaxyx(CHECK_GET(), ret.y, ret.x);
    return ret;
}

int Window::Getch() { return wgetch(CHECK_GET()); }
int Window::Getch(PosYx yx) { return mvwgetch(CHECK_GET(), yx.y, yx.x); }

std::string Window::Getstr(int maxlen)
{
    auto buf = StringBuffer<1024>{maxlen};
    const auto res = wgetnstr(CHECK_GET(), buf.Data(), maxlen);
    if (res != OK) return "";
    return std::move(buf).Str();
}

std::string Window::Getstr(PosYx yx, int maxlen)
{
    auto buf = StringBuffer<1024>{maxlen};
    const auto res = mvwgetnstr(CHECK_GET(), yx.y, yx.x, buf.Data(), maxlen);
    if (res != OK) return "";
    return std::move(buf).Str();
}

Result Window::Addch(Chtype ch) { RETURN_RESULT(waddch(CHECK_GET(), ch.Get())); }
Result Window::Addch(PosYx yx, Chtype ch) { RETURN_RESULT(mvwaddch(CHECK_GET(), yx.y, yx.x, ch.Get())); }
Result Window::Echochar(Chtype ch) { RETURN_RESULT(wechochar(CHECK_GET(), ch.Get())); }

Result Window::Addchstr(std::basic_string_view<Chtype> str)
{
    static_assert(sizeof(Chtype) == sizeof(unsigned), "no padding in Chtype");
    const auto res = waddchnstr(
            CHECK_GET(),
            reinterpret_cast<const chtype*>(str.data()),
            ISize(str));
    RETURN_RESULT(res);
}

Result Window::Addchstr(PosYx yx, std::basic_string_view<Chtype> str)
{
    static_assert(sizeof(Chtype) == sizeof(unsigned), "no padding in Chtype");
    const auto res = mvwaddchnstr(
            CHECK_GET(),
            yx.y, yx.x,
            reinterpret_cast<const chtype*>(str.data()),
            ISize(str));
    RETURN_RESULT(res);
}

Result Window::Addstr(std::string_view str)
{
    RETURN_RESULT(waddnstr(CHECK_GET(), str.data(), ISize(str)));
}

Result Window::Addstr(PosYx yx, std::string_view str)
{
    RETURN_RESULT(mvwaddnstr(CHECK_GET(), yx.y, yx.x, str.data(), ISize(str)));
}

Result Window::Insch(Chtype ch) { RETURN_RESULT(winsch(CHECK_GET(), ch.Get())); }
Result Window::Insch(PosYx yx, Chtype ch) { RETURN_RESULT(mvwinsch(CHECK_GET(), yx.y, yx.x, ch.Get())); }

Result Window::Insstr(std::string_view str)
{
    RETURN_RESULT(winsnstr(CHECK_GET(), str.data(), ISize(str)));
}

Result Window::Insstr(PosYx yx, std::string_view str)
{
    RETURN_RESULT(mvwinsnstr(CHECK_GET(), yx.y, yx.x, str.data(), ISize(str)));
}

Result Window::Delch() { RETURN_RESULT(wdelch(CHECK_GET())); }
Result Window::Delch(PosYx yx) { RETURN_RESULT(mvwdelch(CHECK_GET(), yx.y, yx.x)); }

Chtype Window::Inch() { return Chtype{winch(CHECK_GET())}; }
Chtype Window::Inch(PosYx yx) { return Chtype{mvwinch(CHECK_GET(), yx.y, yx.x)}; }

std::basic_string<Chtype> Window::Inchstr(int maxlen)
{
    static_assert(sizeof(Chtype) == sizeof(unsigned), "no padding in Chtype");
    auto buf = StringBuffer<1024, Chtype>{maxlen};
    const auto res = winchnstr(CHECK_GET(), reinterpret_cast<chtype*>(buf.Data()), maxlen);
    if (res == ERR) return {};
    return std::move(buf).Str();
}

std::basic_string<Chtype> Window::Inchstr(PosYx yx, int maxlen)
{
    static_assert(sizeof(Chtype) == sizeof(unsigned), "no padding in Chtype");
    auto buf = StringBuffer<1024, Chtype>{maxlen};
    const auto res = mvwinchnstr(CHECK_GET(), yx.y, yx.x, reinterpret_cast<chtype*>(buf.Data()), maxlen);
    if (res == ERR) return {};
    return std::move(buf).Str();
}

std::string Window::Instr(int n)
{
    auto buf = StringBuffer<1024>{n};
    const auto res = winnstr(CHECK_GET(), buf.Data(), n);
    if (res == ERR) return "";
    return std::move(buf).Str();
}

std::string Window::Instr(PosYx yx, int n)
{
    auto buf = StringBuffer<1024>{n};
    const auto res = mvwinnstr(CHECK_GET(), yx.y, yx.x, buf.Data(), n);
    if (res == ERR) return "";
    return std::move(buf).Str();
}

Result Window::Scroll(int n) { RETURN_RESULT(wscrl(CHECK_GET(), n)); }

Result Window::Deleteln() { RETURN_RESULT(wdeleteln(CHECK_GET())); }
Result Window::Insertln() { RETURN_RESULT(winsertln(CHECK_GET())); }
Result Window::Insdelln(int n) { RETURN_RESULT(winsdelln(CHECK_GET(), n)); }

bool Window::Enclose(PosYx pos_on_screen) const
{
    return wenclose(CHECK_GET(), pos_on_screen.y, pos_on_screen.x);
}

PosYx Window::TransformToWindow(PosYx pos_on_screen) const
{
    auto ret = pos_on_screen;
    wmouse_trafo(CHECK_GET(), &ret.y, &ret.x, false);
    return ret;
}

PosYx Window::TransformToScreen(PosYx pos_in_window) const
{
    auto ret = pos_in_window;
    wmouse_trafo(CHECK_GET(), &ret.y, &ret.x, true);
    return ret;
}

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
