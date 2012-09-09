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
Implementation for the input events component. See "input events.h" for details.
@author Sheldon Bachstein
@date Jun 27, 2012
*/

#include"input events.h"
#include"..\key codes\key codes.h"
#include"..\exceptions\exceptions.h"
#include<list>


namespace avl
{
namespace utility
{
namespace input_events
{
	// This constant is used to initialize the InputEvent base class. It can be used
	// to identify KeyboardEvent objects.
	const unsigned char KeyboardEvent::KEYBOARD_TYPE = 0x01;


	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseButtonEvent.
	const unsigned char MouseButtonEvent::MOUSE_BUTTON_TYPE = 0x02;


	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseMoveEvent.
	const unsigned char MouseMoveEvent::MOUSE_MOVE_TYPE = 0x03;


	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseScrollEvent.
	const unsigned char MouseScrollEvent::MOUSE_SCROLL_TYPE = 0x04;




	// Takes an integer constant which is used by clients to identify the type
	// of input event this is.
	InputEvent::InputEvent(const unsigned char& initial_type)
		: type(initial_type)
	{
	}




	// Copy constructor.
	InputEvent::InputEvent(const InputEvent& original)
		: type(original.GetType())
	{
	}




	// Destructor.
	InputEvent::~InputEvent()
	{
	}




	// Returns the input type, which is to be defined by derived classes.
	const unsigned char& InputEvent::GetType() const
	{
		return type;
	}




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





	// Takes the individual vertical and horizontal movements.
	MouseMoveEvent::MouseMoveEvent(const short& x, const short& y)
		: InputEvent(MOUSE_MOVE_TYPE), delta_x(x), delta_y(y)
	{
	}




	// Copy constructor.
	MouseMoveEvent::MouseMoveEvent(const MouseMoveEvent& original)
		: InputEvent(original.GetType()), delta_x(original.GetDeltaX()), delta_y(original.GetDeltaY())
	{
	}




	// Destructor.
	MouseMoveEvent::~MouseMoveEvent()
	{
	}





	// Returns the change in x coordinates.
	const short MouseMoveEvent::GetDeltaX() const
	{
		return delta_x;
	}




	// Returns the change in y coordinates.
	const short MouseMoveEvent::GetDeltaY() const
	{
		return delta_y;
	}



	// Constructors:
	// Takes a signed short representing the amount and direction of the scroll
	// event.
	MouseScrollEvent::MouseScrollEvent(const short& initial_delta)
		: InputEvent(MOUSE_SCROLL_TYPE), delta(initial_delta)
	{
	}




	// Copy constructor.
	MouseScrollEvent::MouseScrollEvent(const MouseScrollEvent& original)
		: InputEvent(original.GetType()), delta(original.GetDelta())
	{
	}




	// Destructor.
	MouseScrollEvent::~MouseScrollEvent()
	{
	}




	// Returns the delta scroll value.
	const short& MouseScrollEvent::GetDelta() const
	{
		return delta;
	}




} // input_events
} // utility
} // avl
