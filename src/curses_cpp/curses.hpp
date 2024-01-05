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
#ifndef CURSES_CPP_CURSES_HPP_
#define CURSES_CPP_CURSES_HPP_

#include <cassert>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

using WINDOW = struct _win_st; // NOLINT: Needed for Window::Get

namespace curses
{

enum class Result : int
{
    Err = -1,
    Ok  =  0,
};

constexpr bool operator==(Result a, Result b) { return static_cast<int>(a) == static_cast<int>(b); }
constexpr bool operator!=(Result a, Result b) { return !(a == b); }

constexpr bool operator==(Result a, int b) { return static_cast<int>(a) == b; }
constexpr bool operator!=(Result a, int b) { return !(a == b); }

constexpr bool operator==(int a, Result b) { return a == static_cast<int>(b); }
constexpr bool operator!=(int a, Result b) { return !(a == b); }

enum class Color : int
{
    Black   = 0,
    Red     = 1,
    Green   = 2,
    Yellow  = 3,
    Blue    = 4,
    Magenta = 5,
    Cyan    = 6,
    White   = 7,
};

// Key contains integer constants corresponding to curses KEY_*.
// Using enum class here would make it inconvenient to write
// switch statements over the return value of Getch.
namespace Key {} // NOLINT: CamelCase

enum class Mmask : unsigned;

constexpr Mmask operator|(Mmask a, Mmask b) { return static_cast<Mmask>(static_cast<unsigned>(a) | static_cast<unsigned>(b)); }
constexpr Mmask operator&(Mmask a, Mmask b) { return static_cast<Mmask>(static_cast<unsigned>(a) & static_cast<unsigned>(b)); }
constexpr Mmask operator^(Mmask a, Mmask b) { return static_cast<Mmask>(static_cast<unsigned>(a) ^ static_cast<unsigned>(b)); }

constexpr Mmask& operator|=(Mmask& a, Mmask b) { return a = a | b; }
constexpr Mmask& operator&=(Mmask& a, Mmask b) { return a = a & b; }
constexpr Mmask& operator^=(Mmask& a, Mmask b) { return a = a ^ b; }

enum class CursorVisibility : int
{
    Err = -1,
    Invisible = 0,
    Normal = 1,
    VeryVisible = 2,
};

constexpr bool operator==(CursorVisibility a, Result b) { return a == CursorVisibility::Err && b == Result::Err; }
constexpr bool operator!=(CursorVisibility a, Result b) { return !(a == b); }

constexpr bool operator==(Result a, CursorVisibility b) { return b == a; }
constexpr bool operator!=(Result a, CursorVisibility b) { return !(a == b); }

namespace detail
{

constexpr unsigned CharMask     = 0xFF;
constexpr unsigned AttrShift    = 8;
constexpr unsigned AttrMask     = ~CharMask;
constexpr unsigned ColorMask    = 0xFF00;

} // namespace detail

enum class Attr : unsigned
{
    Normal      = 0U,
    Standout    = 1U << ( 8 + detail::AttrShift),
    Underline   = 1U << ( 9 + detail::AttrShift),
    Reverse     = 1U << (10 + detail::AttrShift),
    Blink       = 1U << (11 + detail::AttrShift),
    Dim         = 1U << (12 + detail::AttrShift),
    Bold        = 1U << (13 + detail::AttrShift),
    Altcharset  = 1U << (14 + detail::AttrShift),
    Invis       = 1U << (15 + detail::AttrShift),
    Protect     = 1U << (16 + detail::AttrShift),
    Horizontal  = 1U << (17 + detail::AttrShift),
    Left        = 1U << (18 + detail::AttrShift),
    Low         = 1U << (19 + detail::AttrShift),
    Right       = 1U << (20 + detail::AttrShift),
    Top         = 1U << (21 + detail::AttrShift),
    Vertical    = 1U << (22 + detail::AttrShift),
};

constexpr bool operator==(Attr a, Attr b) { return static_cast<unsigned>(a) == static_cast<unsigned>(b); }
constexpr bool operator!=(Attr a, Attr b) { return !(a == b); }

constexpr Attr operator|(Attr a, Attr b) { return static_cast<Attr>(static_cast<unsigned>(a) | static_cast<unsigned>(b)); }
constexpr Attr operator&(Attr a, Attr b) { return static_cast<Attr>(static_cast<unsigned>(a) & static_cast<unsigned>(b)); }
constexpr Attr operator^(Attr a, Attr b) { return static_cast<Attr>(static_cast<unsigned>(a) ^ static_cast<unsigned>(b)); }

constexpr Attr& operator|=(Attr& a, Attr b) { return a = a | b; }
constexpr Attr& operator&=(Attr& a, Attr b) { return a = a & b; }
constexpr Attr& operator^=(Attr& a, Attr b) { return a = a ^ b; }

constexpr Attr RemoveColor(Attr attr)
{
    return attr & static_cast<Attr>(~detail::ColorMask);
}

constexpr Attr ColorPair(int pair_number)
{
    using namespace detail;
    assert(0 <= pair_number && pair_number < 256);
    return static_cast<Attr>((pair_number << AttrShift) & ColorMask);
}

constexpr int PairNumber(Attr attr)
{
    using namespace detail;
    return static_cast<int>((static_cast<unsigned>(attr) & ColorMask) >> AttrShift);
}

class Chtype
{
public:
    Chtype() = default;

    constexpr explicit Chtype(unsigned ch) : ch_{ch} {}

    constexpr Chtype(char c, Attr attr = Attr::Normal) : // NOLINT: Allow implicit conversion
        ch_{static_cast<unsigned>(c) | static_cast<unsigned>(attr)}
    {
        assert(c >= 0);
    }

    constexpr Chtype(char c, Attr attr, int pair_number) :
        Chtype{c, RemoveColor(attr) | ColorPair(pair_number)}
    {}

    constexpr unsigned Get() const { return ch_; }
    constexpr char GetChar() const { return static_cast<char>(ch_ & detail::CharMask); }
    constexpr Attr GetAttr() const { return static_cast<Attr>(ch_ & detail::AttrMask); }
    constexpr Attr GetAttrRemoveColor() const { return RemoveColor(GetAttr()); }
    constexpr Attr GetColorPair() const { return static_cast<Attr>(ch_ & detail::ColorMask); }
    constexpr int GetPairNumber() const { return PairNumber(GetColorPair()); }

private:
    unsigned ch_;
};

static_assert(std::is_standard_layout_v<Chtype>, "for std::string and std::string_view");
static_assert(std::is_trivial_v<Chtype>, "for std::string and std::string_view");

constexpr bool operator==(Chtype a, Chtype b) { return a.Get() == b.Get(); }
constexpr bool operator!=(Chtype a, Chtype b) { return !(a == b); }

constexpr Chtype operator|(Chtype ch, Attr attr) { return Chtype{ch.Get() | static_cast<unsigned>(attr)}; }
constexpr Chtype operator|(Attr attr, Chtype ch) { return ch | attr; }

constexpr Chtype operator^(Chtype ch, Attr attr) { return Chtype{ch.Get() ^ static_cast<unsigned>(attr)}; }
constexpr Chtype operator^(Attr attr, Chtype ch) { return ch ^ attr; }

constexpr Chtype& operator|=(Chtype& ch, Attr attr) { return ch = ch | attr; }
constexpr Chtype& operator^=(Chtype& ch, Attr attr) { return ch = ch ^ attr; }

struct PosYx
{
    int y = 0;
    int x = 0;
};

constexpr bool operator==(PosYx a, PosYx b)
{
    return a.y == b.y && a.x == b.x;
}

constexpr bool operator!=(PosYx a, PosYx b)
{
    return !(a == b);
}

struct SizeLinesCols
{
    int lines = 0;
    int cols = 0;
};

constexpr bool operator==(SizeLinesCols a, SizeLinesCols b)
{
    return a.lines == b.lines && a.cols == b.cols;
}

constexpr bool operator!=(SizeLinesCols a, SizeLinesCols b)
{
    return !(a == b);
}

struct ColorPairFgBg
{
    Color fg = Color::Black;
    Color bg = Color::Black;
};

constexpr bool operator==(ColorPairFgBg a, ColorPairFgBg b)
{
    return a.fg == b.fg && a.bg == b.bg;
}

constexpr bool operator!=(ColorPairFgBg a, ColorPairFgBg b)
{
    return !(a == b);
}

struct ColorRgb
{
    // Valid range: 0..1000
    int r = 0;
    int g = 0;
    int b = 0;
};

constexpr bool operator==(ColorRgb a, ColorRgb b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

constexpr bool operator!=(ColorRgb a, ColorRgb b)
{
    return !(a == b);
}

struct BorderSides
{
    Chtype l = 0;
    Chtype r = 0;
    Chtype t = 0;
    Chtype b = 0;
};

struct BorderCorners
{
    Chtype tl = 0;
    Chtype tr = 0;
    Chtype bl = 0;
    Chtype br = 0;
};

class AutoEndwin
{
public:
    [[nodiscard]] AutoEndwin() = default;
    AutoEndwin(const AutoEndwin&) = delete;
    AutoEndwin& operator=(const AutoEndwin&) = delete;
    AutoEndwin(AutoEndwin&& other) noexcept;
    AutoEndwin& operator=(AutoEndwin&& other) noexcept;
    ~AutoEndwin();  // Call endwin unless released by NoAutoEndwin

    AutoEndwin& NoAutoEndwin() & { released_ = true; return *this; }
    AutoEndwin&& NoAutoEndwin() && { released_ = true; return std::move(*this); }

private:
    bool released_ = false;
};

// curs_initscr

[[nodiscard]] AutoEndwin Initscr();
Result Endwin();
bool Isendwin();

int Lines();
int Cols();

// curs_inopts

Result Cbreak(bool enable = true);
Result Nocbreak();

Result Echo(bool enable = true);
Result Noecho();

Result Nl(bool enable = true);
Result Nonl();

Result Raw(bool enable = true);
Result Noraw();

void Qiflush(bool enable = true);
void Noqiflush();

Result Halfdelay(int tenths);

Result Typeahead(int fd);

Result Meta(bool enable = true);

// curs_util

std::string Unctrl(Chtype ch);
std::string Keyname(int key);

// curs_kernel

CursorVisibility CursSet(CursorVisibility vis);

// curs_color

bool HasColors();
bool CanChangeColor();

Result StartColor();

int Colors();
int ColorPairs();

Result InitPair(int pair_number, ColorPairFgBg fg_bg);
ColorPairFgBg PairContent(int pair_number);

Result InitColor(Color color, ColorRgb rgb);
ColorRgb ColorContent(Color color);

// curs_refresh

Result Doupdate();

// curs_getch

Result Ungetch(int ch);
bool HasKey(int ch);

// curs_beep

Result Beep();
Result Flash();

class Window
{
public:
    friend void swap(Window& a, Window& b); // NOLINT: lower_case

    Window() = default;
    Window(const Window& other);
    Window& operator=(const Window& other);
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;
    ~Window();

    explicit Window(SizeLinesCols lines_cols, PosYx top_left = {});

    bool IsEmpty() const { return window_ == nullptr; }
    explicit operator bool() const { return !IsEmpty(); }

    const WINDOW* Get() const { return window_; }
    WINDOW* Get() { return window_; }

    const Window* GetParent() const { return parent_; }
    Window* GetParent() { return parent_; }

    // curs_window

    Window Subwin(SizeLinesCols lines_cols, PosYx top_left_on_screen);
    Window Derwin(SizeLinesCols lines_cols, PosYx top_left_in_parent);

    Result Mvwin(PosYx top_left);
    Result Mvderwin(PosYx viewed_top_left);

    Result Syncok(bool enable = true);
    void Syncup();
    void Cursyncup();
    void Syncdown();

    // curs_opaque

    bool IsCleared() const;
    bool IsIdcok() const;
    bool IsIdlok() const;
    bool IsImmedok() const;
    bool IsKeypad() const;
    bool IsLeaveok() const;
    bool IsNodelay() const;
    bool IsNotimeout() const;
    bool IsPad() const;
    bool IsScrollok() const;
    bool IsSubwin() const;
    bool IsSyncok() const;
    int Getdelay() const;

    // curs_inopts

    Result Keypad(bool enable = true);
    Result Nodelay(bool enable = true);
    Result Notimeout(bool enable = true);
    void Timeout(int delay_ms);

    // curs_outopts

    Result Clearok(bool enable = true);
    Result Idlok(bool enable = true);
    void Idcok(bool enable = true);
    void Immedok(bool enable = true);
    Result Leaveok(bool enable = true);
    Result Scrollok(bool enable = true);
    Result Setscrreg(int top, int bot);

    // curs_attr

    Result Attron(Attr attr);
    Result Attroff(Attr attr);
    Result Attrset(Attr attr);
    Result Colorset(int pair_number);

    Attr Attrget();

    Result Chgat(Attr attr);
    Result Chgat(int n, Attr attr);
    Result Chgat(PosYx yx, Attr attr);
    Result Chgat(PosYx yx, int n, Attr attr);

    // curs_bkgd

    Result Bkgd(Chtype ch);
    void Bkgdset(Chtype ch);
    Chtype Getbkgd();

    // curs_clear

    Result Erase();
    Result Clear();
    Result Clrtobot();
    Result Clrtoeol();

    // curs_refresh

    Result Refresh();
    Result Noutrefresh();

    Result Redrawwin();
    Result Redrawln(int beg_line, int num_lines);

    // curs_border

    Result Border(const BorderSides& sides, const BorderCorners& corners = {});
    Result Box(Chtype verch, Chtype horch) { return Border({verch, verch, horch, horch}); }
    Result Box(const BorderSides& sides = {}) { return Border(sides); }

    Result Hline(Chtype ch, int n);
    Result Hline(PosYx yx, Chtype ch, int n);

    Result Vline(Chtype ch, int n);
    Result Vline(PosYx yx, Chtype ch, int n);

    // curs_move

    Result Move(PosYx yx);

    // curs_getyx

    PosYx Getyx();
    PosYx Getparyx();
    PosYx Getbegyx();
    PosYx Getmaxyx();

    // curs_getch

    int Getch();
    int Getch(PosYx yx);

    // curs_getstr

    std::string Getstr(int maxlen = 1024);
    std::string Getstr(PosYx yx, int maxlen = 1024);

    // curs_addch

    Result Addch(Chtype ch);
    Result Addch(PosYx yx, Chtype ch);
    Result Echochar(Chtype ch);

    // curs_addchstr

    Result Addchstr(std::basic_string_view<Chtype> str);
    Result Addchstr(PosYx yx, std::basic_string_view<Chtype> str);

    // curs_addstr

    Result Addstr(std::string_view str);
    Result Addstr(PosYx yx, std::string_view str);

    // curs_insch

    Result Insch(Chtype ch);
    Result Insch(PosYx yx, Chtype ch);

    // curs_insstr

    Result Insstr(std::string_view str);
    Result Insstr(PosYx yx, std::string_view str);

    // curs_delch

    Result Delch();
    Result Delch(PosYx yx);

    // curs_inch

    Chtype Inch();
    Chtype Inch(PosYx yx);

    // curs_inchstr

    std::basic_string<Chtype> Inchstr(int maxlen = 1024);
    std::basic_string<Chtype> Inchstr(PosYx yx, int maxlen = 1024);

    // curs_instr

    std::string Instr(int maxlen = 1024);
    std::string Instr(PosYx yx, int maxlen = 1024);

    // curs_scroll

    Result Scroll(int n = 1);

    // curs_deleteln

    Result Deleteln();
    Result Insertln();
    Result Insdelln(int n);

private:
    Window SubwinImpl(
            SizeLinesCols lines_cols,
            PosYx top_left,
            const std::string& method); // subwin, derwin, subpad

    WINDOW* window_ = nullptr;
    Window* parent_ = nullptr;
};

// Window implementation

inline void swap(Window& a, Window& b)
{
    using std::swap;
    swap(a.window_, b.window_);
    swap(a.parent_, b.parent_);
}

inline Window::Window(Window&& other) noexcept :
    window_{other.window_}
{
    other.window_ = nullptr;
}

inline Window& Window::operator=(Window&& other) noexcept
{
    auto tmp = std::move(other);
    swap(*this, tmp);
    return *this;
}

namespace Key
{

enum : int
{
    Break = 0401,
    Down,
    Up,
    Left,
    Right,
    Home,
    Backspace,
    F0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    F26,
    F27,
    F28,
    F29,
    F30,
    F31,
    F32,
    F33,
    F34,
    F35,
    F36,
    F37,
    F38,
    F39,
    F40,
    F41,
    F42,
    F43,
    F44,
    F45,
    F46,
    F47,
    F48,
    F49,
    F50,
    F51,
    F52,
    F53,
    F54,
    F55,
    F56,
    F57,
    F58,
    F59,
    F60,
    F61,
    F62,
    F63,
    Dl,
    Il,
    Dc,
    Ic,
    Eic,
    Clear,
    Eos,
    Eol,
    Sf,
    Sr,
    Npage,
    Ppage,
    Stab,
    Ctab,
    Catab,
    Enter,
    Sreset,
    Reset,
    Print,
    //
    Ll,
    A1,
    A3,
    B2,
    C1,
    C3,
    Btab,
    Beg,
    Cancel,
    Close,
    Command,
    Copy,
    Create,
    End,
    Exit,
    Find,
    Help,
    Mark,
    Message,
    Move,
    Next,
    Open,
    Options,
    Previous,
    Redo,
    Reference,
    Refresh,
    Replace,
    Restart,
    Resume,
    Save,
    Sbeg,
    Scancel,
    Scommand,
    Scopy,
    Screate,
    Sdc,
    Sdl,
    Select,
    Send,
    Seol,
    Sexit,
    Sfind,
    Shelp,
    Shome,
    Sic,
    Sleft,
    Smessage,
    Smove,
    Snext,
    Soptions,
    Sprevious,
    Sprint,
    Sredo,
    Sreplace,
    Sright,
    Srsume,
    Ssave,
    Ssuspend,
    Sundo,
    Suspend,
    Undo,
    //
    Mouse,
    Resize,
}; // enum

} // namespace Key

namespace detail
{

constexpr unsigned ButtonReleased = 001U;
constexpr unsigned ButtonPressed  = 002U;
constexpr unsigned ButtonClicked  = 004U;
constexpr unsigned DoubleClicked  = 010U;
constexpr unsigned TripleClicked  = 020U;
constexpr unsigned ReservedEvent  = 040U;

constexpr unsigned MouseMask(int button, unsigned bit)
{
    return bit << ((button - 1) * 5);
}

} // namespace detail

enum class Mmask : unsigned
{
    Button1Pressed       = detail::MouseMask(1, detail::ButtonPressed),
    Button1Released      = detail::MouseMask(1, detail::ButtonReleased),
    Button1Clicked       = detail::MouseMask(1, detail::ButtonClicked),
    Button1DoubleClicked = detail::MouseMask(1, detail::DoubleClicked),
    Button1TripleClicked = detail::MouseMask(1, detail::TripleClicked),
    //
    Button2Pressed       = detail::MouseMask(2, detail::ButtonPressed),
    Button2Released      = detail::MouseMask(2, detail::ButtonReleased),
    Button2Clicked       = detail::MouseMask(2, detail::ButtonClicked),
    Button2DoubleClicked = detail::MouseMask(2, detail::DoubleClicked),
    Button2TripleClicked = detail::MouseMask(2, detail::TripleClicked),
    //
    Button3Pressed       = detail::MouseMask(3, detail::ButtonPressed),
    Button3Released      = detail::MouseMask(3, detail::ButtonReleased),
    Button3Clicked       = detail::MouseMask(3, detail::ButtonClicked),
    Button3DoubleClicked = detail::MouseMask(3, detail::DoubleClicked),
    Button3TripleClicked = detail::MouseMask(3, detail::TripleClicked),
    //
    Button4Pressed       = detail::MouseMask(4, detail::ButtonPressed),
    Button4Released      = detail::MouseMask(4, detail::ButtonReleased),
    Button4Clicked       = detail::MouseMask(4, detail::ButtonClicked),
    Button4DoubleClicked = detail::MouseMask(4, detail::DoubleClicked),
    Button4TripleClicked = detail::MouseMask(4, detail::TripleClicked),
    //
    Button5Pressed       = detail::MouseMask(5, detail::ButtonPressed),
    Button5Released      = detail::MouseMask(5, detail::ButtonReleased),
    Button5Clicked       = detail::MouseMask(5, detail::ButtonClicked),
    Button5DoubleClicked = detail::MouseMask(5, detail::DoubleClicked),
    Button5TripleClicked = detail::MouseMask(5, detail::TripleClicked),
    //
    ButtonShift          = detail::MouseMask(6, 002U),
    ButtonCtrl           = detail::MouseMask(6, 001U),
    ButtonAlt            = detail::MouseMask(6, 004U),
    ReportMousePosition  = detail::MouseMask(6, 010U),
    AllMouseEvents       = ReportMousePosition - 1,
};

} // namespace curses

#endif // Include guard
