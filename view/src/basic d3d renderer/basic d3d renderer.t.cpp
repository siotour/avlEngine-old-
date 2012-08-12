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
Unit test for the basic d3d renderer component. See "basic d3d renderer.h" for details.
@author Sheldon Bachstein
@date January 13, 2011
*/

#include"basic d3d renderer.h"
#include"..\basic win32 window\basic win32 window.h"
#include"..\win32 error\win32 error.h"
#include"..\d3d wrapper\d3d wrapper.h"
#include"..\image\image.h"
#include"..\..\..\utility\src\vector\vector.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<fstream>
#include<list>
#include<utility>


void TestBasicD3DRendererComponent(HINSTANCE instance)
{
	using namespace avl::utility;
	using namespace avl::view;
	
	try
	{	
		// Create a window and renderer.
		const d3d::D3DDisplayProfile profile = d3d::LeastSquaredDisplayProfile(500, 500, false);
		BasicWin32Window window(instance, "BasicD3DRenderer Unit Test", profile.GetWidth(), profile.GetHeight());
		BasicD3DRenderer renderer(window.GetWindowHandle(), profile, Vector(1.0f, 1.0f));



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
		Sprite background(Quad(-1.0f, 1.0f, 1.0f, -1.0f), 0.5f, background_id);
		Sprite spiral(Quad(-0.15f, -0.6f, 0.15f, -0.9f), 0.4f, spiral_id);



		// Put quads into the appropriate container.
		GraphicList graphics;
		graphics.push_back(&spiral);
		graphics.push_back(&background);



		Vector movement(0.003f, 0.003f);
		float rotation = 0.5f;
		float scale = 1.002f;
		

		// While the user keeps the window open...
		while(window.Update() == true)
		{
			// If the window has focus, render the quad.
			if(window.IsActive() == true)
			{
				// Draw quad.
				renderer.RenderGraphics(graphics);

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
	catch(avl::view::d3d::D3DError& d3d)
	{
		throw d3d;
	}
	catch(avl::utility::AssertVerifyFailure& assertion)
	{
		throw assertion;
	}
	catch(avl::view::win32::Win32Error& window)
	{
		throw window;
	}
	catch(avl::utility::Exception& avl)
	{
		throw avl;
	}
	
}