#pragma once
#ifndef AVL_MODEL_ACTION__
#define AVL_MODEL_ACTION__
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
Defines the Action, TimeStep, and EndScene classes.
@author Sheldon Bachstein
@date Jun 26, 2012
*/

#include"..\..\..\utility\src\polymorphic queue\polymorphic queue.h"

namespace avl
{
namespace model
{
	// Forward declaration.
	class Action;

	/** A memory-managed container for holding polymorphic Action
	objects.
	*/
	typedef utility::PolymorphicQueue<const Action> ActionQueue;


	/**
	The base class of the Action hierarchy. Actions represent
	discrete events in the model space which agents may react to.
	*/
	class Action
	{
	public:
		Action();
		Action(const Action& original);
		virtual ~Action();

	private:
		/// NOT IMPLEMENTED.
		const Action& operator=(const Action&);
	};


	/**
	Indicates that agents should update their state with respect
	to some amount of elapsed time.
	*/
	class TimeStep: public Action
	{
	public:
		TimeStep(const double& initial_elapsed_time);
		~TimeStep();

		const double& GetElapsedTime() const;

	private:
		const double elapsed_time;
	};


	/**
	Indicates that the scene has ended.
	*/
	class EndScene: public Action
	{
	public:
		/** Specify the exit code for the scene.
		@param code The exit code associated with how the
		scene ended. 
		*/
		EndScene(const int code);
		EndScene(const EndScene& original);
		~EndScene();

		/** Get the exit code associated with how the scene
		ended.
		@return The exit code associated with how the scene ended.
		*/
		const int GetExitCode() const;

	private:
		/// Used to differentiate different ways that a scene
		/// may end. Possible values are application specific.
		const int exit_code;

	};



} // model
} // avl
#endif // AVL_MODEL_ACTION__
