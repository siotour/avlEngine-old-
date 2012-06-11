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
Unit test for the mouse scroll event component. See "mouse scroll event.h" for details.
@author Sheldon Bachstein
@date September 08, 2011
*/

#include"mouse scroll event.h"



namespace avl
{
namespace input
{

	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseScrollEvent.
	const unsigned char MouseScrollEvent::MOUSE_SCROLL_TYPE = 0x04;


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


}
}