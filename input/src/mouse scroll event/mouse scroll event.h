#pragma once
#ifndef __AVL_INPUT_MOUSE_SCROLL_EVENT__
#define __AVL_INPUT_MOUSE_SCROLL_EVENT__
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
Defines the \ref avl::input::MouseScrollEvent class.
@author: Sheldon Bachstein
@date: Sep 08, 2011
*/


#include"..\input event\input event.h"


namespace avl
{
namespace input
{
	/**
	Used to communicate the user scrolling the mouse wheel. This includes a
	signed integer representing the scroll amount and direction.
	*/
	class MouseScrollEvent: public InputEvent
	{
	public:
		/** A constant used to initialize the InputEvent base class. This constant
		allows clients to identify an event as a MouseScrollEvent.*/
		static const unsigned char MOUSE_SCROLL_TYPE;


		/** Takes a signed short representing the amount and direction of
		the scroll event.
		@param initial_delta The direction and amount in which the mouse
		wheel has been scrolled.
		*/
		MouseScrollEvent(const short& initial_delta);
		/** Copy constructor.
		@param original The object being copied.
		*/
		MouseScrollEvent(const MouseScrollEvent& original);
		/** Basic destructor.*/
		~MouseScrollEvent();


		
		/** Returns the delta scroll value.
		@return The direction and amount in which the mouse wheel has been
		scrolled.
		*/
		const short& GetDelta() const;


	private:
		/// The delta scroll value.
		const short delta;

	};




} //avl
} //input
#endif // __AVL_INPUT_MOUSE_SCROLL_EVENT__