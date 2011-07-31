/**********
 * Author: Sheldon Bachstein
 * Date: Jan 17, 2011
 * Description: Unit test for the textured vertex component. See vertex 2d.h for details.
 * Expected output: 
 **********/

#include"vertex 2d.h"
#include<iostream>


void TestVertex2D()
{
	// Create a vertex component.
	avl::utility::Vertex2D v1(1.0f, 1.0f);

	std::cout<< v1.GetX() << v1.GetY() << "\n";

	v1.SetX(2.0f);
	v1.SetY(2.0f);

	std::cout<< v1.GetX() << v1.GetY() << "\n";

	avl::utility::Vertex2D v2;

	std::cout<< (v1 == v2);
	v2 = v1;
	std::cout<< (v1 == v2);
	std::cout<<"\n\n";
	system("pause");

}