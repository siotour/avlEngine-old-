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
Implementation for the static sprite component. See "static sprite.h" for details.
@author Sheldon Bachstein
@date Aug 13, 2012
*/

#include"static sprite.h"
#include"..\sprite\sprite.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\quad\quad.h"


namespace avl
{
namespace model
{
	// See method declaration for details.
	StaticSprite::StaticSprite()
		: Sprite()
	{
	}

	// See method declaration for details.
	StaticSprite::StaticSprite(const utility::Quad& position, const float z, const utility::TexturedQuad::TextureHandle texture_handle)
		: Sprite(position, z, texture_handle)
	{
	}

	// See method declaration for details.
	StaticSprite::StaticSprite(const StaticSprite& original)
		: Sprite(original)
	{
	}

	// See method declaration for details.
	StaticSprite::~StaticSprite()
	{
	}

	// See method declaration for details.
	const StaticSprite& StaticSprite::operator=(const StaticSprite& rhs)
	{
		Sprite::operator=(rhs);
		return *this;
	}

} // model
} // avl
