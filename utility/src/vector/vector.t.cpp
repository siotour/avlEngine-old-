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
Unit test for the vector component. See "vector.h" for details.
@author Sheldon Bachstein
@date January 17, 2011
*/

#include"vector.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<iostream>


// Anonymous namespace.
namespace
{
	// Output operator.
	std::ostream& operator<<(std::ostream& output, const avl::utility::Vector& vertex)
	{
		output << "(" << vertex.GetX() << ", " << vertex.GetY() << ")";
		return output;
	}
}

void TestVectorComponent()
{
	using avl::utility::Vector;
	using std::cout;

	// Test constructors.
	Vector v1;
	ASSERT(v1.GetX() == 0.0f && v1.GetY() == 0.0f);
	Vector v2(1.0f, 1.0f);
	ASSERT(v2.GetX() == 1.0f && v2.GetY() == 1.0f);
	Vector v3(v2);
	ASSERT(v3.GetX() == 1.0f && v3.GetY() == 1.0f);

	// Test mutators.
	v1.SetX(5.0f);
	v1.SetY(10.5f);
	ASSERT(v1.GetX() == 5.0f && v1.GetY() == 10.5f);

	// Test arithmetic operators and assignment.
	v1 = v2 + v3;
	ASSERT(v1.GetX() == 2.0f && v1.GetY() == 2.0f);
	v1 = v2 - v3;
	ASSERT(v1.GetX() == 0.0f && v1.GetY() == 0.0f);
	v1 += v2;
	ASSERT(v1.GetX() == 1.0f && v1.GetY() == 1.0f);
	v1 -= v3;
	ASSERT(v1.GetX() == 0.0f && v1.GetY() == 0.0f);

	// Test comparison operators.
	ASSERT(v2 == v3);
	ASSERT(v1 != v2);
}