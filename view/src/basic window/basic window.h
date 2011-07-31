#ifndef __AVL_VIEW_BASIC_WINDOW__
#define __AVL_VIEW_BASIC_WINDOW__
/**********
 * Author: Sheldon Bachstein
 * Date: Feb 03, 2010
 * Description: Provides an implementation for a very basic Win32 window. All behavior is default. TODO: there is
 * a bug when creating multiple of these windows and then switching between them when they overlap (whether the
 * windows are in separate instances or a single instance). TODO: There is a bug where when you move the cursor
 * into the window's client area for the first time, it stays "busy" until you move it out of the client area.
 **********/

#include"..\win32 window base\win32 window base.h"
#include<string>
#include<windows.h>

namespace avl
{
namespace view
{
	// See the beginning of the file for details.
	class BasicWindow: public Win32WindowBase
	{
	public:
		// Constructors:
		// Attempts to construct a window with the specified parameters.
		BasicWindow(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height);
		~BasicWindow();
		
		// Manipulators:
		// Performs basic upkeep of the window. Returns true if everyting is as expected, false if the window has been closed.
		// Should be called often.
		const bool Update();
		// Returns true if the window is active, and false if it's not.
		const bool IsActive() const;
		// Window procedure for windows of this class. Deals with window messages.
		static LRESULT CALLBACK RouteWindowMessage(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param);

	private:
		// Status of the window. True if the window is running as expected, false if it has been closed.
		bool window_status;
		// Represents the window's active state. True if the window is active, false if it's inactive.
		bool is_active;

		// NOT IMPLEMENTED.
		BasicWindow(const BasicWindow&);
		const BasicWindow& operator=(const BasicWindow&);
	};





} //avl
} //view
#endif // __AVL_VIEW_BASIC_WINDOW__