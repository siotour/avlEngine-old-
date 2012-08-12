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
Implementation for the render primitive component. See "render primitive.h" for details.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include"render primitive.h"


namespace avl
{
namespace utility
{
	// See method declaration for details.
	RenderPrimitive::RenderPrimitive(const PrimitiveType primitive_type, const float z)
		: type(primitive_type), z_depth(z), is_visible(true)
	{
	}

	// See method declaration for details.
	RenderPrimitive::~RenderPrimitive()
	{
	}

} // utility
} // avl
