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
Implementation for the vector component. See "vector.h" for details.
@author Sheldon Bachstein
@date January 17, 2011
*/

#include"vector.h"


namespace avl
{
namespace utility
{


	// See method declaration for details.
	Vector::Vector()
		: x(0.0f), y(0.0f)
	{
	}

	// See method declaration for details.
	Vector::Vector(const float& x, const float& y)
		: x(x), y(y)
	{
	}

	// See method declaration for details.
	Vector::~Vector()
	{
	}

	// See method declaration for details.
	Vector::Vector(const Vector& original)
		: x(original.GetX()), y(original.GetY())
	{
	}

	// See method declaration for details.
	const float& Vector::GetX() const
	{
		return x;
	}

	// See method declaration for details.
	const float& Vector::GetY() const
	{
		return y;
	}

	// See method declaration for details.
	void Vector::SetX(const float& new_x)
	{
		x = new_x;
	}

	// See method declaration for details.
	void Vector::SetY(const float& new_y)
	{
		y = new_y;
	}

	// See method declaration for details.
	Vector Vector::operator+(const Vector& rhs) const
	{
		return Vector(x + rhs.GetX(), y + rhs.GetY());
	}

	// See method declaration for details.
	Vector Vector::operator-(const Vector& rhs) const
	{
		return Vector(x - rhs.GetX(), y - rhs.GetY());
	}

	// See method declaration for details.
	Vector& Vector::operator+=(const Vector& rhs)
	{
		x += rhs.GetX();
		y += rhs.GetY();
		return *this;
	}

	// See method declaration for details.
	Vector& Vector::operator-=(const Vector& rhs)
	{
		x -= rhs.GetX();
		y -= rhs.GetY();
		return *this;
	}

	// See method declaration for details.
	const bool Vector::operator==(const Vector& rhs) const
	{
		return (x == rhs.GetX() && y == rhs.GetY());
	}

	// See method declaration for details.
	const bool Vector::operator!=(const Vector& rhs) const
	{
		return (x != rhs.GetX() || y != rhs.GetY());
	}

	// See method declaration for details.
	const Vector& Vector::operator=(const Vector& rhs)
	{
		x = rhs.GetX();
		y = rhs.GetY();
		return *this;
	}

}
}