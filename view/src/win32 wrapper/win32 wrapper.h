#pragma once
#ifndef AVL_VIEW_WIN32_WRAPPER__
#define AVL_VIEW_WIN32_WRAPPER__
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
Implements thin wrapper functions for performing common win32 API tasks.
@author Sheldon Bachstein
@date Jun 11, 2012
@todo Update the documentation for these functions.
*/

#include<string>
#include<windows.h>

namespace avl
{
namespace view
{
namespace win32
{
	/** Attempts to register a window class matching the arguments.
	@param instance_handle Handle to the application instance.
	@param window_title The name to register the window class with.
	@param window_procedure A pointer to the window procedure for this window.
	@throws Win32Error If unable to register the window class.
	@todo Clarify when this procedure throws an exception and define the pre-
	and post-conditions.
	@todo Implement icon, small icon, and cursor functionality.
	*/
	void RegisterWindowClass(HINSTANCE instance_handle, const std::string& window_title, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM));

	/** Attempts to create a window matching the specified arguments.
	@note "CreateWindow" is a macro, so that name can't be used.
	@param instance_handle Handle to the application instance.
	@param window_title The title of the window and name of the window class.
	@param width The width of the window's client area.
	@param height The height of the window's client area.
	@return The window handle to the newly created window.
	@throws Win32Error If unable to create the window.
	*/
	HWND MakeWindow(HINSTANCE instance_handle, const std::string& window_title, const int width, const int height);

	/** Attempts to attain a device context for the window with handle
	\a window_handle.
	@param window_handle The handle to the window we want a device context
	for.
	@return The device context handle for the window with handle
	\a window_handle.
	@throw utility::InvalidArgumentException If \a window_handle is nullptr.
	@throw Win32Error If unable to attain the device context.
	@todo When does this procedure throw any exceptions? What are the pre- and
	post-conditions?
	*/
	HDC AttainDeviceContext(HWND window_handle);

	/** A simple forwarding window procedure. With a few exceptions, all messages
	are simply fed to the default window procedure. The exceptions are:
	\li \c WM_CLOSE is posted to the window's message queue.
	\li \c WM_DESTROY results in calling \c PostQuitMessage(0).
	\li \c WM_ACTIVATE is first posted to the window's message queue, and then fed
	to the default window procedure.
	@param window_handle The handle of the window to which the message is addressed.
	@param msg The message type.
	@param w_param Message data.
	@param l_param Message data.
	@return 0 if the message is successfully handled, and non-zero if not.
	*/
	LRESULT CALLBACK RouteWindowMessage(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param);



} // win32
} // view
} // avl
#endif // AVL_VIEW_WIN32_WRAPPER__
