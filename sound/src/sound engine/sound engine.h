#pragma once
#ifndef AVL_SOUND_SOUND_ENGINE__
#define AVL_SOUND_SOUND_ENGINE__
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
Defines the \ref avl::sound::SoundEngine interface.
@author Sheldon Bachstein
@date Jun 17, 2012
*/

#include"..\sound sample\sound sample.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"


namespace avl
{
namespace sound
{

	/**
	Provides an interface for submitting audio data to an audio device and
	accessing that audio data via a handle.
	*/
	class SoundEngine
	{
	public:
		/** Basic constructor.
		*/
		SoundEngine();

		/** Basic destructor.
		*/
		virtual ~SoundEngine();

		/** Makes it possible to play \a new_sample using the returned sound handle.
		@param new_sample The sample to be stored internally and accessed via the
		returned sound handle.
		@return The sound handle by which \a new_sample is to be accessed.
		*/
		virtual const utility::SoundEffect::SoundHandle AddSound(const sound::SoundSample& new_sample) = 0;

		/** Removes the sound sample associated with \a handle from memory, making
		it no longer accessible.
		@post Don't try playing any sounds whose sound handles have been deleted.
		@param handle The handle to the sound sample which is to be deleted.
		*/
		virtual void DeleteSound(const utility::SoundEffect::SoundHandle& handle) = 0;

		/** Stops all currently playing sounds and deallocates all memory storing
		sounds and sound data, and renders all currently issued sound handles invalid
		@post Any previously issued sound handles will be rendered invalid, and
		may in the future become associated with different sounds.
		*/
		virtual void ClearSounds() = 0;

		/** Updates the states of the sound effects in \a sounds. Details may
		vary between different implementations.
		@param sound_effects The SoundEffect objects whose states needs to be updated.
		*/
		virtual void UpdateSounds(utility::SoundEffectList& sound_effects) = 0;

	};



} // sound
} // avl
#endif // AVL_SOUND_SOUND_ENGINE__
