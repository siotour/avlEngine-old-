#pragma once
#ifndef AVL_UTILITY_SPRITE__
#define AVL_UTILITY_SPRITE__
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
Defines the utility::Sprite class, which represents a (possibly animated)
textured quadrilateral in 3-D space.
@author Sheldon Bachstein
@date Jan 21, 2011
@todo Document this component.
*/

#include"..\graphic\graphic.h"
#include"..\vector\vector.h"
#include"..\textured quad\textured quad.h"
#include"..\quad\quad.h"
#include<map>

namespace avl
{
namespace utility
{

	/**
	@todo Document me!!!
	*/
	class Sprite: public Graphic
	{
	public:
		// Forward declaration.
		struct Animation;

		/**
		@post 
		*/
		Sprite();
		Sprite(const Quad& position, const float z, const TexturedQuad::TextureHandle texture_handle, const unsigned short frames_per_row = 1, const unsigned short frames_per_column = 1);
		Sprite(const Sprite& original);
		~Sprite();

		const bool IsIdle() const;
		/** Gets the center for this sprite.
		@return The average of the four vertices of this quad.
		*/
		const Vector GetCenter() const;
		const Quad& GetPosition() const;
		const float GetZ() const;
		const TexturedQuad::TextureHandle GetTextureHandle() const;

		/** Gets the render primitives used to render this object.
		@return The render primitives used to render this object.
		*/
		const RenderPrimitiveList& GetRenderPrimitives() const;


		/** Translates the quad by \a delta_position.
		@param delta_position The amount by which to translate the quad.
		*/
		void Move(const Vector& delta_position);
		/** Translates the quad such that the center is at the coordinate
		\a new_center.
		@param new_center The new center for the quad.
		*/
		void SetCenter(const Vector& new_center);
		/** Transforms the quad to allign with \a new_position.
		@param new_position The position to transform to.
		*/
		void SetPosition(const Quad& new_position);
		/** Rotates the quad by \a theta degrees counter-clockwise.
		@param theta The number of degrees by which to rotate the quad.
		*/
		void Rotate(float theta);
		/** Scales the quad by a factor of \a scale_factor.
		@param scale_factor The factor by which to scale the quad.
		*/
		void Scale(const float& scale_factor);
		/** Sets the z-depth.
		@param new_z The new z-depth.
		*/
		void SetZ(const float new_z);
		void SetTextureHandle(const TexturedQuad::TextureHandle new_texture);
		/**
		@note Invalidates all previously added animations.
		*/
		void SetTextureDimensions(const unsigned short frames_per_row, const unsigned short frames_per_column);


		void Update(const float elapsed_time);
		void AddAnimation(const Animation& new_animation);
		void PlayAnimation(const short animation_id);
		void SetIdleAnimation(const short animation_id);
		void SetRepeat(const bool repeat);


		const Sprite& operator=(const Sprite& rhs);


	private:

		void UpdateAnimationFrame(const Sprite::Animation& animation);

		float accumulated_time;
		TexturedQuad quad;
		bool is_repeating;
		bool is_idle;

		unsigned short current_animation_id;
		unsigned short current_animation_frame;
		unsigned short idle_animation_id;

		typedef std::map<const unsigned short, Animation> AnimationMap;
		AnimationMap animations;

		RenderPrimitiveList render_primitives;


		struct TextureDimensions
		{
		public:
			unsigned short columns;
			unsigned short rows;
			float frame_width;
			float frame_height;
		};

		TextureDimensions texture_dimensions;


	public:
		struct Animation
		{
		public:
			/// The identifier used to reference this animation.
			unsigned short animation_id;
			/// The first frame in the animation sequence (indexed from one).
			unsigned short start_frame;
			/// The number of frames in the animation sequence, including the
			/// start frame.
			unsigned short number_of_frames;
			/// The delay, in seconds, between frames.
			float frame_delay;

		};
	};



	// See method declaration for details.
	inline const bool Sprite::IsIdle() const
	{
		return is_idle;
	}

	// See method declaration for details.
	inline const Vector Sprite::GetCenter() const
	{
		return quad.GetPosition().GetCenter();
	}

	// See method declaration for details.
	inline const Quad& Sprite::GetPosition() const
	{
		return quad.GetPosition();
	}

	// See method declaration for details.
	inline const float Sprite::GetZ() const
	{
		return quad.GetZ();
	}

	// See method declaration for details.
	inline const TexturedQuad::TextureHandle Sprite::GetTextureHandle() const
	{
		return quad.GetTextureHandle();
	}

	// See method declaration for details.
	inline const RenderPrimitiveList& Sprite::GetRenderPrimitives() const
	{
		return render_primitives;
	}

	// See method declaration for details.
	inline void Sprite::Move(const Vector& delta_position)
	{
		quad.AccessPosition().Move(delta_position);
	}

	// See method declaration for details.
	inline void Sprite::SetCenter(const Vector& new_center)
	{
		quad.AccessPosition().SetCenter(new_center);
	}

	// See method declaration for details.
	inline void Sprite::SetPosition(const Quad& new_position)
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
	inline void Sprite::SetTextureHandle(const TexturedQuad::TextureHandle new_texture)
	{
		quad.SetTextureHandle(new_texture);
	}

	// See method declaration for details.
	inline void Sprite::SetRepeat(const bool repeat)
	{
		is_repeating = repeat;
	}



} //avl
} //utility
#endif // AVL_UTILITY_SPRITE__