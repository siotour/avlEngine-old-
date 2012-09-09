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
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include"..\..\..\utility\src\input events\input events.h"
#include"..\..\..\utility\src\timer\timer.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include<list>
#include<memory>


namespace avl
{
namespace model
{

	// See method declaration for details.
	BasicScene::BasicScene(const double& initial_time_step, const utility::Vector& screen_space)
		: time_step(initial_time_step), screen_space_resolution(screen_space)
	{
	}
	
	// See method declaration for details.
	BasicScene::~BasicScene()
	{
		for(auto i = agents.begin(); i != agents.end(); ++i)
		{
			delete *i;
		}
	}
	
	// See method declaration for details.
	const utility::GraphicList BasicScene::GetGraphics()
	{
		// Insert all of the graphics from the scene into a single list.
		utility::GraphicList compiled_graphics;
		const utility::GraphicList* current_agent_graphics;
		if(agents.empty() == false)
		{
			for(auto current_agent = agents.begin(); current_agent != agents.end(); ++current_agent)
			{
				current_agent_graphics = &((*current_agent)->GetGraphics());
				if(current_agent_graphics->empty() == false)
				{
					compiled_graphics.insert(compiled_graphics.end(), current_agent_graphics->begin(), current_agent_graphics->end());
				}
			}
		}
		return compiled_graphics;
	}
	
	// See method declaration for details.
	utility::SoundEffectList BasicScene::GetSoundEffects()
	{
		// Insert all of the sound effects from the scene into a single list.
		utility::SoundEffectList sound_effects;
		if(agents.empty() == false)
		{
			for(auto i = agents.begin(); i != agents.end(); ++i)
			{
				if((*i)->GetSoundEffects().empty() == false)
				{
					sound_effects.insert(sound_effects.end(), (*i)->GetSoundEffects().begin(), (*i)->GetSoundEffects().end());
				}
			}
		}
		return sound_effects;
	}
	
	// See method declaration for details.
	void BasicScene::Update()
	{
		DistributeEnqueuedActions();
		// Keeps track of time which could not be partitioned into
		// discrete time steps.
		static double accumulated_time = 0.0;
		// Get the elapsed time.
		timer.Unpause();
		accumulated_time += timer.Reset();
		// Update the agents.
		if(accumulated_time > time_step)
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
	const bool BasicScene::HasEnded()
	{
		return end_listener.SceneHasEnded();
	}

	// See method declaration for details.
	const int BasicScene::GetExitCode()
	{
		if(end_listener.SceneHasEnded() == false)
		{
			throw utility::InvalidCallException("avl::model::BasicScene::GetExitCode", "The scene has not yet ended.");
		}
		return end_listener.GetExitCode();
	}

	// See method declaration for details.
	void BasicScene::AddAgent(Agent* const agent)
	{
		if(agent == nullptr)
		{
			throw utility::InvalidArgumentException("avl::model::BasicScene::AddAgent", "agent", "Must not be a null pointer.");
		}
		bool result;
		try
		{
			auto result_pair = agents.insert(agent);
			result = result_pair.second;
		}
		catch(const std::bad_alloc&)
		{
			delete agent;
			throw utility::OutOfMemoryError();
		}
		catch(...)
		{
			delete agent;
			throw;
		}

		if(result == false)
		{
			throw utility::InvalidArgumentException("avl::model::BasicScene::AddAgent", "agent", "The specified agent already exists in the scene.");
		}
	}

	// See method declaration for details.
	void BasicScene::RemoveAgent(Agent* const agent)
	{
		if(agent == nullptr)
		{
			throw utility::InvalidArgumentException("avl::model::BasicScene::RemoveAgent", "agent", "Must not be a null pointer.");
		}
		auto position = agents.find(agent);
		if(position != agents.end())
		{
			delete agent;
			agents.erase(position);
		}
	}

	// See method declaration for details.
	void BasicScene::DistributeAction(const Action& action)
	{
		for(auto i = agents.begin(); i != agents.end(); ++i)
		{
			(*i)->React(action);
		}
		end_listener.React(action);
	}

	// See method declaration for details.
	void BasicScene::DistributeEnqueuedActions()
	{
		for(auto agent = agents.begin(); agent != agents.end(); ++agent)
		{
			ActionQueue& actions = (*agent)->GetActions();
			while(actions.empty() == false)
			{
				DistributeAction(actions.front());
				actions.pop();
			}
		}
	}


} // model
} // avl
