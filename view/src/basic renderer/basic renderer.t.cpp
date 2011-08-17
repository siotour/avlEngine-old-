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


		// Load a texture from the disc.
		Image image("tga file.tga");

		// Make sure that the image was successfully loaded.
		if(image.GetHeight() == 0)
		{
			throw false;
		}

		// Add the newly-created texture to the renderer.
		unsigned int texture_id = renderer.AddTexture(image);

		// Done with the texture; safe to delete it now.


		// Create textured quad.
		Sprite sprite1(-0.5, 0.5, 0.5, -0.5, 0.5);




		// Put quads into the appropriate container.
		BasicRenderer::SpriteAndTextureList sprites_and_textures;
		sprites_and_textures.insert(sprites_and_textures.begin(), std::make_pair(sprite1, texture_id));

		

		// While the user keeps the window open...
		while(window.Update() == true)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw quad.
				renderer.RenderSprites(sprites_and_textures);

				// Transform quad.
				//quad1.Rotate(1.5f);
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