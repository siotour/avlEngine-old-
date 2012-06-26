#pragma once
#ifndef AVL_SOUND_SOUND_SAMPLE__
#define AVL_SOUND_SOUND_SAMPLE__
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
Defines the \ref avl::sound::SoundSample class.
@author Sheldon Bachstein
@date Jun 17, 2012
*/

#include<string>
#include<memory>

namespace avl
{
namespace sound
{
	/**
	Contains the data necessary to play a Pulse-Code Modulation sound
	sample.
	*/
	class SoundSample
	{
	public:
		/** Full-spec constructor.
		@param depth The bit depth of each sample.
		@param frequency The number of samples per second.
		@param num_channels The number of audio channels.
		@param size The size of the audio data in bytes.
		@param data The audio data.
		*/
		SoundSample(const unsigned short& depth, const unsigned int frequency, const unsigned int num_channels, const std::size_t size, const char data[]);
		
		/** Copy constructor; transfers ownership of the audio data from
		\a original to the new sample.
		@attention Ownership of the data which \a original contained is
		transferred to the newly constructed object.
		@post The audio data of \a original is set to nullptr and each other
		property is set to zero.
		@param original The object being copied and which loses ownership
		of the audio data.
		*/
		SoundSample(SoundSample& original);

		/** Basic destructor.
		*/
		~SoundSample();
		
		/** Accesses the bit depth of the sample.
		@return The bit depth of the sample.
		*/
		const unsigned short GetBitDepth() const;
		
		/** Accesses the sampling frequency
		@return The number of samples per second.
		*/
		const unsigned int GetFrequency() const;

		/** Accesses the number of channels in the sample.
		@return The number of audio channels sampled.
		*/
		const unsigned short GetNumberOfChannels() const;

		/** Accesses the sample data size in bytes.
		@return The size of the sample's audio data in bytes.
		*/
		const std::size_t GetDataSize() const;
		
		/** Accesses the raw audio data of the sample.
		@return The audio data for the sample.
		*/
		const char* const GetAudioData() const;

	private:
		/// The bit depth of the sample.
		unsigned short bit_depth;
		/// The number of samples per second.
		unsigned int sampling_frequency;
		/// The number of channels sampled.
		unsigned short number_of_channels;
		/// The size of the sample's audio data.
		std::size_t data_size;
		/// The raw audio data.
		std::unique_ptr<const char[]> audio_data;

		/// NOT IMPLEMENTED.
		SoundSample();
		/// NOT IMPLEMENTED.
		const SoundSample& operator=(const SoundSample&);
	};



} // sound
} // avl
#endif // AVL_SOUND_SOUND_SAMPLE__
