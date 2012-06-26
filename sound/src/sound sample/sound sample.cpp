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
namespace sound
{
	// See method declaration for details.
	SoundSample::SoundSample(const unsigned short& depth, const unsigned int frequency, const unsigned int num_channels, const std::size_t size, const char* const data)
		: bit_depth(depth), sampling_frequency(frequency), number_of_channels(num_channels), data_size(size), audio_data(data)
	{
	}

	// See method declaration for details.
	SoundSample::SoundSample(SoundSample& original)
		: bit_depth(original.bit_depth), sampling_frequency(original.sampling_frequency), number_of_channels(original.number_of_channels), data_size(original.data_size), audio_data(original.audio_data.release())
	{
		original.bit_depth = 0;
		original.sampling_frequency = 0;
		original.number_of_channels = 0;
		original.data_size = 0;
	}
		
	// See method declaration for details.
	SoundSample::~SoundSample()
	{
	}
		
	// See method declaration for details.
	const unsigned short SoundSample::GetBitDepth() const
	{
		return bit_depth;
	}
		
	// See method declaration for details.
	const unsigned int SoundSample::GetFrequency() const
	{
		return sampling_frequency;
	}

	// See method declaration for details.
	const unsigned short SoundSample::GetNumberOfChannels() const
	{
		return number_of_channels;
	}

	// See method declaration for details.
	const std::size_t SoundSample::GetDataSize() const
	{
		return data_size;
	}
		
	// See method declaration for details.
	const char* const SoundSample::GetAudioData() const
	{
		return audio_data.get();
	}




} // sound
} // avl
