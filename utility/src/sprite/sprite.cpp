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
#include"..\vertex 2d\vertex 2d.h"
#include"..\exceptions\exceptions.h"
#include"..\assert\assert.h"
#include<cmath>
#include<limits>



namespace avl
{
namespace utility
{
	// Unnamed namespace for utility stuff.
	namespace
	{
		// Pi.
		const double PI = 4.0 * std::atan(1.0);
	}



	// See function declaration for details.
	const unsigned int CountVisibleSprites(const SpriteList& sprites)
	{
		// Count the number of visible sprites and check for nullptr pointers.
		unsigned int number_of_sprites = 0;
		for(SpriteList::const_iterator i = sprites.begin(); i != sprites.end(); ++i)
		{
			// Make sure this pointer is not nullptr.
			if((*i) == nullptr)
			{
				throw InvalidArgumentException("avl::utility::CountSprites()", "sprites", "Contains one or more nullptr pointers.");
			}
			// Count visible sprites.
			else if((*i)->IsVisible() != false)
			{
				++number_of_sprites;
			}
		}
		return number_of_sprites;
	}



	// Basic constructor. Each vertice (both spatial and texture) is initialized to (0, 0),
	// and z-depth is set to 0. The scale is initialized to 1.0.
	Sprite::Sprite()
		: p1(0, 0), p2(0, 0), p3(0, 0), p4(0, 0), q1(0, 0), q2(0, 0), q3(0, 0), q4(0, 0),
		  untransformed_p1(0, 0), untransformed_p2(0, 0), untransformed_p3(0, 0), untransformed_p4(0, 0),
		  center(0, 0), rotation(0.0f), scale(1.0f), z(0.0f), texture_handle(0), is_visible(true)
	{
	}




	// Initializes each of the vertices to those supplied by the user. Winding order is clockwise.
	// p1-p4 are the spatial coordinates, and q1-q4 are the texture coordinates. z is the z-depth
	// of the sprite. texture_handle is the texture handle for the sprite.
	Sprite::Sprite(const Vertex2D& p1, const Vertex2D& p2, const Vertex2D& p3, const Vertex2D& p4,
				   const Vertex2D& q1, const Vertex2D& q2, const Vertex2D& q3, const Vertex2D& q4,
				   const float& z, const TextureHandle texture_handle)
	: p1(p1), p2(p2), p3(p3), p4(p4), q1(q1), q2(q2), q3(q3), q4(q4),
	  untransformed_p1(p1), untransformed_p2(p2), untransformed_p3(p3), untransformed_p4(p4),
	  rotation(0.0f), scale(1.0f), z(z), texture_handle(texture_handle), is_visible(true)
	{
		// Figure out the center.
		FindCenter();
	}




	// Creates an axis-aligned bounding box with texture coordinates ranging from (0, 0) in the
	// bottom-left to (1, 1) in the top-right and with z-depth z. texture_handle is the texture
	// handle of the sprite.
	Sprite::Sprite(const float& left, const float& top, const float& right, const float& bottom,
				   const float& z, const TextureHandle texture_handle)
		: p1(left, bottom), p2(left, top), p3(right, top), p4(right, bottom), q1(0.0f, 0.0f), q2(0.0f, 1.0f), q3(1.0f, 1.0f), q4(1.0f, 0.0f),
		  untransformed_p1(left, bottom), untransformed_p2(left, top), untransformed_p3(right, top), untransformed_p4(right, bottom),
		  rotation(0.0f), scale(1.0f), z(z), texture_handle(texture_handle), is_visible(true)
	{
		// Figure out the center.
		FindCenter();
	}




	// Copy constructor.
	Sprite::Sprite(const Sprite& original)
		: untransformed_p1(original.GetUntransformedP1()), untransformed_p2(original.GetUntransformedP2()), untransformed_p3(original.GetUntransformedP3()), untransformed_p4(original.GetUntransformedP4()),
		p1(original.GetP1()), p2(original.GetP2()), p3(original.GetP3()), p4(original.GetP4()), 
		  q1(original.GetQ1()), q2(original.GetQ2()), q3(original.GetQ3()), q4(original.GetQ4()),
		  center(original.GetCenter()), rotation(original.GetRotation()), scale(original.GetScale()),
		  z(original.GetZ()), texture_handle(original.GetTextureHandle()), is_visible(original.IsVisible())
	{
	}




	// Basic desctructor.
	Sprite::~Sprite()
	{
	}




	// Returns the visibility of the sprite.
	const bool Sprite::IsVisible() const
	{
		return is_visible;
	}




	// Returns the texture handle for this sprite.
	const Sprite::TextureHandle Sprite::GetTextureHandle() const
	{
		return texture_handle;
	}




	// Returns transformed vertex p1.
	const Vertex2D& Sprite::GetUntransformedP1() const
	{
		return untransformed_p1;
	}




	// Returns transformed vertex p2.
	const Vertex2D& Sprite::GetUntransformedP2() const
	{
		return untransformed_p2;
	}




	// Returns transformed vertex p3.
	const Vertex2D& Sprite::GetUntransformedP3() const
	{
		return untransformed_p3;
	}




	// Returns transformed vertex p4.
	const Vertex2D& Sprite::GetUntransformedP4() const
	{
		return untransformed_p4;
	}




	// Returns transformed vertex p1.
	const Vertex2D& Sprite::GetP1() const
	{
		return p1;
	}




	// Returns transformed vertex p2.
	const Vertex2D& Sprite::GetP2() const
	{
		return p2;
	}




	// Returns transformed vertex p3.
	const Vertex2D& Sprite::GetP3() const
	{
		return p3;
	}




	// Returns transformed vertex p4.
	const Vertex2D& Sprite::GetP4() const
	{
		return p4;
	}




	// Returns the center of the sprite.
	const Vertex2D& Sprite::GetCenter() const
	{
		return center;
	}




	// Returns the current rotation of the sprite.
	const float& Sprite::GetRotation() const
	{
		return rotation;
	}




	// Returns the current scale factor for the sprite.
	const float& Sprite::GetScale() const
	{
		return scale;
	}




	// Returns vertex q1.
	const Vertex2D& Sprite::GetQ1() const
	{
		return q1;
	}




	// Returns vertex q2.
	const Vertex2D& Sprite::GetQ2() const
	{
		return q2;
	}




	// Returns vertex q3.
	const Vertex2D& Sprite::GetQ3() const
	{
		return q3;
	}




	// Returns vertex q4.
	const Vertex2D& Sprite::GetQ4() const
	{
		return q4;
	}




	// Returns the z-depth of the sprite.
	const float& Sprite::GetZ() const
	{
		return z;
	}




	// Sets the texture handle for this sprite.
	void Sprite::SetTextureHandle(const Sprite::TextureHandle new_texture_handle)
	{
		texture_handle = new_texture_handle;
	}




	// Sets the sprite's visibility.
	void Sprite::SetVisibility(const bool new_visibility)
	{
		is_visible = new_visibility;
	}




	// Translates the sprite such that its center is at the specified vertex.
	void Sprite::SetCenter(const Vertex2D& new_center)
	{
		// Figure out the difference between the old and new centers.
		Vertex2D difference = new_center - center;

		// Now set the new center.
		center = new_center;

		// Adjust each vertice, including the untransformed ones.
		untransformed_p1 += difference;
		untransformed_p2 += difference;
		untransformed_p3 += difference;
		untransformed_p4 += difference;
		p1 += difference;
		p2 += difference;
		p3 += difference;
		p4 += difference;
	}




	// Moves the sprite's spatial coordinates along the delta vector.
	void Sprite::Move(const Vertex2D& delta)
	{
		untransformed_p1 += delta;
		untransformed_p2 += delta;
		untransformed_p3 += delta;
		untransformed_p4 += delta;

		p1 += delta;
		p2 += delta;
		p3 += delta;
		p4 += delta;

		center += delta;
	}




	// Sets the rotation to the rotation specified. Rotation is counter-clockwise.
	void Sprite::SetRotation(const float& new_rotation)
	{
		// Simplify theta down to between 0 and 360.
		float theta = fmod(new_rotation, 360.0f);

		// If theta is the same as the old rotation, simply return.
		if(theta == rotation)
		{
			return;
		}

		// Save the new rotation.
		rotation = theta;

		// Set each transformed vertice equal to the untransformed version rotated by theta.
		p1 = RotateVertice(center, untransformed_p1, theta);
		p1 = ScaleVertice(center, p1, scale);
		p2 = RotateVertice(center, untransformed_p2, theta);
		p2 = ScaleVertice(center, p2, scale);
		p3 = RotateVertice(center, untransformed_p3, theta);
		p3 = ScaleVertice(center, p3, scale);
		p4 = RotateVertice(center, untransformed_p4, theta);
		p4 = ScaleVertice(center, p4, scale);
	}




	// Rotates the quad counter-clockwise by the degrees specified.
	void Sprite::Rotate(const float& delta_theta)
	{
		SetRotation(rotation + delta_theta);
	}




	//Sets the scale to the specified scale.
	void Sprite::SetScale(float new_scale)
	{
		// The minimum scale is numeric_limits<float>::epsilon().
		if(new_scale < std::numeric_limits<float>::epsilon())
		{
			new_scale = std::numeric_limits<float>::epsilon();
		}

		// If new_scale is the same as the current scale, return.
		if(new_scale == scale)
		{
			return;
		}

		// Save the new scale factor.
		scale = new_scale;

		// Set each transformed vertice equal to the scaled equivalent of its untransformed counterpart.
		p1 = ScaleVertice(center, untransformed_p1, scale);
		p1 = RotateVertice(center, p1, rotation);
		p2 = ScaleVertice(center, untransformed_p2, scale);
		p2 = RotateVertice(center, p2, rotation);
		p3 = ScaleVertice(center, untransformed_p3, scale);
		p3 = RotateVertice(center, p3, rotation);
		p4 = ScaleVertice(center, untransformed_p4, scale);
		p4 = RotateVertice(center, p4, rotation);
	}




	// Scales the quad by the scaling factor specified.
	void Sprite::Scale(const float& delta_scale)
	{
		SetScale(scale * delta_scale);
	}




	// Resets rotation and scale, and then sets the Sprite's vertices and z-depth
	// to chose specified.
	void Sprite::ResetPosition(const Vertex2D& new_p1, const Vertex2D& new_p2, const Vertex2D& new_p3, const Vertex2D& new_p4, const float& new_z)
	{
		// Reset scale and rotation.
		rotation = 0.0f;
		scale = 1.0f;

		// Save the new z-depth.
		z = new_z;

		// Store the new vertices.
		p1 = untransformed_p1 = new_p1;
		p2 = untransformed_p2 = new_p2;
		p3 = untransformed_p3 = new_p3;
		p4 = untransformed_p4 = new_p4;

		// Find the new center.
		FindCenter();
	}




	// Resets the rotation and scale, and then aligns the Sprite's positions to
	// the boundaries and z-depth specified.
	void Sprite::ResetPosition(const float& left, const float& top, const float& right, const float& bottom, const float& new_z)
	{
		// Reset scale and rotation.
		rotation = 0.0f;
		scale = 1.0f;

		// Save the new z-depth.
		z = new_z;

		// Set up the new vertex values.
		p1.SetX(left);
		p1.SetY(bottom);
		p2.SetX(left);
		p2.SetY(top);
		p3.SetX(right);
		p3.SetY(top);
		p4.SetX(right);
		p4.SetY(bottom);

		untransformed_p1.SetX(left);
		untransformed_p1.SetY(bottom);
		untransformed_p2.SetX(left);
		untransformed_p2.SetY(top);
		untransformed_p3.SetX(right);
		untransformed_p3.SetY(top);
		untransformed_p4.SetX(right);
		untransformed_p4.SetY(bottom);

		// Find the new center.
		FindCenter();
	}




	// Sets the texture coordinate for the vertice p1.
	void Sprite::SetQ1(const Vertex2D& q)
	{
		q1 = q;
	}




	// Sets the texture coordinate for the vertice p1.
	void Sprite::SetQ2(const Vertex2D& q)
	{
		q2 = q;
	}




	// Sets the texture coordinate for the vertice p1.
	void Sprite::SetQ3(const Vertex2D& q)
	{
		q3 = q;
	}




	// Sets the texture coordinate for the vertice p1.
	void Sprite::SetQ4(const Vertex2D& q)
	{
		q4 = q;
	}




	// Moves the texture coordinates along the vector delta.
	void Sprite::MoveTexture(const Vertex2D& delta)
	{
		q1 += delta;
		q2 += delta;
		q3 += delta;
		q4 += delta;
	}




	// Sets the z-depth of this sprite.
	void Sprite::SetZ(const float& new_z)
	{
		z = new_z;
	}




	// Assignment operator.
	const Sprite& Sprite::operator=(const Sprite& rhs)
	{
		// Untransformed vertices.
		untransformed_p1 = rhs.GetUntransformedP1();
		untransformed_p2 = rhs.GetUntransformedP2();
		untransformed_p3 = rhs.GetUntransformedP3();
		untransformed_p4 = rhs.GetUntransformedP4();

		// Transformed vertices.
		p1 = rhs.GetP1();
		p2 = rhs.GetP2();
		p3 = rhs.GetP3();
		p4 = rhs.GetP4();

		// Texture coordinates.
		q1 = rhs.GetQ1();
		q2 = rhs.GetQ2();
		q3 = rhs.GetQ3();
		q4 = rhs.GetQ4();

		// Center, rotation, scale, z-depth, and texture handle.
		center = rhs.GetCenter();
		rotation = rhs.GetRotation();
		scale = rhs.GetScale();
		z = rhs.GetZ();
		texture_handle = rhs.GetTextureHandle();
		return *this;
	}




	// Finds the center of the sprite and assigns those coordinates to the member center.
	void Sprite::FindCenter()
	{
		// The x-coordinate of the center will be the average of the x-coordinates of all
		// vertices.
		center.SetX((p1.GetX() + p2.GetX() + p3.GetX() + p4.GetX()) / 4);
		// The y-coordinate of the center will be the average of the y-coordinates of all
		// vertices.
		center.SetY((p1.GetY() + p2.GetY() + p3.GetY() + p4.GetY()) / 4);
	}



	// Rotates untransformed_point about the center_point by theta degrees counter-clockwise.
	Vertex2D Sprite::RotateVertice(const Vertex2D& center_point, const Vertex2D& untransformed_point, float theta)
	{
		// Convert theta from degrees to radians.
		theta = (theta / 180.0f) * (float)PI;

		// Translate untransformed_point such that it relates to the origin as it
		// currently relates to center_point.
		Vertex2D translated_point = untransformed_point - center_point;

		// Now rotate the translated point by theta degrees counter-clockwise.
		const float x_prime = cos(theta) * translated_point.GetX() - sin(theta) * translated_point.GetY();
		const float y_prime = sin(theta) * translated_point.GetX() + cos(theta) * translated_point.GetY();
		translated_point.SetX(x_prime);
		translated_point.SetY(y_prime);


		// Now translate back to be relative to center_point.
		translated_point += center_point;

		// Return the rotated vertice.
		return translated_point;
	}




	// Scales untransformed_point in relation to center_point by scaling_factor.
	Vertex2D Sprite::ScaleVertice(const Vertex2D& center_point, const Vertex2D& untransformed_point, const float& scale_factor)
	{
		// Translate untransformed_point such that it relates to the origin as it
		// currently relates to center_point.
		Vertex2D translated_point = untransformed_point - center_point;

		// Scale each component of translated_point by scale_factor.
		const float x_prime = translated_point.GetX() * scale_factor;
		const float y_prime = translated_point.GetY() * scale_factor;
		translated_point.SetX(x_prime);
		translated_point.SetY(y_prime);

		// Translate the vertice back so that it's relative to center_point.
		translated_point += center_point;

		// Return the scaled vertice.
		return translated_point;
	}


}
}