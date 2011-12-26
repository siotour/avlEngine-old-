#ifndef __AVL_VIEW_BASIC_WINDOW__
#define __AVL_VIEW_BASIC_WINDOW__
/**
@file
Defines the \ref avl::view::BasicWindow class.
@author Sheldon Bachstein
@date Feb 03, 2010
@todo There is a bug when creating multiple of these windows and then switching between them when
they overlap (whether the windows are in separate instances or a single instance).
@todo There is a bug where when you move the cursor into the window's client area for the first
time, it stays "busy" until you move it out of the client area.
*/

#include"..\win32 window base\win32 window base.h"
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
	class BasicWindow: public Win32WindowBase
	{
	public:
		/** Attempts to construct a window with the specified parameters.
		@param instance_handle The handle to the application instance.
		@param window_title Appears in the window's title bar, task bar icon, and task manager.
		@param width The width of the window's client area.
		@param height The height of the window's client area.
		*/
		BasicWindow(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height);
		/** Basic destructor.*/
		~BasicWindow();
		
		
		/** Performs basic upkeep of the window. Should be called often.
		@return Returns true if the window is working properly, and false if the window has closed.
		*/
		const bool Update();

		/** Checks to see if the window is currently in focus.
		@return True if the window is in focus, and false if not.
		*/
		const bool IsActive() const;

		/** The window procedure for windows of this class. With a few exceptions, all messages are simply
		fed to the default window procedure. The exceptions are:
		\li \c WM_CLOSE is posted to the window's message queue.
		\li \c WM_DESTROY results in calling \c PostQuitMessage(0).
		\li \c WM_ACTIVATE is first posted to the window's message queue, and then fed to the default window
		procedure.
		@param window_handle The handle of the window to which the message is addressed.
		@param msg The message type.
		@param w_param Message data.
		@param l_param Message data.
		@return 0 if the message was handled correctly, and false if not.
		*/
		static LRESULT CALLBACK RouteWindowMessage(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param);

	private:
		/// Status of the window. True if the window is running as expected, false if it has been closed.
		bool window_status;
		/// Represents the window's active state. True if the window is active, false if it's inactive.
		bool is_active;

		/// NOT IMPLEMENTED.
		BasicWindow(const BasicWindow&);
		/// NOT IMPLEMENTED.
		const BasicWindow& operator=(const BasicWindow&);
	};





} //avl
} //view
#endif // __AVL_VIEW_BASIC_WINDOW__