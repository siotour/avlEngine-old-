#pragma once
#ifndef AVL_MODEL_ANIMATED_SPRITE__
#define AVL_MODEL_ANIMATED_SPRITE__
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
Defines the AnimatedSprite class.
@author Sheldon Bachstein
@date Aug 13, 2012
*/

#include"..\sprite\sprite.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\quad\quad.h"
#include"..\..\..\utility\src\vector\vector.h"
#include<map>


namespace avl
{
namespace model
{

	/**
	Represents an animated, transformable image in 3D space. See
	\ref avl::model::Sprite for the full interface.
	*/
	class AnimatedSprite: public Sprite
	{
	public:
		// Forward declaration.
		struct Animation;

		/** @post Sets the position vertices to the origin (0, 0). Sets the texture
		dimensions to 1 column and 1 row. The sprite will be invisible until an
		animation is played.
		*/
		AnimatedSprite();
		/** @post The sprite will be invisible until an animation is played.
		*/
		AnimatedSprite(const utility::Quad& position, const float z, const utility::TexturedQuad::TextureHandle texture_handle, const unsigned short columns, const unsigned short rows);
		AnimatedSprite(const AnimatedSprite& original);
		~AnimatedSprite();

		/** Are we idling? A sprite is idle if repeat is off and the last
		animation played has ended. If a sprite is idle and an idle animation
		has been set, then it will play the idle animation repeatedly until
		a new animation is played.
		@return True if the sprite is idle, and false if not.
		*/
		const bool IsIdle() const;
		
		/**
		@post Invalidates all previously added animations.
		@throws utility::InvalidArgumentException If either \a columns or
		\a rows are zero.
		*/
		void SetTextureDimensions(const unsigned short columns, const unsigned short rows);

		/** Updates the state of the animated sprite with respect to the
		passage of \a elapsed_time seconds. If no idle animation is set
		and the last played animation finishes playing, then this sprite
		will become invisible.
		@param elapsed_time The amount of time passed since the last call
		to Update(), in seconds.
		*/
		void Update(const float elapsed_time);
		/** Adds a new animation which will be associated with the
		\c animation_id field of \a new_animation.
		@pre The \c animation_id field of \a new_animation may not be
		zero.
		@throws utility::InvalidArgumentException If the
		\c animation_id field of \a new_animation is zero.
		@throws utility::InvalidArgumentException If the
		\c animation_id field of \a new_animation is already associated
		with an animation.
		@throws utility::InvalidArgumentException If the number of
		frames in the animation is zero.
		@throws utility::InvalidArgumentException If the frame delay
		is not greater than zero.
		@throws utility::InvalidArgumentException If the new animation
		would run past the edge of the texture space.
		@throws utility::OutOfMemoryError If we run out of memory.
		*/
		void AddAnimation(const Animation& new_animation);
		void PlayAnimation(const short animation_id);
		/** Sets the animation which is to be automatically played when all
		other animations have finished playing.
		@param animation_id The identifier associated with the new idle
		animation.
		@throws utility::InvalidArgumentException If \a animation_id is
		not associated with an animation.
		@throws utility::OutOfMemoryError If we run out of memory.
		*/
		void SetIdleAnimation(const short animation_id);
		/** Specifies whether or not played animations should
		automatically be played repeatedly when they finish playing.
		@post If \a repeat is true, then this object will not become idle
		after an animation has been played until repeating is turned off.
		*/
		void SetRepeat(const bool repeat);

		const AnimatedSprite& operator=(const AnimatedSprite& rhs);

	private:
		/** Sets the sprite's state such that it will begin
		idling the next time that Update() is called.
		@post If an idle animation has been set, then that
		animation will be played repeatedly until a new
		animation is played. If no idle animation has been
		set, then the sprite will become invisible.
		*/
		void BeginIdling();
		/** Updates the sprite's state such that it
		currectly displays on the screen.
		@post If there is no animation playing then the sprite
		will become invisible. If there is an animation playing
		then the correct frame of that animation will be displayed.
		*/
		void UpdateAnimationFrame();

		/// The amount of time accumulated between frames in the
		/// animation.
		float accumulated_time;
		/// Do played animations repeat?
		bool is_repeating;
		/// Have all played animations finished playing?
		bool is_idle;

		const Animation* current_animation;
		unsigned short current_animation_id;
		const Animation* idle_animation;
		unsigned short idle_animation_id;
		/// The offset from the first frame in the current
		/// animation to the current frame.
		unsigned short current_frame;

		/// Maps an animation ID to its associated animation.
		typedef std::map<const unsigned short, Animation> AnimationMap;
		AnimationMap animations;

		/**
		Stores texture dimensions in terms of animation frames.
		Only supports even dimsions with no remainders left at any
		edges.
		*/
		struct TextureDimensions
		{
		public:
			unsigned short columns;
			unsigned short rows;
			unsigned short total_frames;
			float frame_width;
			float frame_height;
		};
		/// Keeps track of the dimensions of the texture we're using
		/// in terms of frames.
		TextureDimensions texture_dimensions;


	public:
		/**
		Stores the properties of an animation sequence.
		*/
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
	inline const bool AnimatedSprite::IsIdle() const
	{
		return is_idle;
	}

	// See method declaration for details.
	inline void AnimatedSprite::SetRepeat(const bool repeat)
	{
		is_repeating = repeat;
	}



} // model
} // avl
#endif // AVL_MODEL_ANIMATED_SPRITE__
