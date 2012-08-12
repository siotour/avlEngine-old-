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
Implementation for the textured quad component. See "textured quad.h" for details.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include"textured quad.h"
#include"..\render primitive\render primitive.h"
#include"..\quad\quad.h"
#include"..\vector\vector.h"


namespace avl
{
namespace utility
{
	namespace
	{
		// Default texture position.
		const Quad DEFAULT_TEXTURE_POSITION(0.0f, 1.0f, 1.0f, 0.0f);
	}

	// See method declaration for details.
	TexturedQuad::TexturedQuad()
		: RenderPrimitive(TEXTURED_QUAD, 0.0f), texture_position(DEFAULT_TEXTURE_POSITION), texture_handle(0)
	{
	}

	// See method declaration for details.
	TexturedQuad::TexturedQuad(const Quad& initial_position, const float z_depth, const TextureHandle texture)
		: RenderPrimitive(TEXTURED_QUAD, z_depth), position(initial_position), texture_handle(texture), texture_position(DEFAULT_TEXTURE_POSITION)
	{
	}

	// See method declaration for details.
	TexturedQuad::TexturedQuad(const Quad& initial_position, const float z_depth, const Quad& initial_texture_position, const TextureHandle texture)
		: RenderPrimitive(TEXTURED_QUAD, z_depth), position(initial_position), texture_position(initial_texture_position), texture_handle(texture)
	{
	}

	// See method declaration for details.
	TexturedQuad::TexturedQuad(const TexturedQuad& original)
		: RenderPrimitive(TEXTURED_QUAD, original.z_depth), position(original.position), texture_position(original.texture_position), texture_handle(original.texture_handle)
	{
	}

	// See method declaration for details.
	TexturedQuad::~TexturedQuad()
	{
	}


} // utility
} // avl
