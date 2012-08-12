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
Implementation for the dinput wrapper component. See "dinput wrapper.h" for details.
@author Sheldon Bachstein
@date Jun 13, 2012
*/

#include"dinput wrapper.h"
#include"..\..\..\utility\src\key codes\key codes.h"
#include"..\..\..\utility\src\input events\input events.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include<new>
#include<memory>
/// Defines the direct input version to avoid a compiler warning.
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
#include<dinput.h>
#include<windows.h>


namespace avl
{
namespace input
{
namespace dinput
{
	// See function declaration for details.
	LPDIRECTINPUT8 const GetDirectInput()
	{
		// Attempt to create the DirectInput interface.
		LPDIRECTINPUT8 direct_input;
		HRESULT result;
		result = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8, (void**)&direct_input, nullptr);
		if (FAILED(result))
		{
			utility::Exception("avl::input::dinput::GetDirectInput() -- Unable to create the DirectInput8 interface.");
		}
		return direct_input;
	}


	// See function declaration for details.
	LPDIRECTINPUTDEVICE8 const CreateKeyboardDevice(LPDIRECTINPUT8 const dinput, HWND window_handle, const unsigned int buffer_size)
	{
		if(dinput == nullptr)
		{
			throw new utility::InvalidArgumentException("avl::input::dinput::CreateKeyboardDevice()", "dinput", "Must be non-nullptr");
		}
		// Create the keyboard device.
		HRESULT result;
		LPDIRECTINPUTDEVICE8 keyboard_device = nullptr;
		result = dinput->CreateDevice(GUID_SysKeyboard, &keyboard_device, nullptr);
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateKeyboardDevice() -- Unable to create DirectInput8 keyboard device.");
		}
		// Set the keyboard data format.
		result = keyboard_device->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateKeyboardDevice() -- Unable to set the keyboard device data format.");
		}
		// Set the keyboard device cooperative level.
		result = keyboard_device->SetCooperativeLevel(window_handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateKeyboardDevice() -- Unable to set the keyboard device cooperative level.");
		}
		// Change the keyboard device buffer size.
		DIPROPDWORD buffer_size_property;
		buffer_size_property.diph.dwSize = sizeof(DIPROPDWORD);
		buffer_size_property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		buffer_size_property.diph.dwObj = 0;
		buffer_size_property.diph.dwHow = DIPH_DEVICE;
		buffer_size_property.dwData = buffer_size;
		result = keyboard_device->SetProperty(DIPROP_BUFFERSIZE, &(buffer_size_property.diph));
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateKeyboardDevice() -- Failed to change the default buffer size for the keyboard device.");
		}
		// Return the unacquired keyboard device.
		return keyboard_device;
	}


	// See function declaration for details.
	LPDIRECTINPUTDEVICE8 const CreateMouseDevice(LPDIRECTINPUT8 const dinput, HWND window_handle, const unsigned int buffer_size)
	{
		if(dinput == nullptr)
		{
			throw new utility::InvalidArgumentException("avl::input::dinput::CreateMouseDevice()", "dinput", "Must be non-nullptr");
		}
		// Create the mouse device.
		HRESULT result;
		LPDIRECTINPUTDEVICE8 mouse_device = nullptr;
		result = dinput->CreateDevice(GUID_SysMouse, &mouse_device, nullptr);
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateMouseDevice() -- Unable to create DirectInput8 mouse device.");
		}
		// Set the mouse data format.
		result = mouse_device->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateMouseDevice() -- Unable to set the mouse device data format.");
		}
		// Set the mouse device cooperative level.
		result = mouse_device->SetCooperativeLevel(window_handle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateMouseDevice() -- Unable to set the mouse device cooperative level.");
		}
		// Change the mouse device buffer size.
		DIPROPDWORD buffer_size_property;
		buffer_size_property.diph.dwSize = sizeof(DIPROPDWORD);
		buffer_size_property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		buffer_size_property.diph.dwObj = 0;
		buffer_size_property.diph.dwHow = DIPH_DEVICE;
		buffer_size_property.dwData = buffer_size;
		result = mouse_device->SetProperty(DIPROP_BUFFERSIZE, &(buffer_size_property.diph));
		if (FAILED(result))
		{
			throw utility::Exception("avl::input::dinput::CreateMouseDevice() -- Failed to change the default buffer size for the mouse device.");
		}
		// Return the unacquired mouse device.
		return mouse_device;
	}

	// See function declaration for details.
	bool RetrieveDeviceData(LPDIRECTINPUTDEVICE8 device, std::queue<DIDEVICEOBJECTDATA>& data_queue)
	{
		const DWORD BATCH_SIZE = 15;
		// Holds buffered input data.
		std::unique_ptr<DIDEVICEOBJECTDATA[]> data(new(std::nothrow) DIDEVICEOBJECTDATA[BATCH_SIZE]);
		if(data == nullptr)
		{
			throw utility::OutOfMemoryError();
		}
		// Keeps track of how many elements have been read.
		DWORD number_of_elements;
		// Harvest all of the buffered input data from the device.
		HRESULT result;
		do
		{
			// Read in a batch of data.
			number_of_elements = BATCH_SIZE;
			result = device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), data.get(), &number_of_elements, 0);
			// Was there a failure other than an overflow?
			if(FAILED(result) && result != DI_BUFFEROVERFLOW)
			{
				// Attempt to re-acquire the device.
				result = device->Acquire();
				if (FAILED(result))
				{
					// The device can't be re-acquired yet. Signal the failure.
					return false;
				}
				// We've successfully re-acquired the device. Continue collecting data.
				number_of_elements = BATCH_SIZE;
				continue;
			}
			// Stick the new events in the queue.
			for(DWORD i = 0; i < number_of_elements; ++i)
			{
				data_queue.push(data[i]);
			}
		}while(number_of_elements != 0);
		// Data collected. Return success.
		return true;
	}



	// See declaration for details.
	utility::key_codes::MouseButton::MouseButtonCodes DIKToMB(const DWORD& button)
	{
		using utility::key_codes::MouseButton;
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

	// See declaration for details.
	utility::key_codes::KeyboardKey::KeyboardKeyCodes DIKToKK(const DWORD& key)
	{
		using utility::key_codes::KeyboardKey;
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

} // dinput
} // input
} // avl
