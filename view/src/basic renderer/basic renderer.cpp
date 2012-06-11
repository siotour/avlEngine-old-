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
Implementation for the basic renderer component. See "basic renderer.h" for details.
@author Sheldon Bachstein
@date Jan 13, 2011
*/

#include"basic renderer.h"
#include"..\d3d wrapper\d3d wrapper.h"
#include"..\d3d display profile\d3d display profile.h"
#include"..\d3d error\d3d error.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\image\image.h"
#include<new>
// Makes d3d9 activate additional debug information and checking.
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>




namespace avl
{
namespace view
{
	// Forward declarations. See the function definitions for details.
	namespace
	{
		bool IsImagePartiallyTransparent(const unsigned int size, const unsigned char* const pixel_data);
	}

	// See method declaration for details.
	BasicRenderer::BasicRenderer(HWND window_handle, const d3d::D3DDisplayProfile& profile)
		: display_profile(profile), vertex_format(D3DFVF_XYZ | D3DFVF_TEX1), bytes_per_pixel(4), next_texture_handle(2),
		buffer_length(1000), vertex_buffer(NULL), index_buffer(NULL)
	{
		// Create the Direct3D object.
		d3d = d3d::GetDirect3DObject();
		ASSERT(d3d != NULL);
		// Create a device.
		device = d3d::CreateDevice(*d3d, window_handle, display_profile, present_parameters);
		ASSERT(device != NULL);
		// Create the viewport for the device.
		d3d::CreateViewport(*device, display_profile.GetWidth(), display_profile.GetHeight());
		// Now attempt to ready the device for rendering.
		CheckDeviceState();
		if(is_device_ready == true)
		{
			AcquireUnmanagedAssets();
		}


		// Turn lighting off.
		device->SetRenderState(D3DRS_LIGHTING, false);
		// Set the flexible vertex format to position and texture coordinates only.
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		// Set the device to use a fixed function vertex shader.
		device->SetVertexShader(NULL);

		// Turn alpha testing on.
		device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		// Set the alpha test reference to zero. TODO: Does this HAVE to be 0001 etc.? 0.0 makes more sense.
		device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		// Set the alpha test comparison function to GREATER so that pixels with 0 alpha aren't drawn.
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		// Set the source blending to the source's alpha.
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		// Set the destination blending to the destination's alpha.
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// Set the alpha blending operation to addition.
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}




	// See method declaration for details.
	BasicRenderer::~BasicRenderer()
	{
		// Go through and release each of the textures in the texture map.
		d3d::TexHandleToTex::iterator end = textures.end();
		for(d3d::TexHandleToTex::iterator i = textures.begin(); i != end; ++i)
		{
			i->second->Release();
		}
		// Delete all of the textures from the map.
		textures.clear();
		// Release all unmanaged assets.
		ReleaseUnmanagedAssets();
	}




	// See method declaration for details.
	const utility::Sprite::TextureHandle BasicRenderer::AddTexture(const utility::Image& image)
	{
		ASSERT(image.GetPixelData() != NULL);
		ASSERT(image.GetWidth() > 0);
		ASSERT(image.GetHeight() > 0);
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);
		// This function currently only supports 32-bit textures. Make sure that this image has a 4-byte
		// pixel depth.
		ASSERT(image.GetPixelDepth() == 4);	
		// Create a new texture.
		IDirect3DTexture9* texture = d3d::CreateTexture(*device, image.GetWidth(), image.GetHeight(), D3DFMT_A8R8G8B8);
		// Attempt to load the user's data into the newly-created texture.
		d3d::CopyPixelDataToTexture(*texture, image.GetPixelData(), image.GetWidth(), image.GetHeight(), image.GetPixelDepth());
		// If the image is semi-transparent, set the last bit of its texture handle.
		unsigned long texture_handle = next_texture_handle;
		if(IsImagePartiallyTransparent(image.GetWidth() * image.GetHeight(), image.GetPixelData()) == true)
		{
			texture_handle += 1;
		}
		// Add the newly-created texture to the map of textures with the key value of next_texture_handle.
		std::pair<d3d::TexHandleToTex::iterator, bool> result2 = textures.insert(d3d::TexHandleToTex::value_type(texture_handle, texture));
		ASSERT(result2.second == true);
		// Bump next_texture_handle up to the next possible handle, leaving the last bit unset.
		next_texture_handle += 2;
		// Return the handle used for this texture.
		return texture_handle;
	}




	// See method declaration for details.
	void BasicRenderer::DeleteTexture(const utility::Sprite::TextureHandle& texture_handle)
	{
		// Find the texture within the texture map.
		d3d::TexHandleToTex::iterator i = textures.find(texture_handle);
		ASSERT(i != textures.end());
		// If the specified texture handle doesn't exist, then simply ignore the deletion request.
		if(i == textures.end())
		{
			return;
		}
		// Release the texture.
		i->second->Release();
		// Delete the texture from the map.
		textures.erase(i);
	}




	// See method declaration for details.
	void BasicRenderer::RenderSprites(utility::SpriteList& sprites)
	{
		ASSERT(d3d != false);
		ASSERT(device != false);
		// If the device is not ready for rendering, return.
		if(CheckDeviceState() == true)
		{
			// Clear the screen to black.
			d3d::ClearViewport(*device);
			// Render sprites.
			d3d::RenderSprites(*device, sprites, *vertex_buffer, *index_buffer, textures);
			// Present the scene.
			device->Present(NULL, NULL, NULL, NULL);
		}
	}

	
	
	
	
	// Releases the index buffer and vertex buffer. This is called when the device is lost.
	void BasicRenderer::ReleaseUnmanagedAssets()
	{
		if(vertex_buffer != NULL)
		{
			vertex_buffer->Release();
			vertex_buffer = NULL;
		}
		if(index_buffer != NULL)
		{
			index_buffer->Release();
			index_buffer = NULL;
		}
	}




	// Creates the index buffer and vertex buffer. This is called upon initialization and when the
	// device is reset after having been lost.
	void BasicRenderer::AcquireUnmanagedAssets()
	{
		ASSERT(vertex_buffer == NULL);
		ASSERT(index_buffer == NULL);
		
		// Only create the vertex and index buffers if they were previously released.
		if(vertex_buffer == NULL)
		{
			vertex_buffer = d3d::CreateVertexBuffer(*device, buffer_length);
		}
		if(index_buffer == NULL)
		{
			index_buffer = d3d::CreateIndexBuffer(*device, buffer_length);
		}
		ASSERT(vertex_buffer != NULL);
		ASSERT(index_buffer != NULL);
	}


	// See function declaration for details.
	const bool BasicRenderer::CheckDeviceState()
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);
		// Check the device state.
		HRESULT result = device->TestCooperativeLevel();
		// If the device is ready, set is_device_ready to true and return true.
		if(result == D3D_OK)
		{
			is_device_ready = true;
		}
		// If the device was just lost, release assets and set is_device_ready to false.
		else if(result == D3DERR_DEVICELOST)
		{
			// If the device was just lost, release all unmanaged assets and set is_device_ready
			// to false.
			if(is_device_ready == true)
			{
				ReleaseUnmanagedAssets();
			}
			return false;
		}
		// If the device is ready to be reset, attempt to reset it.
		else if(result == D3DERR_DEVICENOTRESET)
		{
			// If the device was just lost, release all unmanaged assets and set is_device_ready
			// to false.
			if(is_device_ready == true)
			{
				ReleaseUnmanagedAssets();
				is_device_ready = false;
			}
			is_device_ready = d3d::ResetDevice(*device, present_parameters);
		}
		// Otherwise there was some sort of error (internal error?). Throw a D3DError with the error code
		// and a description of the problem.
		else
		{
			throw d3d::D3DError("IDirect3D9::TestCooperativeLevel()", "avl::view::D3DRendererBase::Reset() -- An internal error occurred while checking the device's state.", result);
		}
		// Return the device's state.
		return is_device_ready;
	}







	// Anonymous namespace.
	namespace
	{
		/** Checks to see whether or not an image contains semi-transparent pixels. Semi-transparent pixels
		have an alpha value that is neither the minimum nor the maximum.
		@pre The pixels are in 32-bit RGBA format.
		@param size The number of pixels in \a pixel_data.
		@param pixel_data The pixel data for the image.
		@returns True if \a pixel_data contains semi-transparent pixels, and false if not.
		*/
		bool IsImagePartiallyTransparent(const unsigned int size, const unsigned char* const pixel_data)
		{
			// For each pixel, iterate through it and check the pixel data. If any pixel has an alpha value
			// between 0 and 255 (exclusive), return true. Otherwise return false. Since the pixel data is
			// in little-endian order, the alpha component will be the first component.
			for(unsigned int i = 0; i < size; ++i)
			{
				if(pixel_data[i * 4 + 3] != 0x00 && pixel_data[i * 4 + 3] != 0xFF)
				{
					return true;
				}
			}
			return false;
		}


	}



}
}