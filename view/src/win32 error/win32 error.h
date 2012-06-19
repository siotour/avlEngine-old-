#pragma once
#ifndef AVL_VIEW_WIN32_ERROR__
#define AVL_VIEW_WIN32_ERROR__
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
Implements the \ref avl::view::win32::Win32Error class, which represents an
error or failure due to a win32 API function call.
@author Sheldon Bachstein
@date Jun 11, 2012
*/

#include"..\..\..\utility\src\exceptions\exceptions.h"
#include<string>

namespace avl
{
namespace view
{
namespace win32
{

	/**
	*/
	class Win32Error: public utility::Exception
	{
	public:
		/**
		*/
		Win32Error(const std::string& description);

		/**
		*/
		Win32Error(const Win32Error& original);

		/**
		*/
		virtual ~Win32Error();


	private:
		/// NOT IMPLEMENTED.
		const Win32Error& operator=(const Win32Error&);
	};



} // win32
} // view
} // avl
#endif AVL_VIEW_WIN32_ERROR__
