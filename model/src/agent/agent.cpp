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
Implementation for the agent component. See "agent.h" for details.
@author Sheldon Bachstein
@date Jun 26, 2012
*/

#include"agent.h"
#include"..\reaction\reaction.h"
#include"..\action\action.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include<map>


namespace avl
{
namespace model
{
	
	// See method declaration for details.
	Agent::Agent()
	{
	}
	
	// See method declaration for details.
	Agent::~Agent()
	{
	}

	// See method declaration for details.
	const utility::SpriteList& Agent::GetSprites() const
	{
		return sprite_list;
	}

	// See method declaration for details.
	const utility::SoundEffectList& Agent::GetSoundEffects() const
	{
		return sound_effect_list;
	}

	// See method declaration for details.
	ActionQueue& Agent::ProcessActions()
	{
		return action_queue;
	}

	// See method declaration for details.
	void Agent::React(const Action& action)
	{
		Reactions::iterator reaction = reactions.find(reaction::TypeInfo(typeid(action)));
		if(reaction != reactions.end())
		{
			reaction->second->React(action);
		}
	}
	
	// See method declaration for details.
	void Agent::RegisterSprite(utility::Sprite& new_sprite)
	{
		sprite_list.push_back(&new_sprite);
	}

	// See method declaration for details.
	void Agent::RegisterSoundEffect(utility::SoundEffect& new_sound_effect)
	{
		sound_effect_list.push_back(&new_sound_effect);
	}

	// See method declaration for details.
	void Agent::EnqueueAction(const Action& action)
	{
		action_queue.push(action);
	}



} // model
} // avl
