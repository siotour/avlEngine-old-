#pragma once
#ifndef AVL_SOUND_XAUDIO2_WRAPPER__
#define AVL_SOUND_XAUDIO2_WRAPPER__
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
Provides functions and structures for performing common XAudio2 tasks.
@author Sheldon Bachstein
@date Jun 24, 2012
@todo Document the function declarations in here.
*/

#include<xaudio2.h>
#include"..\sound sample\sound sample.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include<memory>

namespace avl
{
namespace sound
{
namespace xaudio2
{
	/**
	*/
	IXAudio2* const CreateXAudio2();

	/**
	*/
	IXAudio2MasteringVoice* const CreateMasteringVoice(IXAudio2& xaudio2);

	/**
	*/
	IXAudio2SourceVoice* const CreateSourceVoice(IXAudio2& xaudio2, const WAVEFORMATEX format);

	/**
	*/
	const bool UpdateVoice(IXAudio2SourceVoice& voice, XAUDIO2_BUFFER& buffer, utility::SoundEffect& effect);

	/**
	*/
	void ExtractPCMFormatData(const SoundSample& sample, WAVEFORMATEX& format);

	/**
	*/
	void CreateBuffer(const SoundSample& sample, XAUDIO2_BUFFER& buffer);

	/**
	*/
	void PlayBuffer(IXAudio2SourceVoice& voice, XAUDIO2_BUFFER& buffer, const utility::SoundEffect& effect);

	/**
	*/
	void ResumeBuffer(IXAudio2SourceVoice& voice, XAUDIO2_BUFFER& buffer, const utility::SoundEffect& effect);

	/**
	*/
	struct BufferContext
	{
	public:
		BufferContext(const bool looping, const utility::SoundEffect::SoundHandle handle);
		const bool is_looping;
		const utility::SoundEffect::SoundHandle sound_handle;
	};



} // xaudio2
} // sound
} // avl
#endif // AVL_SOUND_XAUDIO2_WRAPPER__
