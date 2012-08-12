#pragma once
#ifndef AVL_UTILITY_QUAD__
#define AVL_UTILITY_QUAD__
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
Defines the \ref avl::utility::Quad class.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include"..\vector\vector.h"

namespace avl
{
namespace utility
{

	/**
	Encapsulates a quadrilateral in two dimensional space.
	*/
	class Quad
	{
	public:

		/** No-spec constructor. Initializes each vertex to (0, 0).
		*/
		Quad();
		/** Constructors a quad as an axis-aligned rectangle.
		@param left The left edge.
		@param top The top edge.
		@param right The right edge.
		@param bottom The bottom edge.
		*/
		Quad(const float left, const float top, const float right, const float bottom);
		/** Full-spec constructor.
		@param p1 The lower-left vertex.
		@param p2 The upper-left vertex.
		@param p3 The upper-right vertex.
		@param p4 The lower-right vertex.
		*/
		Quad(const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4);
		/** Copy constructor.
		@param original The object being copied.
		*/
		Quad(const Quad& original);
		/** Basic destructor.
		*/
		~Quad();

		/** Gets the center for this quad.
		@return The average of the four vertices of this quad.
		*/
		const Vector GetCenter() const;

		/** Gets \ref p1.
		@return \ref p1.
		*/
		const Vector& GetP1() const;
		/** Gets \ref p2.
		@return \ref p2.
		*/
		const Vector& GetP2() const;
		/** Gets \ref p3.
		@return \ref p3.
		*/
		const Vector& GetP3() const;
		/** Gets \ref p4.
		@return \ref p4.
		*/
		const Vector& GetP4() const;

		/** Translates the quad by \a delta_position.
		@param delta_position The amount by which to translate the quad.
		*/
		void Move(const Vector& delta_position);
		/** Translates the quad such that the center is at the voordinate
		\a new_center.
		@param new_center The new center for the quad.
		*/
		void SetCenter(const Vector& new_center);
		/** Rotates the quad by \a theta degrees counter-clockwise.
		@param theta The number of degrees by which to rotate the quad.
		*/
		void Rotate(float theta);
		/** Scales the quad by a factor of \a scale_factor.
		@param scale_factor The factor by which to scale the quad.
		*/
		void Scale(const float& scale_factor);

		/** Sets p1.
		@param p The new \ref p1.
		*/
		void SetP1(const Vector& p);
		/** Sets p2.
		@param p The new \ref p2.
		*/
		void SetP2(const Vector& p);
		/** Sets p3.
		@param p The new \ref p3.
		*/
		void SetP3(const Vector& p);
		/** Sets p4.
		@param p The new \ref p4.
		*/
		void SetP4(const Vector& p);
		
		/** Assignment operator.
		@param rhs The object which this object is being assigned to.
		@return This object after having been assigned to \a rhs.
		*/
		const Quad& operator=(const Quad& rhs);

	private:

		/// The lower-left vertex.
		Vector P1;
		/// The upper-left vertex.
		Vector P2;
		/// The upper-right vertex.
		Vector P3;
		/// The lower-right vertex.
		Vector P4;

	};




	// See method declaration for details.
	inline const Vector& Quad::GetP1() const
	{
		return P1;
	}

	// See method declaration for details.
	inline const Vector& Quad::GetP2() const
	{
		return P2;
	}

	// See method declaration for details.
	inline const Vector& Quad::GetP3() const
	{
		return P3;
	}

	// See method declaration for details.
	inline const Vector& Quad::GetP4() const
	{
		return P4;
	}

	// See method declaration for details.
	inline void Quad::Move(const Vector& delta_position)
	{
		P1 += delta_position;
		P2 += delta_position;
		P3 += delta_position;
		P4 += delta_position;
	}

	// See method declaration for details.
	inline void Quad::SetP1(const Vector& p)
	{
		P1 = p;
	}

	// See method declaration for details.
	inline void Quad::SetP2(const Vector& p)
	{
		P2 = p;
	}

	// See method declaration for details.
	inline void Quad::SetP3(const Vector& p)
	{
		P3 = p;
	}

	// See method declaration for details.
	inline void Quad::SetP4(const Vector& p)
	{
		P4 = p;
	}

	// See method declaration for details.
	inline const Quad& Quad::operator=(const Quad& rhs)
	{
		P1 = rhs.P1;
		P2 = rhs.P2;
		P3 = rhs.P3;
		P4 = rhs.P4;
		return *this;
	}


} // utility
} // avl
#endif // AVL_UTILITY_QUAD__
