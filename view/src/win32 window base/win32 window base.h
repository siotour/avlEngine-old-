#ifndef __AVL_VIEW_WIN32_WINDOW_BASE__
#define __AVL_VIEW_WIN32_WINDOW_BASE__
/**********
 * Author: Sheldon Bachstein
 * Date: Feb 01, 2010
 * Description: Provides an interface for a generic Win32 window, as well as some basic functionality. Win32WindowBase is a partially-
 * implemented skeleton used to build higher-level Win32 windows on. Also defines an exception class for dealing with exceptions during
 * window creation.
 **********/

#include<string>
#include<windows.h>
#include"..\..\..\utility\src\exception\exception.h"


namespace avl
{
namespace view
{
	// See the beginning of the file for details.
	class Win32WindowBase
	{
	public:
		// Constructors:
		// Attempts to create a window with the specified properties. If an error occurs, will throw a WindowCreationFailure.
		Win32WindowBase(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM));
		virtual ~Win32WindowBase();

		// Accessors:
		HWND GetWindowHandle() const;
		HDC GetDeviceContextHandle() const;

		// Manipulators:
		// General upkeep of the window. Should be called as often as possible. A return value of true means the window is functioning
		// normally, false means that the window has been closed.
		virtual const bool Update() = 0;
	protected:
		// Attempts to register a window class matching the parameters.
		void RegisterWindowClass(HINSTANCE instance_handle, const char* const& window_title, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM));
		// Attempts to create a window matching the parameters. Note: "CreateWindow" is a macro, so it can't be used as a name.
		void MakeWindow(HINSTANCE instance_handle, const char* const& window_title, const int width, const int height);
		// Attempts to attain a device context for this window.
		void AttainDeviceContext();

		// The handle to the window.
		HWND window_handle;
		// The handle to the device context for the window.
		HDC device_context_handle;

		// NOT IMPLEMENTED:
		Win32WindowBase(const avl::view::Win32WindowBase&);
		const avl::view::Win32WindowBase& operator=(const avl::view::Win32WindowBase&);
	};




	// Represents a failure while attempting to create a window.
	class WindowCreationFailure: public avl::utility::Exception
	{
		public:
		enum Type{CLASS_REGISTRATION, WINDOW_CREATION, DEVICE_CONTEXT};

		// Constructors:
		WindowCreationFailure(const avl::view::WindowCreationFailure::Type& type);
		~WindowCreationFailure();
		WindowCreationFailure(const avl::view::WindowCreationFailure& original);

		// Accessors:
		const avl::view::WindowCreationFailure::Type& GetType() const;
	private:
		// The type of window creation failure.
		const avl::view::WindowCreationFailure::Type type;

		// NOT IMPLEMENTED
		const WindowCreationFailure& operator=(const WindowCreationFailure&);
	};



} //avl
} //view
#endif // __AVL_VIEW_WIN32_WINDOW_BASE__