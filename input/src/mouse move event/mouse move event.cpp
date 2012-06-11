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
/*
@file
Unit test for the mouse move event component. See "mouse move event.h" for details.
@author Sheldon Bachstein
@date Sep 08, 2011
*/

#include"mouse move event.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"


namespace avl
{
namespace input
{

	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseMoveEvent.
	const unsigned char MouseMoveEvent::MOUSE_MOVE_TYPE = 0x03;




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


}
}