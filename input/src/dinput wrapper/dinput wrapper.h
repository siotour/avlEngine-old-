#pragma once
#ifndef AVL_INPUT_DINPUT_WRAPPER__
#define AVL_INPUT_DINPUT_WRAPPER__
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
Implements thin wrapper functions for common Direct Input tasks.
@author Sheldon Bachstein
@date Jun 13, 2012
*/

#include"..\input device\input device.h"
#include"..\key codes\key codes.h"
#include"..\keyboard event\keyboard event.h"
#include"..\mouse button event\mouse button event.h"
#include"..\mouse move event\mouse move event.h"
#include"..\mouse scroll event\mouse scroll event.h"
#include<queue>
/// Defines the direct input version to avoid a compiler warning.
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
#include<dinput.h>


namespace avl
{
namespace input
{
namespace dinput
{
	/** Creates a new Direct Input interface.
	@return A new Direct Input interface.
	@throw utility::Exception If unable to create the interface.
	*/
	LPDIRECTINPUT8 const GetDirectInput();

	/** Creates and initializes a new keyboard interface.
	@param dinput The Direct Input interface to use.
	@param window_handle The handle to the window from which to gather input data.
	@param buffer_size The size of the buffer for the keyboard device.
	@return The newly initialized keyboard device.
	@throw utility::InvalidArgumentException If \a dinput is NULL.
	@throw utility::Exception If unable to create a new keyboard device.
	*/
	LPDIRECTINPUTDEVICE8 const CreateKeyboardDevice(LPDIRECTINPUT8 const dinput, HWND window_handle, const unsigned int buffer_size);

	/** Creates and initializes a new mouse interface.
	@param dinput The Direct Input interface to use.
	@param window_handle The handle to the window from which to gather input data.
	@param buffer_size The size of the buffer for the mouse device.
	@return The newly initialized mouse device.
	@throws utility::InvalidArgumentException If \a dinput is NULL.
	@throws utility::Exception If unable to create a new mouse device.
	*/
	LPDIRECTINPUTDEVICE8 const CreateMouseDevice(LPDIRECTINPUT8 const dinput, HWND window_handle, const unsigned int buffer_size);


	/** Attempts to retrieve the buffered data from an input device. Any data
	extracted from the device's buffer will be queued in \a data_queue.
	@param device The device from which to read input.
	@param data_queue The queue in which to store extracted data.
	@return True if the retrieval was a complete success. False if unable to
	finish retrieving the buffered data. Note that even if false is returned,
	some buffered data may have been extracted.
	@throws utility::OutOfMemoryError If we run out of storage space.
	*/
	bool RetrieveDeviceData(LPDIRECTINPUTDEVICE8 device, std::queue<DIDEVICEOBJECTDATA>& data_queue);



	/** Given a DirectInput mouse button code, returns the equivalent avl mouse button
	code, if one exists.
	@param vkey The direct input virtual key code.
	@return The avl mouse button equivalent of \a vkey.
	*/
	key_codes::MouseButton::MouseButtonCodes DIKToMB(const DWORD& vkey);

	/** Given a DirectInput DIK key code, returns the equivalent avl key code,
	if one exists.
	@param vkey The direct input virtual key code.
	@return The avl keyboard key equivalent to \a vkey.
	*/
	key_codes::KeyboardKey::KeyboardKeyCodes DIKToKK(const DWORD& vkey);

} // dinput
} // input
} // avl
#endif AVL_INPUT_DINPUT_WRAPPER__
