#pragma once
#ifndef __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__
#define __AVL_INPUT_DIRECT_INPUT_INPUT_DEVICE__
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
Defines the interface for the input::DirectInputInputDevice class, which is used for input retrieval.
@author Sheldon Bachstein
@date Sep 08, 2011
@todo Go through the source code and document all thrown exceptions.
*/


#include"..\input device\input device.h"
#include"..\..\..\utility\src\input events\input events.h"
#include<queue>
#include<windows.h>
// Defines the direct input version to avoid a compiler warning.
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
#include<dinput.h>



namespace avl
{
namespace input
{

	/** Uses Direct Input to retrieve input events from the keyboard and mouse devices.
	@todo Modify this class to also record the sequence ID of each input event, and then
	sort the container which input events are stored in by that sequence ID.
	@todo Extract the code which modifies indices into the mouse button state to be
	zero-based rather than the direct input defined non-zero based.
	*/
	class DirectInputInputDevice: public InputDevice
	{
	public:
		/** Given the handle to a window, creates an input device to monitor input for
		that window.
		@param initial_window_handle The handle of the window we're getting input for.
		*/
		DirectInputInputDevice(const HWND& initial_window_handle);
		/** Frees all resources for this device.
		*/
		~DirectInputInputDevice();

		/** Polls for mouse and keyboard input and returns a queue of any new input
		events. If there are no new inputs, then the returned queue will be empty.
		@attention The returned queue contains pointers to the input events, but
		calling this function does not give you ownership of these pointers. These
		pointers will remain valid only until the next call to this function.
		@return An utility::utility::input_events::InputQueue containing all InputEvents since the last call to PollInput()
		or since this device was created.
		*/
		utility::input_events::InputQueue GetInput();

	private:

		/** Resets the keyboard and mouse states so that any currently pressed keys or buttons
		are released and appropriate InputEvents are generated.
		@post For each \c true value in the \ref keyboard_state and \ref mouse_button_state arrays,
		that value will be set to \c false and an appropriate InputEvent will be added to
		\a queue to indicate that the key/button is considered released.
		@param queue A queue into which any InputEvents should be inserted.
		*/
		void ResetDeviceStates(utility::input_events::InputQueue& queue);
		/** Attempts to poll the keyboard for new input data. Any new input events
		are appended to \a queue.
		@param queue A queue into which any InputEvents should be inserted.
		*/
		void PollKeyboard(utility::input_events::InputQueue& queue);
		/** Attempts to poll the mouse for new input data. Any new input events
		are appended to queue.
		@param queue A queue into which any InputEvents should be inserted.
		*/
		void PollMouse(utility::input_events::InputQueue& queue);

		/** Releases any acquired resources.
		*/
		void ReleaseResources();

		/// The handle of the window receiving input.
		const HWND window_handle;

		/// Pointer to the DirectInput8 interface.
		LPDIRECTINPUT8 dinput;
		/// Pointer to the keyboard device.
		LPDIRECTINPUTDEVICE8 keyboard_device;
		/// Pointer to the mouse device.
		LPDIRECTINPUTDEVICE8 mouse_device;

		/** Keyboard state. This is needed to keep track of which buttons are down
		when the device is lost, so that key-up messages can be sent.*/
		char keyboard_state[256];
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