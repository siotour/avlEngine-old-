#pragma once
#ifndef AVL_SOUND_XAUDIO2_SOUND_ENGINE__
#define AVL_SOUND_XAUDIO2_SOUND_ENGINE__
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
Defines the \ref XAudio2SoundEngine class.
@author Sheldon Bachstein
@date Jun 18, 2012
*/

#include"..\sound engine\sound engine.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include<map>
#include<xaudio2.h>


namespace avl
{
namespace sound
{

	/**
	Implements the \ref avl::sound::SoundEngine interface using the XAudio2
	API.
	@todo DestroyVoice() is a blocking function, so calling it from within UpdateSounds()
	often is a bad idea. Consider using some voice pooling strategies (i.e. a vector of
	source voices).
	*/
	class XAudio2SoundEngine: public SoundEngine
	{
	public:
		/** Basic constructor.
		*/
		XAudio2SoundEngine();

		/** Basic destructor.
		*/
		~XAudio2SoundEngine();

		/** Makes it possible to play \a new_sample using the returned sound handle.
		@param new_sample The sample to be stored internally and accessed via the
		returned sound handle.
		@return The sound handle by which \a new_sample is to be accessed.
		*/
		const utility::SoundEffect::SoundHandle AddSound(const sound::SoundSample& new_sample);

		/** Removes the sound sample associated with \a handle from memory, making
		it no longer accessible.
		@post Don't try playing any sounds whose sound handles have been deleted.
		@param handle The handle to the sound sample which is to be deleted.
		*/
		void DeleteSound(const utility::SoundEffect::SoundHandle& handle);

		/** Stops all currently playing sounds and deallocates all memory storing
		sounds and sound data, and renders all currently issued sound handles invalid
		@post Any previously issued sound handles will be rendered invalid, and
		may in the future become associated with different sounds.
		*/
		void ClearSounds();

		/** Updates the states of the sound effects in \a sounds. Details may
		vary between different implementations.
		@param sound_effects The SoundEffect objects whose state needs to be updated.
		*/
		void UpdateSounds(utility::SoundEffectList& sound_effects);


	private:

		/** Deletes all unused voices.
		@param sound_effects Used to determine whether or not a voice is still
		in use.
		*/
		void CleanupVoices(utility::SoundEffectList& sound_effects);

		/** Deletes all sound data, voices, and all other resources.
		*/
		void ReleaseResources();

		/// Keeps track of which sound handles have already been issued.
		utility::SoundEffect::SoundHandle next_handle;

		/** Contains all of the information about a sound sample
		necessary to play that sample.
		*/
		typedef std::pair<WAVEFORMATEX, XAUDIO2_BUFFER> SoundData;
		/** Maps sound handles to the format information and buffers which
		represent that sound.
		*/
		typedef std::map<const utility::SoundEffect::SoundHandle, SoundData* const> SoundHandleToSound;

		/** Maps sound effect addresses to source voices, but never dereferences
		these addresses.
		*/
		typedef std::map<const utility::SoundEffect*, IXAudio2SourceVoice* const> SoundEffectToVoice;

		/// The XAudio2 interface.
		IXAudio2* xaudio2;
		/// The mastering voice through which all source voices are played.
		IXAudio2MasteringVoice* mastering_voice;
		/// All currently loaded sounds and their associated sound handles.
		SoundHandleToSound sounds;
		/// All currently active source voices and their associated sound effect
		/// addresses.
		SoundEffectToVoice voices;

		/// NOT IMPLEMENTED.
		XAudio2SoundEngine(const XAudio2SoundEngine&);
		/// NOT IMPLEMENTED.
		const XAudio2SoundEngine& operator=(const XAudio2SoundEngine&);
	};



} // sound
} // avl
#endif // AVL_SOUND_XAUDIO2_SOUND_ENGINE__
