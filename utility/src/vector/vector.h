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
Defines the \ref avl::utility::Vector class which encapsulates
a single 2-dimensional vector.
@author Sheldon Bachstein
@date Jan 17, 2011
*/



namespace avl
{
namespace utility
{
	/**
	Encapsulates a 2 dimensional vector.
	*/
	class Vector
	{
	public:
		/// The vector with zero magnitude.
		static const Vector ZeroVector;


		/** Default constructor.
		@post \ref x and \ref y will be initialized to 0.
		*/
		Vector();
		Vector(const float& x, const float& y);
		~Vector();
		Vector(const Vector& original);

		const float Length() const;

		const float& GetX() const;
		const float& GetY() const;
		
		const bool operator==(const Vector& rhs) const;
		const bool operator!=(const Vector& rhs) const;

		void SetX(const float& new_x);
		void SetY(const float& new_y);
		
		Vector operator+(const Vector& rhs) const;
		Vector operator-(const Vector& rhs) const;
		Vector operator*(const Vector& rhs) const;
		/** @throws DivideByZeroError If either component of \a rhs
		is 0.*/
		Vector operator/(const Vector& rhs) const;

		Vector& operator+=(const Vector& rhs);
		Vector& operator-=(const Vector& rhs);
		Vector& operator*=(const Vector& rhs);
		/** @throws DivideByZeroError If either component of \a rhs
		is 0.*/
		Vector& operator/=(const Vector& rhs);

		Vector operator*(const float rhs) const;
		/// @throws DivideByZeroError If \a rhs is zero.
		Vector operator/(const float rhs) const;

		Vector& operator*=(const float rhs);
		/// @throws DivideByZeroError If \a rhs is 0.
		Vector& operator/=(const float rhs);
		
		const Vector& operator=(const Vector& rhs);

	private:
		/// The x coordinate.
		float x;
		/// The y coordinate.
		float y;
	};


	// See method declaration for details.
	inline const float& Vector::GetX() const
	{
		return x;
	}

	// See method declaration for details.
	inline const float& Vector::GetY() const
	{
		return y;
	}

	// See method declaration for details.
	inline void Vector::SetX(const float& new_x)
	{
		x = new_x;
	}

	// See method declaration for details.
	inline void Vector::SetY(const float& new_y)
	{
		y = new_y;
	}

	// See method declaration for details.
	inline const bool Vector::operator==(const Vector& rhs) const
	{
		return (x == rhs.GetX() && y == rhs.GetY());
	}

	// See method declaration for details.
	inline const bool Vector::operator!=(const Vector& rhs) const
	{
		return (x != rhs.GetX() || y != rhs.GetY());
	}

	// See method declaration for details.
	inline Vector Vector::operator+(const Vector& rhs) const
	{
		return Vector(x + rhs.GetX(), y + rhs.GetY());
	}

	// See method declaration for details.
	inline Vector Vector::operator-(const Vector& rhs) const
	{
		return Vector(x - rhs.GetX(), y - rhs.GetY());
	}

	// See method declaration for details.
	inline Vector Vector::operator*(const Vector& rhs) const
	{
		return Vector(x * rhs.GetX(), y * rhs.GetY());
	}

	// See method declaration for details.
	inline Vector& Vector::operator+=(const Vector& rhs)
	{
		x += rhs.GetX();
		y += rhs.GetY();
		return *this;
	}

	// See method declaration for details.
	inline Vector& Vector::operator-=(const Vector& rhs)
	{
		x -= rhs.GetX();
		y -= rhs.GetY();
		return *this;
	}

	// See method declaration for details.
	inline Vector& Vector::operator*=(const Vector& rhs)
	{
		x *= rhs.GetX();
		y *= rhs.GetY();
		return *this;
	}

	// See method declaration for details.
	inline Vector Vector::operator*(const float rhs) const
	{
		return Vector(x * rhs, y * rhs);
	}

	// See method declaration for details.
	inline Vector& Vector::operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	// See method declaration for details.
	inline const Vector& Vector::operator=(const Vector& rhs)
	{
		x = rhs.GetX();
		y = rhs.GetY();
		return *this;
	}


} //avl
} //utility
#endif // AVL_UTILITY_VECTOR__