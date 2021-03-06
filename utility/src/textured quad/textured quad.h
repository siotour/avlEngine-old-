#pragma once
#ifndef AVL_UTILITY_TEXTURED_QUAD__
#define AVL_UTILITY_TEXTURED_QUAD__
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
Defines the \ref avl::utility::TexturedQuad class.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include"..\render primitive\render primitive.h"
#include"..\quad\quad.h"

namespace avl
{
namespace utility
{

	/**
	Represents a textured quad, aligned with the XY plane,
	in 3-D space.
	*/
	class TexturedQuad: public RenderPrimitive
	{
	public:
		/**
		Used to associate a TexturedQuad with a texture to be
		rendered on it.
		*/
		typedef unsigned int TextureHandle;

		/** @post Initializes each vertex to (0, 0) and the texture
		coordinates to (0, 0), (0, 1), (1, 1), and (1, 0), respectively.
		Additionally initializes the texture handle to 0 and the z-depth
		to 0.
		*/
		TexturedQuad();
		/** @post Initializes the texture coordinates to
		(0, 0), (0, 1), (1, 1), and (1, 0), respectively.
		@param initial_position The position.
		@param z_depth Position along the z-axis.
		@param texture The associated texture handle.
		*/
		TexturedQuad(const Quad& initial_position, const float z_depth, const TextureHandle texture);
		/**	@param initial_position The quad's vertices.
		@param z_depth The z-depth of the object.
		@param initial_texture_position The texture coordinates for this quad.
		@param texture The handle to the texture for this quad.
		*/
		TexturedQuad(const Quad& initial_position, const float z_depth, const Quad& initial_texture_position, const TextureHandle texture);
		TexturedQuad(const TexturedQuad& original);
		~TexturedQuad();

		const Quad& GetPosition() const;
		const Quad& GetTexturePosition() const;
		const TextureHandle GetTextureHandle() const;

		/** Gets a non-const reference to the position.
		@return The position of this textured quad.
		*/
		Quad& AccessPosition();
		/** Gets a non-const reference to the texture position.
		@return The texture position.
		*/
		Quad& AccessTexturePosition();

		void SetPosition(const Quad& new_position);
		void SetTexturePosition(const Quad& new_texture_position);
		void SetTextureHandle(const TextureHandle new_handle);

		const TexturedQuad& operator=(const TexturedQuad& rhs);

	private:
		Quad position;
		Quad texture_position;
		TextureHandle texture_handle;
	};



	// See method declaration for details.
	inline const Quad& TexturedQuad::GetPosition() const
	{
		return position;
	}

	// See method declaration for details.
	inline const Quad& TexturedQuad::GetTexturePosition() const
	{
		return texture_position;
	}

	// See method declaration for details.
	inline const TexturedQuad::TextureHandle TexturedQuad::GetTextureHandle() const
	{
		return texture_handle;
	}

	// See method declaration for details.
	inline Quad& TexturedQuad::AccessPosition()
	{
		return position;
	}

	// See method declaration for details.
	inline Quad& TexturedQuad::AccessTexturePosition()
	{
		return texture_position;
	}
	
	// See method declaration for details.
	inline void TexturedQuad::SetPosition(const Quad& new_position)
	{
		position = new_position;
	}

	// See method declaration for details.
	inline void TexturedQuad::SetTexturePosition(const Quad& new_texture_position)
	{
		texture_position = new_texture_position;
	}

	// See method declaration for details.
	inline void TexturedQuad::SetTextureHandle(const TexturedQuad::TextureHandle new_handle)
	{
		texture_handle = new_handle;
	}

	// See method declaration for details.
	inline const TexturedQuad& TexturedQuad::operator=(const TexturedQuad& rhs)
	{
		position = rhs.position;
		texture_position = rhs.texture_position;
		texture_handle = rhs.texture_handle;
		return *this;
	}


} // utility
} // avl
#endif // AVL_UTILITY_TEXTURED_QUAD__
