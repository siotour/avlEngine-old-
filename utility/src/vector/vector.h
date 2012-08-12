#pragma once
#ifndef AVL_UTILITY_VECTOR__
#define AVL_UTILITY_VECTOR__
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
Defines the \ref utility::Vector class which encapsulates a single 2-dimensional vector.
@author Sheldon Bachstein
@date Jan 17, 2011
*/



namespace avl
{
namespace utility
{
	/** Encapsulates a 2 dimensional vector. */
	class Vector
	{
	public:
		/** Default destructor.
		@post \ref x and \ref y will be initialized to 0.
		*/
		Vector();
		/** Full-spec constructor.
		@param x The coordinate along the x axis.
		@param y The coordinate along the y axis.
		*/
		Vector(const float& x, const float& y);
		/** Basic destructor.*/
		~Vector();
		/** Copy constructor.
		@param original The vector being copied.
		*/
		Vector(const Vector& original);

		/** Returns this vector's x coordinate.
		@return The x coordinate of this vector.
		*/
		const float& GetX() const;
		/** Returns this vector's y coordinate.
		@return The y coordinate of this vector.
		*/
		const float& GetY() const;
		
		/** Compares for equality.
		@param rhs The vector against which to compare this one.
		*/
		const bool operator==(const Vector& rhs) const;
		/** Compares for inequality.
		@param rhs The vector against which to compare this one.
		*/
		const bool operator!=(const Vector& rhs) const;

		/** Sets this vector's x coordinate.
		@param new_x The new x coordinate.
		*/
		void SetX(const float& new_x);
		/** Sets this vector's y coordinate.
		@param new_y The new y coordinate.
		*/
		void SetY(const float& new_y);
		
		/** Addition operator.
		@param rhs The other vector with which to sum this one.
		@return The vector which is the sum of this vector and \a rhs.
		*/
		Vector operator+(const Vector& rhs) const;
		/** Subtraction operator.
		@param rhs The other vector with which to form the different with this one.
		@return The vector which is the difference of this vector and \a rhs.
		*/
		Vector operator-(const Vector& rhs) const;
		
		/** Adds \a rhs to this vector.
		@param rhs The vector to add to this one.
		@return This vector after it has had \a rhs added to it.
		*/
		Vector& operator+=(const Vector& rhs);
		/** Subtracts \a rhs from this vector.
		@param rhs The vector to sutract from this one.
		@return This vector after it has had \a rhs subtracted from it.
		*/
		Vector& operator-=(const Vector& rhs);
		
		/** Assignment operator.
		@param rhs The vector to set this one equal to.
		@return This vector after it has been assigned to \a rhs.
		*/
		const Vector& operator=(const Vector& rhs);

	private:
		/// The x coordinate.
		float x;
		/// The y coordinate.
		float y;
	};




} //avl
} //utility
#endif // AVL_UTILITY_VECTOR__