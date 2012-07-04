#pragma once
#ifndef AVL_MODEL_REACTION__
#define AVL_MODEL_REACTION__
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
<Autogenerated stub>
@author Sheldon Bachstein
@date Jun 27, 2012
*/


#include<typeinfo>

namespace avl
{
namespace model
{
	// Forward declaration.
	class Action;
namespace reaction
{
	/** A thin wrapper around the type_info class.
	*/
	class TypeInfo
	{
	public:
		/* Basic constructor.
		@param initial_info The object type info.
		*/
		explicit TypeInfo(const std::type_info& initial_info);

		/** Basic destructor.
		*/
		~TypeInfo();

		/** Used for ordering elements in a container.
		*/
		bool operator<(const TypeInfo& rhs) const;

	private:
		/// The object type info.
		const std::type_info& info;

	};


	/**
	Represents a reaction to some action.
	*/
	class Reaction
	{
	public:
		/** Basic constructor.
		*/
		Reaction();

		/** Copy constructor.
		@param original The object being copied.
		*/
		Reaction(const Reaction& original);

		/** Basic destructor.
		*/
		virtual ~Reaction();

		/** Executes a reaction to \a action.
		@param action The action to react to.
		*/
		virtual void React(const Action& action) = 0;
	};

	
	
	/**
	Used to execute member functions (methods) as a reaction to an action.
	*/
	template<class AgentType, class ActionType>
	class DynamicReaction: public Reaction
	{
	public:
		/** A method which reacts to an action.*/
		typedef void (AgentType::*ReactionMethod)(const ActionType&);

		/** Basic constructor.
		@param initial_agent The agent which is to perform the reaction.
		@param initial_reaction_method The method belonging to \a initial_agent
		which is a reaction to an action.
		*/
		explicit DynamicReaction(AgentType& initial_agent, ReactionMethod initial_reaction_method);

		/** Copy constructor.
		@param original The object being copied.
		*/
		explicit DynamicReaction(const DynamicReaction& original);

		/** Basic destructor.
		*/
		~DynamicReaction();

		/** Executes an agent's reaction to \a action.
		@param action The action to react to.
		*/
		void React(const Action& action);

	private:
		/// The object which is performing the reaction.
		AgentType& agent;
		/// The reaction method of the agent which is reacting.
		ReactionMethod reaction_method;

	};




	// See method declaration for details.
	template<class AgentType, class ActionType>
	DynamicReaction<AgentType, ActionType>::DynamicReaction(AgentType& initial_agent, ReactionMethod initial_reaction_method)
		: agent(initial_agent), reaction_method(initial_reaction_method)
	{
	}

	// See method declaration for details.
	template<class AgentType, class ActionType>
	DynamicReaction<AgentType, ActionType>::DynamicReaction(const DynamicReaction& original)
		: agent(original.agent), reaction_method(original.reaction_method)
	{
	}

	// See method declaration for details.
	template<class AgentType, class ActionType>
	DynamicReaction<AgentType, ActionType>::~DynamicReaction()
	{
	}

	// See method declaration for details.
	template<class AgentType, class ActionType>
	void DynamicReaction<AgentType, ActionType>::React(const Action& action)
	{
		(agent.*reaction_method)(static_cast<const ActionType&>(action));
	}


} // reaction
} // model
} // avl
#endif // AVL_MODEL_REACTION__
