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
Implementation for the basic d3d renderer component. See "basic d3d renderer.h" for details.
@author Sheldon Bachstein
@date Jan 13, 2011
*/

#include"basic d3d renderer.h"
#include"..\d3d wrapper\d3d wrapper.h"
#include"..\image\image.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\vector\vector.h"
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

	// See method declaration for details.
	BasicD3DRenderer::BasicD3DRenderer(HWND window_handle, const d3d::D3DDisplayProfile& profile, const avl::utility::Vector& screen_space)
		: Renderer(screen_space), display_profile(profile), vertex_format(D3DFVF_XYZ | D3DFVF_TEX1), bytes_per_pixel(4), next_texture_handle(1),
		buffer_length(1000), d3d(nullptr), device(nullptr), textured_vertex_buffer(nullptr), colored_vertex_buffer(nullptr), index_buffer(nullptr), is_device_ready(false)
	{
		try
		{
			// Create the Direct3D object.
			d3d = d3d::GetDirect3DObject();
			ASSERT(d3d != nullptr);
			// Create a device.
			device = d3d::CreateDevice(*d3d, window_handle, display_profile, present_parameters);
			ASSERT(device != nullptr);
			// Create the viewport for the device.
			d3d::CreateViewport(*device, display_profile.GetWidth(), display_profile.GetHeight());
			// Set the scaling for the device to normalize the vertice x and y coordinates.
			d3d::SetScreenScaling(*device, 1.0f / screen_space_resolution.GetX(), 1.0f / screen_space_resolution.GetY());
			// Now attempt to ready the device for rendering.
			CheckDeviceState();		
		}
		catch(...)
		{
			// Clean up our mess.
			Release();
			throw;
		}
	}


	// See method declaration for details.
	BasicD3DRenderer::~BasicD3DRenderer()
	{
		Release();
	}


	// See method declaration for details.
	const utility::TexturedQuad::TextureHandle BasicD3DRenderer::AddTexture(const view::Image& image)
	{
		ASSERT(image.GetPixelData() != nullptr);
		ASSERT(image.GetWidth() > 0);
		ASSERT(image.GetHeight() > 0);
		ASSERT(d3d != nullptr);
		ASSERT(device != nullptr);
		// This function currently only supports 32-bit textures. Make sure that this image has a 4-byte
		// pixel depth.
		ASSERT(image.GetPixelDepth() == 4);
		// Load the user's pixel data into a new texture.
		IDirect3DTexture9* texture = d3d::CreateTexture(*device, image.GetWidth(), image.GetHeight(), D3DFMT_A8R8G8B8);
		d3d::CopyPixelDataToTexture(*texture, image.GetPixelData(), image.GetWidth(), image.GetHeight(), image.GetPixelDepth());

		// Is there a texture handle that we can reuse?
		utility::TexturedQuad::TextureHandle texture_handle;
		if(reusable_texture_handles.empty() == false)
		{
			texture_handle = reusable_texture_handles.front();
			reusable_texture_handles.pop();
		}
		else
		{
			texture_handle = next_texture_handle;
			++next_texture_handle;
		}
		// Map the new texture handle to this texture.
		try
		{
			d3d::TextureContext new_texture(*texture, image.IsTranslucent());
			auto result2 = textures.insert(d3d::TexHandleToTexContext::value_type(texture_handle, new_texture));
			ASSERT(result2.second == true);
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
		// Return the handle used for this texture.
		return texture_handle;
	}


	// See method declaration for details.
	void BasicD3DRenderer::DeleteTexture(const utility::TexturedQuad::TextureHandle& texture_handle)
	{
		// Find the texture within the texture map.
		d3d::TexHandleToTexContext::iterator i = textures.find(texture_handle);
		ASSERT(i != textures.end());
		// If the specified texture handle doesn't exist, then simply ignore the deletion request.
		if(i == textures.end())
		{
			return;
		}
		// Release the texture.
		i->second.texture.Release();
		// Delete the texture from the map.
		textures.erase(i);
		// Save the handle to be reused.
		try
		{
			reusable_texture_handles.push(texture_handle);
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}


	// See method declaration for details.
	void BasicD3DRenderer::ClearTextures()
	{
		// Go through and release each of the textures in the texture map.
		d3d::TexHandleToTexContext::iterator end = textures.end();
		for(d3d::TexHandleToTexContext::iterator i = textures.begin(); i != end; ++i)
		{
			i->second.texture.Release();
		}
		// Delete all of the textures from the map.
		textures.clear();
		// Reset the texture handles.
		while(reusable_texture_handles.empty() == false)
		{
			reusable_texture_handles.pop();
		}
		next_texture_handle = 1;
	}


	// See method declaration for details.
	void BasicD3DRenderer::RenderGraphics(const utility::GraphicList& graphics)
	{
		ASSERT(d3d != false);
		ASSERT(device != false);
		// If the device is not ready for rendering, return.
		if(CheckDeviceState() == true)
		{
			ASSERT(textured_vertex_buffer != nullptr);
			ASSERT(colored_vertex_buffer != nullptr);
			ASSERT(index_buffer != nullptr);
			// Clear the screen to black.
			d3d::ClearViewport(*device);
			// Render sprites.
			d3d::RenderContext render_context(*device, *index_buffer, *textured_vertex_buffer, *colored_vertex_buffer);
			device->BeginScene();
			d3d::GraphicBatch batch(graphics, textures);
			batch.Render(render_context);
			device->EndScene();
			// Present the scene.
			device->Present(nullptr, nullptr, nullptr, nullptr);
		}
	}


	// Releases the index buffer and vertex buffer. This is called when the device is lost.
	void BasicD3DRenderer::ReleaseUnmanagedAssets()
	{
		if(textured_vertex_buffer != nullptr)
		{
			textured_vertex_buffer->Release();
			textured_vertex_buffer = nullptr;
		}
		if(colored_vertex_buffer != nullptr)
		{
			colored_vertex_buffer->Release();
			colored_vertex_buffer = nullptr;
		}
		if(index_buffer != nullptr)
		{
			index_buffer->Release();
			index_buffer = nullptr;
		}
	}


	// Creates the index buffer and vertex buffer. This is called upon initialization and when the
	// device is reset after having been lost.
	void BasicD3DRenderer::AcquireUnmanagedAssets()
	{
		ASSERT(textured_vertex_buffer == nullptr);
		ASSERT(colored_vertex_buffer == nullptr);
		ASSERT(index_buffer == nullptr);
		
		// Only create the vertex and index buffers if they were previously released.
		if(textured_vertex_buffer == nullptr)
		{
			textured_vertex_buffer = d3d::CreateVertexBuffer(*device, buffer_length);
		}
		if(colored_vertex_buffer == nullptr)
		{
			colored_vertex_buffer = d3d::CreateVertexBuffer(*device, buffer_length);
		}
		if(index_buffer == nullptr)
		{
			index_buffer = d3d::CreateIndexBuffer(*device, buffer_length);
		}
		ASSERT(textured_vertex_buffer != nullptr);
		ASSERT(colored_vertex_buffer != nullptr);
		ASSERT(index_buffer != nullptr);
	}


	// See method declaration for details.
	void BasicD3DRenderer::SetDeviceStates()
	{
		// Turn lighting off.
		device->SetRenderState(D3DRS_LIGHTING, false);
		// Set the flexible vertex format to position and texture coordinates only.
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		// Set the device to use a fixed function vertex shader.
		device->SetVertexShader(nullptr);
		// Turn alpha testing on.
		device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		// Set the alpha test reference to zero.
		device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000000);
		// Set the alpha test comparison function to GREATER so that pixels with 0 alpha aren't drawn.
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		// Set the source blending to the source's alpha.
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		// Set the destination blending to the destination's alpha.
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// Set the alpha blending operation to addition.
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}


	// See method declaration for details.
	void BasicD3DRenderer::Release()
	{
		// Release all textures.
		ClearTextures();
		// Release all unmanaged assets.
		ReleaseUnmanagedAssets();
		// Release all Direct3D interfaces.
		if(device != nullptr)
		{
			device->Release();
			device = nullptr;
		}
		if(d3d != nullptr)
		{
			d3d->Release();
			d3d = nullptr;
		}
	}


	// See method declaration for details.
	const bool BasicD3DRenderer::CheckDeviceState()
	{
		ASSERT(d3d != nullptr);
		ASSERT(device != nullptr);
		// Check the device state.
		HRESULT result = device->TestCooperativeLevel();
		// If the device is okay, ensure that the renderer's state is good.
		if(result == D3D_OK)
		{
			if(is_device_ready == false)
			{
				SetDeviceStates();
				AcquireUnmanagedAssets();
			}
			ASSERT(textured_vertex_buffer != nullptr);
			ASSERT(colored_vertex_buffer != nullptr);
			ASSERT(index_buffer != nullptr);
			is_device_ready = true;
		}
		// If the device was just lost, release assets and set is_device_ready to false.
		else if(result == D3DERR_DEVICELOST)
		{
			if(is_device_ready == true)
			{
				ReleaseUnmanagedAssets();
				is_device_ready = false;
			}
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
			// If we're able to reset the device, then we need to reacquire unmanaged assets.
			if(d3d::ResetDevice(*device, present_parameters) == true)
			{
				SetDeviceStates();
				AcquireUnmanagedAssets();
				is_device_ready = true;
			}
		}
		// Otherwise there was some sort of error (internal error?).
		else
		{
			throw d3d::D3DError("IDirect3D9::TestCooperativeLevel()", "avl::view::D3DRendererBase::Reset() -- An internal error occurred while checking the device's state.", result);
		}
		// Return the device's state.
		return is_device_ready;
	}



}
}