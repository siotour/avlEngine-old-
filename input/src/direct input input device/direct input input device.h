#ifndef __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__
#define __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: 
 **********/


#include<queue>
#include<windows.h>
// Define the direct input version to avoid a compiler warning.
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


	// See the beginning of the file for details.
	class DirectInputInputDevice
	{
	public:
		// A queue of pointers to InputEvents. This is how polled input is
		// reported to the client.
		typedef std::queue<const InputEvent* const> EventQueue;


		// Constructors:
		// Requires the handle of the window which is receiving input.
		DirectInputInputDevice(const HWND& initial_window_handle);
		// Destructor.
		~DirectInputInputDevice();

		// Polls for mouse and keyboard input and returns a queue of any new input
		// events. If there is no new input, the returned queue will be empty.
		// Note: The returned queue contains pointers to the input events. Calling
		// this function assumes ownership of these pointers and they should be
		// deleted.
		// TODO: Use smart pointers for the queue.
		EventQueue PollInput();



	private:

		// Goes through the keyboard_state and mouse_button_state arrays and, for
		// each true value, adds to queue a key- or button-up event for the appropriate
		// key or button and then sets that value to false.
		void ResetDeviceStates(EventQueue& queue);
		// Attempts to poll the keyboard for new input data. Any new input events
		// are appended to queue.
		void PollKeyboard(EventQueue& queue);
		// Attempts to poll the mouse for new input data. Any new input events
		// are appended to queue.
		void PollMouse(EventQueue& queue);

		// The handle of the window receiving input.
		const HWND window_handle;

		// Pointer to the DirectInput8 interface.
		LPDIRECTINPUT8 direct_input_interface;
		// Pointer to the keyboard device.
		LPDIRECTINPUTDEVICE8 keyboard_device;
		// Pointer to the mouse device.
		LPDIRECTINPUTDEVICE8 mouse_device;

		// Keyboard state. This is needed to keep track of which buttons are down
		// when the device is lost, so that key-up messages can be sent.
		bool keyboard_state[256];
		// Mouse button states. This is needed to keep track of which buttons are
		// down whent he device is lost so that button-release messages can be
		// sent.
		bool mouse_button_state[8];


		// Not implemented:
		DirectInputInputDevice(const DirectInputInputDevice&);
		DirectInputInputDevice& operator=(const DirectInputInputDevice&);

	};




} //avl
} //input
#endif // __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__