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
Implementation for the basic scene component. See "basic scene.h" for details.
@author Sheldon Bachstein
@date Jun 30, 2012
*/

#include"basic scene.h"
#include"..\agent\agent.h"
#include"..\action\action.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include"..\..\..\utility\src\input events\input events.h"
#include"..\..\..\utility\src\timer\timer.h"
#include<list>
#include<memory>


namespace avl
{
namespace model
{

	// See method declaration for details.
	BasicScene::BasicScene(const double& initial_time_step, const utility::Vertex2D& screen_space)
		: time_step(initial_time_step), screen_space_resolution(screen_space)
	{

	}
	
	// See method declaration for details.
	BasicScene::~BasicScene()
	{
	}
	
	// See method declaration for details.
	utility::SpriteList BasicScene::GetSprites()
	{

		// Insert all of the sprites from the scene into a single list.
		utility::SpriteList sprites;
		if(agents.empty() == false)
		{
			std::for_each(agents.begin(), agents.end(), [&sprites](const std::auto_ptr<Agent>& agent)
			{
				if(agent->GetSprites().empty() == false)
				{
					sprites.insert(sprites.end(), agent->GetSprites().begin(), agent->GetSprites().end());
				}
			});
		}
		return sprites;
	}
	
	// See method declaration for details.
	utility::SoundEffectList BasicScene::GetSoundEffects()
	{
		// Insert all of the sound effects from the scene into a single list.
		utility::SoundEffectList sound_effects;
		if(agents.empty() == false)
		{
			std::for_each(agents.begin(), agents.end(), [&sound_effects](const std::auto_ptr<Agent>& agent)
			{
				if(agent->GetSoundEffects().empty() == false)
				{
					sound_effects.insert(sound_effects.end(), agent->GetSoundEffects().begin(), agent->GetSoundEffects().end());
				}
			});
		}
		return sound_effects;
	}
	
	// See method declaration for details.
	void BasicScene::Update()
	{
		// Keeps track of time which could not be partitioned into
		// discrete time steps.
		static double accumulated_time = 0.0;
		// Get the elapsed time.
		timer.Unpause();
		accumulated_time += timer.Reset();
		// Update the agents.
		if(accumulated_time > time_step && accumulated_time > 0.0)
		{
			// No time step specified? Use the entire elapsed time.
			if(time_step <= 0.0)
			{
				DistributeAction(TimeStep(accumulated_time));
				accumulated_time = 0.0;
			}
			// Time step has been specified; update in steps.
			else
			{
				while(accumulated_time >= time_step)
				{
					DistributeAction(TimeStep(time_step));
					accumulated_time -= time_step;
				}
			}
		}
	}

	// See method declaration for details.
	void BasicScene::Pause()
	{
		timer.Pause();
	}

	// See method declaration for details.
	void BasicScene::DistributeAction(const Action& action)
	{
		if(agents.empty() == false)
		{
			std::for_each(agents.begin(), agents.end(), [&action](std::auto_ptr<Agent>& agent)
			{
				agent->React(action);
			});
		}
	}


} // model
} // avl
