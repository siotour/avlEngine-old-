/**********
 * Author: Sheldon Bachstein
 * Date: Sep 25, 2011
 * Description: Unit test for the text box component. See text box.h for details.
 * Expected output: 
 **********/

#include"text box.h"
#include"..\..\..\input\src\direct input input device\direct input input device.h"
#include"..\..\..\input\src\input event\input event.h"
#include"..\..\..\input\src\keyboard event\keyboard event.h"
#include"..\..\..\input\src\mouse move event\mouse move event.h"
#include"..\..\..\input\src\mouse button event\mouse button event.h"
#include"..\..\..\input\src\mouse scroll event\mouse scroll event.h"
#include"..\..\..\view\src\basic renderer\basic renderer.h"
#include"..\..\..\view\src\basic window\basic window.h"
#include"..\..\..\view\src\d3d renderer base\d3d renderer base.h"
#include"..\..\..\view\src\d3d display profile\d3d display profile.h"
#include"..\vertex 2d\vertex 2d.h"
#include"..\sprite\sprite.h"
#include"..\assert\assert.h"
#include"..\image\image.h"
#include<fstream>
#include<list>
#include<utility>

void TestTextBoxComponent(HINSTANCE instance)
{
	using avl::view::BasicWindow;
	using avl::view::BasicRenderer;
	using avl::utility::Vertex2D;
	using avl::utility::Sprite;
	using avl::utility::TextBox;
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
		Image font("Font.tga");
		Image background_image("background.tga");

		// Make sure that the images were successfully loaded.
		if(font.GetHeight() == 0 || background_image.GetHeight() == 0)
		{
			throw false;
		}

		// Add the newly-created texture to the renderer.
		unsigned int font_handle = renderer.AddTexture(font);
		unsigned int background_handle = renderer.AddTexture(background_image);

		// Done with the texture; safe to delete now.


		// Create a few sprites.
		Sprite background(-1.0f, 1.0f, 1.0f, -1.0f, 0.5f, background_handle);
		TextBox text(-0.8f, 0.8f, 0.8f, 0.4f, 0.1f, 16, 1, TextBox::AUTO_ADJUST_SIZE, font_handle, 256);


		text.SetText("\"A Wizard's greatest fear is losing his mind.\"\n\t-A Great Philosopher, 1877 BC.");

		



		// Put quads into the appropriate container.
		Sprite::SpriteList sprites;
		sprites.insert(sprites.begin(), &background);
		sprites.splice(sprites.begin(), text.GetSprites());

		

		// While the user keeps the window open...
		while(window.Update() == true)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw sprites.
				renderer.RenderSprites(sprites);
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