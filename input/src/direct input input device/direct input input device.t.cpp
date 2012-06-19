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
Unit test for the direct input input device component. See "d3d error.h" for details.
@author Sheldon Bachstein
@date September 08, 2011
*/

#include"direct input input device.h"
#include"..\input event\input event.h"
#include"..\keyboard event\keyboard event.h"
#include"..\mouse move event\mouse move event.h"
#include"..\mouse button event\mouse button event.h"
#include"..\mouse scroll event\mouse scroll event.h"
#include"..\..\..\view\src\basic d3d renderer\basic d3d renderer.h"
#include"..\..\..\view\src\basic win32 window\basic win32 window.h"
#include"..\..\..\view\src\win32 error\win32 error.h"
#include"..\..\..\view\src\basic d3d renderer\basic d3d renderer.h"
#include"..\..\..\view\src\d3d display profile\d3d display profile.h"
#include"..\..\..\view\src\d3d error\d3d error.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\image\image.h"
#include<fstream>
#include<list>
#include<utility>


void TestDirectInputInputDeviceComponent(HINSTANCE instance)
{
	using avl::view::BasicWin32Window;
	using avl::view::BasicD3DRenderer;
	using avl::utility::Vertex2D;
	using avl::utility::Sprite;
	using avl::utility::Image;
	using avl::input::DirectInputInputDevice;
	using avl::input::InputQueue;

	
	//try
	{	
		// Create a window and renderer.
		avl::view::d3d::D3DDisplayProfile profile = avl::view::d3d::LeastSquaredDisplayProfile(640, 480, true);
		BasicWin32Window window(instance, "BasicD3DRenderer Unit Test", profile.GetWidth(), profile.GetHeight());
		BasicD3DRenderer renderer(window.GetWindowHandle(), profile);
		DirectInputInputDevice input(window.GetWindowHandle());



		// Load the textures from the disk.
		Image spiral_image("spiral.tga");
		Image background_image("background.tga");

		// Make sure that the images were successfully loaded.
		if(spiral_image.GetHeight() == 0 || background_image.GetHeight() == 0)
		{
			throw false;
		}

		// Add the newly-created texture to the renderer.
		unsigned int spiral_id = renderer.AddTexture(spiral_image);
		unsigned int background_id = renderer.AddTexture(background_image);

		// Done with the texture; safe to delete now.


		// Create a few sprites.
		Sprite background(-1, 1, 1, -1, 0.5, background_id);
		Sprite spiral(-0.15f, -0.6f, 0.15f, -0.9f, 0.4f, spiral_id);



		// Put quads into the appropriate container.
		avl::utility::SpriteList sprites;
		sprites.insert(sprites.begin(), &spiral);
		sprites.insert(sprites.begin(), &background);



		Vertex2D movement;
		const float speed = 0.007f;
		
		bool exit = false;

		// While the user keeps the window open...
		while(window.Update() == true && exit == false)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw quad.
				renderer.RenderSprites(sprites);

				// Get input.
				InputQueue queue = input.GetInput();

				// Transform sprites.
				spiral.Move(movement);

				while(queue.empty() == false)
				{
					const avl::input::InputEvent* const input = queue.front();
					queue.pop();

					if(input->GetType() == avl::input::KeyboardEvent::KEYBOARD_TYPE)
					{
						const avl::input::KeyboardEvent* const keyboard = dynamic_cast<const avl::input::KeyboardEvent* const>(input);
						
						if(keyboard->GetKey() == avl::input::key_codes::KeyboardKey::kk_escape)
						{
							exit = true;
						}
						else if(keyboard->GetKey() == avl::input::key_codes::KeyboardKey::kk_up)
						{
							if(keyboard->IsPressed() == true)
							{
								movement.SetY(speed);
							}
							else
							{
								movement.SetY(0.0f);
							}
						}
						else if(keyboard->GetKey() == avl::input::key_codes::KeyboardKey::kk_down)
						{
							if(keyboard->IsPressed() == true)
							{
								movement.SetY(-speed);
							}
							else
							{
								movement.SetY(0.0f);
							}
						}
						else if(keyboard->GetKey() == avl::input::key_codes::KeyboardKey::kk_right)
						{
							if(keyboard->IsPressed() == true)
							{
								movement.SetX(speed);
							}
							else
							{
								movement.SetX(0.0f);
							}
						}
						else if(keyboard->GetKey() == avl::input::key_codes::KeyboardKey::kk_left)
						{
							if(keyboard->IsPressed() == true)
							{
								movement.SetX(-speed);
							}
							else
							{
								movement.SetX(0.0f);
							}
						}
					}

					delete input;
				}

			}
		}
	}
	//catch(avl::view::d3d::D3DError& d3d)
	//{
	//	throw d3d;
	//}
	//catch(avl::utility::AssertVerifyFailure& assertion)
	//{
	//	throw assertion;
	//}
	//catch(avl::view::win32::Win32Error& window)
	//{
	//	throw window;
	//}
	//catch(avl::utility::Exception& avl)
	//{
	//	throw avl;
	//}
	
}