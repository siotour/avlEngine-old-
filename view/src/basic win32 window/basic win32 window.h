#pragma once
#ifndef AVL_VIEW_BASIC_WIN32_WINDOW__
#define AVL_VIEW_BASIC_WIN32_WINDOW__
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
Defines the \ref avl::view::BasicWin32Window class.
@author Sheldon Bachstein
@date Feb 03, 2010
@todo There is a bug when creating multiple of these windows and then switching between them when
they overlap (whether the windows are in separate instances or a single instance).
@todo There is a bug where when you move the cursor into the window's client area for the first
time, it stays "busy" until you move it out of the client area.
*/

#include"..\window\window.h"
#include<string>
#include<windows.h>

namespace avl
{
namespace view
{
	/**
	Provides an implementation for a very basic Win32 window. All behavior is default.
	@todo Expand on 'default behavior.'
	*/
	class BasicWin32Window: public Window
	{
	public:
		/** Attempts to construct a window with the specified parameters.
		@param instance_handle The handle to the application instance.
		@param window_title Appears in the window's title bar, task bar icon, and task manager.
		@param width The width of the window's client area.
		@param height The height of the window's client area.
		*/
		BasicWin32Window(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height);
		/** Basic destructor.*/
		~BasicWin32Window();
		
		
		/** Performs basic upkeep of the window. Should be called often.
		@return Returns true if the window is working properly, and false if the window has closed.
		*/
		const bool Update();

		/** Checks to see if the window is currently in focus.
		@return True if the window is in focus, and false if not.
		*/
		const bool IsActive();
		/** Returns the window's handle.
		@return The window's handle.
		*/
		HWND GetWindowHandle() const;
		/** Returns the device context handle for this window.
		@return The handle to the device context created for this window.
		*/
		HDC GetDeviceContextHandle() const;

	private:
		/// Status of the window. True if the window is running as expected, false if it has been closed.
		bool window_status;
		/// Represents the window's active state. True if the window is active, false if it's inactive.
		bool is_active;

		/// The handle to the window.
		HWND window_handle;
		/// The handle to the device context for the window.
		HDC device_context_handle;

		/// NOT IMPLEMENTED.
		BasicWin32Window(const BasicWin32Window&);
		/// NOT IMPLEMENTED.
		const BasicWin32Window& operator=(const BasicWin32Window&);
	};





} //avl
} //view
#endif // AVL_VIEW_BASIC_WIN32_WINDOW__