#pragma once
#ifndef AVL_INPUT_INPUT_DEVICE__
#define AVL_INPUT_INPUT_DEVICE__
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
Provides a standard interface for input classes via the \ref avl::input::InputDevice
abstract class.
@author Sheldon Bachstein
@date Jun 12, 2012
@todo Document this component.
*/

#include"..\..\..\utility\src\input events\input events.h"
#include<queue>

namespace avl
{
namespace input
{

	/** 
	*/
	class InputDevice
	{
	public:
		/** Basic constructor.*/
		InputDevice();

		/** Basic destructor.*/
		virtual ~InputDevice();

		/** Returns 
		*/
		virtual utility::input_events::InputQueue GetInput() = 0;

	};



} // input
} // avl
#endif // AVL_INPUT_INPUT_DEVICE__
