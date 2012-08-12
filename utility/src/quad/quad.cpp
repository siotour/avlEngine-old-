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
Implementation for the quad component. See "quad.h" for details.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include"quad.h"
#include"..\vector\vector.h"
#include<cmath>


namespace avl
{
namespace utility
{
	// Definitions and forward declarations.
	namespace
	{
		// A constanct for Pi.
		const double PI = 4.0 * std::atan(1.0);
		// Forward declarations. See definitions for details.
		void RotateVertex(Vector& original_point, const Vector& center, float theta);
		void ScaleVertex(Vector& original_point, const Vector& center, const float& scale_factor);
	}



	// See method declaration for details.
	Quad::Quad()
		: P1(0.0f, 0.0f), P2(0.0f, 0.0f), P3(0.0f, 0.0f), P4(0.0f, 0.0f)
	{
	}

	// See method declaration for details.
	Quad::Quad(const float left, const float top, const float right, const float bottom)
		: P1(left, bottom), P2(left, top), P3(right, top), P4(right, bottom)
	{
	}

	// See method declaration for details.
	Quad::Quad(const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4)
		: P1(p1), P2(p2), P3(p3), P4(p4)
	{
	}

	// See method declaration for details.
	Quad::Quad(const Quad& original)
		: P1(original.P1), P2(original.P2), P3(original.P3), P4(original.P4)
	{
	}

	// See method declaration for details.
	Quad::~Quad()
	{
	}

	// See method declaration for details.
	const Vector Quad::GetCenter() const
	{
		Vector center;
		// The x-coordinate of the center will be the average of the x-coordinates of all
		// vertices.
		center.SetX((P1.GetX() + P2.GetX() + P3.GetX() + P4.GetX()) / 4);
		// The y-coordinate of the center will be the average of the y-coordinates of all
		// vertices.
		center.SetY((P1.GetY() + P2.GetY() + P3.GetY() + P4.GetY()) / 4);
		return center;
	}

	// See method declaration for details.
	void Quad::SetCenter(const Vector& new_center)
	{
		// Figure out the difference between the old and new centers.
		const Vector delta_position = new_center - GetCenter();
		// Move to the new center.
		Move(delta_position);
	}

	// See method declaration for details.
	void Quad::Rotate(float theta)
	{
		// Convert theta from degrees to radians.
		theta = (theta / 180.0f) * (float)PI;
		// Rotate each point.
		const Vector center = GetCenter();
		RotateVertex(P1, center, theta);
		RotateVertex(P2, center, theta);
		RotateVertex(P3, center, theta);
		RotateVertex(P4, center, theta);
	}

	// See method declaration for details.
	void Quad::Scale(const float& scale_factor)
	{
		// Scale each point.
		const Vector center = GetCenter();
		ScaleVertex(P1, center, scale_factor);
		ScaleVertex(P2, center, scale_factor);
		ScaleVertex(P3, center, scale_factor);
		ScaleVertex(P4, center, scale_factor);
	}



	// Anonymous namespace.
	namespace
	{

		/** Rotates \a original_point about \a center by \a theta radians
		counter-clockwise.
		@post \a original_point will be rotated by \a theta radians counter-clockwise.
		@param original_point [IN/OUT] The point being rotated.
		@param center The center about which to rotate.
		@param theta The degrees by which to rotate.
		*/
		void RotateVertex(Vector& original_point, const Vector& center, const float theta)
		{
			// Translate original_point such that it relates to the origin as it
			// currently relates to center.
			original_point -= center;
			// Now rotate the translated point by theta radians counter-clockwise.
			const float x_prime = cos(theta) * original_point.GetX() - sin(theta) * original_point.GetY();
			const float y_prime = sin(theta) * original_point.GetX() + cos(theta) * original_point.GetY();
			original_point.SetX(x_prime);
			original_point.SetY(y_prime);
			// Now translate back to be relative to center.
			original_point += center;
		}


		/** Scales \a original_point about \a center by a factor of \a scale_factor.
		@post \a original_point will be scales by a factor of \a scale_factor about
		\a center.
		@param original_point [IN/OUT] The point being scaled.
		@param center The center about which to scale.
		@param scale_factor The factor by which to scale.
		*/
		void ScaleVertex(Vector& original_point, const Vector& center, const float& scale_factor)
		{
			// Translate untransformed_point such that it relates to the origin as it
			// currently relates to center_point.
			original_point -= center;
			// Scale each component of original_point by scale_factor.
			const float x_prime = original_point.GetX() * scale_factor;
			const float y_prime = original_point.GetY() * scale_factor;
			original_point.SetX(x_prime);
			original_point.SetY(y_prime);
			// Translate the vertice back so that it's relative to center_point.
			original_point += center;
		}

	}

} // utility
} // avl
