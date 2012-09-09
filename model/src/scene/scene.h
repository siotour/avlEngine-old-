#pragma once
#ifndef AVL_MODEL_SCENE__
#define AVL_MODEL_SCENE__
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
Defines the Scene interface.
@author Sheldon Bachstein
@date Jun 27, 2012
*/

#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include"..\..\..\utility\src\input events\input events.h"


namespace avl
{
namespace model
{
	/**
	Represents a distinct model space with any number of interactive
	agents.
	*/
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		/** Retrieves the graphics representing this scene.
		@return The graphics representing the scene.
		*/
		virtual const utility::GraphicList GetGraphics() = 0;
		/** Retrieves the sound effects representing this scene.
		@return The sound effects representing the scene.
		*/
		virtual utility::SoundEffectList GetSoundEffects() = 0;
		
		/** Applies input actions to the model space as is
		appropriate.
		*/
		virtual void ProcessInput(utility::input_events::InputQueue& input) = 0;
		
		/** Updates the scene.
		*/
		virtual void Update() = 0;
		/** Pauses the scene so that elapsed time is not accumulated until
		the next call to Update().
		*/
		virtual void Pause() = 0;

		/** Returns true if the scene has ended.
		@return True if the scene has ended, and false if not.
		*/
		virtual const bool HasEnded() = 0;
		/** Returns the exit code for the end of the scene.
		@pre The scene must have ended.
		@return The exit code describing the end of the scene.
		@throws utility::InvalidCallException If the scene has not
		yet ended.
		@sa HasEnded
		*/
		virtual const int GetExitCode() = 0;

	private:
		/// NOT IMPLEMENTED.
		Scene(const Scene&);
		/// NOT IMPLEMENTED.
		const Scene& operator=(const Scene&);

	};




} // model
} // avl
#endif // AVL_MODEL_SCENE__
