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
Implementation for the xaudio2 sound engine component. See "xaudio2 sound engine.h" for details.
@author Sheldon Bachstein
@date Jun 18, 2012
*/

#include"xaudio2 sound engine.h"
#include"..\sound engine\sound engine.h"
#include"..\xaudio2 wrapper\xaudio2 wrapper.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include<map>
#include<algorithm>
#include<new>
#include<memory>
#include<cstring>
#include<xaudio2.h>



namespace avl
{
namespace sound
{


	// See method declaration for details.
	XAudio2SoundEngine::XAudio2SoundEngine()
		: next_handle(1), xaudio2(nullptr), mastering_voice(nullptr)
	{
		try
		{
			CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			xaudio2 = xaudio2::CreateXAudio2();
			mastering_voice = xaudio2::CreateMasteringVoice(*xaudio2);
		}
		catch(...)
		{
			ReleaseResources();
			throw;
		}
	}

	// See method declaration for details.
	XAudio2SoundEngine::~XAudio2SoundEngine()
	{
		ReleaseResources();
	}

	// See method declaration for details.
	const utility::SoundEffect::SoundHandle XAudio2SoundEngine::AddSound(const sound::SoundSample& new_sample)
	{
		// Check for valid audio data.
		if(new_sample.GetAudioData() == nullptr)
		{
			throw utility::InvalidArgumentException("avl::sound::XAudio2SoundEngine::AddSound()", "new_sample", "Must contain a non-null audio data pointer.");
		}
		// Make sure that the sample's bit depth is acceptable.
		if(new_sample.GetBitDepth() != 8 && new_sample.GetBitDepth() != 16 && new_sample.GetBitDepth() != 32)
		{
			throw utility::InvalidArgumentException("avl::sound::XAudio2SoundEngine::AddSound()", "new_sample", "Must have a bit depth of 8, 16, or 32.");
		}
		// Make sure that the entire sample will fit in a buffer.
		if(new_sample.GetDataSize() > XAUDIO2_MAX_BUFFER_BYTES)
		{
			throw utility::InvalidArgumentException("avl::sound::XAudio2SoundEngine::AddSound()", "new_sample", "Contains too much data to fit into a buffer.");
		}
		// Save the format information and create a buffer.
		std::unique_ptr<SoundData> sound_data(new(std::nothrow) SoundData());
		if(sound_data == false)
		{
			throw utility::OutOfMemoryError();
		}
		xaudio2::ExtractPCMFormatData(new_sample, sound_data->first);
		xaudio2::CreateBuffer(new_sample, sound_data->second);
		// Save the sound sample's data and issue a new texture handle. Clean up if this fails.
		try
		{
			sounds.insert(std::make_pair(next_handle, sound_data.get()));
			sound_data.release();
		}
		catch(...)
		{
			delete[] reinterpret_cast<const char*>(sound_data->second.pAudioData);
			throw;
		}
		// Return the handle for the new sample, and prepare for the next handle.
		return next_handle++;
	}

	// See method declaration for details.
	void XAudio2SoundEngine::DeleteSound(const utility::SoundEffect::SoundHandle& handle)
	{
		SoundHandleToSound::iterator element = sounds.find(handle);
		if(element != sounds.end())
		{
			// Delete the audio data.
			delete[] reinterpret_cast<const char*>(element->second->second.pAudioData);
			element->second->second.pAudioData = NULL;
			// Delete the SoundData object.
			delete element->second;
			// Erase this entry.
			sounds.erase(element);
		}
	}

	// See method declaration for details.
	void XAudio2SoundEngine::ClearSounds()
	{
		// Unload all of the currently loaded sound data.
		for(SoundHandleToSound::iterator i = sounds.begin(); i != sounds.end(); ++i)
		{
			if(i->second->second.pAudioData != NULL)
			{
				delete[] reinterpret_cast<const unsigned char*>(i->second->second.pAudioData);
				i->second->second.pAudioData = NULL;
			}
			delete i->second;
		}
		sounds.clear();
		// Destroy all of the source voices.
		for(SoundEffectToVoice::iterator i = voices.begin(); i != voices.end(); ++i)
		{
			i->second->Stop(0);
			i->second->FlushSourceBuffers();
			i->second->DestroyVoice();
		}
		voices.clear();
		// Begin issuing sound handles from 1 again.
	}

	// See method declaration for details.
	void XAudio2SoundEngine::UpdateSounds(utility::SoundEffectList& sound_effects)
	{
		SoundEffectToVoice::iterator voice;
		SoundHandleToSound::iterator sound;
		for(utility::SoundEffectList::iterator effect = sound_effects.begin(); effect != sound_effects.end(); ++effect)
		{
			sound = sounds.find((*effect)->GetSoundHandle());
			if(sound == sounds.end())
			{
				throw utility::InvalidArgumentException("avl::sound::XAudio2SoundEngine::UpdateSounds()", "sound_effects", "One or more sound effects contain an invalid sound handle.");
			}
			voice = voices.find(*effect);
			if(voice != voices.end())
			{
				if(xaudio2::UpdateVoice(*(voice->second), sound->second->second, *(*effect)) == true)
				{
					voices.erase(voice);
				}
			}
			// voice == voices.end()
			else
			{
				if((*effect)->IsPlaying() == true)
				{
					// Create new voice.
					IXAudio2SourceVoice* new_sound = xaudio2::CreateSourceVoice(*xaudio2, sound->second->first);
					voices.insert(std::make_pair(*effect, new_sound));
					// Prepare and submit buffer.
					xaudio2::PlayBuffer(*new_sound, sound->second->second, *(*effect));
					(*effect)->Reset(false);
				}
			}
		}
		// Clean up source voices which have finished playing and which haven't been updated.
		CleanupVoices(sound_effects);
	}

	// See method declaration for details.
	void XAudio2SoundEngine::CleanupVoices(utility::SoundEffectList& sound_effects)
	{
		// Iterate through voices and find any finished (buffers < 1) voices
		// which don't have a corresponding sound sample in sounds -- delete them.
		XAUDIO2_VOICE_STATE voice_state;
		for(auto voice = voices.begin(); voice != voices.end(); ++voice)
		{
			voice->second->GetState(&voice_state/*, XAUDIO2_VOICE_NOSAMPLESPLAYED*/);
			if(voice_state.BuffersQueued < 1)
			{
				utility::SoundEffectList::const_iterator effect = std::find(sound_effects.begin(), sound_effects.end(), voice->first);
				if(effect == sound_effects.end())
				{
					// Delete voice.
					voice->second->Stop();
					voice->second->DestroyVoice();
					voices.erase(voice);
				}
			}
		}
	}

	// See method declaration for details.
	void XAudio2SoundEngine::ReleaseResources()
	{
		// Clear all sound data and voices.
		ClearSounds();
		// Destroy the mastering voice.
		if(mastering_voice != NULL)
		{
			mastering_voice->DestroyVoice();
			mastering_voice = NULL;
		}
		// Destroy the XAudio2 interface.
		if(xaudio2 != NULL)
		{
			xaudio2->Release();
			xaudio2 = NULL;
		}
		CoUninitialize();
	}



} // sound
} // avl
