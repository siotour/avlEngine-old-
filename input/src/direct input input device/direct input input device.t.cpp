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
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\key codes\key codes.h"
#include"..\..\..\utility\src\input events\input events.h"
#include"..\..\..\view\src\basic d3d renderer\basic d3d renderer.h"
#include"..\..\..\view\src\basic win32 window\basic win32 window.h"
#include"..\..\..\view\src\image\image.h"
#include"..\..\..\utility\src\vector\vector.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\timer\timer.h"
#include<list>
#include<utility>


void TestDirectInputInputDeviceComponent(HINSTANCE instance)
{
	using namespace avl::utility;
	using namespace avl::view;
	using namespace avl::input;

	
	//try
	{	
		// Create a window and renderer.
		d3d::D3DDisplayProfile profile = d3d::LeastSquaredDisplayProfile(640, 480, false);
		BasicWin32Window window(instance, "Direct Input Input Device Unit Test", profile.GetWidth(), profile.GetHeight());
		BasicD3DRenderer renderer(window.GetWindowHandle(), profile, Vector(1.0f, 1.0f));
		DirectInputInputDevice input(window.GetWindowHandle());



		// Load the textures from the disk.
		Image explosion_image("Assets\\explosion.tga");

		// Make sure that the images were successfully loaded.
		if(explosion_image.GetHeight() == 0)
		{
			throw FileNotFoundException("Assets\\explosion.tga");
		}

		// Add the newly-created textures to the renderer.
		unsigned int explosion_handle = renderer.AddTexture(explosion_image);
		

		// Done with the texture; safe to delete now.


		// Create a few textured quads.
		Sprite explosion(Quad(-0.2f, 0.2f, 0.2f, -0.2f), 0.5f, explosion_handle, 4, 4);

		Sprite::Animation animation1;
		animation1.animation_id = 1;
		animation1.frame_delay = 0.04f;
		animation1.number_of_frames = 16;
		animation1.start_frame = 1;

		explosion.AddAnimation(animation1);
		explosion.PlayAnimation(1);
		explosion.SetRepeat(true);

		// Put quads into the appropriate container.
		GraphicList graphics;
		graphics.push_back(&explosion);



		Vector movement;
		const float speed = 0.007f;
		
		bool exit = false;

		// While the user keeps the window open...
		Timer timer;
		while(window.Update() == true && exit == false)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw quad.
				renderer.RenderGraphics(graphics);

				// Get input.
				input_events::InputQueue queue = input.GetInput();

				// Transform sprites.
				explosion.Move(movement);
				explosion.Update((float)timer.Reset());

				while(queue.empty() == false)
				{
					const input_events::InputEvent& input = *queue.front().get();

					if(input.GetType() == input_events::KeyboardEvent::KEYBOARD_TYPE)
					{
						const input_events::KeyboardEvent& keyboard = *dynamic_cast<const input_events::KeyboardEvent*>(&input);
						
						if(keyboard.GetKey() == key_codes::KeyboardKey::kk_escape)
						{
							exit = true;
						}
						else if(keyboard.GetKey() == key_codes::KeyboardKey::kk_up)
						{
							if(keyboard.IsPressed() == true)
							{
								movement.SetY(speed);
							}
							else
							{
								movement.SetY(0.0f);
							}
						}
						else if(keyboard.GetKey() == key_codes::KeyboardKey::kk_down)
						{
							if(keyboard.IsPressed() == true)
							{
								movement.SetY(-speed);
							}
							else
							{
								movement.SetY(0.0f);
							}
						}
						else if(keyboard.GetKey() == key_codes::KeyboardKey::kk_right)
						{
							if(keyboard.IsPressed() == true)
							{
								movement.SetX(speed);
							}
							else
							{
								movement.SetX(0.0f);
							}
						}
						else if(keyboard.GetKey() == key_codes::KeyboardKey::kk_left)
						{
							if(keyboard.IsPressed() == true)
							{
								movement.SetX(-speed);
							}
							else
							{
								movement.SetX(0.0f);
							}
						}
					}

					queue.pop();
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