#pragma once
#ifndef AVL_MODEL_BASIC_SCENE__
#define AVL_MODEL_BASIC_SCENE__
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
Defines the BasicScene class.
@author Sheldon Bachstein
@date Jun 30, 2012
*/

#include"..\scene\scene.h"
#include"..\end scene listener\end scene listener.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\sound effect\sound effect.h"
#include"..\..\..\utility\src\timer\timer.h"
#include"..\..\..\utility\src\vector\vector.h"
#include<set>


namespace avl
{
namespace model
{
	// Forward declarations.
	class Action;
	class Agent;

	/**
	Provides a simple, easy-to-use implementation for most of the Scene
	interface. Acts as a base class for a typical Scene.
	@todo Provide a virtual function for converting from
	utility::input_events::InputEvent objects to analogous Action objects
	and then distributing those actions to the agents in the scene.
	*/
	class BasicScene: public Scene
	{
	public:
		/**
		@param initial_time_step The amount of time, in seconds, at which
		to update each agent in the scene. If this is 0 or less then no 
		timestep will be used, and agents will be updated at whichever pace
		Update() is called.
		@param screen_space The screen resolution. The vector running from
		the bottom left corner to the top right corner.
		*/
		BasicScene(const double& initial_time_step, const utility::Vector& screen_space);
		virtual ~BasicScene();

		/** Retrieves the graphics representing this scene.
		@return The graphics representing the scene.
		*/
		const utility::GraphicList GetGraphics();
		/** Retrieves the sound effects representing this scene.
		@return The sound effects representing the scene.
		*/
		utility::SoundEffectList GetSoundEffects();
		
		/** Updates the scene. If a timestep was specified upon
		creation, then the scene will only be updated in chunks of
		time as specified by that timestep.
		*/
		virtual void Update();
		/** Pauses the scene so that elapsed time is not accumulated until
		the next call to Update().
		*/
		virtual void Pause();
		/** Returns true if this scene has ended.
		@return True if the scene has ended, and false if not.
		*/
		const bool HasEnded();
		/** Returns the exit code for the end of the scene. May only be
		called when the scene has actually ended.
		@pre This scene must have ended when you call this.
		@return The exit code of the scene.
		@throws utility::InvalidCallException If called before the scene
		has ended.
		*/
		const int GetExitCode();

	protected:
		/** Adds \a agent into the scene and takes ownership of \a agent.
		@post Ownership of \a agent will be maintained by this object. Do not
		delete \a agent after calling this function.
		@param agent The agent to be added to the scene.
		@throws utility::OutOfMemoryError If we run out of memory.
		@throws utility::InvalidArgumentException If \a agent already exists
		in the scene.
		*/
		void AddAgent(Agent* const agent);
		/** Removes \a agent from the scene and deletes it, if it exists
		within the scene.
		@post \a agent will be deleted if it exists within the scene.
		@param agent The agent to be removed from the scene.
		*/
		void RemoveAgent(Agent* const agent);
		/** Distribute an action to all agents in the scene.
		@param action The action to be distributed.
		*/
		void DistributeAction(const Action& action);

		/// The resolution of the visible screen space. The x component
		/// specifies the width of the visible screen space along the x
		/// axis, and the y component specifies the width of the
		/// visible screen space along hte y axis. The center of the
		/// screen is the point (0, 0).
		const utility::Vector screen_space_resolution;

		/// Listens for EndScene actions.
		EndSceneListener end_listener;
		/// The agents acting in the scene.
		std::set<Agent* const> agents;
		/// The timestep at which agents are updated.
		const double time_step;
		/// Used to track time changes.
		utility::Timer timer;

	private:
		/** Distributes and consumes all actions which have been enqueued
		by the agents in this scene.
		*/
		void DistributeEnqueuedActions();

		/// NOT IMPLEMENTED.
		BasicScene(const BasicScene&);
		/// NOT IMPLEMENTED.
		const BasicScene& operator=(const BasicScene&);
	};



} // model
} // avl
#endif // AVL_MODEL_BASIC_SCENE__
