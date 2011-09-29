/**********
 * Author: Sheldon Bachstein
 * Date: Jan 13, 2011
 * Description: Unit test for the basic renderer component. See basic renderer.h for details.
 * Expected output: 
 **********/

#include"basic renderer.h"
#include"..\basic window\basic window.h"
#include"..\d3d renderer base\d3d renderer base.h"
#include"..\d3d display profile\d3d display profile.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\image\image.h"
#include<fstream>
#include<list>
#include<utility>


void TestBasicRendererComponent(HINSTANCE instance)
{
	using avl::view::BasicWindow;
	using avl::view::BasicRenderer;
	using avl::utility::Vertex2D;
	using avl::utility::Sprite;
	using avl::utility::Image;
	
	try
	{	
		// Create a window and renderer.
		const std::vector<avl::view::D3DDisplayProfile> profiles = avl::view::EnumerateDisplayProfiles();
		BasicWindow window(instance, "BasicRenderer Unit Test", profiles[42].GetWidth(), profiles[42].GetHeight());
		BasicRenderer renderer(window.GetWindowHandle(), profiles[42]);



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
		Sprite background(-1.0f, 1.0f, 1.0f, -1.0f, 0.5f, background_id);
		Sprite spiral(-0.15f, -0.6f, 0.15f, -0.9f, 0.4f, spiral_id);



		// Put quads into the appropriate container.
		Sprite::SpriteList sprites;
		sprites.insert(sprites.begin(), &spiral);
		sprites.insert(sprites.begin(), &background);



		Vertex2D movement(0.003f, 0.003f);
		float rotation = 0.5f;
		float scale = 1.002f;
		

		// While the user keeps the window open...
		while(window.Update() == true)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw quad.
				renderer.RenderSprites(sprites);

				// Transform sprites.
				spiral.Move(movement);
				spiral.Rotate(rotation);
				spiral.Scale(scale);

				if(spiral.GetCenter().GetX() < -0.8f || spiral.GetCenter().GetX() > 0.8f)
				{
					movement.SetX(-1.0f * movement.GetX());
					rotation *= -1.0f;
					scale -= 0.004f;
				}
				if(spiral.GetCenter().GetY() < -0.8f || spiral.GetCenter().GetY() > 0.8f)
				{
					movement.SetY(-1.0f * movement.GetY());
					rotation *= -1.0f;
					scale += 0.004f;
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