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

using namespace curses;

static_assert(Key::Break     == KEY_BREAK);
static_assert(Key::Down      == KEY_DOWN);
static_assert(Key::Up        == KEY_UP);
static_assert(Key::Left      == KEY_LEFT);
static_assert(Key::Right     == KEY_RIGHT);
static_assert(Key::Home      == KEY_HOME);
static_assert(Key::Backspace == KEY_BACKSPACE);
static_assert(Key::F0        == KEY_F0);
static_assert(Key::F1        == KEY_F(1));
static_assert(Key::F2        == KEY_F(2));
static_assert(Key::F3        == KEY_F(3));
static_assert(Key::F4        == KEY_F(4));
static_assert(Key::F5        == KEY_F(5));
static_assert(Key::F6        == KEY_F(6));
static_assert(Key::F7        == KEY_F(7));
static_assert(Key::F8        == KEY_F(8));
static_assert(Key::F9        == KEY_F(9));
static_assert(Key::F10       == KEY_F(10));
static_assert(Key::F11       == KEY_F(11));
static_assert(Key::F12       == KEY_F(12));
static_assert(Key::F13       == KEY_F(13));
static_assert(Key::F14       == KEY_F(14));
static_assert(Key::F15       == KEY_F(15));
static_assert(Key::F16       == KEY_F(16));
static_assert(Key::F17       == KEY_F(17));
static_assert(Key::F18       == KEY_F(18));
static_assert(Key::F19       == KEY_F(19));
static_assert(Key::F20       == KEY_F(20));
static_assert(Key::F21       == KEY_F(21));
static_assert(Key::F22       == KEY_F(22));
static_assert(Key::F23       == KEY_F(23));
static_assert(Key::F24       == KEY_F(24));
static_assert(Key::F25       == KEY_F(25));
static_assert(Key::F26       == KEY_F(26));
static_assert(Key::F27       == KEY_F(27));
static_assert(Key::F28       == KEY_F(28));
static_assert(Key::F29       == KEY_F(29));
static_assert(Key::F30       == KEY_F(30));
static_assert(Key::F31       == KEY_F(31));
static_assert(Key::F32       == KEY_F(32));
static_assert(Key::F33       == KEY_F(33));
static_assert(Key::F34       == KEY_F(34));
static_assert(Key::F35       == KEY_F(35));
static_assert(Key::F36       == KEY_F(36));
static_assert(Key::F37       == KEY_F(37));
static_assert(Key::F38       == KEY_F(38));
static_assert(Key::F39       == KEY_F(39));
static_assert(Key::F40       == KEY_F(40));
static_assert(Key::F41       == KEY_F(41));
static_assert(Key::F42       == KEY_F(42));
static_assert(Key::F43       == KEY_F(43));
static_assert(Key::F44       == KEY_F(44));
static_assert(Key::F45       == KEY_F(45));
static_assert(Key::F46       == KEY_F(46));
static_assert(Key::F47       == KEY_F(47));
static_assert(Key::F48       == KEY_F(48));
static_assert(Key::F49       == KEY_F(49));
static_assert(Key::F50       == KEY_F(50));
static_assert(Key::F51       == KEY_F(51));
static_assert(Key::F52       == KEY_F(52));
static_assert(Key::F53       == KEY_F(53));
static_assert(Key::F54       == KEY_F(54));
static_assert(Key::F55       == KEY_F(55));
static_assert(Key::F56       == KEY_F(56));
static_assert(Key::F57       == KEY_F(57));
static_assert(Key::F58       == KEY_F(58));
static_assert(Key::F59       == KEY_F(59));
static_assert(Key::F60       == KEY_F(60));
static_assert(Key::F61       == KEY_F(61));
static_assert(Key::F62       == KEY_F(62));
static_assert(Key::F63       == KEY_F(63));
static_assert(Key::Dl        == KEY_DL);
static_assert(Key::Il        == KEY_IL);
static_assert(Key::Dc        == KEY_DC);
static_assert(Key::Ic        == KEY_IC);
static_assert(Key::Eic       == KEY_EIC);
static_assert(Key::Clear     == KEY_CLEAR);
static_assert(Key::Eos       == KEY_EOS);
static_assert(Key::Eol       == KEY_EOL);
static_assert(Key::Sf        == KEY_SF);
static_assert(Key::Sr        == KEY_SR);
static_assert(Key::Npage     == KEY_NPAGE);
static_assert(Key::Ppage     == KEY_PPAGE);
static_assert(Key::Stab      == KEY_STAB);
static_assert(Key::Ctab      == KEY_CTAB);
static_assert(Key::Catab     == KEY_CATAB);
static_assert(Key::Enter     == KEY_ENTER);
static_assert(Key::Sreset    == KEY_SRESET);
static_assert(Key::Reset     == KEY_RESET);
static_assert(Key::Print     == KEY_PRINT);
//
static_assert(Key::Ll        == KEY_LL);
static_assert(Key::A1        == KEY_A1);
static_assert(Key::A3        == KEY_A3);
static_assert(Key::B2        == KEY_B2);
static_assert(Key::C1        == KEY_C1);
static_assert(Key::C3        == KEY_C3);
static_assert(Key::Btab      == KEY_BTAB);
static_assert(Key::Beg       == KEY_BEG);
static_assert(Key::Cancel    == KEY_CANCEL);
static_assert(Key::Close     == KEY_CLOSE);
static_assert(Key::Command   == KEY_COMMAND);
static_assert(Key::Copy      == KEY_COPY);
static_assert(Key::Create    == KEY_CREATE);
static_assert(Key::End       == KEY_END);
static_assert(Key::Exit      == KEY_EXIT);
static_assert(Key::Find      == KEY_FIND);
static_assert(Key::Help      == KEY_HELP);
static_assert(Key::Mark      == KEY_MARK);
static_assert(Key::Message   == KEY_MESSAGE);
static_assert(Key::Move      == KEY_MOVE);
static_assert(Key::Next      == KEY_NEXT);
static_assert(Key::Open      == KEY_OPEN);
static_assert(Key::Options   == KEY_OPTIONS);
static_assert(Key::Previous  == KEY_PREVIOUS);
static_assert(Key::Redo      == KEY_REDO);
static_assert(Key::Reference == KEY_REFERENCE);
static_assert(Key::Refresh   == KEY_REFRESH);
static_assert(Key::Replace   == KEY_REPLACE);
static_assert(Key::Restart   == KEY_RESTART);
static_assert(Key::Resume    == KEY_RESUME);
static_assert(Key::Save      == KEY_SAVE);
static_assert(Key::Sbeg      == KEY_SBEG);
static_assert(Key::Scancel   == KEY_SCANCEL);
static_assert(Key::Scommand  == KEY_SCOMMAND);
static_assert(Key::Scopy     == KEY_SCOPY);
static_assert(Key::Screate   == KEY_SCREATE);
static_assert(Key::Sdc       == KEY_SDC);
static_assert(Key::Sdl       == KEY_SDL);
static_assert(Key::Select    == KEY_SELECT);
static_assert(Key::Send      == KEY_SEND);
static_assert(Key::Seol      == KEY_SEOL);
static_assert(Key::Sexit     == KEY_SEXIT);
static_assert(Key::Sfind     == KEY_SFIND);
static_assert(Key::Shelp     == KEY_SHELP);
static_assert(Key::Shome     == KEY_SHOME);
static_assert(Key::Sic       == KEY_SIC);
static_assert(Key::Sleft     == KEY_SLEFT);
static_assert(Key::Smessage  == KEY_SMESSAGE);
static_assert(Key::Smove     == KEY_SMOVE);
static_assert(Key::Snext     == KEY_SNEXT);
static_assert(Key::Soptions  == KEY_SOPTIONS);
static_assert(Key::Sprevious == KEY_SPREVIOUS);
static_assert(Key::Sprint    == KEY_SPRINT);
static_assert(Key::Sredo     == KEY_SREDO);
static_assert(Key::Sreplace  == KEY_SREPLACE);
static_assert(Key::Sright    == KEY_SRIGHT);
static_assert(Key::Srsume    == KEY_SRSUME);
static_assert(Key::Ssave     == KEY_SSAVE);
static_assert(Key::Ssuspend  == KEY_SSUSPEND);
static_assert(Key::Sundo     == KEY_SUNDO);
static_assert(Key::Suspend   == KEY_SUSPEND);
static_assert(Key::Undo      == KEY_UNDO);
//
static_assert(Key::Mouse     == KEY_MOUSE);
static_assert(Key::Resize    == KEY_RESIZE);
