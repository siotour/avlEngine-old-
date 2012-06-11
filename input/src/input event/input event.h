#pragma once
#ifndef __AVL_INPUT_INPUT_EVENT__
#define __AVL_INPUT_INPUT_EVENT__
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
Defines the InputEvent class, which is the base class for all input events.
@author Sheldon Bachstein
@date Sep 08, 2011
@todo Find a better way for derived classes to be identified. If necessary, use RTTI or the template
magic used in previous engine.
*/



namespace avl
{
namespace input
{
	/** Acts as a base class for all input events. Derived classes must
	define a literal by which to identify that class.
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




} //avl
} //input
#endif // __AVL_INPUT_INPUT_EVENT__