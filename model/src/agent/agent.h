#pragma once
#ifndef AVL_MODEL_AGENT__
#define AVL_MODEL_AGENT__
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
Defines the Agent class.
@author Sheldon Bachstein
@date Jun 26, 2012
*/

#include"..\reaction\reaction.h"
#include"..\action\action.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include<map>

namespace avl
{
namespace model
{

	/**
	Represents an autonomous object within a simulation. Has the abilities
	of displaying some graphic representation, audio representation, reacting
	to Actions within the simulation, and is capable of producing new Actions.
	@todo Consider new methods of passing Actions up the hierarchy (i.e. to
	the enclosing scene).
	@todo Consider embedding a list of quads into this class for collision
	detection.
	*/
	class Agent
	{
	public:
		Agent();
		virtual ~Agent();

		const utility::GraphicList& GetGraphics() const;
		const utility::SoundEffectList& GetSoundEffects() const;


		//const utility::QuadList& GetBoundingQuads() const;

		/** Gets the actions which have been enqueued by this agent.
		@return The actions which have been enqueued by this agent.
		These are intended to be consumed by the caller of this
		function.
		*/
		ActionQueue& GetActions();

		/** React to a specific type of action.
		@note \a action will be forwarded to the reaction method registered
		for that specific Action subclass, if such a method has been
		registered.
		@param action The action to react to.
		*/
		void React(const Action& action);

	protected:

		/** Register a method to be called to react to Actions of
		type \c ActionType.
		@note Only one reaction may be registered for each action type.
		@param agent The Agent which reacts to the ActionType.
		@param reaction_method The method of \a agent which reacts to
		a specific type of action.
		@throws utility::InvalidArgumentException If a reaction method
		has already been registered for \c ActionType.
		@throws utility::OutOfMemoryError If we run out of memory.
		*/
		template<class AgentType, class ActionType>
		void RegisterReaction(AgentType& agent, void (AgentType::*reaction_method)(const ActionType&));

		/** Register a graphic to be rendered to the screen.
		@note This function does not manage the memory for
		\a new_graphic.
		@param new_graphic The graphic to be rendered.
		@throws utility::OutOfMemoryError If we run out of memory.
		*/
		void AddGraphic(const utility::Graphic* const new_graphic);
		/** Stop a graphic from being rendered to the scree.
		@param graphic The graphic which is to not be rendered
		any longer.
		*/
		void RemoveGraphic(const utility::Graphic* const graphic);

		/** Register a sound effect to be sent to the sound engine.
		@param new_sound_effect The effect to be sent to the sound
		engine.
		@throws utility::OutOfMemoryError If we run out of memory.
		*/
		void AddSoundEffect(utility::SoundEffect* const new_sound_effect);
		/** Register a sound effect to be send to the sound engine.
		@param sound_effect The effect to be sent to the sound
		engine.
		*/
		void RemoveSoundEffect(utility::SoundEffect* const sound_effect);

		/** Registers an action to be processed and consumed by
		the scene.
		@post Calling this function passes ownership of \a action.
		to this agent. Do not attempt to delete \a action after this
		call.
		@param action The action to be processed.
		@throws utility::OutOfMemoryError If we run out of memory.
		*/
		void EnqueueAction(const Action* const action);

	private:
		/// Maps action types to their registered reaction methods.
		typedef std::map<reaction::TypeInfo, reaction::Reaction* const> Reactions;

		Reactions reactions;
		utility::GraphicList graphic_list;
		utility::SoundEffectList sound_effect_list;
		ActionQueue action_queue;

	};



	// See method declaration for details.
	template<class AgentType, class ActionType>
	void Agent::RegisterReaction(AgentType& agent, void (AgentType::*reaction_method)(const ActionType&))
	{
		bool reaction_already_registered;
		try
		{
			auto result = reactions.insert(std::make_pair(reaction::TypeInfo(typeid(ActionType)), new(std::nothrow) reaction::DynamicReaction<AgentType, ActionType>(agent, reaction_method)));
			reaction_already_registered = result.second;
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
		if(reaction_already_registered == true)
		{
			throw utility::InvalidArgumentException("avl::model::Agent::RegisterReaction()", "reaction_method", "A method has already been registered to react to this type of Action. Only one reaction may be registered for each Action type.");
		}
	}



} // model
} // avl
#endif // AVL_MODEL_AGENT__
