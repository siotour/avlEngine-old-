#pragma once
#ifndef AVL_SOUND_LOAD_WAV_FILE__
#define AVL_SOUND_LOAD_WAV_FILE__
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
Provides functionality for loading WAV files.
@note Currently only supports uncompressed PCM format.
@author Sheldon Bachstein
@date Jun 23, 2012
*/

#include"..\sound sample\sound sample.h"
#include<string>

namespace avl
{
namespace sound
{

	/**
	@todo Make use of the (soon to be added) utility::FileFormatException.
	*/
	SoundSample LoadWAVFile(const std::string& file_name);



} // sound
} // avl
#endif // AVL_SOUND_LOAD_WAV_FILE__
