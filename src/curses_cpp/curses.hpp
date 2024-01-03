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
#include <type_traits>

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

} // namespace curses

#endif // Include guard
