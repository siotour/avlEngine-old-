#pragma once
#ifndef __AVL_INPUT_KEY_CODES__
#define __AVL_INPUT_KEY_CODES__
/* Copyright 2012 Sheldon Bachstein
This file is part of the avl Library.

The avl Library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The avl Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the avl Library.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
@file
Defines enumerations for input key/button codes. There are separate
enumerations for mouse buttons, keyboard modifier keys (capslock, shift, ctrl, etc.),
and keyboard keys.
@author Sheldon Bachstein
@date Sep 03, 2011
*/




namespace avl
{
namespace input
{
namespace key_codes
{


	/** Houses the MouseButtonCodes enumeration values.*/
	struct MouseButton
	{
	/** Identifies which mouse button has been pressed or released.*/
	enum MouseButtonCodes
	{
		mb_null				= 0,	/**< No button or an unrecognized button.*/
		mb_left				= 1,	/**< Left/primary mouse button.*/
		mb_right			= 2,	/**< Right/secondary mouse button.*/
		mb_middle			= 3,	/**< Middle mouse button.*/
	};
	};


	/** Houses the KeyboardModifierCodes enumeration values.*/
	struct KeyboardModifier
	{
	/** Mutually inclusive keyboard modifier identifiers. Any number of these may
	be set within a bit field.*/
	enum KeyboardModifierCodes
	{
		km_none				= 0,	/**< None/unrecognized.*/
		km_leftshift		= 1,	/**< Left shift.*/
		km_rightshift		= 2,	/**< Right shift.*/
		km_leftctrl			= 4,	/**< Left control.*/
		km_rightctrl		= 8,	/**< Right control.*/
		km_leftalt			= 16,	/**< Left alt.*/
		km_rightalt			= 32,	/**< Right alt.*/
		km_numlock			= 64,	/**< Number lock.*/
		km_capslock			= 128	/**< Caps lock.*/
	};
	};


	/** Houses the KeyboardKeyCodes enumeration.*/
	struct KeyboardKey
	{
	/** Identifiers for all of the standard keyboard keys.
	@note Only the unshifted versions of keys are represented. If you want to detect the
	activated of a shifted key (such as asterisk, double quote, or a curly brace), then
	you need to check for one of the shift modifier keys.
	@sa \ref KeyboardModifier::KeyboardModifierCodes
	*/
	enum KeyboardKeyCodes
	{
		kk_null				= 0,	/**< None/unrecognized.*/

		// Numbers:
		kk_0				= 1,	/**< Zero.*/
		kk_1				= 2,	/**< One.*/
		kk_2				= 3,	/**< Two.*/
		kk_3				= 4,	/**< Three.*/
		kk_4				= 5,	/**< Four.*/
		kk_5				= 6,	/**< Five.*/
		kk_6				= 7,	/**< Six.*/
		kk_7				= 8,	/**< Seven.*/
		kk_8				= 9,	/**< Eight.*/
		kk_9				= 10,	/**< Nine.*/
		
		// Letters:
		kk_a				= 11,	/**< a.*/
		kk_b				= 12,	/**< b.*/
		kk_c				= 13,	/**< c.*/
		kk_d				= 14,	/**< d.*/
		kk_e				= 15,	/**< e.*/
		kk_f				= 16,	/**< f.*/
		kk_g				= 17,	/**< g.*/
		kk_h				= 18,	/**< h.*/
		kk_i				= 19,	/**< i.*/
		kk_j				= 20,	/**< j.*/
		kk_k				= 21,	/**< k.*/
		kk_l				= 22,	/**< l.*/
		kk_m				= 23,	/**< m.*/
		kk_n				= 24,	/**< n.*/
		kk_o				= 25,	/**< o.*/
		kk_p				= 26,	/**< p.*/
		kk_q				= 27,	/**< q.*/
		kk_r				= 28,	/**< r.*/
		kk_s				= 29,	/**< s.*/
		kk_t				= 30,	/**< t.*/
		kk_u				= 31,	/**< u.*/
		kk_v				= 32,	/**< v.*/
		kk_w				= 33,	/**< w.*/
		kk_x				= 34,	/**< x.*/
		kk_y				= 35,	/**< y.*/
		kk_z				= 36,	/**< z.*/

		// Punctuation:
		kk_comma			= 37,	/**< ,*/
		kk_period			= 38,	/**< Period.*/
		kk_semicolon		= 39,	/**< ;*/
		kk_apostrophe		= 40,	/**< '*/
		kk_accent			= 41,	/**< ` (Left of the '1' key).*/

		// Control characters:
		kk_space			= 42,	/**< Space bar.*/
		kk_tab				= 43,	/**< Tab.*/
		kk_backspace		= 44,	/**< Backspace.*/
		kk_enter			= 45,	/**< Enter/return.*/
		kk_escape			= 46,	/**< Escape.*/

		// Home/end cluster:
		kk_insert			= 47,	/**< Insert.*/
		kk_delete			= 48,	/**< Delete.*/
		kk_home				= 49,	/**< Home.*/
		kk_end				= 50,	/**< End.*/
		kk_pageup			= 51,	/**< Page Up.*/
		kk_pagedown			= 52,	/**< Page Down.*/
		
		// Symbols:
		kk_minus			= 53,	/**< '-'*/
		kk_slash			= 54,	/**< /*/
		kk_equals			= 55,	/**< =*/
		kk_backslash		= 56,	/**< '\'*/
		kk_leftbracket		= 57,	/**< [*/
		kk_rightbracket		= 58,	/**< ]*/

		// Arrows.
		kk_up				= 59,	/**< Up arrow.*/
		kk_down				= 60,	/**< Down arrow.*/
		kk_right			= 61,	/**< Right arrow.*/
		kk_left				= 62,	/**< Left arrow.*/
	
		// Function keys
		kk_f1				= 63,	/**< F1.*/
		kk_f2				= 64,	/**< F2.*/
		kk_f3				= 65,	/**< F3.*/
		kk_f4				= 66,	/**< F4.*/
		kk_f5				= 67,	/**< F5.*/
		kk_f6				= 68,	/**< F6.*/
		kk_f7				= 69,	/**< F7.*/
		kk_f8				= 70,	/**< F8.*/
		kk_f9				= 71,	/**< F9.*/
		kk_f10				= 72,	/**< F10.*/
		kk_f11				= 73,	/**< F11.*/
		kk_f12				= 74,	/**< F12.*/

		// Modifier keys.
		kk_numlock			= 78,	/**< Number lock key.*/
		kk_capslock			= 79,	/**< Caps lock key.*/
		kk_scrolllock		= 80,	/**< Scroll lock key.*/
		kk_rightshift		= 81,	/**< Right shift key.*/
		kk_leftshift		= 81,	/**< Left shift key.*/
		kk_rightctrl		= 83,	/**< Right control key.*/
		kk_leftctrl			= 84,	/**< Left control key.*/
		kk_rightalt			= 85,	/**< Right alt key.*/
		kk_leftalt			= 86,	/**< Left alt key.*/
		kk_rightwindow		= 87,	/**< Right Windows key.*/
		kk_leftwindow		= 88,	/**< Left Windows key.*/
			
		// Numpad
		kk_np_0				= 89,	/**< Number pad 0.*/
		kk_np_1				= 90,	/**< Number pad 1.*/
		kk_np_2				= 91,	/**< Number pad 2.*/
		kk_np_3				= 92,	/**< Number pad 3.*/
		kk_np_4				= 93,	/**< Number pad 4.*/
		kk_np_5				= 94,	/**< Number pad 5.*/
		kk_np_6				= 95,	/**< Number pad 6.*/
		kk_np_7				= 96,	/**< Number pad 7.*/
		kk_np_8				= 97,	/**< Number pad 8.*/
		kk_np_9				= 98,	/**< Number pad 9.*/
		kk_np_period		= 99,	/**< Number pad .*/
		kk_np_slash			= 100,	/**< Number pad '/'*/
		kk_np_star			= 101,	/**< Number pad '*'.*/
		kk_np_minus			= 102,	/**< Number pad -*/
		kk_np_plus			= 103,	/**< Number pad +*/
		kk_np_enter			= 104,	/**< Number pad enter/return.*/
			
		// Misc.:
		kk_menu				= 105	/**< Menu button (between right alt and right control).*/
	};
	};

} // key_codes
} // input
} // avl
#endif // __AVL_INPUT_KEY_CODES__