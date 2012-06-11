#pragma once
#ifndef __AVL_INPUT_MOUSE_BUTTON_EVENT__
#define __AVL_INPUT_MOUSE_BUTTON_EVENT__
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
Implements the \ref avl::input::MouseButtonEvent.
@author Sheldon Bachstein
@date Sep 08, 2011
*/


#include"..\input event\input event.h"
#include"..\key codes\key codes.h"


namespace avl
{
namespace input
{
	/**
	Encapsulates an input event from the mouse
	buttons. Includes which button the event concerns, and whether the button
	was pressed or released.
	*/
	class MouseButtonEvent: public InputEvent
	{
	public:
		/** A constant used to initialize the InputEvent base class. This constant
		allows clients to identify an event as a MouseButtonEvent.*/
		static const unsigned char MOUSE_BUTTON_TYPE;


		
		/** Takes a button and whether the button was pressed or released.
		@param initial_button The button which was pressed or released.
		@param is_pressed True if \a initial_button was pressed, and false if it
		was released.
		*/
		MouseButtonEvent(const key_codes::MouseButton::MouseButtonCodes& initial_button, const bool is_pressed);
		/** Copy constructor.
		@param original The object being copied.
		*/
		MouseButtonEvent(const MouseButtonEvent& original);
		/** Basic destructor.*/
		~MouseButtonEvent();


		
		/** Returns the button which was pressed or released.
		@return The button which was pressed or released.
		*/
		const key_codes::MouseButton::MouseButtonCodes& GetButton() const;
		/** Returns true if the button was pressed, and false if it was released.
		@return True if \ref button was pressed, and false if it was released.
		*/
		const bool IsPressed() const;


	private:
		/// The button which was pressed/released.
		const key_codes::MouseButton::MouseButtonCodes button;
		/// Was \ref button pressed or released?
		const bool pressed;

	};





} //avl
} //input
#endif // __AVL_INPUT_MOUSE_BUTTON_EVENT__