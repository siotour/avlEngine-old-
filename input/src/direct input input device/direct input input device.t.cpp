/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: Unit test for the direct input input device component. See direct input input device.h for details.
 * Expected output: 
 **********/

#include"direct input input device.h"
#include"..\input event\input event.h"
#include"..\keyboard event\keyboard event.h"
#include"..\mouse move event\mouse move event.h"
#include"..\mouse button event\mouse button event.h"
#include"..\mouse scroll event\mouse scroll event.h"
#include"..\..\..\view\src\basic renderer\basic renderer.h"
#include"..\..\..\view\src\basic window\basic window.h"
#include"..\..\..\view\src\d3d renderer base\d3d renderer base.h"
#include"..\..\..\view\src\d3d display profile\d3d display profile.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\image\image.h"
#include<fstream>
#include<list>
#include<utility>


void TestDirectInputInputDeviceComponent(HINSTANCE instance)
{
	using avl::view::BasicWindow;
	using avl::view::BasicRenderer;
	using avl::utility::Vertex2D;
	using avl::utility::Sprite;
	using avl::utility::Image;
	using avl::input::DirectInputInputDevice;

	
	try
	{	
		// Create a window and renderer.
		const std::vector<avl::view::D3DDisplayProfile> profiles = avl::view::EnumerateDisplayProfiles();
		BasicWindow window(instance, "BasicRenderer Unit Test", profiles[42].GetWidth(), profiles[42].GetHeight());
		BasicRenderer renderer(window.GetWindowHandle(), profiles[42]);
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
		Sprite::SpriteList sprites;
		sprites.insert(sprites.begin(), &spiral);
		sprites.insert(sprites.begin(), &background);



		Vertex2D movement;
		const float speed = 0.007f;
		

		// While the user keeps the window open...
		while(window.Update() == true)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw quad.
				renderer.RenderSprites(sprites);

				// Get input.
				DirectInputInputDevice::EventQueue queue = input.PollInput();

				// Transform sprites.
				spiral.Move(movement);

				while(queue.empty() == false)
				{
					const avl::input::InputEvent* const input = queue.front();
					queue.pop();

					if(input->GetType() == avl::input::KeyboardEvent::KEYBOARD_TYPE)
					{
						const avl::input::KeyboardEvent* const keyboard = dynamic_cast<const avl::input::KeyboardEvent* const>(input);
						if(keyboard->GetKey() == avl::input::key_codes::KeyboardKey::kk_up)
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
	catch(avl::view::D3DError& d3d)
	{
		throw d3d;
	}
	catch(avl::utility::AssertionFailure& assertion)
	{
		throw assertion;
	}
	catch(avl::utility::VerificationFailure& verification)
	{
		throw verification;
	}
	catch(avl::view::WindowCreationFailure& window)
	{
		throw window;
	}
	catch(avl::utility::Exception& avl)
	{
		throw avl;
	}
	
}