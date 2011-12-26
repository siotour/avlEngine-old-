#ifndef __AVL_VIEW_WIN32_WINDOW_BASE__
#define __AVL_VIEW_WIN32_WINDOW_BASE__
/**
@file
Provides an interface for a generic Win32 window, as well as some basic functionality. Also defines
an exception class for dealing with exceptions during window creation.
@author Sheldon Bachstein
@date Feb 01, 2010
*/

#include<string>
#include<windows.h>
#include"..\..\..\utility\src\exception\exception.h"


namespace avl
{
namespace view
{
	/**
	A partially-implemented skeleton used to build
	higher-level Win32 windows on.
	*/
	class Win32WindowBase
	{
	public:
		/** Attempts to create a window with the specified properties. If an error occurs, will throw
		a WindowCreationFailure.
		@param instance_handle Handle to the application instance.
		@param window_title The name which will appear in the window's title bar, task bar icon, and task manager.
		@param width The window's client-area width.
		@param height The window's client-area height.
		@param window_procedure A handle to the window procedure for this window.
		@throws WindowCreationFailure If unable to create the window.
		@todo Clarify when an exception may be thrown from this procedure.
		@todo Define the pre- and post-conditions for this constructor.
		*/
		Win32WindowBase(HINSTANCE instance_handle, const std::string& window_title, const int& width, const int& height, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM));
		/** Basic destructor.*/
		virtual ~Win32WindowBase();

		/** Returns the window's handle.
		@return The window's handle.
		*/
		HWND GetWindowHandle() const;
		/** Returns the device context handle for this window.
		@return The handle to the device context created for this window.
		*/
		HDC GetDeviceContextHandle() const;

		/** General upkeep of the window; should be called often. A return value of true means the window is functioning
		normally, false means that the window has been closed.
		@return True means the window is performing normally, and false means the window has been closed.
		*/
		virtual const bool Update() = 0;

	protected:
		/** Attempts to register a window class matching the parameters.
		@param instance_handle Handle to the application instance.
		@param window_title The title of the window and name of the window class.
		@param window_procedure A handle to the window procedure for this window.
		@throws WindowCreationFailure If unable to create the window.
		@todo Clarify when this procedure throws an exception and define the pre- and post-conditions.
		*/
		void RegisterWindowClass(HINSTANCE instance_handle, const char* const& window_title, LRESULT (CALLBACK * const window_procedure)(HWND, UINT, WPARAM, LPARAM));

		/** Attempts to create a window matching the parameters.
		@note "CreateWindow" is a macro, so it can't be used as a name.
		@param instance_handle Handle to the application instance.
		@param window_title The title of the window and name of the window class.
		@param width The width of the window's client area.
		@param height The height of the window's client area.
		*/
		void MakeWindow(HINSTANCE instance_handle, const char* const& window_title, const int width, const int height);

		/** Attempts to attain a device context for this window.
		@todo When does this procedure throw any exceptions? What are the pre- and post-conditions?
		*/
		void AttainDeviceContext();

		/// The handle to the window.
		HWND window_handle;
		/// The handle to the device context for the window.
		HDC device_context_handle;

		/// NOT IMPLEMENTED.
		Win32WindowBase(const avl::view::Win32WindowBase&);
		/// NOT IMPLEMENTED.
		const avl::view::Win32WindowBase& operator=(const avl::view::Win32WindowBase&);
	};




	/**
	Represents a failure while attempting to create a window.
	*/
	class WindowCreationFailure: public avl::utility::Exception
	{
		public:
		/** Defines the different types of possible window creation failures.
		*/
		enum Type{CLASS_REGISTRATION,	/**< An error occurred while registering the window class.*/
				  WINDOW_CREATION,		/**< An error occurred while creating the window.*/
				  DEVICE_CONTEXT};		/**< An error occurred while obtaining a device context.*/

		/** Basic constructor.
		@param type The type of window creation failure which has occurred.
		*/
		WindowCreationFailure(const avl::view::WindowCreationFailure::Type& type);
		/** Basic destructor.*/
		~WindowCreationFailure();
		/** Copy constructor.
		@param original The object being copied.
		*/
		WindowCreationFailure(const avl::view::WindowCreationFailure& original);

		/** What type of window creation failure occurred?
		@return The type of window creation failure which occurred.
		*/
		const avl::view::WindowCreationFailure::Type& GetType() const;
	private:
		/// The type of window creation failure.
		const avl::view::WindowCreationFailure::Type type;

		/// NOT IMPLEMENTED.
		const WindowCreationFailure& operator=(const WindowCreationFailure&);
	};



} //avl
} //view
#endif // __AVL_VIEW_WIN32_WINDOW_BASE__