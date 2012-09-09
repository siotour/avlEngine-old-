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
Implementation for the animated sprite component. See "animated sprite.h" for details.
@author Sheldon Bachstein
@date Aug 13, 2012
*/

#include"animated sprite.h"
#include"..\sprite\sprite.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\vector\vector.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\quad\quad.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<map>
#include<cmath>


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
	AnimatedSprite::AnimatedSprite()
		: Sprite(), accumulated_time(0.0f), is_repeating(false), is_idle(true), current_animation(nullptr), current_animation_id(0), current_frame(0), idle_animation(nullptr), idle_animation_id(0)
	{
		SetTextureDimensions(1, 1);
		UpdateAnimationFrame();
	}

	// See method declaration for details.
	AnimatedSprite::AnimatedSprite(const Quad& initial_quad, const float z, const TexturedQuad::TextureHandle texture_handle, const unsigned short columns, const unsigned short rows)
		: Sprite(initial_quad, z, texture_handle), accumulated_time(0.0f), is_repeating(false), is_idle(true), current_animation(nullptr), current_animation_id(0), current_frame(0), idle_animation(nullptr), idle_animation_id(0)
	{
		SetTextureDimensions(columns, rows);
		UpdateAnimationFrame();
	}

	// See method declaration for details.
	AnimatedSprite::AnimatedSprite(const AnimatedSprite& original)
		: Sprite(original), accumulated_time(original.accumulated_time), is_repeating(original.is_repeating), is_idle(original.is_idle), current_animation_id(original.current_animation_id), current_frame(original.current_frame), idle_animation_id(original.idle_animation_id)
	{
		current_animation = &animations.find(current_animation_id)->second;
		idle_animation = &animations.find(idle_animation_id)->second;
		UpdateAnimationFrame();
	}

	// See method declaration for details.
	AnimatedSprite::~AnimatedSprite()
	{
	}

	// See method declaration for details.
	void AnimatedSprite::SetTextureDimensions(const unsigned short columns, const unsigned short rows)
	{
		if(columns < 1)
		{
			throw InvalidArgumentException("avl::utility::AnimatedSprite::SetTextureDimensions()", "columns", "Must be at least one.");
		}
		texture_dimensions.columns = columns;
		texture_dimensions.frame_width = 1.0f / columns;
		if(rows < 1)
		{
			throw InvalidArgumentException("avl::utility::AnimatedSprite::SetTextureDimensions()", "rows", "Must be at least one.");
		}
		texture_dimensions.rows = rows;
		texture_dimensions.frame_height = 1.0f / rows;
		texture_dimensions.total_frames = texture_dimensions.rows * texture_dimensions.columns;

		current_animation = nullptr;
		current_animation_id = 0;
		idle_animation = nullptr;
		idle_animation_id;
		animations.clear();
	}

	// See method declaration for details.
	void AnimatedSprite::Update(const float elapsed_time)
	{
		/*
		if(AnimationIsPlaying() == true)
		{
			UpdateFrame(elapsed_time);
			UpdateAnimation();
		}
		*/

		if(current_animation != nullptr)
		{
			accumulated_time += elapsed_time;
			// Is the current frame completed?
			while(accumulated_time >= current_animation->frame_delay)
			{
				accumulated_time -= current_animation->frame_delay;
				++current_frame;
				// Is the current animation completed?
				if(current_frame >= current_animation->number_of_frames)
				{
					// If we're repeating then restart the current animation.
					if(is_repeating == true)
					{
						current_frame = 0;
					}
					// Transition to the idle animation.
					else
					{
						BeginIdling();
					}
				}
			}
		}
		UpdateAnimationFrame();
	}

	// See method declaration for details.
	void AnimatedSprite::AddAnimation(const Animation& new_animation)
	{
		if(new_animation.animation_id == 0)
		{
			throw InvalidArgumentException("avl::utility::AnimatedSprite::AddAnimation()", "new_animation", "Zero cannot be used as an animation ID.");
		}
		else if(new_animation.number_of_frames == 0)
		{
			throw InvalidArgumentException("avl::utility::AnimatedSprite::AddAnimation()", "new_animation", "Each animation must have at least one frame.");
		}
		else if(new_animation.frame_delay <= 0.0f)
		{
			throw InvalidArgumentException("avl::utility::AnimatedSprite::AddAnimation()", "new_animation", "The frame delay must be greater than zero.");
		}
		else if(new_animation.start_frame + new_animation.number_of_frames - 1 > texture_dimensions.total_frames)
		{
			throw InvalidArgumentException("avl::utility::AnimatedSprite::AddAnimation()", "new_animation", "The specified animation overflows the texture space (start frame + number of frames is too big).");
		}
		try
		{
			auto result = animations.insert(std::make_pair(new_animation.animation_id, new_animation));
			if(result.second == false)
			{
				throw InvalidArgumentException("avl::utility::AnimatedSprite::AddAnimation()", "new_animation", "An animation with this animation ID already exists.");
			}
		}
		catch(const std::bad_alloc&)
		{
			throw OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void AnimatedSprite::PlayAnimation(const short animation_id)
	{
		if(current_animation_id != animation_id)
		{
			auto animation = animations.find(animation_id);
			if(animation == animations.end())
			{
				throw InvalidArgumentException("avl::utility::AnimatedSprite::PlayAnimation()", "animation_id", "Must be a valid ID to an animation previously added using AnimatedSprite::AddAnimation().");
			}
			current_animation_id = animation_id;
			current_animation = &(animation->second);
		}
		current_frame = 0;
		accumulated_time = 0.0f;
		is_idle = false;
		UpdateAnimationFrame();
	}

	// See method declaration for details.
	void AnimatedSprite::SetIdleAnimation(const short animation_id)
	{
		if(idle_animation_id != animation_id)
		{
			auto animation = animations.find(animation_id);
			if(animation == animations.end())
			{
				throw InvalidArgumentException("avl::utility::AnimatedSprite::SetIdleAnimation()", "animation_id", "Must be a valid ID to an animation previously added using AnimatedSprite::AddAnimation().");
			}
			idle_animation_id = animation_id;
			idle_animation = &(animation->second);

			if(is_idle == true)
			{
				BeginIdling();
				UpdateAnimationFrame();
			}
		}
	}

	// See method declaration for details.
	const AnimatedSprite& AnimatedSprite::operator=(const AnimatedSprite& rhs)
	{
		Sprite::operator=(rhs);
		accumulated_time = rhs.accumulated_time;
		is_repeating = rhs.is_repeating;
		is_idle = rhs.is_idle;
		idle_animation_id = rhs.idle_animation_id;
		try
		{
			animations = rhs.animations;
		}
		catch(const std::bad_alloc&)
		{
			throw OutOfMemoryError();
		}
		current_animation_id = rhs.current_animation_id;
		current_frame = rhs.current_frame;
		current_animation = &animations.find(current_animation_id)->second;
		idle_animation = &animations.find(idle_animation_id)->second;
		UpdateAnimationFrame();
		return *this;
	}

	// See method declaration for details.
	void AnimatedSprite::BeginIdling()
	{
		is_idle = true;
		if(current_animation != idle_animation)
		{
			current_animation_id = idle_animation_id;
			current_animation = idle_animation;
		}
		current_frame = 0;
	}

	// See method declaration for details.
	void AnimatedSprite::UpdateAnimationFrame()
	{
		ASSERT(texture_dimensions.frame_width >= 0.0f);
		ASSERT(texture_dimensions.frame_height >= 0.0f);

		if(current_animation == nullptr)
		{
			quad.SetVisibility(false);
			return;
		}
		else
		{
			quad.SetVisibility(true);
		}

		// Figure out which row and column the current current_animation is in.
		const unsigned short row = (current_animation->start_frame + current_frame - 1) / texture_dimensions.columns;
		const unsigned short column = (current_animation->start_frame + current_frame - 1) % texture_dimensions.columns;
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



} // model
} // avl
