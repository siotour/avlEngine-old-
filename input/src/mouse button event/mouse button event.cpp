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
Unit test for the mouse button event component. See "mouse button event.h" for details.
@author Sheldon Bachstein
@date Sep 08, 2011
*/

#include"mouse button event.h"
#include"..\key codes\key codes.h"




namespace avl
{
namespace input
{


	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseButtonEvent.
	const unsigned char MouseButtonEvent::MOUSE_BUTTON_TYPE = 0x02;


	// Constructors:
	// Takes a button and whether the button was pressed or released.
	MouseButtonEvent::MouseButtonEvent(const key_codes::MouseButton::MouseButtonCodes& initial_button, const bool is_pressed)
		: InputEvent(MOUSE_BUTTON_TYPE), button(initial_button), pressed(is_pressed)
	{
	}




	// Copy constructor.
	MouseButtonEvent::MouseButtonEvent(const MouseButtonEvent& original)
		: InputEvent(original.GetType()), button(original.GetButton()), pressed(original.IsPressed())
	{
	}




	// Destructor.
	MouseButtonEvent::~MouseButtonEvent()
	{
	}




	// Returns the button which was pressed.
	const key_codes::MouseButton::MouseButtonCodes& MouseButtonEvent::GetButton() const
	{
		return button;
	}




	// Returns true if the button was pressed, and false if it was released.
	const bool MouseButtonEvent::IsPressed() const
	{
		return pressed;
	}


}
}

