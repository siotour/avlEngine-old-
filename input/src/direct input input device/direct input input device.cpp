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
Unit test for the direct input input device component. See "direct input input device.h" for details.
@author Sheldon Bachstein
@date Sep 08, 2011
*/

#include"direct input input device.h"
#include"..\dinput wrapper\dinput wrapper.h"
#include"..\input event\input event.h"
#include"..\keyboard event\keyboard event.h"
#include"..\mouse move event\mouse move event.h"
#include"..\mouse button event\mouse button event.h"
#include"..\mouse scroll event\mouse scroll event.h"

#include"..\key codes\key codes.h"

#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"

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

	// See method declaration for details.
	DirectInputInputDevice::DirectInputInputDevice(const HWND& initial_window_handle)
		: window_handle(initial_window_handle), dinput(NULL), keyboard_device(NULL), mouse_device(NULL)
	{
		// We need a valid window handle.
		if(window_handle == NULL)
		{
			throw utility::InvalidArgumentException("avl::input::DirectInputInputDevice::DirectInputInputDevice()", "initial_window_handle", "initial_window_handle must be non-NULL");
		}
		try
		{
			// Create the DirectInput interface.
			dinput = dinput::GetDirectInput();
			// Create a keyboard device.
			keyboard_device = dinput::CreateKeyboardDevice(dinput, window_handle, 30);
			// Create a mouse device.
			mouse_device = dinput::CreateMouseDevice(dinput, window_handle, 30);
		
		
			// Aquire the keyboard and mouse devices.
			keyboard_device->Acquire();
			mouse_device->Acquire();



			// Set the initial mouse state.
			DIMOUSESTATE mouse;
			mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
			for (int i = 0; i < 3; i++)
			{
				mouse_button_state[i] = (mouse.rgbButtons[i] != 0) ? true : false;
			}

			// Set the initial keyboard state.
			keyboard_device->GetDeviceState(256 * sizeof(char), &keyboard_state);
		}
		// If anything went wrong, clean up our mess.
		catch(...)
		{
			ReleaseResources();
			throw;
		}
	}


	// See method declaration for details.
	DirectInputInputDevice::~DirectInputInputDevice()
	{
		ReleaseResources();
	}


	// See method declaration for details.
	InputQueue DirectInputInputDevice::GetInput()
	{
		InputQueue queue;

		PollKeyboard(queue);
		PollMouse(queue);

		return queue;
	}


	// See method declaration for details.
	void DirectInputInputDevice::ResetDeviceStates(InputQueue& queue)
	{
		// Reset the state of each currently pressed mouse button.
		const MouseButtonEvent* button_event;
		for(DWORD button = 0; button < 8; ++button)
		{
			if(mouse_button_state[button] == true)
			{
				// Generate the appropriate button-release event.
				button_event = new MouseButtonEvent(dinput::DIKToMB(button), false);
				if(button_event == NULL)
				{
					throw utility::OutOfMemoryError();
				}
				queue.push(button_event);
				// Set the button state to released.
				mouse_button_state[button] = false;
			}
		}
		// Reset the state of each current pressed keyboard key.
		const KeyboardEvent* key_event;
		for(DWORD key = 0; key < 256; ++key)
		{
			// Is this key pressed?
			if(keyboard_state[key] == true)
			{
				// Generate the appropriate key-release event.
				key_event = new KeyboardEvent(dinput::DIKToKK(key), false);
				if(key_event == NULL)
				{
					throw utility::OutOfMemoryError();
				}
				queue.push(key_event);
				// Set the key state to released.
				keyboard_state[key] = false;
			}
		}
	}


	// See method declaration for details.
	void DirectInputInputDevice::PollKeyboard(InputQueue& queue)
	{
		// These variables hold the raw data from the keyboard.
		std::queue<DIDEVICEOBJECTDATA> data_queue;
		DIDEVICEOBJECTDATA data;
		// Retrieve the data from the keyboard.
		bool device_okay = dinput::RetrieveDeviceData(keyboard_device, data_queue);
		// Now process the raw data into input events.
		const KeyboardEvent* key_event;
		while(data_queue.empty() == false)
		{
			data = data_queue.front();
			data_queue.pop();
			// Was the key pressed or released?
			bool pressed = ((data.dwData & 0x80) != 0) ? true : false;
			// Add a new KeyboardEvent to the queue.
			key_event = new KeyboardEvent(dinput::DIKToKK(data.dwOfs), pressed);
			if(key_event == NULL)
			{
				throw utility::Exception("avl::input::DirectInputInputDevice::PollKeyboard() -- Unable to allocate memory for a new KeyboardEvent.");
			}
			queue.push(key_event);
			// Save the internal key state.
			keyboard_state[data.dwOfs] = pressed;
		}
		// Now reset the device states if necessary.
		if(device_okay == false)
		{
			ResetDeviceStates(queue);
		}
	}


	// See method declaration for details.
	void DirectInputInputDevice::PollMouse(InputQueue& queue)
	{
		// These variables hold the raw data from the mouse.
		std::queue<DIDEVICEOBJECTDATA> data_queue;
		DIDEVICEOBJECTDATA data;
		// Retrieve the data from the mouse.
		bool device_okay = dinput::RetrieveDeviceData(mouse_device, data_queue);
		// Now process the raw data into input events.
		DWORD current_event_data;
		DWORD next_event_data;
		InputEvent* event = NULL;
		while(data_queue.empty() == false)
		{
			data = data_queue.front();
			data_queue.pop();
			// Figure out what kind of event we're dealing with.
			switch(data.dwOfs)
			{
			case DIMOFS_X:
				// ***Falls through***
			case DIMOFS_Y:
				// Mouse move event.
				current_event_data = data.dwData;
				next_event_data = 0;
				// If data_queue isn't empty, see if we can combine this event and the next one.
				if(data_queue.empty() == false && data_queue.front().dwSequence == data.dwSequence)
				{
					next_event_data = data_queue.front().dwData;
					data_queue.pop();
				}
				// Is current_event_data x movement or y movement?
				event = (data.dwOfs == DIMOFS_X) ? new(std::nothrow) MouseMoveEvent((short)current_event_data, (short)next_event_data) : new(std::nothrow) MouseMoveEvent((short)next_event_data, (short)current_event_data);
				break;
			case DIMOFS_Z:
				// Mouse wheel event.
				event = new(std::nothrow) MouseScrollEvent((short)data.dwOfs);
				break;
			default:
				// Mouse button event.
				const bool pressed = (data.dwData & 0x80) ? true : false;
				event = new MouseButtonEvent(dinput::DIKToMB(data.dwOfs), pressed);
				// Save the internal button state.
				mouse_button_state[data.dwOfs] = pressed;
				break;
			}
			// Did we run out of memory?
			if(event == NULL)
			{
				throw utility::OutOfMemoryError();
			}
			queue.push(event);
		}
		// Now reset the device states if necessary.
		if(device_okay == false)
		{
			ResetDeviceStates(queue);
		}
	}


	// See method declaration for details.
	void DirectInputInputDevice::ReleaseResources()
	{
		// Release the keyboard device.
		if (keyboard_device != NULL)
		{
			keyboard_device->Unacquire();
			keyboard_device->Release();
			keyboard_device = NULL;
		}
		// Release the mouse device.
		if (mouse_device != NULL)
		{
			mouse_device->Unacquire();
			mouse_device->Release();
			mouse_device = NULL;
		}
		// Release the DirectInput interface.
		if (dinput != NULL)
		{
			dinput->Release();
			dinput = NULL;
		}
	}


}
}