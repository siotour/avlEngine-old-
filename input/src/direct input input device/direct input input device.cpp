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

#include"..\input event\input event.h"
#include"..\keyboard event\keyboard event.h"
#include"..\mouse move event\mouse move event.h"
#include"..\mouse button event\mouse button event.h"
#include"..\mouse scroll event\mouse scroll event.h"

#include"..\key codes\key codes.h"

#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"

// Define the direct input version to avoid a compiler warning.
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
#include<dinput.h>
#include<windows.h>

#include<queue>




namespace avl
{
namespace input
{


	// Forward declaration.
	namespace
	{
		// See definitions for details.
		key_codes::MouseButton::MouseButtonCodes DIKToMB(const DWORD& vkey);
		key_codes::KeyboardKey::KeyboardKeyCodes DIKToKK(const DWORD& vkey);
	}




	// Requires the handle of the window which is receiving input.
	DirectInputInputDevice::DirectInputInputDevice(const HWND& initial_window_handle)
		: window_handle(initial_window_handle)
	{
		// If initial_window_handle is NULL, throw a utility::Exception.
		if(window_handle == NULL)
		{
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- The supplied window handle is NULL.");
		}

		// Stores the results of system calls.
		HRESULT result;




		// Attempt to create the DirectInput interface. If this fails, throw a utility::Exception.
		result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8, (void**)&direct_input_interface, NULL);
		if (FAILED(result))
		{
			utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to create the DirectInput8 interface.");
		}

		// Attempt to create the keyboard device. If this fails, throw a utility::Exception.
		result = direct_input_interface->CreateDevice(GUID_SysKeyboard, &keyboard_device, NULL);
		if (FAILED(result))
		{
			// Release the DirectInput interface.
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to create DirectInput8 keyboard device.");
		}

		// Attempt to create the mouse device. If this fails, throw a utility::Exception.
		result = direct_input_interface->CreateDevice(GUID_SysMouse, &mouse_device, NULL);
		if (FAILED(result))
		{
			// Release the keyboard device.
			if (keyboard_device != NULL)
			{
				keyboard_device->Unacquire();
				keyboard_device->Release();
				keyboard_device = NULL;
			}

			// Release the DirectInput interface.
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to create DirectInput8 mouse device.");
		}




		// Set the keyboard data format. If this fails, throw a utility::Exception.
		result = keyboard_device->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
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
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to set the keyboard device data format.");
		}

		// Set the mouse device data format. If this fails, throw a utility::Exception.
		result = mouse_device->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result))
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
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to set the keyboard device data format.");
		}



		// Set the keyboard device cooperative level. If this fails, throw a utility::Exception.
		result = keyboard_device->SetCooperativeLevel(window_handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result))
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
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to set the keyboard device cooperative level.");
		}

		// Set the mouse device cooperative level. If this fails, throw a utility::Exception.
		result = mouse_device->SetCooperativeLevel(window_handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result))
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
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Unable to set the mouse device cooperative level.");
		}




		// Set up a DirectInputProperty (DIPROPDWORD) structure with information for
		// changing a device buffer size.
		const DWORD buffer_size = 30;
		DIPROPDWORD buffer_size_property;

		buffer_size_property.diph.dwSize = sizeof(DIPROPDWORD);
		buffer_size_property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		buffer_size_property.diph.dwObj = 0;
		buffer_size_property.diph.dwHow = DIPH_DEVICE;
		buffer_size_property.dwData = buffer_size;



		// Change the keyboard device buffer size. If this fails, throw a utility::Exception.
		result = keyboard_device->SetProperty(DIPROP_BUFFERSIZE, &(buffer_size_property.diph));
		if (FAILED(result))
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
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Failed to change the default buffer size for the keyboard device.");
		}

		// Change the mouse device buffer size. If this fails, throw a utility::Exception.
		result = mouse_device->SetProperty(DIPROP_BUFFERSIZE, &(buffer_size_property.diph));
		if (FAILED(result))
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
			if (direct_input_interface != NULL)
			{
				direct_input_interface->Release();
				direct_input_interface = NULL;
			}
			throw utility::Exception("avl::input::DirectInputInputDevice::DirectInputInputDevice() -- Failed to change the default buffer size for the mouse device.");
		}



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




	// Destructor. Releases the keyboard and mouse devices, and then the
	// DirectInput8 interface.
	DirectInputInputDevice::~DirectInputInputDevice()
	{
		ASSERT(keyboard_device != NULL);
		ASSERT(mouse_device != NULL);
		ASSERT(direct_input_interface != NULL);

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
		if (direct_input_interface != NULL)
		{
			direct_input_interface->Release();
			direct_input_interface = NULL;
		}
	}




	// Polls for mouse and keyboard input and returns a queue of any new input
	// events. If there is no new input, the returned queue will be empty.
	DirectInputInputDevice::EventQueue DirectInputInputDevice::PollInput()
	{
		EventQueue queue;

		PollKeyboard(queue);
		PollMouse(queue);

		return queue;
	}




	// Goes through the keyboard_state and mouse_button_state arrays and, for
	// each true value, adds to queue a key- or button-up event for the appropriate
	// key or button and then sets that value to false.
	void DirectInputInputDevice::ResetDeviceStates(EventQueue& queue)
	{
		// Reset the mouse state.
		for(DWORD i = 0; i < 8; ++i)
		{
			// Is this button pressed?
			if(mouse_button_state[i] == true)
			{
				// If this button is pressed, add a button released event to the queue.
				// If unable to allocate memory for the event, throw a utility::Exception.
				const MouseButtonEvent* const button = new MouseButtonEvent(DIKToMB(i), false);
				if(button == NULL)
				{
					throw utility::Exception("avl::input::DirectInputInputDevice::ResetDeviceStates() -- Unable to allocate memory for a new MouseButtonEvent.");
				}
				queue.push(button);

				// Set the button to released.
				mouse_button_state[i] = false;
			}
		}


		// Reset the keyboard state.
		for(DWORD i = 0; i < 256; ++i)
		{
			// Is this key pressed?
			if(keyboard_state[i] == true)
			{
				// If this key is pressed, add a key released event to the queue.
				// If unable to allocate memory for the event, throw a utility::Exception.
				const KeyboardEvent* const key = new KeyboardEvent(DIKToKK(i), false);
				if(key == NULL)
				{
					throw utility::Exception("avl::input::DirectInputInputDevice::ResetDeviceStates() -- Unable to allocate memory for a new KeyboardEvent.");
				}
				queue.push(key);

				// Set the key to released.
				keyboard_state[i] = false;
			}
		}
	}




	// Attempts to poll the keyboard for new input data. Any new input events
	// are appended to queue.
	void DirectInputInputDevice::PollKeyboard(EventQueue& queue)
	{
		// Structure to hold input data from the keyboard.
		DIDEVICEOBJECTDATA data;
		// Number of input elements to read.
		DWORD number_of_elements = 1;
		// Temporary storage of system calls.
		HRESULT result;

		for(result = keyboard_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &number_of_elements, 0); number_of_elements > 0; result = keyboard_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &number_of_elements, 0))
		{
			// If the data failed (and a buffer overflow is not a failure), reset the
			// devices.
			if (FAILED(result) && result != DI_BUFFEROVERFLOW)
			{
				// Something went wrong. It's likely that the device was lost, so try to
				// aquire the keyboard device. If that fails, reset the devices and return.
				result = keyboard_device->Acquire();
				if (FAILED(result))
				{
					// Reset the devices and return.
					ResetDeviceStates(queue);
					return;
				}
			}
			// There is data in the buffer; process it.
			else
			{
				// Reset number_of_elements to 1 for the next iteration.
				number_of_elements = 1;

				// Was the key pressed or released?
				bool pressed = ((data.dwData & 0x80) != 0) ? true : false;

				// Add a new KeyboardEvent to the queue. If unable to allocate
				// the memory, throw a utility::Exception.
				const KeyboardEvent* key = new KeyboardEvent(DIKToKK(data.dwOfs), pressed);
				if(key == NULL)
				{
					throw utility::Exception("avl::input::DirectInputInputDevice::PollKeyboard() -- Unable to allocate memory for new KeyboardEvent.");
				}
				queue.push(key);

				// Save the internal key state.
				keyboard_state[data.dwOfs] = pressed;
			}
		}
	}




	// Attempts to poll the mouse for new input data. Any new input events
	// are appended to queue.
	void DirectInputInputDevice::PollMouse(EventQueue& queue)
	{
		// Holds information about each buffered event.
		DIDEVICEOBJECTDATA data;
		// Number of events to extract from the buffer per iteration.
		DWORD number_of_elements = 1;
		// Result of system calls.
		HRESULT result;

		// Move movement variables:
		// Used to keep track of event sequence numbers. This is necessary since
		// mouse movement is reported as x-movement and y-movement separately.
		DWORD previous_sequence = 0;
		// Stores the x and y components of a mouse movement.
		DWORD delta_x = 0;
		DWORD delta_y = 0;
		// A flag indicating that data is being stored from a previous movement
		// event.
		bool storing_data = false;


		for(result = mouse_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &number_of_elements, 0); number_of_elements > 0; result = mouse_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &number_of_elements, 0))
		{
			// Check if the data request failed. Note that a buffer overflow is not a
			// failure.
			if (FAILED(result) && result != DI_BUFFEROVERFLOW)
			{
				// Attempt to aquire the mouse device.
				result = mouse_device->Acquire();

				// If the aquisition failed, reset the devices and exit the loop.
				if (FAILED(result))
				{
					ResetDeviceStates(queue);
					return;
				}
			}
			// There is new data in the buffer. Process it.
			else
			{
				// Reset the number_of_elements for the next loop.
				number_of_elements = 1;

				// Simply to stop compiler error C2361:
				const MouseScrollEvent* wheel = NULL;


				// Switch depending on the type of event data.
				switch (data.dwOfs)
				{
				case DIMOFS_X:
					// Horizontal movement.
					// Falls through.
				case DIMOFS_Y:
					// Vertical movement.

					// Is there no data stored?
					if(storing_data == false)
					{
						// If not, just store this data for the next event.
						storing_data = true;
						previous_sequence = data.dwSequence;

						if(data.dwOfs == DIMOFS_X)
						{
							delta_x = data.dwData;
						}
						else
						{
							delta_y = data.dwData;
						}
					}
					// If there is data stored already:
					else
					{
						// If this event has the same sequence number as the previous event, then
						// the two events can be combined into a single message.
						if(data.dwSequence == previous_sequence)
						{
							if(data.dwOfs == DIMOFS_X)
							{
								delta_x = data.dwData;
							}
							else
							{
								delta_y = data.dwData;
							}

							// Pack the data into a MouseMoveEvent. If unable to allocate the memory,
							// then throw a utility::Exception.
							const MouseMoveEvent* const move = new MouseMoveEvent((short)delta_x, (short)delta_y);
							if(move == NULL)
							{
								throw utility::Exception("avl::input::DirectInputInputDevice::PollMouse() -- Unable to allocate memory for a new MouseMoveEvent.(1)");
							}
							queue.push(move);

							// We're no longer storing data.
							storing_data = false;
							delta_x = 0;
							delta_y = 0;
						}
						// If this event has a different sequence number than the previous event, then
						// the two events can't be combined.
						else
						{
							// Pack the old data into a MouseMoveEvent. If unable to allocate the memory,
							// then throw a utility::Exception.
							const MouseMoveEvent* const move = new MouseMoveEvent((short)delta_x, (short)delta_y);
							if(move == NULL)
							{
								throw utility::Exception("avl::input::DirectInputInputDevice::PollMouse() -- Unable to allocate memory for a new MouseMoveEvent.(2)");
							}
							queue.push(move);

							// Now clear the previous data, and store the new event data.
							if(data.dwOfs == DIMOFS_X)
							{
								delta_x = data.dwData;
								delta_y = 0;
							}
							else
							{
								delta_x = 0;
								delta_y = data.dwData;
							}
						}
					}
					break;
				case DIMOFS_Z:
					// Mouse wheel scroll. Add a MouseScrollEvent to the queue. If unable to allocate
					// memory for the event, throw a utility::Exception.
					wheel = new MouseScrollEvent((short)data.dwOfs);
					if(wheel == NULL)
					{
						throw utility::Exception("avl::input::DirectInputInputDevice::PollMouse() -- Unable to allocate memory for a new MouseScrollEvent.");
					}
					queue.push(wheel);
					break;
				default:
					// A mouse button was pressed. Add a MouseButtonEvent to the queue for this
					// button. If unable to allocate memory for the event, throw a
					// utility::Exception.
					const bool pressed = (data.dwData & 0x80) ? true : false;
					const MouseButtonEvent* button = new MouseButtonEvent(DIKToMB(data.dwOfs), pressed);
					if(button == NULL)
					{
						throw utility::Exception("avl::input::DirectInputInputDevice::PollMouse() -- Unable to allocate memory for a new MouseButtonEvent.");
					}
					queue.push(button);

					// Save the internal button state.
					mouse_button_state[data.dwOfs] = pressed;
					break;
				}
			}
		}

		// If there is still any mouse movement data being stored, then put it into an event now.
		// Pack the old data into a MouseMoveEvent. If unable to allocate the memory,
		// then throw a utility::Exception.
		const MouseMoveEvent* const move = new MouseMoveEvent((short)delta_x, (short)delta_y);
		if(move == NULL)
		{
			throw utility::Exception("avl::input::DirectInputInputDevice::PollMouse() -- Unable to allocate memory for a new MouseMoveEvent.(3)");
		}
		queue.push(move);
	}








	namespace
	{
		// Given a DirectInput mouse button code, returns the equivalent avl mouse button code,
		// if one exists.
		key_codes::MouseButton::MouseButtonCodes DIKToMB(const DWORD& button)
		{
			using key_codes::MouseButton;
			switch(button)
			{
				case DIMOFS_BUTTON0:
					// Left mouse button.
					return MouseButton::mb_left;
				case DIMOFS_BUTTON1:
					// Right mouse button.
					return MouseButton::mb_right;
				case DIMOFS_BUTTON2:
					// Middle mouse button.
					return MouseButton::mb_middle;
				default:
					return MouseButton::mb_null;
			}
		}




		// Given a DirectInput DIK key code, returns the equivalent avl key code, if one exists.
		key_codes::KeyboardKey::KeyboardKeyCodes DIKToKK(const DWORD& key)
		{
			using key_codes::KeyboardKey;
			switch(key)
			{
			// Numbers:
			case DIK_0:
				return KeyboardKey::kk_0;
			case DIK_1:
				return KeyboardKey::kk_1;
			case DIK_2:
				return KeyboardKey::kk_2;
			case DIK_3:
				return KeyboardKey::kk_3;
			case DIK_4:
				return KeyboardKey::kk_4;
			case DIK_5:
				return KeyboardKey::kk_5;
			case DIK_6:
				return KeyboardKey::kk_6;
			case DIK_7:
				return KeyboardKey::kk_7;
			case DIK_8:
				return KeyboardKey::kk_8;
			case DIK_9:
				return KeyboardKey::kk_9;

			// Letters:
			case DIK_A:
				return KeyboardKey::kk_a;
			case DIK_B:
				return KeyboardKey::kk_b;
			case DIK_C:
				return KeyboardKey::kk_c;
			case DIK_D:
				return KeyboardKey::kk_d;
			case DIK_E:
				return KeyboardKey::kk_e;
			case DIK_F:
				return KeyboardKey::kk_f;
			case DIK_G:
				return KeyboardKey::kk_g;
			case DIK_H:
				return KeyboardKey::kk_h;
			case DIK_I:
				return KeyboardKey::kk_i;
			case DIK_J:
				return KeyboardKey::kk_j;
			case DIK_K:
				return KeyboardKey::kk_k;
			case DIK_L:
				return KeyboardKey::kk_l;
			case DIK_M:
				return KeyboardKey::kk_m;
			case DIK_N:
				return KeyboardKey::kk_n;
			case DIK_O:
				return KeyboardKey::kk_o;
			case DIK_P:
				return KeyboardKey::kk_p;
			case DIK_Q:
				return KeyboardKey::kk_q;
			case DIK_R:
				return KeyboardKey::kk_r;
			case DIK_S:
				return KeyboardKey::kk_s;
			case DIK_T:
				return KeyboardKey::kk_t;
			case DIK_U:
				return KeyboardKey::kk_u;
			case DIK_V:
				return KeyboardKey::kk_v;
			case DIK_W:
				return KeyboardKey::kk_w;
			case DIK_X:
				return KeyboardKey::kk_x;
			case DIK_Y:
				return KeyboardKey::kk_y;
			case DIK_Z:
				return KeyboardKey::kk_z;

			// Punctuation:
			case DIK_COMMA:
				return KeyboardKey::kk_comma;
			case DIK_PERIOD:
				return KeyboardKey::kk_period;
			case DIK_SEMICOLON:
				return KeyboardKey::kk_semicolon;
			case DIK_APOSTROPHE:
				return KeyboardKey::kk_apostrophe;
			case DIK_GRAVE:
				return KeyboardKey::kk_accent;
			
			// Control characters:
			case DIK_SPACE:
				return KeyboardKey::kk_space;
			case DIK_TAB:
				return KeyboardKey::kk_tab;
			case DIK_BACKSPACE:
				return KeyboardKey::kk_backspace;
			case DIK_RETURN:
				return KeyboardKey::kk_enter;
			case DIK_ESCAPE:
				return KeyboardKey::kk_escape;
			
			// Home/end cluster:
			case DIK_INSERT:
				return KeyboardKey::kk_insert;
			case DIK_DELETE:
				return KeyboardKey::kk_delete;
			case DIK_HOME:
				return KeyboardKey::kk_home;
			case DIK_END:
				return KeyboardKey::kk_end;
			case DIK_PRIOR:
				return KeyboardKey::kk_pageup;
			case DIK_NEXT:
				return KeyboardKey::kk_pagedown;

			// Arrows
			case DIK_UP:
				return KeyboardKey::kk_up;
			case DIK_DOWN:
				return KeyboardKey::kk_down;
			case DIK_RIGHT:
				return KeyboardKey::kk_right;
			case DIK_LEFT:
				return KeyboardKey::kk_left;

			// Function keys
			case DIK_F1:
				return KeyboardKey::kk_f1;
			case DIK_F2:
				return KeyboardKey::kk_f2;
			case DIK_F3:
				return KeyboardKey::kk_f3;
			case DIK_F4:
				return KeyboardKey::kk_f4;
			case DIK_F5:
				return KeyboardKey::kk_f5;
			case DIK_F6:
				return KeyboardKey::kk_f6;
			case DIK_F7:
				return KeyboardKey::kk_f7;
			case DIK_F8:
				return KeyboardKey::kk_f8;
			case DIK_F9:
				return KeyboardKey::kk_f9;
			case DIK_F10:
				return KeyboardKey::kk_f10;
			case DIK_F11:
				return KeyboardKey::kk_f11;
			case DIK_F12:
				return KeyboardKey::kk_f12;

			// Modifier keys:
			case DIK_NUMLOCK:
				return KeyboardKey::kk_numlock;
			case DIK_CAPSLOCK:
				return KeyboardKey::kk_capslock;
			case DIK_SCROLL:
				return KeyboardKey::kk_scrolllock;
			case DIK_RSHIFT:
				return KeyboardKey::kk_rightshift;
			case DIK_LSHIFT:
				return KeyboardKey::kk_leftshift;
			case DIK_RCONTROL:
				return KeyboardKey::kk_rightctrl;
			case DIK_LCONTROL:
				return KeyboardKey::kk_leftctrl;
			case DIK_RALT:
				return KeyboardKey::kk_rightalt;
			case DIK_LALT:
				return KeyboardKey::kk_leftalt;
			case DIK_RWIN:
				return KeyboardKey::kk_rightwindow;
			case DIK_LWIN:
				return KeyboardKey::kk_leftwindow;

			// Numpad:
			case DIK_NUMPAD0:
				return KeyboardKey::kk_np_0;
			case DIK_NUMPAD1:
				return KeyboardKey::kk_np_1;
			case DIK_NUMPAD2:
				return KeyboardKey::kk_np_2;
			case DIK_NUMPAD3:
				return KeyboardKey::kk_np_3;
			case DIK_NUMPAD4:
				return KeyboardKey::kk_np_4;
			case DIK_NUMPAD5:
				return KeyboardKey::kk_np_5;
			case DIK_NUMPAD6:
				return KeyboardKey::kk_np_6;
			case DIK_NUMPAD7:
				return KeyboardKey::kk_np_7;
			case DIK_NUMPAD8:
				return KeyboardKey::kk_np_8;
			case DIK_NUMPAD9:
				return KeyboardKey::kk_np_9;
			case DIK_DECIMAL:
				return KeyboardKey::kk_np_period;
			case DIK_NUMPADSLASH:
				return KeyboardKey::kk_np_slash;
			case DIK_NUMPADSTAR:
				return KeyboardKey::kk_np_star;
			case DIK_NUMPADMINUS:
				return KeyboardKey::kk_np_minus;
			case DIK_NUMPADPLUS:
				return KeyboardKey::kk_np_plus;
			case DIK_NUMPADENTER:
				return KeyboardKey::kk_np_enter;

			// Misc.:
			case DIK_APPS:
				return KeyboardKey::kk_menu;

			// Default:
			default:
				return KeyboardKey::kk_null;
			}			
		}			
	}


}
}