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
Unit test for the xaudio2 sound engine component. See "xaudio2 sound engine.h" for details.
@author Sheldon Bachstein
@date Jun 18, 2012
*/

#include"xaudio2 sound engine.h"
#include"..\sound sample\sound sample.h"
#include"..\load wav file\load wav file.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include<new>
#include<iostream>
#include<cmath>

using avl::sound::XAudio2SoundEngine;
using avl::sound::SoundSample;
using avl::sound::LoadWAVFile;
using avl::utility::SoundEffect;
using avl::utility::SoundEffectList;

void TestXAudio2SoundEngineComponent()
{
	XAudio2SoundEngine engine;

	SoundSample sample = LoadWAVFile("assets/Try again.wav");

	SoundEffect::SoundHandle handle = engine.AddSound(sample);

	SoundEffect effect(handle);
	effect.Play();
	effect.Loop(true);
	SoundEffectList list;
	list.insert(list.end(), &effect);
	
	std::size_t iterations = 0;
	while(true)
	{
		//++iterations;
		engine.UpdateSounds(list);
	}

	system("pause");
}