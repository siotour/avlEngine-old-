#pragma once
#ifndef AVL_MODEL_SPRITE__
#define AVL_MODEL_SPRITE__
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
Defines the Sprite class, which is the base class of both StaticSprite
and AnimatedSprite.
@author Sheldon Bachstein
@date Aug 19, 2012
*/

#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\vector\vector.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\quad\quad.h"


namespace avl
{
namespace model
{

	/**
	Provides functionality common to both StaticSprite and
	AnimatedSprite.
	*/
	class Sprite: public utility::Graphic
	{
	public:

		virtual ~Sprite();

		/** Gets the center of the sprite's position.
		@return The mean of the four vertices of this quad.
		*/
		const utility::Vector GetCenter() const;
		const utility::Quad& GetPosition() const;
		const float GetZ() const;
		const utility::TexturedQuad::TextureHandle GetTextureHandle() const;
		/** Gets the render primitives used to render this sprite.
		@return The render primitives used to render this sprite.
		*/
		const utility::RenderPrimitiveList& GetRenderPrimitives() const;

		/** Move the position by \a delta_position.
		@param delta_position The vector along which to move.
		*/
		void Move(const utility::Vector& delta_position);
		/** Moves the position such that the center is at the point
		\a new_center.
		@param new_center The new center to move to.
		*/
		void SetCenter(const utility::Vector& new_center);
		void SetPosition(const utility::Quad& new_position);
		/** Rotates the position quad by \a theta degrees counter-clockwise
		about the center.
		@param theta The number of degrees by which to rotate the quad.
		*/
		void Rotate(float theta);
		/** Scales the position quad about the center by a factor of
		\a scale_factor.
		@param scale_factor The factor by which to scale the quad.
		*/
		void Scale(const float& scale_factor);
		void SetZ(const float new_z);
		void SetTextureHandle(const utility::TexturedQuad::TextureHandle new_texture);

		const Sprite& operator=(const Sprite& rhs);

	protected:
		/** Initializes each vertice to the origin (0, 0) and uses a z-depth of 0.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		Sprite();
		/** @throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		Sprite(const utility::Quad& position, const float z, const utility::TexturedQuad::TextureHandle texture_handle);
		/** @throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		Sprite(const Sprite& original);

		utility::TexturedQuad quad;
		utility::RenderPrimitiveList render_primitives;

	private:
		/** @throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertQuadIntoRenderPrimitives();
	};


	
	// See method declaration for details.
	inline const utility::Vector Sprite::GetCenter() const
	{
		return quad.GetPosition().GetCenter();
	}

	// See method declaration for details.
	inline const utility::Quad& Sprite::GetPosition() const
	{
		return quad.GetPosition();
	}

	// See method declaration for details.
	inline const float Sprite::GetZ() const
	{
		return quad.GetZ();
	}

	// See method declaration for details.
	inline const utility::TexturedQuad::TextureHandle Sprite::GetTextureHandle() const
	{
		return quad.GetTextureHandle();
	}

	// See method declaration for details.
	inline const utility::RenderPrimitiveList& Sprite::GetRenderPrimitives() const
	{
		return render_primitives;
	}

	// See method declaration for details.
	inline void Sprite::Move(const utility::Vector& delta_position)
	{
		quad.AccessPosition().Move(delta_position);
	}

	// See method declaration for details.
	inline void Sprite::SetCenter(const utility::Vector& new_center)
	{
		quad.AccessPosition().SetCenter(new_center);
	}

	// See method declaration for details.
	inline void Sprite::SetPosition(const utility::Quad& new_position)
	{
		quad.SetPosition(new_position);
	}

	// See method declaration for details.
	inline void Sprite::SetZ(const float new_z)
	{
		quad.SetZ(new_z);
	}

	// See method declaration for details.
	inline void Sprite::Rotate(float theta)
	{
		quad.AccessPosition().Rotate(theta);
	}

	// See method declaration for details.
	inline void Sprite::Scale(const float& scale_factor)
	{
		quad.AccessPosition().Scale(scale_factor);
	}

	// See method declaration for details.
	inline void Sprite::SetTextureHandle(const utility::TexturedQuad::TextureHandle new_texture)
	{
		quad.SetTextureHandle(new_texture);
	}

	// See method declaration for details.
	inline const Sprite& Sprite::operator=(const Sprite& rhs)
	{
		quad = rhs.quad;
		return *this;
	}

} // model
} // avl
#endif // AVL_MODEL_SPRITE__
