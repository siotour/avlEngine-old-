#pragma once
#ifndef AVL_UTILITY_SOUND_EFFECT__
#define AVL_UTILITY_SOUND_EFFECT__
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
Defines the \ref avl::utility::SoundEffect class and its associated
\ref avl::utility::SoundEffectList container.
@author Sheldon Bachstein
@date Jun 17, 2012
*/

#include<list>

namespace avl
{
namespace utility
{
	// Forward declaration.
	class SoundEffect;

	/** Used for storing multiple SoundEffect objects in a light, mergeable, and
	sortable container. Intended to be used to send a collection of SoundEffect
	objects to a sound core.
	*/
	typedef std::list<SoundEffect* const> SoundEffectList;

	/**
	Represents a sound effect to be played using a sound engine.
	*/
	class SoundEffect
	{
	public:
		/** Used to associate a SoundEffect object with the sound data it
		represents.
		*/
		typedef unsigned int SoundHandle;

		/** Basic constructor.
		@post \ref sound_handle is initialized to 0, the new effect will be paused,
		unlooping, and have a volume of 1.0f.
		*/
		SoundEffect();

		/** Basic constructor.
		@post The newly constructor effect will be paused, unlooping, and have
		a volume of 1.0f.
		@param handle The sound which this object represents.
		*/
		SoundEffect(const SoundHandle handle);

		/** Basic destructor.
		*/
		~SoundEffect();

		/** Associates this object with a new sound.
		@param new_handle The handle to the new sound.
		*/
		void SetSoundHandle(const SoundHandle new_handle);
		
		/** Sets the volume for this sound effect.
		@note Valid values of \a new_volume range from 0.0 to 1.0. Values
		outside of this range will be capped to the nearest valid value.
		@param new_volume The new volume.
		*/
		void SetVolume(const float& new_volume);
		
		/** Plays this sound effect, or resets it if it's already playing.
		@note If left alone by the user, this property will remain set until
		the sound has finished playing and the sound engine updates the
		property.
		@post This function unpauses a sound after Pause() has been called.
		Calling this function while a sound is playing resets the sound to
		play from the beginning.
		*/
		void Play();
		
		/** Pauses this sound effect's playing.
		@post This function pauses a sound after Play() has been called.
		*/
		void Pause();
		
		/** Pauses and resets this sound.
		*/
		void Stop();

		/** Turns looping on or off.
		*/
		void Loop(const bool loop);

		/** Changes whether or not this sound effect needs to be reset to
		the beginning.
		@note This property, once set, will stay set until the sound
		engine has processed it.
		@param set_reset The value to assign to the reset property.
		Defaults to true.
		*/
		void Reset(const bool set_reset = true);
		
		/** Returns the sound handle for this sound effect.
		@return The handle to the sound for this sound effect.
		*/
		const SoundHandle GetSoundHandle() const;
		
		/** Returns the volume for this sound effect.
		@return The volume of this sound in the range of 0.0 to 1.0.
		*/
		const float GetVolume()const ;

		/** Is this sound currently playing?
		@return True if this sound is unpaused, and false if it's paused.
		*/
		const bool IsPlaying() const;
		
		/** Is this sound looping?
		@return True if this sound is looping, and false if not.
		*/
		const bool IsLooping() const;
		
		/** Does this sound effect need to be reset to the beginning?
		@return True if this sound effect needs to be reset to the
		beginning, and false if not.
		*/
		const bool IsReset() const;

	private:

		/// The handle to the sound which is represented by this object.
		SoundHandle sound_handle;
		/// The volume of this sound effect, ranging from 0.0 to 1.0.
		float volume;
		/// Is this sound effect currently playing?
		bool is_playing;
		/// Is this sound effect currently looping?
		bool is_looping;
		/// Does this sound effect need to be reset to the beginning?
		bool reset;

	};



} // utility
} // avl
#endif // AVL_UTILITY_SOUND_EFFECT__
