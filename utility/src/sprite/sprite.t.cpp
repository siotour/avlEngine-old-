/**********
 * Author: Sheldon Bachstein
 * Date: Jan 21, 2011
 * Description: Unit test for the textured quad component. See textured quad.h for details.
 * Expected output: 
 **********/

#include"textured quad.h"
#include"..\textured vertex\textured vertex.h"
#include<iostream>

void operator<<(std::ostream& io, const avl::utility::TexturedQuad& q)
{
	avl::utility::TexturedVertex v = q.GetP1();
	io<< "<" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ", " << v.GetU() << ", " << v.GetV() << ">\n";

	v = q.GetP2();
	io<< "<" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ", " << v.GetU() << ", " << v.GetV() << ">\n";

	v = q.GetP3();
	io<< "<" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ", " << v.GetU() << ", " << v.GetV() << ">\n";

	v = q.GetP4();
	io<< "<" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ", " << v.GetU() << ", " << v.GetV() << ">\n";
}

void TestTexturedQuadComponent()
{
	avl::utility::TexturedQuad quad1;

	avl::utility::TexturedVertex p1(10.0f, 10.0f, 0.0f, 0.0f, 0.0f);
	avl::utility::TexturedVertex p2(12.0f, 10.0f, 0.0f, 1.0f, 0.0f);
	avl::utility::TexturedVertex p3(12.0f, 8.0f, 0.0f, 0.0f, 1.0f);
	avl::utility::TexturedVertex p4(10.0f, 8.0f, 0.0f, 1.0f, 1.0f);

	quad1 = avl::utility::TexturedQuad(p1, p2, p3, p4);

	std::cout<< "Original Quad:\n" <<  quad1;

	quad1.Rotate(90.0f);

	std::cout<< "\nSecondary Quad:\n" << quad1;


	system("pause");
}