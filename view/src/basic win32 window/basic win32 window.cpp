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
Implementation for the basic win32 window component. See "basic win32 window.h" for details.
@author Sheldon Bachstein
@date February 03, 2010
*/

#include"basic win32 window.h"
#include"..\win32 wrapper\win32 wrapper.h"
#include"..\..\..\utility\src\assert\assert.h"



namespace avl
{
namespace view
{

	// See method declaration for details.
	BasicWin32Window::BasicWin32Window(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height)
	{
		// Attempt to register the window class.
		win32::RegisterWindowClass(instance_handle, window_title.c_str(), win32::RouteWindowMessage);
		// Attempt to create the window.
		window_handle = win32::MakeWindow(instance_handle, window_title.c_str(), width, height);
		// Attempt to attain a device context for this window.
		win32::AttainDeviceContext(window_handle);
		// The window is now ready.
		window_status = true;
		// Show the window.
		ShowWindow(window_handle, SW_SHOW);
		// Update the window for the first time.
		// TODO: Under what conditions would this function fail? Is an exception appropriate when it fails?
		UpdateWindow(window_handle);
	}

	// See method declaration for details.
	BasicWin32Window::~BasicWin32Window()
	{
		window_status = false;
		is_active = false;
		PostQuitMessage(0);
	}

	// See method declaration for details.
	const bool BasicWin32Window::Update()
	{
		ASSERT(window_handle != nullptr);
		ASSERT(device_context_handle != nullptr);
		// Loop through all of the messages in the message queue that are for this
		// window, removing each one and handling it appropriately.
		MSG message;
		while (PeekMessage(&message, window_handle, 0, 0, PM_REMOVE) != false)
		{
			// Handle each message type appropriately.
			switch (message.message)
			{
			case WM_CLOSE:
				// The user has requested the application to close. Destroy the window and make it known that it has been destroyed.
				window_status = false;
				DestroyWindow(window_handle);
				break;
			case WM_DESTROY:
				// NOTE: Attempts at posting WM_DESTROY to a window's message queue failed, so it's just going to be
				// handled in RouteWindowMessage().
				/*// The window has already been removed from the screen. Now's a good time to post the quit message.
				PostQuitMessage(0);
				break;*/
			case WM_QUIT:
				// The application has been requested to quit. Make it known that the window has been closed.
				window_status = false;
				break;
			case WM_ACTIVATE:
				// The window has been activated or deactivated. If the user specified that this window is to handle switching resolutions
				// for a fullscreen effect, now is the time to do so.
				
				// If the window has been activated, set is_active to true. If it's deactivated, set is_active to false.
				is_active = (LOWORD(message.wParam) != WA_INACTIVE);
				break;
			default:
				// For all other messages, translate them and then dispatch them to the window procedure.
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
		}
		// Return the status of the window (true means all is okay, false means the window has been closed).
		return window_status;
	}

	// See method declaration for details.
	const bool BasicWin32Window::IsActive()
	{
		return is_active;
	}

	// See method declaration for details.
	HWND BasicWin32Window::GetWindowHandle() const
	{
		return window_handle;
	}

	// See method declaration for details.
	HDC BasicWin32Window::GetDeviceContextHandle() const
	{
		return device_context_handle;
	}

}
}