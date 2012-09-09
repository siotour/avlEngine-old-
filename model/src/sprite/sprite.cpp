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
Implementation for the sprite component. See "sprite.h" for details.
@author Sheldon Bachstein
@date Aug 19, 2012
*/

#include"sprite.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\vector\vector.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\quad\quad.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"


namespace avl
{
namespace model
{

	using utility::Vector;
	using utility::Quad;
	using utility::TexturedQuad;
	using utility::OutOfMemoryError;
	using utility::InvalidArgumentException;

	// See method declaration for details.
	Sprite::Sprite()
	{
		InsertQuadIntoRenderPrimitives();
	}

	// See method declaration for details.
	Sprite::Sprite(const Quad& initial_quad, const float z, const TexturedQuad::TextureHandle texture_handle)
		: quad(initial_quad, z, texture_handle)
	{
		InsertQuadIntoRenderPrimitives();
	}

	// See method declaration for details.
	Sprite::Sprite(const Sprite& original)
		: quad(original.quad)
	{
		InsertQuadIntoRenderPrimitives();
	}

	// See method declaration for details.
	Sprite::~Sprite()
	{
	}

	// See method declaration for details.
	void Sprite::InsertQuadIntoRenderPrimitives()
	{
		try
		{
			render_primitives.push_back(&quad);
		}
		catch(const std::bad_alloc&)
		{
			throw OutOfMemoryError();
		}
	}

} // model
} // avl
