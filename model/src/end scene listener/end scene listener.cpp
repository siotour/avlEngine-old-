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
Implementation for the end scene listener component. See "end scene listener.h" for details.
@author Sheldon Bachstein
@date Aug 19, 2012
*/

#include"end scene listener.h"
#include"..\agent\agent.h"
#include"..\action\action.h"


namespace avl
{
namespace model
{
	// See method declaration for details.
	EndSceneListener::EndSceneListener()
		: exit_code(0), scene_has_ended(false)
	{
		RegisterReaction(*this, &EndSceneListener::EndTheScene);
	}

	// See method declaration for details.
	EndSceneListener::~EndSceneListener()
	{
	}

	// See method declaration for details.
	const bool EndSceneListener::SceneHasEnded() const
	{
		return scene_has_ended;
	}

	// See method declaration for details.
	const int EndSceneListener::GetExitCode() const
	{
		return exit_code;
	}

	// See method declaration for details.
	void EndSceneListener::EndTheScene(const EndScene& end_scene)
	{
		scene_has_ended = true;
		exit_code = end_scene.GetExitCode();
	}



} // model
} // avl
