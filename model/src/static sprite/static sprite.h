#pragma once
#ifndef AVL_MODEL_STATIC_SPRITE__
#define AVL_MODEL_STATIC_SPRITE__
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
Defines the StaticSprite class.
@author Sheldon Bachstein
@date Aug 13, 2012
*/

#include"..\sprite\sprite.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\quad\quad.h"
#include"..\..\..\utility\src\vector\vector.h"


namespace avl
{
namespace model
{

	/**
	Represents an unanimated, transformable image in 3D space. See
	\ref avl::model::Sprite for the full interface.
	*/
	class StaticSprite: public Sprite
	{
	public:

		/** Initializes each vertice to the origin (0, 0) and uses a z-depth of 0.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		StaticSprite();
		/**
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		StaticSprite(const utility::Quad& position, const float z, const utility::TexturedQuad::TextureHandle texture_handle);
		/**
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		StaticSprite(const StaticSprite& original);
		~StaticSprite();

		void SetTexturePosition(const utility::Quad& new_position);

		const StaticSprite& operator=(const StaticSprite& rhs);

	};

	// See method declaration for details.
	inline void StaticSprite::SetTexturePosition(const utility::Quad& new_position)
	{
		quad.SetTexturePosition(new_position);
	}


} // model
} // avl
#endif // AVL_MODEL_STATIC_SPRITE__
