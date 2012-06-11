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
Defines the \ref avl::view::BasicRenderer class.
@author Sheldon Bachstein
@date Jan 13, 2011
@todo There's a memory leak while using this class. Enable maximum debugging in the DirectX9
control panel for details.
*/

#include"..\renderer\renderer.h"
#include"..\d3d display profile\d3d display profile.h"
#include"..\d3d wrapper\d3d wrapper.h"
#include"..\..\..\utility\src\sprite\sprite.h"
// Makes d3d9 activate additional debug information and checking.
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>

namespace avl
{
// Forward declaration.
namespace utility{class Image;}

namespace view
{
	

	// Forward declaration.
	class D3DDisplayProfile;

	/**
	Implements the \ref avl::view::Renderer interface using Direct3D.
	*/
	class BasicRenderer: public Renderer
	{
	public:
		/** Attempts to create a renderer to render to the window represented by
		\a window_handle using the display profile \a profile.
		@param window_handle The handle to the window to render to.
		@param profile The display profile to initialize the device with.
		@throws RendererException If unable to create the D3D object.
		@throws d3d::D3DError If unable to create the Direct3D device or if
		unable to set the viewport.
		*/
		BasicRenderer(HWND window_handle, const d3d::D3DDisplayProfile& profile);
		/** Destroys the Direct3D device and releases all assets.
		*/
		~BasicRenderer();

		
		/** Attempts to create a texture for \a image.
		@param image The image data used to create the texture.
		@return A handle to the created texture.
		@throws D3DError If unable to create the texture.
		@todo This function currently only supports 32-bit textures.
		*/
		const utility::Sprite::TextureHandle AddTexture(const utility::Image& image);

		/** Releases the texture associated with the texture handle \a texture_handle.
		If \a texture_handle is not associated with a texture, then nothing happens.
		@warning Don't try to render sprites using a deleted texture handle.
		@param texture_handle The handle to the texture to be deleted.
		*/
		void DeleteTexture(const utility::Sprite::TextureHandle& texture_handle);

		/** Renders a list of sprites.
		@warning \a sprites may very well be modified during this operation (though
		no actual sprites will be modified). If you need a copy of \a sprites after
		the call to this method, then make that copy before this method call.
		@post \a sprites may well not contain the same (or any) sprites. Make sure to
		keep your own separate copy if you need \a sprites for something else.
		@param sprites The sprites to be rendered.
		@throws RendererException If \a sprites contains one or more NULL pointers.
		@throws D3DError If there is an error while using the device.
		@todo Consider using a list of std::tr1::shared_ptrs.
		*/
		void RenderSprites(utility::SpriteList& sprites);

	private:

		/** Releases \ref index_buffer and \ref vertex_buffer. This is called when the device is lost.
		*/
		void ReleaseUnmanagedAssets();

		/** Creates \ref index_buffer and \ref vertex_buffer. This is called upon initialization and
		when the device is reset after having been lost.
		@throws d3d::D3DError If unable to create either the vertex or index buffers.
		*/
		void AcquireUnmanagedAssets();

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
		d3d::TexHandleToTex textures;
		/// Used to assign a unique texture handle to each created texture. This imposes the restriction that it
		/// is highly inadvisable to attempt to load more than UINT_MAX/2-1 (defined in limits.h) textures throughout
		/// the duration of a program.
		unsigned int next_texture_handle;

		/// The vertex buffer.
		IDirect3DVertexBuffer9* vertex_buffer;
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
		BasicRenderer(const BasicRenderer&);
		/// NOT IMPLEMENTED.
		const BasicRenderer& operator=(const BasicRenderer&);
	};



} //avl
} //view
#endif // AVL_VIEW_BASIC_RENDERER__