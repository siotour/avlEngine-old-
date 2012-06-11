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
Unit test for the basic window component. See "basic window.h" for details.
@author Sheldon Bachstein
@date February 03, 2010
*/

#include"basic window.h"
#include"..\..\..\utility\src\assert\assert.h"



namespace avl
{
namespace view
{



	// Passes all parameters on to the Win32WindowBase constructor, in addition to the address of the
	// BasicWindow::RouteWindowsMessage function to be used as the window procedure.
	BasicWindow::BasicWindow(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height)
	: Win32WindowBase(instance_handle, window_title, width, height, &BasicWindow::RouteWindowMessage), window_status(true), is_active(false)
	{
	}




	// Makes sure that the window status is set to false.
	BasicWindow::~BasicWindow()
	{
		window_status = false;
		is_active = false;
	}




	// Processes any messages in the message queue.
	const bool BasicWindow::Update()
	{
		ASSERT(window_handle != NULL);
		ASSERT(device_context_handle != NULL);

		MSG message;

		// Loop through all of the messages in the message queue that are for this window, removing each one and handling it 
		// appropriately.
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
				if(LOWORD(message.wParam) == WA_INACTIVE)
				{
					is_active = false;
				}
				else
				{
					is_active = true;
				}
				break;
			default:
				// For all other messages, translate them and then dispatch them so that RouteWindowMessage can tell windows to deal with them.
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
		}

		// Return the status of the window (true means all is okay, false means the window has been closed).
		return window_status;
	}




	// Returns true if the window is active, and false if it's inactive.
	const bool BasicWindow::IsActive() const
	{
		return is_active;
	}




	// Static function used as a window procedure for all windows created using this class. Attempts to post unqueued window messages to
	// their windows' message queue. Windows sends some window messages through a window's window procedure and posts some directly to
	// a window's message queue. The idea here is that any messages sent to a window's window procedure will be posted to that window's
	// message queue instead, and that window can handle those messages when it has time.
	LRESULT CALLBACK BasicWindow::RouteWindowMessage(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
	{
		// Different actions are necessary for different types of messages.
		switch(message)
		{
		// Messages which the window will handle entirely should be posted to that window's message queue and success (zero)
		// should be returned to windows.
		case WM_CLOSE:
			VERIFY(PostMessage(window_handle, message, w_param, l_param) == TRUE);
			return 0;
			break;
		case WM_DESTROY:
			// Attempts at posting WM_DESTROY to the message queue failed, so it will be handled here.
			// The application has already been requested to close. Post a quit message.
			PostQuitMessage(0);
			return 0;
			break;
		// Messages which will be handled by Windows, but which the window needs to know about, should be posted to the queue and then passed
		// on to the default window procedure.
		case WM_ACTIVATE:
			VERIFY(PostMessage(window_handle, message, w_param, l_param) == TRUE);
			// **Falls through.**
		// Messages handled entirely by Windows should be passed on to the default window procedure.
		default:
			return DefWindowProc(window_handle, message, w_param, l_param);
			break;
		}
	}



}
}