#pragma once
#ifndef AVL_MODEL_END_SCENE_LISTENER__
#define AVL_MODEL_END_SCENE_LISTENER__
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
Defines the EndSceneListener class.
@author Sheldon Bachstein
@date Aug 19, 2012
*/

#include"..\agent\agent.h"


namespace avl
{
namespace model
{
	// Forward declaration.
	class EndScene;

	/**
	Listens for EndScene actions and saves the end state.
	*/
	class EndSceneListener: public Agent
	{
	public:
		EndSceneListener();
		~EndSceneListener();

		const bool SceneHasEnded() const;
		const int GetExitCode() const;
		/** Reacts to EndScene actions.
		@param end_scene The action to react to.
		*/
		void EndTheScene(const EndScene& end_scene);

	private:
		bool scene_has_ended;
		int exit_code;

		/// NOT IMPLEMENTED.
		EndSceneListener(const EndSceneListener&);
		/// NOT IMPLEMENTED.
		EndSceneListener& operator=(const EndSceneListener&);
	};



} // model
} // avl
#endif // AVL_MODEL_END_SCENE_LISTENER__
