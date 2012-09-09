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
Provides the framework for allowing Agent subclasses to easily
register methods to be called in response to any type of
Action.
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
	/** A thin wrapper around the type_info class. Allows for
	sorting and is cleaner than using containers of type_info
	pointers.
	*/
	class TypeInfo
	{
	public:
		explicit TypeInfo(const std::type_info& initial_info);
		~TypeInfo();

		bool operator<(const TypeInfo& rhs) const;

	private:
		const std::type_info& info;

	};


	/**
	Represents a reaction to some Action.
	*/
	class Reaction
	{
	public:
		Reaction();
		Reaction(const Reaction& original);
		virtual ~Reaction();

		/** Reacts to the occurrence of \a action.
		@param action The action to react to.
		*/
		virtual void React(const Action& action) = 0;

	private:
		/// NOT IMPLEMENTED.
		Reaction& operator=(const Reaction&);
	};

	
	
	/**
	Allows arbitrary member functions (methods) to be used to
	react Actions.
	*/
	template<class AgentType, class ActionType>
	class DynamicReaction: public Reaction
	{
	public:
		/** The type of method used to react.*/
		typedef void (AgentType::*ReactionMethod)(const ActionType&);

		/** Basic constructor.
		@param initial_agent The object which
		\a initial_reaction_method belongs to.
		@param initial_reaction_method The method belonging to
		\a initial_agent which is called as a reaction.
		*/
		explicit DynamicReaction(AgentType& initial_agent, ReactionMethod initial_reaction_method);
		explicit DynamicReaction(const DynamicReaction& original);
		~DynamicReaction();

		/** Reacts to the occurrence of \a action by calling
		the specified member function on the specified object
		(as specified in the constructor).
		@param action The action to react to.
		*/
		void React(const Action& action);

	private:
		/// The object which will perform the reaction.
		AgentType& agent;
		/// The reaction method of the object which will react.
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
