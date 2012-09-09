#pragma once
#ifndef AVL_VIEW_BASIC_RENDERER__
#define AVL_VIEW_BASIC_RENDERER__
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
Defines the \ref avl::view::BasicD3DRenderer class.
@author Sheldon Bachstein
@date Jan 13, 2011
@todo Add the capability to render to any sized window. Simply choose the smallest
profile which will render to the entire window and set the viewport appropriately.
*/

#include"..\renderer\renderer.h"
#include"..\d3d wrapper\d3d wrapper.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\vector\vector.h"
#include<queue>
// Makes d3d9 activate additional debug information and checking.
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>

namespace avl
{
namespace view
{
	

	// Forward declarations.
	class D3DDisplayProfile;
	class Image;

	/**
	Implements the \ref avl::view::Renderer interface using Direct3D.
	@todo Make the vertex/index buffers resize themselves on demand if necessary.
	@todo Add the capability to render lines, filled quads, and filled circles.
	*/
	class BasicD3DRenderer: public Renderer
	{
	public:
		/** Attempts to create a renderer to render to the window represented by
		\a window_handle using the display profile \a profile.
		@param window_handle The handle to the window to render to.
		@param profile The display profile to initialize the device with.
		@param screen_space The adjusted screen resolution for the renderer. 
		The center of the screen will be at (0, 0). The x component will specify
		the distance from the center to either side, and the y component will
		specify the distance from the center to the top or bottom.
		@throws RendererException If unable to create the D3D object.
		@throws d3d::D3DError If unable to create the Direct3D device or if
		unable to set the viewport.
		*/
		BasicD3DRenderer(HWND window_handle, const d3d::D3DDisplayProfile& profile, const avl::utility::Vector& screen_space);
		/** Destroys the Direct3D device and releases all assets.
		*/
		~BasicD3DRenderer();

		
		/** Attempts to create a texture for \a image.
		@param image The image data used to create the texture.
		@return A handle to the created texture.
		@throws D3DError If unable to create the texture.
		@todo This function currently only supports 32-bit textures.
		*/
		const utility::TexturedQuad::TextureHandle AddTexture(const Image& image);

		/** Releases the texture associated with the texture handle \a texture_handle.
		If \a texture_handle is not associated with a texture, then nothing happens.
		@warning Don't try to render sprites using a deleted texture handle.
		@param texture_handle The handle to the texture to be deleted.
		*/
		void DeleteTexture(const utility::TexturedQuad::TextureHandle& texture_handle);

		/** Deletes all textures and renders the handles associated with them invalid.
		@post All previously issued texture handles will be rendered invalid, but they
		may become associated with new textures in the future.
		*/
		void ClearTextures();

		/** Renders \a graphics to the screen.
		@param graphics The graphics to be rendered.
		@throws RendererException If one of the objects in \a primitives contains a texture
		handle which isn't associated with a texture or if an error makes it impossible to
		perform the rendering.
		*/
		void RenderGraphics(const utility::GraphicList& graphics);

	private:

		/** Releases our index and two vertex buffers. This is called when the device is
		lost.
		*/
		void ReleaseUnmanagedAssets();

		/** Creates an index buffer and two vertex buffers. This is called upon
		initialization and when the device is reset after having been lost.
		@throws d3d::D3DError If unable to create either the vertex or index buffers.
		*/
		void AcquireUnmanagedAssets();

		/** Sets all of the necessary render states for the device.
		*/
		void SetDeviceStates();
		
		/** Releases all Direct3D interfaces.
		*/
		void Release();

		/** Checks to see if the device is ready to render, and attempts to reset the device if
		the device has been lost.
		@return True if the device is ready to render to, and false if not.
		@throws d3d::D3DError If an error occurs with the device or if the unmanaged assets are
		unable to be re-acquired.
		*/
		const bool CheckDeviceState();

		/// Indicates whether or not the device is ready for rendering.
		bool is_device_ready;

		/// Maintains a map of valid texture handles and their associated textures.
		d3d::TexHandleToTexContext textures;
		/// Used to assign a unique texture handle to each created texture. This imposes the restriction that it
		/// is highly inadvisable to attempt to load more than UINT_MAX textures at a time.
		unsigned int next_texture_handle;
		/// Keeps track of texture handles which have been freed so that they may be reused.
		std::queue<utility::TexturedQuad::TextureHandle> reusable_texture_handles;

		/// Buffer for textured vertices.
		IDirect3DVertexBuffer9* textured_vertex_buffer;
		/// Buffer for colored vertices.
		IDirect3DVertexBuffer9* colored_vertex_buffer;
		/// The index buffer.
		IDirect3DIndexBuffer9* index_buffer;
		/// The size of the vertex buffer and index buffer, in terms of the objects being stored in them.
		const unsigned int buffer_length;

		/// Contains information on the device details.
		const d3d::D3DDisplayProfile display_profile;

		/// Handle to the window used for rendering.
		HWND window_handle;
		/// The d3d object.
		IDirect3D9* d3d;
		/// The d3d device.
		IDirect3DDevice9* device;

		/// The present paramters for the device.
		D3DPRESENT_PARAMETERS present_parameters;

		/// Used to store the type of vertex format used for sprites.
		const unsigned int vertex_format;
		/// Used to store the number of bytes per pixel.
		const unsigned int bytes_per_pixel;


		/// NOT IMPLEMENTED.
		BasicD3DRenderer(const BasicD3DRenderer&);
		/// NOT IMPLEMENTED.
		const BasicD3DRenderer& operator=(const BasicD3DRenderer&);
	};



} //avl
} //view
#endif // AVL_VIEW_BASIC_RENDERER__