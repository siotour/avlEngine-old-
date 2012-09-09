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
Implementation for the xaudio2 wrapper component. See "xaudio2 wrapper.h" for details.
@author Sheldon Bachstein
@date Jun 24, 2012
*/

#include"xaudio2 wrapper.h"
#include<xaudio2.h>
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"


namespace avl
{
namespace sound
{
namespace xaudio2
{
	// See function definitions below for details.
	namespace
	{
		/**
		*/
		class BufferContextDeleter: public IXAudio2VoiceCallback
		{
		public:
			/**
			*/
			void __stdcall OnBufferEnd(void* buffer_context);
			/** */
			void __stdcall OnBufferStart(void* buffer_context);
			/** */
			void __stdcall OnLoopEnd(void* buffer_context);
			/** */
			void __stdcall OnStreamEnd();
			/** */
			void __stdcall OnVoiceError(void* buffer_context, HRESULT	error);
			/** */
			void __stdcall OnVoiceProcessingPassEnd();
			/** */
			void __stdcall OnVoiceProcessingPassStart(UINT32 bytes_required);
		};

		BufferContextDeleter buffer_context_deleter;
	}




	// See function declaration for details.
	IXAudio2* const CreateXAudio2()
	{
		IXAudio2* xaudio2 = nullptr;
		HRESULT result = XAudio2Create(&xaudio2);
		if(FAILED(result))
		{
			throw utility::Exception("avl::sound::xaudio2::CreateXAudio2() -- Unable to create the IXAudio2 interface.");
		}
		return xaudio2;
	}

	// See function declaration for details.
	IXAudio2MasteringVoice* const CreateMasteringVoice(IXAudio2& xaudio2)
	{
		IXAudio2MasteringVoice* voice = nullptr;
		HRESULT result = xaudio2.CreateMasteringVoice(&voice);
		if(FAILED(result))
		{
			throw utility::Exception("avl::sound::xaudio2::CreateMasteringVoice() -- Unable to create the mastering voice.");
		}
		return voice;
	}

	// See function declaration for details.
	IXAudio2SourceVoice* const CreateSourceVoice(IXAudio2& xaudio2, const WAVEFORMATEX format)
	{
		IXAudio2SourceVoice* voice;
		// Use a BufferContextDeleter to clean up the BufferContexts for any buffers
		// submitted to this voice.
		HRESULT result = xaudio2.CreateSourceVoice(&voice, &format, 0, 1.0f, &buffer_context_deleter);
		if(FAILED(result))
		{
			throw utility::Exception("avl::sound::xaudio2::CreateSourceVoice() -- Unable to create the source voice.");
		}
		return voice;
	}

	// See function declaration for details.
	const bool UpdateVoice(IXAudio2SourceVoice& voice, XAUDIO2_BUFFER& buffer, utility::SoundEffect& effect)
	{
		voice.SetVolume(effect.GetVolume());
		XAUDIO2_VOICE_STATE	voice_state;
		voice.GetState(&voice_state/*, XAUDIO2_VOICE_NOSAMPLESPLAYED*/);
		BufferContext* const buffer_context = reinterpret_cast<BufferContext*>(voice_state.pCurrentBufferContext);

		if(effect.IsPlaying() == true)
		{
			if(buffer_context != nullptr && (effect.IsReset() == true || effect.GetSoundHandle() != buffer_context->sound_handle))
			{
				// Reset buffer & continue playing.
				PlayBuffer(voice, buffer, effect);
				effect.Reset(false);
			}
			else if(voice_state.BuffersQueued < 1 || buffer_context == nullptr)
			{
				if(effect.IsLooping() == true)
				{
					// Reset buffer & continue playing.
					PlayBuffer(voice, buffer, effect);
				}
				else
				{
					// Delete voice.
					voice.Stop();
					voice.DestroyVoice();
					effect.Pause();
					return true;
				}
			}
			else
			{
				if(effect.IsLooping() == buffer_context->is_looping)
				{
					// Continue playing.
					voice.Start();
				}
				else
				{
					// Toggle loop & resume playing.
					ResumeBuffer(voice, buffer, effect);
				}
			}
		}
		// At this point: effect.IsPlaying() == false
		else
		{
			if(buffer_context != nullptr && effect.IsReset() == false && effect.GetSoundHandle() == buffer_context->sound_handle && effect.IsLooping() == buffer_context->is_looping && voice_state.BuffersQueued > 0)
			{
				// Stop the voice & pause the effect.
				voice.Stop();
			}
			else
			{
				// Delete voice.
				voice.Stop();
				voice.FlushSourceBuffers();
				voice.DestroyVoice();
				effect.Reset(false);
				return true;
			}
		}
		return false;
	}

	// See function declaration for details.
	void ExtractPCMFormatData(const SoundSample& sample, WAVEFORMATEX& format)
	{
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.wBitsPerSample = sample.GetBitDepth();
		format.nChannels = sample.GetNumberOfChannels();
		format.nSamplesPerSec = sample.GetFrequency();
		format.nBlockAlign = (WORD)(format.nChannels * format.wBitsPerSample / 8);
		format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;
		format.cbSize = 0;
	}

	// See function declaration for details.
	void CreateBuffer(const SoundSample& sample, XAUDIO2_BUFFER& buffer)
	{
		// Create a deep copy of the audio sample data.
		std::unique_ptr<char[]> audio_data(new(std::nothrow) char[sample.GetDataSize()]);
		if(audio_data == false)
		{
			throw utility::OutOfMemoryError();
		}
		memcpy(audio_data.get(), sample.GetAudioData(), sample.GetDataSize());
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.PlayLength = (sample.GetDataSize() * 8) / (sample.GetBitDepth() * sample.GetNumberOfChannels());
		buffer.AudioBytes = sample.GetDataSize();
		buffer.pAudioData = reinterpret_cast<const BYTE* const>(audio_data.release());
	}

	// See function declaration for details.
	void PlayBuffer(IXAudio2SourceVoice& voice, XAUDIO2_BUFFER& buffer, const utility::SoundEffect& effect)
	{
		HRESULT result = voice.Stop();
		if(FAILED(result))
		{
			throw utility::Exception("avl::sound::xaudio2::PlayBuffer() -- Unable to stop source voice.");
		}
		// Discard all buffers.
		result = voice.FlushSourceBuffers();
		if(FAILED(result))
		{
			throw utility::Exception("avl::sound::xaudio2::PlayBuffer() -- Unable to flush buffers.");
		}
		// Save some contextual information about the sound effect's state for this buffer.
		BufferContext* context = new(std::nothrow) BufferContext(effect.IsLooping(), effect.GetSoundHandle());
		if(context == nullptr)
		{
			throw utility::OutOfMemoryError();
		}
		buffer.pContext = context;
		buffer.LoopCount = (effect.IsLooping() == true) ? XAUDIO2_MAX_LOOP_COUNT : 0;
		result = voice.SubmitSourceBuffer(&buffer);
		if(FAILED(result))
		{
			delete context;
			throw utility::Exception("avl::sound::xaudio2::PlayBuffer() -- Unable to submit buffer.");
		}
		voice.Start();
		if(FAILED(result))
		{
			throw utility::Exception("avl::sound::xaudio2::PlayBuffer() -- Unable to start source voice.");
		}
	}

	// See function declaration for details.
	void ResumeBuffer(IXAudio2SourceVoice& voice, XAUDIO2_BUFFER& buffer, const utility::SoundEffect& effect)
	{
		XAUDIO2_VOICE_STATE voice_state;
		voice.GetState(&voice_state);
		// Resume the buffer from the current sample.
		buffer.PlayBegin = (UINT32)(voice_state.SamplesPlayed % buffer.PlayLength);
		PlayBuffer(voice, buffer, effect);
	}





	// See function declaration for details.
	BufferContext::BufferContext(const bool looping, const utility::SoundEffect::SoundHandle handle)
		: is_looping(looping), sound_handle(handle)
	{
	}

	// See function declaration for details.
	void BufferContextDeleter::OnBufferEnd(void* buffer_context)
	{
		if(buffer_context != nullptr)
		{
			delete (BufferContext*)buffer_context;
			buffer_context = nullptr;
		}
	}

	void BufferContextDeleter::OnBufferStart(void* buffer_context){}
	void BufferContextDeleter::OnLoopEnd(void* buffer_context){}
	void BufferContextDeleter::OnStreamEnd(){}
	void BufferContextDeleter::OnVoiceError(void* buffer_context, HRESULT	error){}
	void BufferContextDeleter::OnVoiceProcessingPassEnd(){}
	void BufferContextDeleter::OnVoiceProcessingPassStart(UINT32 bytes_required){}



} // xaudio2
} // sound
} // avl
