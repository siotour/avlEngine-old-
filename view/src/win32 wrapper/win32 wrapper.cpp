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
Implementation for the win32 wrapper component. See "win32 wrapper.h" for details.
@author Sheldon Bachstein
@date Jun 11, 2012
*/

#include"win32 wrapper.h"
#include"..\win32 error\win32 error.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<string>
#include<windows.h>


namespace avl
{
namespace view
{
namespace win32
{
	// See method declaration for details.
	void RegisterWindowClass(HINSTANCE instance_handle, const std::string& window_title, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM))
	{
		// First, create and fill a WNDCLASSEX structure.
		WNDCLASSEX window_class;
		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		window_class.lpfnWndProc = window_procedure;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = instance_handle;
		window_class.hIcon = nullptr;
		window_class.hCursor = nullptr;
		window_class.hbrBackground = nullptr;
		window_class.lpszMenuName = nullptr;
		window_class.lpszClassName = window_title.c_str();
		window_class.hIconSm = nullptr;

		// Attempt to register the class.
		ATOM window_atom = RegisterClassEx(&window_class);
		// If registering the window class failed, throw a WindowCreationFailure.
		if (window_atom == 0)
		{
			std::string description = "Unable to register the window class titled " + window_title + ".";
			throw Win32Error(description);
		}
	}


	// See method declaration for details.
	HWND MakeWindow(HINSTANCE instance_handle, const std::string& window_title, const int width, const int height)
	{
		// Attempt to create the window.
		HWND window_handle = CreateWindowEx(WS_EX_LEFT, window_title.c_str(), window_title.c_str(), WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, instance_handle, nullptr);

		// Did we fail to create the window?
		if (window_handle == nullptr)
		{
			std::string description = "Unable to create the window titled " + window_title + ".";
			throw Win32Error(description);
		}
		return window_handle;
	}

	// See function declaration for details.
	HDC AttainDeviceContext(HWND window_handle)
	{
		if(window_handle == nullptr)
		{
			throw utility::InvalidArgumentException("avl::view::win32::AttainDeviceContext()", "window_handle", "window_handle must be non-nullptr");
		}
		// Attempt to get the device context handle.
		HDC device_context_handle = GetDC(window_handle);
		// If attempt failed, throw WindowCreationFailure.
		if (device_context_handle == nullptr)
		{
			throw Win32Error("Unable to attain the device context handle for a window.");
		}
		return device_context_handle;
	}


	// See function declaration for details.
	LRESULT CALLBACK RouteWindowMessage(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
	{
		// Different actions are necessary for different types of messages.
		switch(message)
		{
		// Messages which the window will handle entirely should be posted to that
		//window's message queue and success (zero) should be returned to windows.
		case WM_CLOSE:
			VERIFY(PostMessage(window_handle, message, w_param, l_param) == TRUE);
			return 0;
			break;
		case WM_DESTROY:
			// Attempts at posting WM_DESTROY to the message queue failed, so it
			// will be handled here. The application has already been requested to
			// close. Post a quit message.
			PostQuitMessage(0);
			return 0;
			break;
		// Messages which will be handled by Windows, but which the window needs to
		// know about, should be posted to the queue and then passed on to the
		// default window procedure.
		case WM_ACTIVATE:
			VERIFY(PostMessage(window_handle, message, w_param, l_param) == TRUE);
			// **Falls through.**
		// Messages handled entirely by Windows should be passed on to the default window procedure.
		default:
			return DefWindowProc(window_handle, message, w_param, l_param);
			break;
		}
	}



} // win32
} // view
} // avl
