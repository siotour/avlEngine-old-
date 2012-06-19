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
Implementation for the sound sample component. See "sound sample.h" for details.
@author Sheldon Bachstein
@date Jun 17, 2012
*/

#include"sound sample.h"
#include<cstddef>


namespace avl
{
namespace utility
{
	// See method declaration for details.
	SoundSample::SoundSample(const unsigned short& depth, const unsigned int frequency, const unsigned char* const data)
		: byte_depth(depth), sampling_frequency(frequency), audio_data(data)
	{
	}
		
	// See method declaration for details.
	SoundSample::~SoundSample()
	{
		if(audio_data != NULL)
		{
			delete[] audio_data;
			audio_data = NULL;
		}
	}
		
	// See method declaration for details.
	const unsigned short SoundSample::GetByteDepth() const
	{
		return byte_depth;
	}
		
	// See method declaration for details.
	const unsigned int SoundSample::GetFrequency() const
	{
		return sampling_frequency;
	}
		
	// See method declaration for details.
	const unsigned char* const SoundSample::GetAudioData() const
	{
		return audio_data;
	}




} // utility
} // avl
