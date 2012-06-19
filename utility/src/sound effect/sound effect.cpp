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
Implementation for the sound effect component. See "sound effect.h" for details.
@author Sheldon Bachstein
@date Jun 17, 2012
*/

#include"sound effect.h"


namespace avl
{
namespace utility
{
	// See method declaration for details.
	SoundEffect::SoundEffect(const SoundEffect::SoundHandle handle)
		: sound_handle(handle), volume(1.0f), is_playing(false), is_looping(false), reset(false)
	{
	}

	// See method declaration for details.	
	SoundEffect::~SoundEffect()
	{
	}

	// See method declaration for details.	
	void SoundEffect::SetSoundHandle(const SoundEffect::SoundHandle new_handle)
	{
		sound_handle = new_handle;
	}
		
	// See method declaration for details.	
	void SoundEffect::SetVolume(const float& new_volume)
	{
		volume = new_volume;
		// Make sure that the volume is normalized.
		if(volume < 0.0f)
		{
			volume = 0.0f;
		}
		else if(volume > 1.0f)
		{
			volume = 1.0f;
		}
	}
		
	// See method declaration for details.	
	void SoundEffect::Play(const bool loop = false)
	{
		is_playing = true;
		is_looping = loop;
	}
		
	// See method declaration for details.	
	void SoundEffect::Pause()
	{
		is_playing = false;
	}
		
	// See method declaration for details.	
	void SoundEffect::Stop()
	{
		is_playing = false;
		reset = true;
	}

	// See method declaration for details.	
	void SoundEffect::Reset(const bool set_reset = true)
	{
		reset = set_reset;
	}
		
	// See method declaration for details.	
	const SoundEffect::SoundHandle SoundEffect::GetSoundHandle() const
	{
		return sound_handle;
	}
		
	// See method declaration for details.	
	const float SoundEffect::GetVolume() const
	{
		return volume;
	}

	// See method declaration for details.
	const bool SoundEffect::IsPlaying() const
	{
		return is_playing;
	}
		
	// See method declaration for details.	
	const bool SoundEffect::IsLooping() const
	{
		return is_looping;
	}
		
	// See method declaration for details.
	const bool SoundEffect::IsReset() const
	{
		return reset;
	}




} // utility
} // avl
