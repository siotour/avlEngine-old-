#ifndef __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__
#define __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__
/**
@file
Defines the interface for the input::DirectInputInputDevice class, which is used for input retrieval.
@author Sheldon Bachstein
@date Sep 08, 2011
@todo Go through the source code and document all thrown exceptions.
*/


#include<queue>
#include<windows.h>
/// Define the direct input version to avoid a compiler warning.
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
#include<dinput.h>



namespace avl
{
namespace input
{
	// Forward declaration.
	class InputEvent;


	/** Uses Direct Input to retrieve input events from the keyboard and mouse devices.
	@todo Modify this class to also record the sequence ID of each input event, and then
	sort the container which input events are stored in by that sequence ID.
	*/
	class DirectInputInputDevice
	{
	public:
		/** A queue of pointers to InputEvents. This is how polled input is reported
		to the client.*/
		typedef std::queue<const InputEvent* const> EventQueue;


		/** Given the handle to a window, creates an input device to monitor input for
		that window.
		@param initial_window_handle The handle of the window we're getting input for.
		*/
		DirectInputInputDevice(const HWND& initial_window_handle);
		/** Frees all resources for this device.*/
		~DirectInputInputDevice();

		/** Polls for mouse and keyboard input and returns a queue of any new input
		events. If there are no new inputs, then the returned queue will be empty.
		@attention The returned queue contains pointers to the input events. Calling
		this function assumes ownership of these pointers and they should be
		deleted when you're done with them.
		@todo Perhaps this function should strictly maintain ownership of the returned
		EventQueue, and specify that all of the pointers will remain valid up until the
		next poll. And then, at each call to PollInput, any old InputEvent pointers are
		deleted.
		@return An EventQueue containing all InputEvents since the last call to PollInput()
		or since this device was created.
		*/
		EventQueue PollInput();



	private:

		/** Resets the keyboard and mouse states so that any currently pressed keys or buttons
		are released and appropriate InputEvents are generated.
		@post For each \c true value in the \ref keyboard_state and \ref mouse_button_state arrays,
		that value will be set to \c false and an appropriate InputEvent will be added to
		\a queue to indicate that the key/button is considered released.
		@param queue A queue into which any InputEvents should be inserted.
		*/
		void ResetDeviceStates(EventQueue& queue);
		/** Attempts to poll the keyboard for new input data. Any new input events
		are appended to \a queue.
		@param queue A queue into which any InputEvents should be inserted.
		*/
		void PollKeyboard(EventQueue& queue);
		/** Attempts to poll the mouse for new input data. Any new input events
		are appended to queue.
		@param queue A queue into which any InputEvents should be inserted.
		*/
		void PollMouse(EventQueue& queue);

		/// The handle of the window receiving input.
		const HWND window_handle;

		/// Pointer to the DirectInput8 interface.
		LPDIRECTINPUT8 direct_input_interface;
		/// Pointer to the keyboard device.
		LPDIRECTINPUTDEVICE8 keyboard_device;
		/// Pointer to the mouse device.
		LPDIRECTINPUTDEVICE8 mouse_device;

		/** Keyboard state. This is needed to keep track of which buttons are down
		when the device is lost, so that key-up messages can be sent.*/
		bool keyboard_state[256];
		/** Mouse button states. This is needed to keep track of which buttons are
		down whent he device is lost so that button-release messages can be
		sent.*/
		bool mouse_button_state[8];


		/// Not implemented.
		DirectInputInputDevice(const DirectInputInputDevice&);
		/// Not implemented.
		DirectInputInputDevice& operator=(const DirectInputInputDevice&);

	};




} //avl
} //input
#endif // __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__