/**********
 * Author: Sheldon Bachstein
 * Date: Jan 13, 2011
 * Description: See basic renderer.h for details.
 **********/

#include"basic renderer.h"
#include"..\d3d renderer base\d3d renderer base.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\..\..\utility\src\textured vertex\textured vertex.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>
#include<cstring>
#include<map>
#include<queue>




namespace avl
{
namespace view
{



	// Passes the arguments on to D3DRendererBase and then does some initialization.
	BasicRenderer::BasicRenderer(HWND window_handle, const D3DDisplayProfile& profile)
	: D3DRendererBase(window_handle, profile), vertex_format(D3DFVF_XYZ | D3DFVF_TEX1), bytes_per_pixel(4), next_texture_handle(1),
	  buffer_length(1000), vertex_buffer(NULL), index_buffer(NULL)
	{
		// Create the vertex and index buffers.
		AcquireUnmanagedAssets();

		// Turn lighting off.
		device->SetRenderState(D3DRS_LIGHTING, false);

		// Turn alpha testing on.
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		// Set the alpha test reference to zero.
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		// Set the alpha test comparison function to GREATER so that pixels with 0 alpha aren't drawn.
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		// Turn alpha blending on.
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		// Set the source blending to the source's alpha.
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		// Set the destination blending to the destination's alpha.
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}




	// Releases all of the textures in the textures map and then clears the map. Releases
	// the vertex and index buffers.
	BasicRenderer::~BasicRenderer()
	{
		// Go through and release each of the textures in the texture map.
		TexIDToTex::iterator end = textures.end();
		for(TexIDToTex::iterator i = textures.begin(); i != end; ++i)
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
	const unsigned int BasicRenderer::AddTexture(const unsigned int& width, const unsigned int& height, const BYTE* const pixel_data)
	{
		ASSERT(pixel_data != NULL);
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);

		// Create a new texture.
		IDirect3DTexture9* texture = CreateTexture(width, height, D3DFMT_A8R8G8B8);

		// Attempt to load the user's data into the newly-created texture.
		CopyPixelDataToTexture(*texture, pixel_data, width, height, bytes_per_pixel);

		// Add the newly-created texture to the map of textures with the key value of next_texture_handle.
		std::pair<TexIDToTex::iterator, bool> result2 =
			textures.insert(TexIDToTex::value_type(next_texture_handle, texture));

		ASSERT(result2.second == true);

		// Return the handle used for this texture, and then increment next_texture_handle so that the same
		// handle isn't used for multiple textures.
		return next_texture_handle++;
	}




	// Releases the texture associated with the specified handle and deletes it from the texture map. Don't 
	// try to draw with textures that have been deleted. Don't attempt to delete the same texture
	// multiple times.
	void BasicRenderer::DeleteTexture(const unsigned int& texture_handle)
	{
		// Find the texture within the texture map.
		TexIDToTex::iterator i = textures.find(texture_handle);
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




	// Renders a series of textured quads...
	void BasicRenderer::RenderTexturedQuads(TexIDToVectorOfTexQuads& textured_quad_map)
	{
		ASSERT(D3DRendererBase::d3d != false);
		ASSERT(D3DRendererBase::device != false);

		// If the device is not ready for rendering, return.
		if(Reset() == false)
		{
			return;
		}

		// If the input map is empty, simply present the scene and return.
		if(textured_quad_map.size() < 1)
		{
			device->Present(NULL, NULL, NULL, NULL);
			return;
		}

	

		// The number of quads to be rendered for each individual texture handle.
		typedef std::pair<const unsigned int, const unsigned int> QuadsPerTexID;

		// Stores how many quads there are for each texture handle.
		std::queue<QuadsPerTexID> quads_per_tex_id;

		// Stores all of the vertices so that they can later be written to the vertex buffer.
		std::queue<const utility::TexturedVertex* const> processed_vertices;

		// Stores all of the indices so that they can later be written to the index buffer. 16-bit
		// ints are used.
		std::queue<const unsigned short> processed_indices;



		// For each set of quads using a common texture handle...
		unsigned int current_vertex = 0;
		TexIDToVectorOfTexQuads::iterator end = textured_quad_map.end();
		for(TexIDToVectorOfTexQuads::iterator i = textured_quad_map.begin(); i != end; ++i)
		{
			ASSERT(i->second.size() >= 1);
			// Record how many quads are to be rendered for this particular texture handle.
			quads_per_tex_id.push(QuadsPerTexID(i->first, i->second.size()));

			// For each quad using this texture...
			VectorOfTexQuads::iterator end2 = i->second.end();
			for(VectorOfTexQuads::iterator j = i->second.begin(); j != end2; ++j)
			{
				// Copy each vertice from the current quad into the queue of processed
				// vertices.
				processed_vertices.push(&(*j)->GetP1());
				processed_vertices.push(&(*j)->GetP2());
				processed_vertices.push(&(*j)->GetP3());
				processed_vertices.push(&(*j)->GetP4());

				// Copy the corresponding set of indices for the vertices composing
				// this quad into the queue of processed indices. Each quad consists
				// of two triangles which will be rendered individually.
				// Triangle one:
				processed_indices.push(current_vertex);
				processed_indices.push(current_vertex + 1);
				processed_indices.push(current_vertex + 2);

				// Triangle two:
				processed_indices.push(current_vertex + 2);
				processed_indices.push(current_vertex + 3);
				processed_indices.push(current_vertex);

				// Update the vertice count.
				current_vertex += 4;
			}
		}

	
		// Attempt to write the processed vertices to the vertex buffer.
		FillVertexBuffer(*vertex_buffer, processed_vertices);

		// Attempt to write the processed indices to the index buffer.
		FillIndexBuffer(*index_buffer, processed_indices);



		// Clear the previous scene.
		ClearViewport();
		
		// Begin the scene.
		D3DRendererBase::device->BeginScene();

		// Set the vertex buffer stream and index_buffer stream.
		device->SetStreamSource(0, vertex_buffer, 0, sizeof(utility::TexturedVertex));
		device->SetIndices(index_buffer);


		// Set the flexible vertex format to position and texture coordinates only.
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		// Set the device to use a fixed function vertex shader.
		device->SetVertexShader(NULL);



		// Keeps track of how many vertices and indices have been drawn.
		unsigned int base_vertex = 0;
		unsigned int base_index = 0;

		// For each set of quads sharing a texture handle...
		do
		{
			// Get the texture handle and number of associated quads to be rendered with that texture.
			QuadsPerTexID quads_using_current_tex = quads_per_tex_id.front();
			quads_per_tex_id.pop();

			// Set the texture for this set of quads to be the current texture.
			device->SetTexture(0, textures[quads_using_current_tex.first]);
		
			// Two triangles per quad.
			const unsigned int number_of_triangles = quads_using_current_tex.second * 2;
			// Four vertices per quad.
			const unsigned int number_of_vertices = number_of_triangles * 2;

			// Draw the triangles.
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, base_vertex, number_of_vertices, base_index, number_of_triangles);

			// Update the base vertex and base index.
			base_vertex += number_of_vertices;
			base_index += number_of_triangles * 3;

		} while(quads_per_tex_id.empty() == false);


		// End the scene.
		D3DRendererBase::device->EndScene();

		// Present the scene.
		device->Present(NULL, NULL, NULL, NULL);
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