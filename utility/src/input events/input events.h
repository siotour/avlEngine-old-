#pragma once
#ifndef AVL_UTILITY_INPUT_EVENTS__
#define AVL_UTILITY_INPUT_EVENTS__
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
Defines the InputQueue type for storing InputEvent objects, and
the InputEvent hierarchy of classes.
@author Sheldon Bachstein
@date Jun 27, 2012
*/

#include"..\key codes\key codes.h"
#include"..\polymorphic queue\polymorphic queue.h"

namespace avl
{
namespace utility
{
namespace input_events
{
	// Forward declaration.
	class InputEvent;

	typedef PolymorphicQueue<const InputEvent> InputQueue;

	/**
	Acts as a base class for all input events. Derived classes must
	define a literal by which to identify that class.
	@todo Try to find a better way to differentiate different input event types. Is
	RTTI appropriate? The use of a constant for each derived class is less than ideal.
	*/
	class InputEvent
	{
	public:
		/** Takes an integer constant which is used by clients to identify the type
		of input event this is.
		@param initial_type A literal used to identify different input event types.
		Derived classes should define a new one.
		*/
		InputEvent(const unsigned char& initial_type);
		/** Copy constructor.
		@param original The object being copied.
		*/
		InputEvent(const InputEvent& original);
		/** Basic destructor.*/
		virtual ~InputEvent();

		/** Used to identify what type of event this is.
		@return A literal used to differentiate different input event types.
		*/
		const unsigned char& GetType() const;



	private:
		/// Identifies what type of input event this is.
		const unsigned char type;
	};


	/**
	Encapsulates a single input event from
	the keyboard. Includes the key which the event concerns, and whether
	that key was pressed or released.
	*/
	class KeyboardEvent: public InputEvent
	{
	public:
		/** This constant is used to initialize the InputEvent base class. It can be used
		to identify KeyboardEvent objects.*/
		static const unsigned char KEYBOARD_TYPE;

		/** Takes a key which this event concerns, and a bool indicated of whether it was
		pressed or released.
		@param initial_key The key which has been pressed or released.
		@param pressed True if \a initialKey was pressed, or false if it was released.
		*/
		KeyboardEvent(const key_codes::KeyboardKey::KeyboardKeyCodes& initial_key, const bool pressed);
		/** Copy constructor.
		@param original The object being copied.
		*/
		KeyboardEvent(const KeyboardEvent& original);
		/** Basic destructor.*/
		~KeyboardEvent();


		
		/** Returns the key which was pressed or released.
		@return The key which was pressed or released.
		*/
		const key_codes::KeyboardKey::KeyboardKeyCodes& GetKey() const;
		/** Returns true if the key was pressed, and false if it was released.
		@return True if the key was pressed, and false if it was released.
		*/
		const bool IsPressed() const;


	private:
		/// The key which was pressed/released.
		const key_codes::KeyboardKey::KeyboardKeyCodes key;
		/// Was \ref key pressed or released?
		const bool is_pressed;

	};


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



} // input_events
} // utility
} // avl
#endif // AVL_UTILITY_INPUT_EVENTS__
