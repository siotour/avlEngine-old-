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
Unit test for the input event component. See "d3d error.h" for details.
@author Sheldon Bachstein
@date September 08, 2011
*/

#include"input event.h"


namespace avl
{
namespace input
{
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



}
}