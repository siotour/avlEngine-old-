#pragma once
#ifndef AVL_VIEW_WINDOW__
#define AVL_VIEW_WINDOW__
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
Provides a standard interface for window classes via the \ref avl::view::Window
abstract class.
@author Sheldon Bachstein
@date Jun 11, 2012
*/



namespace avl
{
namespace view
{

	/**
	Defines a basic interface for window functionality.
	*/
	class Window
	{
	public:
		/** Basic constructor.*/
		Window();

		/** Basic destructor.*/
		virtual ~Window();

		/** Meant to be called at least once per frame and should maintain
		the window.
		@return True if the window is healthy, and false if it's not. False
		means that this window needs to be destroyed.
		*/
		virtual const bool Update() = 0;

		/** Used to check if the window has focus or not.
		@return True if the window is active, and false if not.
		*/
		virtual const bool IsActive() = 0;

	private:

		/// NOT IMPLEMENTED.
		Window(const Window&);
		/// NOT IMPLEMENTED.
		const Window& operator=(const Window&);
	};



} // view
} // avl
#endif // AVL_VIEW_WINDOW__
