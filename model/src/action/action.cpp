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
Implementation for the action component. See "action.h" for details.
@author Sheldon Bachstein
@date Jun 26, 2012
*/

#include"action.h"


namespace avl
{
namespace model
{
	// Action class.
	//

	// See method declaration for details.
	Action::Action()
	{
	}

	// See method declaration for details.
	Action::Action(const Action& original)
	{
	}

	// See method declaration for details.
	Action::~Action()
	{
	}


	// TimeStep class.
	//

	// See method declaration for details.
	TimeStep::TimeStep(const double& initial_elapsed_time)
		: elapsed_time(initial_elapsed_time)
	{

	}

	// See method declaration for details.
	TimeStep::~TimeStep()
	{
	}

	// See method declaration for details.
	const double& TimeStep::GetElapsedTime() const
	{
		return elapsed_time;
	}




} // model
} // avl
