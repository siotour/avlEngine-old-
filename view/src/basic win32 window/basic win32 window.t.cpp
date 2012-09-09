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
Unit test for the basic win32 window component. See "basic win32 window.h" for details.
@author Sheldon Bachstein
@date February 03, 2010
*/

#include"basic win32 window.h"


void TestBasicWin32WindowComponent(HINSTANCE hInstance)
{
	avl::view::BasicWin32Window basic_window(hInstance, "Basic Win32 Window Component Test", 500, 500);

	while(basic_window.Update() == true)
	{
	}
}