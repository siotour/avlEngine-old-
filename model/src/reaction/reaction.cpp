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
Implementation for the reaction component. See "reaction.h" for details.
@author Sheldon Bachstein
@date Jun 27, 2012
*/

#include"reaction.h"
#include"..\action\action.h"
#include<typeinfo>


namespace avl
{
namespace model
{
namespace reaction
{
	// See method declaration for details.
	TypeInfo::TypeInfo(const std::type_info& initial_info)
		: info(initial_info)
	{
	}

	// See method declaration for details.
	TypeInfo::~TypeInfo()
	{
	}

	// See method declaration for details.
	bool TypeInfo::operator<(const TypeInfo& rhs) const
	{
		return info.before(rhs.info) != 0;
	}



	// See method declaration for details.
	Reaction::Reaction()
	{
	}

	// See method declaration for details.
	Reaction::Reaction(const Reaction& original)
	{
	}

	// See method declaration for details.
	Reaction::~Reaction()
	{
	}



} // reaction
} // model
} // avl
