#pragma once
#ifndef AVL_UTILITY_GRAPHIC__
#define AVL_UTILITY_GRAPHIC__
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
Defines the Graphic class and the GraphicList container type.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include"..\render primitive\render primitive.h"
#include<list>

namespace avl
{
namespace utility
{
	// Forward declaration.
	class Graphic;

	typedef std::list<const Graphic* const> GraphicList;

	/**
	The abstract base class of all non-primitive renderable objects.
	*/
	class Graphic
	{
	public:
		Graphic();
		virtual ~Graphic();
		/** Gets the render primitives used to render this object.
		@return The render primitives used to render this object.
		*/
		virtual const RenderPrimitiveList& GetRenderPrimitives() const = 0;

	private:
		/// NOT IMPLEMENTED.
		Graphic(const Graphic&);
		/// NOT IMPLEMENTED.
		Graphic& operator=(const Graphic&);
	};


} // utility
} // avl
#endif // AVL_UTILITY_GRAPHIC__
