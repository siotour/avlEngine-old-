/**********
 * Author: Sheldon Bachstein
 * Date: Jan 21, 2011
 * Description: Unit test for the textured quad component. See sprite.h for details.
 * Expected output: 
 **********/

#include"sprite.h"
#include"..\vertex 2d\vertex 2d.h"
#include<string>
#include<iostream>


namespace
{
	using avl::utility::Sprite;
	using avl::utility::Vertex2D;
	using std::cout;
	using std::cin;
	std::ostream& operator<<(std::ostream& output, const avl::utility::Sprite& sprite)
	{
		Vertex2D p1 = sprite.GetP1();
		Vertex2D p2 = sprite.GetP2();
		Vertex2D p3 = sprite.GetP3();
		Vertex2D p4 = sprite.GetP4();

		Vertex2D up1 = sprite.GetUntransformedP1();
		Vertex2D up2 = sprite.GetUntransformedP2();
		Vertex2D up3 = sprite.GetUntransformedP3();
		Vertex2D up4 = sprite.GetUntransformedP4();

		Vertex2D q1 = sprite.GetQ1();
		Vertex2D q2 = sprite.GetQ2();
		Vertex2D q3 = sprite.GetQ3();
		Vertex2D q4 = sprite.GetQ4();
		output << "Untransformed Vertices:\t (" << up1.GetX() << ", " << up1.GetY() << "), ";
		output << "(" << up2.GetX() << ", " << up2.GetY() << "), ";
		output << "(" << up3.GetX() << ", " << up3.GetY() << "), ";
		output << "(" << up4.GetX() << ", " << up4.GetY() << ")\n";
		output << "Transformed Vertices:\t (" << p1.GetX() << ", " << p1.GetY() << "), ";
		output << "(" << p2.GetX() << ", " << p2.GetY() << "), ";
		output << "(" << p3.GetX() << ", " << p3.GetY() << "), ";
		output << "(" << p4.GetX() << ", " << p4.GetY() << ")\n";
		output << "Texture Vertices:\t (" << q1.GetX() << ", " << q1.GetY() << "), ";
		output << "(" << q2.GetX() << ", " << q2.GetY() << "), ";
		output << "(" << q3.GetX() << ", " << q3.GetY() << "), ";
		output << "(" << q4.GetX() << ", " << q4.GetY() << ")\n";
		output << "Center: (" << sprite.GetCenter().GetX() << ", " << sprite.GetCenter().GetY() << ") Rot: " << sprite.GetRotation() << " Scale: " << sprite.GetScale() << " Z: " << sprite.GetZ();

		return output;
	}




	// Displays the menu to the user.
	void DisplayMenu()
	{
		cout << "\t0) Exit\n"
				"\t1) Move\n"
				"\t2) Set Center\n"
				"\t3) Move Texture\n"
				"\t4) Set Texture\n"
				"\t5) Rotate\n"
				"\t6) Set Rotation\n"
				"\t7) Scale\n"
				"\t8) Set Scale\n"
				"\t9) Set Z\n";
	}




	// Gets an integer input value from the user.
	const unsigned int GetMenuChoice()
	{
		unsigned int input;
		
		do
		{
			cout << "Enter a number between 0 and 9:\n> ";
			cin >> input;
		} while(input < 0 || input > 9);

		return input;
	}


	const Vertex2D GetVertex()
	{
		float x, y;

		cout << "X Coordinate? > ";
		cin >> x;
		cout << "Y Coordinate? > ";
		cin >> y;

		return Vertex2D(x, y);
	}

}


void TestSpriteComponent()
{
	//// Stores the user's menu choices.
	//unsigned int menu_choice;
	//// Stores the user's float input values.
	//float user_float;

	//// Set floating point precision to a fixed 3 decimals.
	//cout.setf(std::ios::fixed, std::ios::floatfield);
	//cout.precision(3);

	//// Create the sprite.
	//Sprite sprite(0.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0);


	//// Main program loop. Gets the user's menu choice and then performs the requested action.
	//do
	//{
	//	// Clear the screen.
	//	cout << "\n\n\n";

	//	// Display the sprite.
	//	cout << sprite;

	//	cout << "\n";

	//	// Display the menu.
	//	DisplayMenu();


	//	// Get the user's menu choice.
	//	menu_choice = GetMenuChoice();


	//	// Process the user's menu choice.
	//	switch(menu_choice)
	//	{
	//	case 1:
	//		// Move.
	//		cout << "Specify the movement vector:\n";
	//		sprite.Move(GetVertex());
	//		break;
	//	case 2:
	//		// Set Center.
	//		cout << "Specify the new center:\n";
	//		sprite.SetCenter(GetVertex());
	//		break;
	//	case 3:
	//		// Move Texture.
	//		cout << "Specify the movement vector:\n";
	//		sprite.MoveTexture(GetVertex());
	//		break;
	//	case 4:
	//		// Set Texture.
	//		cout << "Specify Q1:\n";
	//		sprite.SetQ1(GetVertex());
	//		cout << "Specify Q2:\n";
	//		sprite.SetQ2(GetVertex());
	//		cout << "Specify Q3:\n";
	//		sprite.SetQ3(GetVertex());
	//		cout << "Specify Q4:\n";
	//		sprite.SetQ4(GetVertex());
	//		break;
	//	case 5:
	//		// Rotate.
	//		cout << "Specify theta:\n> ";
	//		cin >> user_float;
	//		sprite.Rotate(user_float);
	//		break;
	//	case 6:
	//		// Set Rotation.
	//		cout << "Specify theta:\n> ";
	//		cin >> user_float;
	//		sprite.SetRotation(user_float);
	//		break;
	//	case 7:
	//		// Scale.
	//		cout << "Specify scale:\n> ";
	//		cin >> user_float;
	//		sprite.Scale(user_float);
	//		break;
	//	case 8:
	//		// Set Scale.
	//		cout << "Specify scale:\n> ";
	//		cin >> user_float;
	//		sprite.SetScale(user_float);
	//		break;
	//	case 9:
	//		// Set Z.
	//		cout << "Specify Z:\n> ";
	//		cin >> user_float;
	//		sprite.SetZ(user_float);
	//		break;
	//	}

	//
	//
	//
	//
	//
	//
	//
	//
	//} while(menu_choice != 0);
	//
	//
	//
	//system("pause");
}