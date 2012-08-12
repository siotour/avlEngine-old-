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
@date January 21, 2011
*/

#include"sprite.h"
#include"..\graphic\graphic.h"
#include"..\vector\vector.h"
#include"..\textured quad\textured quad.h"
#include"..\quad\quad.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<map>
#include<cmath>



namespace avl
{
namespace utility
{
	//// Forward declarations. See definitions for details.
	//namespace
	//{
	//	void SetAnimationFrame(Quad& texture_position, const float frame_width, const float frame_height, const unsigned short frame);
	//}



	// See method declaration for details.
	Sprite::Sprite()
		: accumulated_time(0.0f), is_repeating(false), is_idle(false), current_animation_frame(0)
	{
		SetTextureDimensions(1, 1);
		render_primitives.push_back(&quad);
	}

	// See method declaration for details.
	Sprite::Sprite(const Quad& initial_quad, const float z, const TexturedQuad::TextureHandle handle, const unsigned short frames_per_row, const unsigned short frames_per_column)
		: accumulated_time(0.0f), is_repeating(false), is_idle(false), current_animation_id(0), current_animation_frame(0), idle_animation_id(0), quad(initial_quad, z, handle)
	{
		SetTextureDimensions(frames_per_row, frames_per_column);
		render_primitives.push_back(&quad);
	}

	// See method declaration for details.
	Sprite::Sprite(const Sprite& original)
		: accumulated_time(original.accumulated_time), is_repeating(original.is_repeating), is_idle(original.is_idle), current_animation_id(original.current_animation_id), current_animation_frame(original.current_animation_frame), idle_animation_id(original.idle_animation_id), quad(original.quad), animations(original.animations)
	{
		render_primitives.push_back(&quad);
	}

	// See method declaration for details.
	Sprite::~Sprite()
	{
	}

	// See method declaration for details.
	void Sprite::SetTextureDimensions(const unsigned short frames_per_row, const unsigned short frames_per_column)
	{
		if(frames_per_row < 1)
		{
			throw InvalidArgumentException("avl::utility::Sprite::SetTextureDimensions()", "frames_per_row", "Must be at least one.");
		}
		texture_dimensions.columns = frames_per_row;
		texture_dimensions.frame_width = 1.0f / frames_per_row;
		if(frames_per_column < 1)
		{
			throw InvalidArgumentException("avl::utility::Sprite::SetTextureDimensions()", "frames_per_column", "Must be at least one.");
		}
		texture_dimensions.rows = frames_per_column;
		texture_dimensions.frame_height = 1.0f / frames_per_column;

		current_animation_id = 0;
		idle_animation_id = 0;
		animations.clear();
	}

	// See method declaration for details.
	void Sprite::Update(const float elapsed_time)
	{
		// Indicates that we need to update the frame.
		bool must_update_frame = false;
		// If no animation has been played then begin idling.
		if(current_animation_id == 0)
		{
			is_idle = true;
			// If there's no idle animation set up, then there's nothing to do.
			if(idle_animation_id == 0)
			{
				return;
			}
			must_update_frame = true;
			current_animation_id = idle_animation_id;
		}

		// Update the current animation.
		accumulated_time += elapsed_time;
		auto animation = animations.find(current_animation_id);
		ASSERT(animation != animations.end());

		// Is the current frame completed?
		if(accumulated_time >= animation->second.frame_delay)
		{
			must_update_frame = true;
			accumulated_time -= animation->second.frame_delay;
			++current_animation_frame;
			// Is the current animation completed?
			if(current_animation_frame >= animation->second.number_of_frames)
			{
				// If we're repeating then restart the current animation.
				if(is_repeating == true)
				{
					current_animation_frame = 0;
				}
				// Transition to the idle animation.
				else
				{
					is_idle = true;
					current_animation_frame = 0;
					current_animation_id = idle_animation_id;
					// Now update the idle animation.
					if(current_animation_id == 0)
					{
						accumulated_time = 0.0f;
						return;
					}
				}
			}
		}
		if(must_update_frame == true)
		{
			// Display the correct frame.
			UpdateAnimationFrame(animation->second);
		}
	}

	// See method declaration for details.
	void Sprite::AddAnimation(const Animation& new_animation)
	{
		if(new_animation.animation_id == 0)
		{
			throw InvalidArgumentException("avl::utility::Sprite::AddAnimation()", "new_animation", "Zero cannot be used as an animation ID.");
		}
		animations.insert(std::make_pair(new_animation.animation_id, new_animation));
	}

	// See method declaration for details.
	void Sprite::PlayAnimation(const short animation_id)
	{
		auto animation = animations.find(animation_id);
		if(animation == animations.end())
		{
			throw InvalidArgumentException("avl::utility::Sprite::PlayAnimation()", "animation_id", "Must be a valid ID to an animation previously added using Sprite::AddAnimation().");
		}
		current_animation_id = animation_id;
		current_animation_frame = 0;
		accumulated_time = 0.0f;
		is_idle = false;
		UpdateAnimationFrame(animation->second);
	}

	// See method declaration for details.
	void Sprite::SetIdleAnimation(const short animation_id)
	{
		auto animation = animations.find(animation_id);
		if(animation == animations.end())
		{
			// Animation doesn't exist.
			throw InvalidArgumentException("avl::utility::Sprite::SetIdleAnimation()", "animation_id", "Must be a valid ID to an animation previously added using Sprite::AddAnimation().");
		}
		idle_animation_id = animation_id;
		if(is_idle == true)
		{
			// If we're idle, we need to be playing the idle animation.
			if(current_animation_id != idle_animation_id)
			{
				// The current animation is distinct from the idle animation. Transition
				// to the idle animation.
				current_animation_id = idle_animation_id;
				current_animation_frame = 0;
				accumulated_time = 0.0f;

				UpdateAnimationFrame(animation->second);
			}
		}
	}

	// See method declaration for details.
	const Sprite& Sprite::operator=(const Sprite& rhs)
	{
		accumulated_time = rhs.accumulated_time;
		is_repeating = rhs.is_repeating;
		is_idle = rhs.is_idle;
		current_animation_id = rhs.current_animation_id;
		current_animation_frame = rhs.current_animation_frame;
		idle_animation_id = rhs.idle_animation_id;
		quad = rhs.quad;
		animations = rhs.animations;
		return *this;
	}

	// See method declaration for details.
	void Sprite::UpdateAnimationFrame(const Sprite::Animation& animation)
	{
		ASSERT(texture_dimensions.frame_width >= 0.0f);
		ASSERT(texture_dimensions.frame_height >= 0.0f);

		// Figure out which row and column the current animation is in.
		const unsigned short row = (animation.start_frame + current_animation_frame - 1) / texture_dimensions.columns;
		const unsigned short column = (animation.start_frame + current_animation_frame - 1) % texture_dimensions.columns;
		ASSERT((column + 1) * texture_dimensions.frame_width <= 1.0f);
		ASSERT((row + 1) * texture_dimensions.frame_height <= 1.0f);

		const Vector p1(column * texture_dimensions.frame_width, 1.0f - (row + 1) * texture_dimensions.frame_height);
		const Vector p2(column * texture_dimensions.frame_width, 1.0f - row * texture_dimensions.frame_height);
		const Vector p3((column + 1) * texture_dimensions.frame_width, 1.0f - row * texture_dimensions.frame_height);
		const Vector p4((column + 1) * texture_dimensions.frame_width, 1.0f - (row + 1) * texture_dimensions.frame_height);
		// Display the specified frame.
		quad.AccessTexturePosition().SetP1(p1);
		quad.AccessTexturePosition().SetP2(p2);
		quad.AccessTexturePosition().SetP3(p3);
		quad.AccessTexturePosition().SetP4(p4);
	}



	//// Forward declarations. See definitions for details.
	//namespace
	//{

	//	void SetAnimationFrame(Quad& texture_position, const float frame_width, const float frame_height, const unsigned short frame)
	//	{
	//		ASSERT(frame_width >= 0.0f);
	//		ASSERT(frame_height >= 0.0f);
	//		// Figure out the number of columns of frames in the texture.
	//		const float columns_unrounded = 1.0f / frame_width;
	//		const unsigned short columns = static_cast<const unsigned short>(floor(columns_unrounded + 0.5f));
	//		ASSERT(columns >= 1);
	//		// Figure out which row and column the current animation is in.
	//		const unsigned short row = (frame - 1) / columns;
	//		const unsigned short column = (frame - 1) % columns;
	//		ASSERT((column + 1) * frame_width <= 1.0f);
	//		ASSERT((row + 1) * frame_height <= 1.0f);

	//		const Vector p1(column * frame_width, 1.0f - (row + 1) * frame_height);
	//		const Vector p2(column * frame_width, 1.0f - row * frame_height);
	//		const Vector p3((column + 1) * frame_width, 1.0f - row * frame_height);
	//		const Vector p4((column + 1) * frame_width, 1.0f - (row + 1) * frame_height);
	//		// Display the specified frame.
	//		texture_position.SetP1(p1);
	//		texture_position.SetP2(p2);
	//		texture_position.SetP3(p3);
	//		texture_position.SetP4(p4);
	//	}
	//}



} //avl
} //utility