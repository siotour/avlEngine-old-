#pragma once
#ifndef __AVL_INPUT_MOUSE_MOVE_EVENT__
#define __AVL_INPUT_MOUSE_MOVE_EVENT__
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
Implements the \ref avl::input::MouseMoveEvent.
@author Sheldon Bachstein
@date Sep 08, 2011
*/


#include"..\input event\input event.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"


namespace avl
{
namespace input
{
	/**
	Encapsulates a movement input event from a mouse device.
	Includes the change in x and y coordinates.
	*/
	class MouseMoveEvent: public InputEvent
	{
	public:
		/** A constant used to initialize the InputEvent base class. This constant
		allows clients to identify an event as a MouseMoveEvent.*/
		static const unsigned char MOUSE_MOVE_TYPE;



		/** Constructs a new event where \a x and \a y are the movement along the
		vertical and horizontal axes.
		@param x Movement along the vertical axis.
		@param y Movement along the horizontal axis.
		*/
		MouseMoveEvent(const short& x, const short& y);

		/** Copy constructor.
		@param original The object being copied.
		*/

		MouseMoveEvent(const MouseMoveEvent& original);
		/** Basic  destructor.*/
		~MouseMoveEvent();

		

		/** Returns the change in x coordinates.
		@return The mouse movement along the vertical axis.
		*/
		const short GetDeltaX() const;
		/** Returns the change in y coordinates.
		@return The mouse movement along the horizontal axis.
		*/
		const short GetDeltaY() const;


	private:
		/// The mouse movement along the horizontal axis.
		const short delta_x;
		/// The mouse movement along the vertical axis.
		const short delta_y;


	};



} //avl
} //input
#endif // __AVL_INPUT_MOUSE_MOVE_EVENT__