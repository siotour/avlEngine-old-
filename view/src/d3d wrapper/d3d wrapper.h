#pragma once
#ifndef AVL_VIEW_D3D_D3D_WRAPPER__
#define AVL_VIEW_D3D_D3D_WRAPPER__
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
Implements thin wrapper functions for performing common Direct3D tasks.
@author Sheldon Bachstein
@date Mar 24, 2012
*/

#include"..\d3d display profile\d3d display profile.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include<vector>
#include<map>
// For window handles.
#include<Windows.h>
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>


namespace avl
{
namespace view
{
namespace d3d
{
	/// Maps texture handles to texture objects.
	typedef std::map<const utility::Sprite::TextureHandle, IDirect3DTexture9*> TexHandleToTex;


	/** Attempts to find the closest fitting display profile matching the parameters.
	@post If a match is found, then it will be fullscreen if and only if \a fullscreen
	is true.
	@param width The desired width.
	@param height The desired height.
	@param fullscreen Do you want a fullscreen or windowed profile?
	@throws RendererException If unable to find an appropriate display profile.
	@throws D3DError If enumeration of a display mode fails.
	*/
	const D3DDisplayProfile LeastSquaredDisplayProfile(const int width, const int height, const bool fullscreen);

	/** This function attempts to enumerate the default display adapter, using the HAL
	device, for all legal display profiles with multisampling disabled.
	@return A vector containing all legal display profiles.
	@throws RendererException If unable to create the D3D object.
	@throws D3DError If enumeration of a display mode fails.
	@sa avl::view::D3DDisplayProfile
	*/
	DisplayProfiles EnumerateDisplayProfiles();

	/** Checks to see whether or not the HAL Direct3D device on the default adapter supports
	all of the functionality required by the D3DRendererBase. The functionality checked is:
	\li Alpha testing is supported.
	\li \c DrawIndexedPrimitive() is supported.
	\li Hardware transformations and lightning are supported.
	\li Textures with alpha channels are supported.
	\li Non-power-of-two textures are supported.
	\li Non-square textures are supported.
	@param d3d The d3d object on which to check the capabilities.
	@return True if the HAL device supports the required functionality, and false if not.
	@throws D3DError If unable to create the D3D object.
	@todo This function needs to be refactored.
	*/
	bool CheckDeviceCapabilities(IDirect3D9& d3d);

	/** Attempts to obtain a pointer to a Direct3D9 object.
	@return The obtained Direct3D9 pointer.
	@throws D3DError If unable to obtain a Direct3D9 pointer.
	*/
	IDirect3D9* const GetDirect3DObject();

	/** Attempts to create the Direct3D device. Uses a 16-bit depth/stencil buffer, and allows
	Direct3D to maintain it.
	@param d3d The Direct3D object used to create the device.
	@param window_handle The handle to the window to render to.
	@param profile Describes the resolution, fullscreen mode, and backbuffer format of the desired device.
	@param present_parameters [OUT] The present parameters used to create the device will be stored here.
	@throws D3DError If unable to create the device.
	*/
	IDirect3DDevice9* CreateDevice(IDirect3D9& d3d, HWND& window_handle, const D3DDisplayProfile& profile, D3DPRESENT_PARAMETERS& present_parameters);

	/** Attemps to reset \a device.
	@param device The device which is to be reset.
	@param present_parameters The parameters with which to reset \a device.
	@return True if successful and false if the device is lost.
	@throws d3d::D3DError If an error occurs while resetting \a device.
	*/
	const bool ResetDevice(IDirect3DDevice9& device, D3DPRESENT_PARAMETERS& present_parameters);

	
	/** Creates a basic viewport for the device. Uses Z ranging from 0.0 to 1.0.
	@param device The device on which to create the viewport.
	@param width The desired width of the viewport in pixels.
	@param height The desired height of the viewport in pixels.
	@throws D3DError If unable to set the viewport.
	*/
	void CreateViewport(IDirect3DDevice9& device, const unsigned int& width, const unsigned int& height);

	/** Attempts to clear the backbuffer to black and the z-buffer to 1.0f.
	@param device The device on which to clear the viewport.
	@throws D3DError If unable to clear the viewport.
	*/
	void ClearViewport(IDirect3DDevice9& device);

	/** Attempts to copy \a pixel_data into the destination texture, taking into account the pitch of the destiantion
	surface.
	@pre \a destination must be a lockable Direct3D surface, and \a pixel_data must point to a block of pixel data
	of size \c width*height*bytes_per_pixel bytes. width and height are the dimensions of the pixel data, and
	bytes_per_pixel specifies the amount of data per pixel (which depends on the pixel format).
	@note This function delegates to CopyPixelDataToSurface(), so thrown exceptions may describe the problem as
	originating there.
	@param destination The texture to which \a pixel_data is to be copied.
	@param pixel_data The image data to be copied to \a destination.
	@param width The width in pixels of \a pixel_data.
	@param height The height in pixels of \a pixel_data.
	@param bytes_per_pixel The number of bytes of data per pixel.
	@throws InvalidArgumentException If \a pixel_data is \c NULL.
	@throws D3DError If unable to lock or unlock the surface of \a destination.
	*/
	void CopyPixelDataToTexture(IDirect3DTexture9& destination, const unsigned char* const pixel_data,
								const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel);

	/** Attempts to copy pixel_data into the destination surface, taking into account the pitch of \a destination.
	@pre \a destination must be a lockable Direct3D surface, and \a pixel_data must point to a block of pixel data
	of size \c width*height*bytes_per_pixel bytes.
	@param destination The surface to which \a pixel_data is to be copied.
	@param pixel_data The image data to be copied to \a destination.
	@param width The width of \a pixel_data in pixels.
	@param height The height of \a pixel_data in pixels.
	@param bytes_per_pixel The number of bytes of data for each pixel in \a pixel_data.
	@throws InvalidArgumentException If \a pixel_data is \c NULL.
	@throws D3DError If unable to lock or unlock \a destination.
	*/
	void CopyPixelDataToSurface(IDirect3DSurface9& destination, const unsigned char* const pixel_data,
									const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel);
		
	/** Checks to see if the device supports textures in the specified format.
	@param d3d A Direct3D9 object on which to test the texture format.
	@param adapter_format The format of the adapter that the texture is to be used with.
	@param format The texture format to check whether or not the device supports.
	@return True if the device supports textures of the format \a format, and false if not.
	*/
	bool IsTextureFormatOk(IDirect3D9& d3d, D3DFORMAT& adapter_format, D3DFORMAT& format);
		
	/** Attempts to create a texture with the specified width, height, and format.
	Textures are created in the \c MANAGED Direct3D pool and with 1 level.
	@param device The device on which to create the texture.
	@param width The desired width of the texture.
	@param height The desired height of the texture.
	@param format The desired format of the texture.
	@note You can check to see if the device supports the creation of textures of a format by using
	\ref avl::view::D3DRendererBase::IsTextureFormatOk().
	@sa avl::view::D3DRendererBase::IsTextureFormatOk()
	@throws D3DError If the creation of the texture fails.
	*/
	IDirect3DTexture9* CreateTexture(IDirect3DDevice9& device, const unsigned int& width, const unsigned int& height, D3DFORMAT format);
		
	/** Attempts to create a vertex buffer capable of storing \a buffer_length vertices. The expected vertex format
	consists of spatial coordinates (XYZ) and one set of texture coordinates. The created buffer is dynamic and
	write-only and in the default pool.
	@param device The device in which to create the buffer.
	@param buffer_length The number of vertices which the buffer should hold.
	@return The newly created vertex buffer.
	@throws D3DError If unable to create the buffer.
	*/
	IDirect3DVertexBuffer9* CreateVertexBuffer(IDirect3DDevice9& device, const unsigned int& buffer_length);
		
	/** Attempts to create an index buffer capable of storing \a buffer_length 16-bit indices.
	The created buffer is dynamic and write-only and in the default pool.
	@param device The device in which to create the buffer.
	@param buffer_length The number of indices which the buffer should hold.
	@return The newly created index buffer.
	@throws D3DError If unable to create the index buffer.
	*/
	IDirect3DIndexBuffer9* CreateIndexBuffer(IDirect3DDevice9& device, const unsigned int& buffer_length);
		
	/** Attempts to copy the contents of \a source to \a vertex_buffer.
	@param vertex_buffer The destination vertex buffer.
	@param source The source of data to be copied to \a vertex_buffer.
	@throws RendererException If \a vertex_buffer can't hold \a size bytes.
	@throws D3DError If unable to lock \a vertex_buffer.
	*/
	void FillVertexBuffer(IDirect3DVertexBuffer9& vertex_buffer, std::vector<float>& source);
		
	/** Attempts to copy the contents of \a source to \a vertex_buffer.
	@param index_buffer The destination index buffer.
	@param source The source of data to be copied to \a index_buffer.
	@throws RendererException If \a index_buffer can't hold \a size bytes.
	@throws D3DError If unable to lock \a index_buffer.
	*/
	void FillIndexBuffer(IDirect3DIndexBuffer9& index_buffer, std::vector<UINT16>& source);

	/** Renders a series of sprites.
	@attention \a sprites may very well be modified during this operation (though
	no actual sprites will be modified). If you need a copy of \a sprites after
	the call to this method, then make that copy before this method call.
	@post \a sprites may well not contain the same (or any) sprites. Make sure to
	keep your own separate copy if you need \a sprites for something else.
	@param device The device to render to.
	@param sprites The sprites to be rendered.
	@param vertex_buffer The buffer to use to render the sprites.
	@throws RendererException If \a sprites contains one or more NULL pointers.
	@throws D3DError If there is an error while using the device.
	@todo Consider using a list of std::tr1::shared_ptrs.
	@todo Describe the preconditions for vertex_buffer, index_buffer, and textures. Describe
	what happens if we can't render to the device (i.e. focus lost).
	*/
	void RenderSprites(IDirect3DDevice9& device, utility::SpriteList& sprites, IDirect3DVertexBuffer9& vertex_buffer, IDirect3DIndexBuffer9& index_buffer, TexHandleToTex& textures);



} // d3d
} // view
} // avl
#endif AVL_VIEW_D3D_D3D_WRAPPER__
