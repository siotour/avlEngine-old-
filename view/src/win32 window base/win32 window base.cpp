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
Unit test for the win32 window base component. See "win32 window base.h" for details.
@author Sheldon Bachstein
@date February 01, 2010
@todo Make this component at least minimally exception safety, preferably commit-or-rollback
safe (see Wikipedia: Exception Safety).
@todo Figure out why the program locks up when creating multiple windows and then switching between them
when they are overlapped (I think the fact that they are registered with the same class name contributes).
*/

#include<windows.h>
#include"win32 window base.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<string>
#include<map>




namespace avl
{
namespace view
{


	// Attempts to create a window with the specified parameters. instance_handle should be the handle to the process instance. Window title 
	// will be the name used to register the window class and will show up in the taskbar icon. width and height describe the dimensions of 
	// the window. window_procedure should be a pointer to the window proc function for this window. If registering the window class fails,
	// a WindowCreationFailure will be thrown with 
	// avl::utility::WindowCreationFailure::Type::CLASS_REGISTRATION. If creating the window fails, a WindowCreationFailure will be thrown with
	// avl::utility::WindowCreationFailure::Type::WINDOW_CREATION. If unable to attain a device context for this window, a WindowCreationFailure
	// will be thrown with avl::utility::WindowCreationFailure::Type::DEVICE_CONTEXT.
	Win32WindowBase::Win32WindowBase(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM))
		: window_handle(NULL), device_context_handle(NULL)
	{
		// Attempt to register the window class.
		RegisterWindowClass(instance_handle, window_title.c_str(), window_procedure);

		// Attempt to create the window.
		MakeWindow(instance_handle, window_title.c_str(), width, height);

		// Attempt to attain a device context for this window.
		AttainDeviceContext();

		// Show the window.
		ShowWindow(window_handle, SW_SHOW);

		// Update the window for the first time.
		// TODO: Under what conditions would this function fail? Is an exception appropriate when it fails?
		UpdateWindow(window_handle);
	}




	// Releases the window's device context, destroys the window, and invalidates its window handle.
	Win32WindowBase::~Win32WindowBase()
	{
		// TODO: All of this code should be in a protected member function which is called in the
		// destructor and before any exceptions are thrown due to problems creating the window. This
		// is because if such an exception is thrown, the destructor for this window may very well
		// never be called.
		// Release the device context.
		// TODO: Why does this assertion fail when the window is destroyed with a destructor...?
		ASSERT(device_context_handle != NULL);
		ReleaseDC(window_handle, device_context_handle);

		// TODO: Is this necessary? It wasn't in the old version, but makes sense.
		// Destroy the window.
		ASSERT(window_handle != NULL);
		DestroyWindow(window_handle);
	
		// Invalidate the window handle and device context.
		window_handle = NULL;
		device_context_handle = NULL;
	}




	// Returns the window handle.
	HWND Win32WindowBase::GetWindowHandle() const
	{
		return window_handle;
	}





	// Returns the device context handle.
	HDC Win32WindowBase::GetDeviceContextHandle() const
	{
		return device_context_handle;
	}






	// First creates and fills a WNDCLASSEX structure. Then attempts to register that window class. If registration fails,
	// throws a WindowCreationFailure of Type WindowCreationFailure::Type::CLASS_REGISTRATION.
	void Win32WindowBase::RegisterWindowClass(HINSTANCE instance_handle, const char *const& window_title, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM))
	{
		// First, create and fill a WNDCLASSEX structure.
		WNDCLASSEX window_class;
		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		window_class.lpfnWndProc = window_procedure;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = instance_handle;
		// TODO: Implement icon, small icon, and cursor functionality.
		window_class.hIcon = NULL;
		window_class.hCursor = NULL;
		window_class.hbrBackground = NULL;
		window_class.lpszMenuName = NULL;
		window_class.lpszClassName = window_title;
		window_class.hIconSm = NULL;

		// Attempt to register the class.
		ATOM window_atom = RegisterClassEx(&window_class);

		// If registering the window class failed, throw a WindowCreationFailure.
		if (window_atom == 0)
		{
			throw WindowCreationFailure(WindowCreationFailure::CLASS_REGISTRATION);
		}
	}




	// Attempts to create a window belonging to the process instance instance_handle with the title window_title, the width and height of width 
	// and height, and with x- and y-positions at default. If the attempt fails, throws a WindowCreationFailure of Type
	// WindowCreationFailure::Type::WINDOW_CREATION.
	void Win32WindowBase::MakeWindow(HINSTANCE instance_handle, const char* const& window_title, const int width, const int height)
	{
		// Attempt to create the window.
		window_handle = CreateWindowEx(WS_EX_LEFT, window_title, window_title, WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, instance_handle, NULL);

		// If creating the window failed, throw a WindowCreationFailure.
		if (window_handle == NULL)
		{
			throw WindowCreationFailure(WindowCreationFailure::WINDOW_CREATION);
		}
	}




	// Attempts to attain a device context for this window. If this fails, throws a WindowCreationFailure with Type
	// WindowCreationFailure::Type::DEVICE_CONTEXT.
	void Win32WindowBase::AttainDeviceContext()
	{
		// Attempt to get the device context handle.
		device_context_handle = GetDC(window_handle);

		// If attempt failed, throw WindowCreationFailure.
		if (device_context_handle == NULL)
		{
			throw WindowCreationFailure(WindowCreationFailure::DEVICE_CONTEXT);
		}
	}




	// Constructor for WindowCreationFailure.
	WindowCreationFailure::WindowCreationFailure(const WindowCreationFailure::Type& type)
		: type(type)
	{
		// Set the proper description depending on the error type.
		description = "Unable to create a Win32 Window: ";
		switch(type)
		{
		case CLASS_REGISTRATION:
			description += "Registering the class failed.";
			break;
		case WINDOW_CREATION:
			description += "Window creation failed.";
			break;
		case DEVICE_CONTEXT:
			description += "Unable to obtain a device context.";
			break;
		}
	}




	// Destructor for WindowCreationFailure.
	WindowCreationFailure::~WindowCreationFailure()
	{
	}




	// Copy constructor. Required so that WindowCreationFailures can be thrown by value.
	WindowCreationFailure::WindowCreationFailure(const WindowCreationFailure& original)
		: Exception(original), type(original.GetType())
	{
	}




	// Returns the type of window creation failure which occurred.
	const WindowCreationFailure::Type& WindowCreationFailure::GetType() const
	{
		return type;
	}




}
}