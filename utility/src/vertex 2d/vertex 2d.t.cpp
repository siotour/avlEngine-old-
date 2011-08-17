/**********
 * Author: Sheldon Bachstein
 * Date: Jan 17, 2011
 * Description: Unit test for the textured vertex component. See vertex 2d.h for details.
 * Expected output: 
 **********/

#include"vertex 2d.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<iostream>


// Anonymous namespace.
namespace
{
	// Output operator.
	std::ostream& operator<<(std::ostream& output, const avl::utility::Vertex2D& vertex)
	{
		output << "(" << vertex.GetX() << ", " << vertex.GetY() << ")";
		return output;
	}
}

void TestVertex2DComponent()
{
	using avl::utility::Vertex2D;
	using std::cout;

	// Test constructors.
	Vertex2D v1;
	ASSERT(v1.GetX() == 0.0f && v1.GetY() == 0.0f);
	Vertex2D v2(1.0f, 1.0f);
	ASSERT(v2.GetX() == 1.0f && v2.GetY() == 1.0f);
	Vertex2D v3(v2);
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