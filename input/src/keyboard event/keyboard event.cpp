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
Unit test for the keyboard event component. See "keyboard event.h" for details.
@author Sheldon Bachstein
@date Sep 08, 2011
*/

#include"keyboard event.h"
#include"..\input event\input event.h"
#include"..\key codes\key codes.h"


namespace avl
{
namespace input
{
	// This constant is used to initialize the InputEvent base class. It can be used
	// to identify KeyboardEvent objects.
	const unsigned char KeyboardEvent::KEYBOARD_TYPE = 0x01;




	// Takes a key which this event concerns, and a bool indicated whether it was
	// pressed or released.
	KeyboardEvent::KeyboardEvent(const key_codes::KeyboardKey::KeyboardKeyCodes& initial_key, const bool pressed)
		: InputEvent(KEYBOARD_TYPE), key(initial_key), is_pressed(pressed)
	{
	}




	// Copy constructor.
	KeyboardEvent::KeyboardEvent(const KeyboardEvent& original)
		: InputEvent(original.GetType()), key(original.GetKey()), is_pressed(original.IsPressed())
	{
	}




	// Destructor.
	KeyboardEvent::~KeyboardEvent()
	{
	}




	// Returns the key which was pressed.
	const key_codes::KeyboardKey::KeyboardKeyCodes& KeyboardEvent::GetKey() const
	{
		return key;
	}




	// Returns true if the key was pressed, and false if it was released.
	const bool KeyboardEvent::IsPressed() const
	{
		return is_pressed;
	}



}
}