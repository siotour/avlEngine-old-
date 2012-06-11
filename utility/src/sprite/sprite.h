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
Defines the utility::Sprite class, which represents a textured quadrilateral in 3-D space.
@author Sheldon Bachstein
@date Jan 21, 2011
*/


#include"..\vertex 2d\vertex 2d.h"
#include<list>

namespace avl
{
namespace utility
{
	// Forward declaration.
	class Sprite;

	/** Used for storing multiple Sprites in a light, sortable container. Intended to
	be used to send a collection of Sprites to a renderer.*/
	typedef std::list<Sprite* const> SpriteList;

	/** Counts the number of visible sprites contained in \a sprites.
	@pre \a sprites must not contain any null pointers.
	@param sprites The sprites to be counter (if they're visible).
	@throws InvalidArgumentException If one of the pointers in \a sprites is NULL.
	*/
	const unsigned int CountVisibleSprites(const SpriteList& sprites);


	/** Encapsulates a basic textured quadrilateral in 3-D space, oriented to be parallel to
	the XY plane. Includes accessors and mutators for the spatial coordinates as well as the texture
	coordinates. Additionally, defines a TextureHandle type which is used to represent textures.
	*/
	class Sprite
	{
	public:
		/** Represents a handle to a texture which is meant to be rendered to a sprite.*/
		typedef unsigned int TextureHandle;


		/** Basic constructor. Initializes each vertice to the origin with texture coordinates of (0, 0).
		*/
		Sprite();
		/** Full-spec constructor.
		@attention Winding order is clockwise.
		@param p1 Spatial coordinate. Adjacent to \a p2 and \a p4; opposite to \a p3.
		@param p2 Spatial coordinate. Adjacent to \a p1 and \a p3; opposite to \a p4.
		@param p3 Spatial coordinate. Adjacent to \a p3 and \a p4; opposite to \a p1.
		@param p4 Spatial coordinate. Adjacent to \a p3 and \a p1; opposite to \a p2.
		@param q1 Texture coordinate. Adjacent to \a q2 and \a q4; opposite to \a q3.
		@param q2 Texture coordinate. Adjacent to \a q1 and \a q3; opposite to \a q4.
		@param q3 Texture coordinate. Adjacent to \a q3 and \a q4; opposite to \a q1.
		@param q4 Texture coordinate. Adjacent to \a q3 and \a q1; opposite to \a q2.
		@param z The z-depth of the quadrilateral.
		@param texture_handle A handle to a texture meant to be rendered to this
		quadrilateral.
		*/
		Sprite(const Vertex2D& p1, const Vertex2D& p2, const Vertex2D& p3, const Vertex2D& p4,
			   const Vertex2D& q1, const Vertex2D& q2, const Vertex2D& q3, const Vertex2D& q4,
			   const float& z, const TextureHandle texture_handle);
		/** Creates an axis-aligned bounding box.
		@post The texture coordinates \ref q1, \ref q2, \ref q3, and \ref q4 will be initialized
		such that they span from (0, 0) in the bottom-left corner to (1, 1) in the top-right
		corner.
		@param left The leftmost boundary of the box.
		@param top The topmost boundary of the box.
		@param right The rightmost boundary of the box.
		@param bottom The bottommost boundary of the box.
		@param z The z-depth of the box.
		@param texture_handle A handle to a texture meant to be rendered to this box.
		*/
		Sprite(const float& left, const float& top, const float& right, const float& bottom,
			   const float& z, const TextureHandle texture_handle);
		/** Copy constructor.
		@param original The object being copied.
		*/
		Sprite(const Sprite& original);
		/** Basic destructor.*/
		~Sprite();

		/** Returns the visibility of the sprite.
		@return True if the sprite is visible, and false if not.
		*/
		const bool IsVisible() const;

		/** Returns the texture handle for this sprite.
		@return The texture handle of this sprite.
		*/
		const TextureHandle GetTextureHandle() const;

		/** Gets \ref untransformed_p1.
		@return \ref untransformed_p1.
		*/
		const Vertex2D& GetUntransformedP1() const;
		/** Gets \ref untransformed_p2.
		@return \ref untransformed_p2.
		*/
		const Vertex2D& GetUntransformedP2() const;
		/** Gets \ref untransformed_p3.
		@return \ref untransformed_p3.
		*/
		const Vertex2D& GetUntransformedP3() const;
		/** Gets \ref untransformed_p4.
		@return \ref untransformed_p4.
		*/
		const Vertex2D& GetUntransformedP4() const;

		/** Gets \ref p1.
		@return \ref p1.
		*/
		const Vertex2D& GetP1() const;
		/** Gets \ref p2.
		@return \ref p2.
		*/
		const Vertex2D& GetP2() const;
		/** Gets \ref p3.
		@return \ref p3.
		*/
		const Vertex2D& GetP3() const;
		/** Gets \ref p4.
		@return \ref p4.
		*/
		const Vertex2D& GetP4() const;

		/** Returns the center of the sprite.
		@return \ref center.
		*/
		const Vertex2D& GetCenter() const;
		/** Returns the current rotation of the sprite.
		@return \ref rotation.
		*/
		const float& GetRotation() const;
		/** Returns the current scale of the sprite.
		@return \ref scale.
		*/
		const float& GetScale() const;

		/** Gets \ref q1.
		@return \ref q1.
		*/
		const Vertex2D& GetQ1() const;
		/** Gets \ref q2.
		@return \ref q2.
		*/
		const Vertex2D& GetQ2() const;
		/** Gets \ref q3.
		@return \ref q3.
		*/
		const Vertex2D& GetQ3() const;
		/** Gets \ref q4.
		@return \ref q4.
		*/
		const Vertex2D& GetQ4() const;

		/** Returns the z-depth of the sprite.
		@return \ref z.
		*/
		const float& GetZ() const;




		/** Sets the sprite's visibility.
		@param new_visibility The new visibility for the sprite.
		*/
		void SetVisibility(const bool new_visibility);

		/** Sets the texture handle for this sprite.
		@param new_texture_handle The new texture handle.
		*/
		void SetTextureHandle(const TextureHandle new_texture_handle);

		/** Translates the sprite such that its center is at the specified vertex.
		@param new_center The new center for the sprite.
		*/
		void SetCenter(const Vertex2D& new_center);
		/** Moves the spatial coordinates along the vector \a delta.
		@param delta A vector along which to move the sprite.
		*/
		void Move(const Vertex2D& delta);
		/** Sets the rotation to \a new_rotation. Rotation is counter-clockwise.
		@param new_rotation The new rotation of the sprite in degrees.
		*/
		void SetRotation(const float& new_rotation);
		/** Rotates the quad counter-clockwise by \a delta_theta degrees.
		@param delta_theta The amount to rotate the sprite in degrees.
		*/
		void Rotate(const float& delta_theta);
		/** Sets the scale to \a new_scale.
		@param new_scale The new scale of the sprite.
		*/
		void SetScale(float new_scale);
		/** Scales the quad by the scaling factor \a delta_scale.
		If \ref scale is initially 3.0 and \a delta_scale is 2.0, then \ref scale
		will be 6.0 after a call to this function.
		@param delta_scale The relative factor by which to scale the sprite.
		*/
		void Scale(const float& delta_scale);

		/** Resets \ref rotation and \ref scale, and then sets the Sprite's spatial vertices and z-depth
		to those specified.
		@attention Winding order is clockwise.
		@param new_p1 Spatial coordinate. Adjacent to \a new_p2 and \a new_p4; opposite to \a new_p3.
		@param new_p2 Spatial coordinate. Adjacent to \a new_p1 and \a new_p3; opposite to \a new_p4.
		@param new_p3 Spatial coordinate. Adjacent to \a new_p3 and \a new_p4; opposite to \a new_p1.
		@param new_p4 Spatial coordinate. Adjacent to \a new_p3 and \a new_p1; opposite to \a new_p2.
		@param new_z The new z-depth of this quadrilateral.
		*/
		void ResetPosition(const Vertex2D& new_p1, const Vertex2D& new_p2, const Vertex2D& new_p3, const Vertex2D& new_p4, const float& new_z);
		/** Resets the \ref rotation and \ref scale, and then aligns the Sprite's position to the
		boundaries and z-depth specified.
		@param left The leftmost boundary of the box.
		@param top The topmost boundary of the box.
		@param right The rightmost boundary of the box.
		@param bottom The bottommost boundary of the box.
		@param new_z The z-depth of the box.
		*/
		void ResetPosition(const float& left, const float& top, const float& right, const float& bottom, const float& new_z);

		/** Sets q1.
		@param q Texture coordinate adjacent to \ref q4 and \ref q2; opposite to \ref q3.
		*/
		void SetQ1(const Vertex2D& q);
		/** Sets q2.
		@param q Texture coordinate adjacent to \ref q1 and \ref q3; opposite to \ref q4.
		*/
		void SetQ2(const Vertex2D& q);
		/** Sets q3.
		@param q Texture coordinate adjacent to \ref q2 and \ref q4; opposite to \ref q1.
		*/
		void SetQ3(const Vertex2D& q);
		/** Sets q4.
		@param q Texture coordinate adjacent to \ref q3 and \ref q1; opposite to \ref q2.
		*/
		void SetQ4(const Vertex2D& q);

		/** Moves the texture coordinates along the vector \a delta.
		@param delta A vector along which to move each texture coordinate.
		*/
		void MoveTexture(const Vertex2D& delta);

		/** Sets the z-depth of this sprite.
		@param new_z The new z-depth.
		*/
		void SetZ(const float& new_z);

		/** Assignment operator.
		@param rhs The Sprite to assign this one to.
		*/
		const Sprite& operator=(const Sprite& rhs);

	private:
		/** Finds the center of the sprite.
		@post \ref center will accurately be the center of this quadrilateral.
		*/
		void FindCenter();
		/** Rotates \a untransformed_point about the \a center_point by \a theta degrees counter-clockwise.
		@param center_point The point around which to rotate \a untransformed_point.
		@param untransformed_point The vertex to be rotated.
		@param theta The amount to rotate \a untransformed_point in degrees counter-clockwise.
		*/
		Vertex2D RotateVertice(const Vertex2D& center_point, const Vertex2D& untransformed_point, float theta);
		/** Scales \a untransformed_point in relation to \a center_point by \a scaling_factor.
		@param center_point The point about which to scale \a untransformed_point.
		@param untransformed_point The point to be scaled.
		@param scale_factor The relative factor by which to scale \a untransformed_point.
		*/
		Vertex2D ScaleVertice(const Vertex2D& center_point, const Vertex2D& untransformed_point, const float& scale_factor);

		/// Is this sprite currently visible or invisible?
		bool is_visible;

		/// The handle of the texture for this sprite.
		TextureHandle texture_handle;

		/// Untransformed spatial coordinate. Adjacent to \ref untransformed_p4 and
		/// \ref untransformed_p2, and opposite to \ref untransformed_p3.
		Vertex2D untransformed_p1;
		/// Untransformed spatial coordinate. Adjacent to \ref untransformed_p1 and
		/// \ref untransformed_p3, and opposite to \ref untransformed_p4.
		Vertex2D untransformed_p2;
		/// Untransformed spatial coordinate. Adjacent to \ref untransformed_p2 and
		/// \ref untransformed_p4, and opposite to \ref untransformed_p1.
		Vertex2D untransformed_p3;
		/// Untransformed spatial coordinate. Adjacent to \ref untransformed_p3 and
		/// \ref untransformed_p1, and opposite to \ref untransformed_p2.
		Vertex2D untransformed_p4;


		/// Transformed spatial coordinate. Adjacent to \ref p4 and \ref p1, and opposite to \ref p3.
		Vertex2D p1;
		/// Transformed spatial coordinate. Adjacent to \ref p1 and \ref p2, and opposite to \ref p4.
		Vertex2D p2;
		/// Transformed spatial coordinate. Adjacent to \ref p2 and \ref p3, and opposite to \ref p1.
		Vertex2D p3;
		/// Transformed spatial coordinate. Adjacent to \ref p3 and \ref p4, and opposite to \ref p2.
		Vertex2D p4;

		/// The center of the sprite.
		Vertex2D center;
		/// The current rotation in degrees counter-clockwise..
		float rotation;
		/// The current scale.
		float scale;

		/// The z-depth of the sprite.
		float z;

		/// Texture coordinate. Adjacent to \ref q4 and \ref q2, opposite to \ref q3.
		Vertex2D q1;
		/// Texture coordinate. Adjacent to \ref q1 and \ref q3, opposite to \ref q4.
		Vertex2D q2;
		/// Texture coordinate. Adjacent to \ref q2 and \ref q4, opposite to \ref q1.
		Vertex2D q3;
		/// Texture coordinate. Adjacent to \ref q3 and \ref q1, opposite to \ref q2.
		Vertex2D q4;
	};




} //avl
} //utility
#endif // AVL_UTILITY_SPRITE__