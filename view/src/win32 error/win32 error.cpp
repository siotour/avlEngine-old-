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
Implementation for the win32 error component. See "win32 error.h" for details.
@author Sheldon Bachstein
@date Jun 11, 2012
*/

#include"win32 error.h"


namespace avl
{
namespace view
{
namespace win32
{
	// See method declaration for details.
	Win32Error::Win32Error(const std::string& description)
		: Exception(description)
	{
	}

	// See method declaration for details.
	Win32Error::Win32Error(const Win32Error& original)
		: Exception(original)
	{
	}

	// See method declaration for details.
	Win32Error::~Win32Error()
	{
	}


} // win32
} // view
} // avl
