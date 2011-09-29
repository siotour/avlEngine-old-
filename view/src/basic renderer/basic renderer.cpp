/**********
 * Author: Sheldon Bachstein
 * Date: Jan 13, 2011
 * Description: See basic renderer.h for details.
 **********/

#include"basic renderer.h"
#include"..\d3d renderer base\d3d renderer base.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\image\image.h"
// Makes d3d9 activate additional debug information and checking.
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>
#include<cstring>
#include<map>
#include<list>
#include<queue>
#include<algorithm>




namespace avl
{
namespace view
{
	// Anonymous namespace.
	namespace
	{
	// This function is used to compare two Sprites. Returns true if sprite a
	// is less than or equal to sprite b.
	// Sorting criteria, from most important to least:
	//		1) Opaque > transparent
	//		2) If Opaque, z-depth from nearest to farthest.
	//		3) If Transparent, z-depth from farthest to nearest.
	//		4) TextureID, from lowest to highest.
	// The lowest bit in a utility::Sprite::TextureHandle is set if the texture
	// is transparent and not set if it's opaque.
	bool SortSprites(const utility::Sprite* const a, const utility::Sprite* const b)
	{
		ASSERT(a != NULL && b != NULL);

		// Is a opaque?
		if((a->GetTextureHandle() & 0x01) == false)
		{
			// Is b translucent? If so, return true.
			if((b->GetTextureHandle() & 0x01) == true)
			{
				return true;
			}
			// If both are opaque, does one have a lesser z-depth? If so, return that one.
			else if(a->GetZ() != b->GetZ())
			{
				return (a->GetZ() < b->GetZ()) ? true : false;
			}
			// If the z-depths are the same, return the one with a lesser texture ID.
			else
			{
				return (a->GetTextureHandle() < b->GetTextureHandle()) ? true : false;
			}
		}
		// Is a translucent?
		else
		{
			// Is b opaque? If so, return false.
			if((b->GetTextureHandle() & 0x01) == false)
			{
				return false;
			}
			// If both are translucent, does one have a greater z-depth? If so, return that one.
			else if(a->GetZ() != b->GetZ())
			{
				return (a->GetZ() > b->GetZ()) ? true : false;
			}
			// If the z-depths are the same, return the one with a lesser texture ID.
			else
			{
				return (a->GetTextureHandle() < b->GetTextureHandle()) ? true : false;
			}
		}
	}
	}



	// Passes the arguments on to D3DRendererBase and then does some initialization.
	BasicRenderer::BasicRenderer(HWND window_handle, const D3DDisplayProfile& profile)
	: D3DRendererBase(window_handle, profile), vertex_format(D3DFVF_XYZ | D3DFVF_TEX1), bytes_per_pixel(4), next_texture_handle(0),
	  buffer_length(1000), vertex_buffer(NULL), index_buffer(NULL)
	{
		// Create the vertex and index buffers.
		AcquireUnmanagedAssets();

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




	// Releases all of the textures in the textures map and then clears the map. Releases
	// the vertex and index buffers.
	BasicRenderer::~BasicRenderer()
	{
		// Go through and release each of the textures in the texture map.
		TexHandleToTex::iterator end = textures.end();
		for(TexHandleToTex::iterator i = textures.begin(); i != end; ++i)
		{
			i->second->Release();
		}

		// Delete all of the textures from the map.
		textures.clear();


		// Release all unmanaged assets.
		ReleaseUnmanagedAssets();
	}




	// Attempts to create a texture with the specified width and height, in a 32-bit format (RGBA), with
	// the specified data. pixel_data must be in 32-bit, RGBA form. It will be copied and not deleted. The
	// return will be a handle for the texture which can be used to draw with it and delete it.
	const unsigned int BasicRenderer::AddTexture(const utility::Image& image)
	{
		ASSERT(image.GetPixelData() != NULL);
		ASSERT(image.GetWidth() > 0);
		ASSERT(image.GetHeight() > 0);
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);

		// This function currently only supports 32-bit textures. Make sure that this image has a 4-byte
		// pixel depth. TODO: Make it so that any sort of image can be loaded.
		ASSERT(image.GetPixelDepth() == 4);
		
		// Create a new texture.
		IDirect3DTexture9* texture = CreateTexture(image.GetWidth(), image.GetHeight(), D3DFMT_A8R8G8B8);

		// Attempt to load the user's data into the newly-created texture.
		CopyPixelDataToTexture(*texture, image.GetPixelData(), image.GetWidth(), image.GetHeight(), image.GetPixelDepth());

		// If the image is semi-transparent, set the last bit of its texture handle.
		unsigned long texture_handle = next_texture_handle;
		if(IsImagePartiallyTransparent(image.GetWidth() * image.GetHeight(), image.GetPixelData()) == true)
		{
			texture_handle += 1;
		}

		// Add the newly-created texture to the map of textures with the key value of next_texture_handle.
		std::pair<TexHandleToTex::iterator, bool> result2 =
			textures.insert(TexHandleToTex::value_type(texture_handle, texture));

		ASSERT(result2.second == true);

		// Bump next_texture_handle up to the next possible handle, leaving the last bit unset.
		next_texture_handle += 2;

		// Return the handle used for this texture.
		return texture_handle;
	}




	// Releases the texture associated with the specified handle and deletes it from the texture map. Don't 
	// try to draw with textures that have been deleted. Don't attempt to delete the same texture
	// multiple times.
	void BasicRenderer::DeleteTexture(const unsigned int& texture_handle)
	{
		// Find the texture within the texture map.
		TexHandleToTex::iterator i = textures.find(texture_handle);
		ASSERT(i != textures.end());

		// If the specified texture handle doesn't exist, throw utility::Exception describing the problem.
		if(i == textures.end())
		{
			throw utility::Exception("avl::view::BasicRenderer::DeleteTexture() -- The specified texture handle does not "
									 "correspond to an existing texture.");
		}

		// Release the texture.
		i->second->Release();

		// Delete the texture from the map.
		textures.erase(i);
	}




	// Renders a series of sprites...
	void BasicRenderer::RenderSprites(utility::Sprite::SpriteList sprites)
	{
		ASSERT(D3DRendererBase::d3d != false);
		ASSERT(D3DRendererBase::device != false);


		// If the device is not ready for rendering, return.
		if(Reset() == false)
		{
			return;
		}

		// If there are no sprites to render, simply clear the viewport, present the scene and return.
		if(sprites.size() < 1)
		{
			ClearViewport();
			device->Present(NULL, NULL, NULL, NULL);
			return;
		}



		// Used throughout this function.
		const utility::Sprite::SpriteList::iterator end = sprites.end();

		// Count the number of visible sprites to be drawn. Additionally, make sure that all
		// of the pointers in sprites are not NULL. If a pointer is NULL, throw a
		// utility::Exception describing the problem.
		unsigned int number_of_sprites = 0;
		for(utility::Sprite::SpriteList::iterator i = sprites.begin(); i != end; ++i)
		{
			// Make sure this pointer is not NULL.
			if((*i) == NULL)
			{
				throw utility::Exception("avl::view::BasicRenderer::RenderSprites() -- The supplied SpriteList contains one or more NULL pointers.");
			}
			// Is this sprite visible?
			else if((*i)->IsVisible() == true)
			{
				// Count this sprite.
				++number_of_sprites;
			}
		}



		// Sort sprites for rendering purposes.
		sprites.sort(SortSprites);



		// The vertex information to be written to the vertex buffer. Each vertex has an x, y, and z
		// coordinate, and u and v texture coordinates; each sprite has 4 vertices.
		float* vertex_data = new float[number_of_sprites * 5 * 4];
		const unsigned int vertex_data_size = sizeof(float) * number_of_sprites * 5 * 4;
		// The index information to be written to the index buffer. Each sprite consists of two
		// triangles, or 6 indices.
		UINT16* index_data = new UINT16[number_of_sprites * 6];
		const unsigned int index_data_size = sizeof(UINT16) * number_of_sprites * 6;




		// Now that the sprites are sorted, we can process them. The following loop does:
		//	1) Stores the index data for each sprite; to be written to the index buffer.
		//	2) Stores the vertex data for each sprite; to be written to the vertex buffer.
		//	3) Stores the order of texture IDs for opaque sprites, and the number of sprites
		//	   using them.
		//	4) Stores the order of texture IDs for transparent sprites, and the number of
		//	   sprites using them.
		
		// Temp variables.
		unsigned int current_vertex = 0;
		unsigned int current_sprite = 0;
		unsigned int sprites_using_texture_handle = 0;
		utility::Sprite::TextureHandle previous_texture_handle = (*(sprites.begin()))->GetTextureHandle();

		// These queues contain pairs corresonding to (texture id, number of sprites using it).
		// There's one for opaque sprites, and one for transparent.
		typedef std::queue<std::pair<const utility::Sprite::TextureHandle, const unsigned int>> TextureHandleUseQueue;
		TextureHandleUseQueue opaque_texture_ids;
		TextureHandleUseQueue transparent_texture_ids;
		
		for(utility::Sprite::SpriteList::iterator i = sprites.begin(); i != end; ++i)
		{
			// Only process visible sprites.
			if((*i)->IsVisible() == true)
			{
				// 1)
				// Store the index data for the current sprite.
				// First triangle.
				index_data[current_sprite * 6] = current_vertex;
				index_data[current_sprite * 6 + 1] = current_vertex + 1;
				index_data[current_sprite * 6 + 2] = current_vertex + 2;
				// Second triangle.
				index_data[current_sprite * 6 + 3] = current_vertex + 2;
				index_data[current_sprite * 6 + 4] = current_vertex + 3;
				index_data[current_sprite * 6 + 5] = current_vertex;
				++current_sprite;



				// 2)
				// Store the vertices for the current sprite.
				// P1
				vertex_data[current_vertex * 5] = (*i)->GetP1().GetX();
				vertex_data[current_vertex * 5 + 1] = (*i)->GetP1().GetY();
				vertex_data[current_vertex * 5 + 2] = (*i)->GetZ();
				vertex_data[current_vertex * 5 + 3] = (*i)->GetQ1().GetX();
				vertex_data[current_vertex * 5 + 4] = (*i)->GetQ1().GetY();
				// P2
				++current_vertex;
				vertex_data[current_vertex * 5] = (*i)->GetP2().GetX();
				vertex_data[current_vertex * 5 + 1] = (*i)->GetP2().GetY();
				vertex_data[current_vertex * 5 + 2] = (*i)->GetZ();
				vertex_data[current_vertex * 5 + 3] = (*i)->GetQ2().GetX();
				vertex_data[current_vertex * 5 + 4] = (*i)->GetQ2().GetY();
				// P3
				++current_vertex;
				vertex_data[current_vertex * 5] = (*i)->GetP3().GetX();
				vertex_data[current_vertex * 5 + 1] = (*i)->GetP3().GetY();
				vertex_data[current_vertex * 5 + 2] = (*i)->GetZ();
				vertex_data[current_vertex * 5 + 3] = (*i)->GetQ3().GetX();
				vertex_data[current_vertex * 5 + 4] = (*i)->GetQ3().GetY();
				// P4
				++current_vertex;
				vertex_data[current_vertex * 5] = (*i)->GetP4().GetX();
				vertex_data[current_vertex * 5 + 1] = (*i)->GetP4().GetY();
				vertex_data[current_vertex * 5 + 2] = (*i)->GetZ();
				vertex_data[current_vertex * 5 + 3] = (*i)->GetQ4().GetX();
				vertex_data[current_vertex * 5 + 4] = (*i)->GetQ4().GetY();
				++current_vertex;




				// If this sprite has the same texture ID as the last one, just increment the
				// accumulator.
				if((*i)->GetTextureHandle() == previous_texture_handle)
				{
					++sprites_using_texture_handle;
				}
				// 3)
				// Stores the order of texture IDs for opaque sprites, and the number of sprites
				// using them.
				else if((previous_texture_handle & 0x01) == false)
				{
					opaque_texture_ids.push(std::make_pair(previous_texture_handle, sprites_using_texture_handle));
					sprites_using_texture_handle = 1;
					previous_texture_handle = (*i)->GetTextureHandle();
				}
				// 4)
				// Stores the order of texture IDs for transparent sprites, and the number of
				// sprites using them.
				else
				{
					transparent_texture_ids.push(std::make_pair(previous_texture_handle, sprites_using_texture_handle));
					sprites_using_texture_handle = 1;
					previous_texture_handle = (*i)->GetTextureHandle();
				}



				// The last sprite in the list of sprites is a special case. Normally this loop would
				// skip writing its texture ID to the proper queue, so I do it manually here.
				if(std::distance(i, end) == 1)
				{
					// 3)
					// Stores the order of texture IDs for opaque sprites, and the number of sprites
					// using them.
					if(((*i)->GetTextureHandle() & 0x01) == false)
					{
						opaque_texture_ids.push(std::make_pair((*i)->GetTextureHandle(), sprites_using_texture_handle));
					}
					// 4)
					// Stores the order of texture IDs for transparent sprites, and the number of
					// sprites using them.
					else
					{
						transparent_texture_ids.push(std::make_pair((*i)->GetTextureHandle(), sprites_using_texture_handle));
					}
				}
			}
		}



		// Now write the vertex data to the vertex buffer.
		FillVertexBuffer(*vertex_buffer, reinterpret_cast<const unsigned char* const>(vertex_data), vertex_data_size);
		delete[] vertex_data;


		// Now write the index data to the index buffer.
		FillIndexBuffer(*index_buffer, reinterpret_cast<const unsigned char* const>(index_data), index_data_size);
		delete[] index_data;
	

		



		// Clear the previous scene.
		ClearViewport();
		
		// Begin the scene.
		D3DRendererBase::device->BeginScene();

		// Set the vertex buffer stream and index buffer stream.
		device->SetStreamSource(0, vertex_buffer, 0, sizeof(float) * 5);
		device->SetIndices(index_buffer);


		// Set the flexible vertex format to position and texture coordinates only.
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		// Set the device to use a fixed function vertex shader.
		device->SetVertexShader(NULL);


		// Keeps track of how many vertices and indices have been drawn.
		unsigned int base_vertex = 0;
		unsigned int base_index = 0;


		// Turn z-buffering on and alpha blending off.
		device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		// Render all opaque sprites.
		while(opaque_texture_ids.empty() == false)
		{
			// Set the correct texture for this set of sprites.
			device->SetTexture(0, textures[opaque_texture_ids.front().first]);

			// Two triangles per sprite.
			const unsigned int number_of_triangles = opaque_texture_ids.front().second * 2;
			// Four vertices per sprite.
			const unsigned int number_of_vertices = number_of_triangles * 2;

			// Draw the triangles.
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, base_vertex, number_of_vertices, base_index, number_of_triangles);

			// Update the base vertex and base index.
			base_vertex += number_of_vertices;
			base_index += number_of_triangles * 3;

			// Pop the front element off of the queue.
			opaque_texture_ids.pop();
		}




		// Turn z-buffering off and alpha blending on.
		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		// Render all transparent sprites.
		while(transparent_texture_ids.empty() == false)
		{
			// Set the correct texture for this set of sprites.
			device->SetTexture(0, textures[transparent_texture_ids.front().first]);

			// Two triangles per sprite.
			const unsigned int number_of_triangles = transparent_texture_ids.front().second * 2;
			// Four vertices per sprite.
			const unsigned int number_of_vertices = number_of_triangles * 2;

			// Draw the triangles.
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, base_vertex, number_of_vertices, base_index, number_of_triangles);

			// Update the base vertex and base index.
			base_vertex += number_of_vertices;
			base_index += number_of_triangles * 3;

			// Pop the front element off of the queue.
			transparent_texture_ids.pop();
		}

		

		// End the scene.
		D3DRendererBase::device->EndScene();

		// Present the scene.
		device->Present(NULL, NULL, NULL, NULL);
	}




	// Iterates through the pixel data for an image with an alpha channel, checking for semi-transparency.
	// If semi-transparency is found, true is returned. Otherwise false. The image data is assumed to be
	// 32-bits per pixel in RGBA format. size is the number of pixels in the image, and pixel_data is a
	// pointer to the pixel data.
	bool BasicRenderer::IsImagePartiallyTransparent(const unsigned int size, const BYTE* const pixel_data)
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
			vertex_buffer = CreateVertexBuffer(buffer_length);
		}
		if(index_buffer == NULL)
		{
			index_buffer = CreateIndexBuffer(buffer_length);
		}

		ASSERT(vertex_buffer != NULL);
		ASSERT(index_buffer != NULL);
	}




}
}