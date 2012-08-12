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
Implementation for the wrapper functions component. See "wrapper functions.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"wrapper functions.h"
#include"..\render context\render context.h"
#include"..\texture context\texture context.h"
#include"..\d3d display profile\d3d display profile.h"
#include"..\..\renderer\renderer.h"
#include"..\d3d error\d3d error.h"
#include"..\..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\..\utility\src\render primitive\render primitive.h"
#include"..\..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\..\utility\src\assert\assert.h"
#include<string>
#include<queue>
#include<list>
#include<typeinfo>
#include<queue>
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
	// Anonymous namespace forward declarations. See function definitions (at the end of this file) for details.
	namespace
	{
		/// Contains textured quad render primitives.
		typedef std::list<utility::TexturedQuad* const> TexturedQuadList;
		/// Maps a texture handle to the number of quads using it.
		typedef std::queue<std::pair<const utility::TexturedQuad::TextureHandle, const unsigned int>> TextureHandleUseQueue;

		// Display profile choice helpers.
		const unsigned int FindDifferenceFactor(const D3DDisplayProfile& profile, const unsigned int width, const unsigned int height);
		void EnumerateDisplayModes(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles);
		void EnumerateAdapterModes(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles, const D3DFORMAT& display);
		void EnumerateWindowedMode(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles, const D3DFORMAT& display, const D3DDISPLAYMODE& display_mode, const bool is_fullscreen);
		void VerifyModeCompatibility(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles, const D3DFORMAT& display, const D3DFORMAT& backbuffer, const bool is_fullscreen, const unsigned int width, const unsigned int height);
	}





	// See function declaration for details.
	const D3DDisplayProfile LeastSquaredDisplayProfile(const int width, const int height, const bool fullscreen)
	{
		// Retrieve all possible display profiles.
		const DisplayProfiles profiles = EnumerateDisplayProfiles();
		// Current and best difference factors.
		unsigned int difference_factor;
		unsigned int best_difference_factor = UINT_MAX;
		// End of the profiles, and the best matching profile.
		const DisplayProfiles::const_iterator end = profiles.end();
		DisplayProfiles::const_iterator best_match = end;
		for(DisplayProfiles::const_iterator i = profiles.begin(); i != end; ++i)
		{
			// We only care about display profiles matching the user's fullscreen preference.
			if(i->IsFullscreen() == fullscreen)
			{
				// Is this profile better than the previous best?
				difference_factor = FindDifferenceFactor(*i, width, height);
				if(difference_factor < best_difference_factor)
				{
					best_match = i;
					best_difference_factor = difference_factor;
				}
			}
		}
		// Did we fail to find a suitable match?
		if(best_match == end)
		{
			throw RendererException("avl::view::d3d::LeastSquaredDisplayProfile() -- Unable to find an appropriate display profile.");
		}
		// Otherwise we've found a good profile.
		return *best_match;
	}



	// See function declaration for details.
	DisplayProfiles EnumerateDisplayProfiles()
	{
		// The first step is to create the IDirect3D9 object.
		IDirect3D9& d3d = *GetDirect3DObject();
		// Valid display profiles will be stored here.
		std::vector<const D3DDisplayProfile> profiles;
		// If the HAL device doesn't support all required functionality, just return
		// the empty vector.
		if(CheckDeviceCapabilities(d3d) == false)
		{
			return profiles;
		}
		// Enumerate all display modes.
		EnumerateDisplayModes(d3d, profiles);
		// Release the d3d object.
		d3d.Release();
		// Return the available display profiles.
		return profiles;
	}




	// See function declaration for details.
	bool CheckDeviceCapabilities(IDirect3D9& d3d)
	{
		// Obtain the device capabilities.
		D3DCAPS9 caps;
		HRESULT result = d3d.GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		if(FAILED(result))
		{
			throw D3DError("IDirect3D9::GetDeviceCaps()", "avl::view::d3d::Device::CheckDeviceCapabilities() -- Unable to get the device capabilities.", result);
		}
		// This remains true unless the device does not support required functionality.
		bool caps_ok = true;
		// Alpha testing.
		if(caps.AlphaCmpCaps == D3DPCMPCAPS_ALWAYS || caps.AlphaCmpCaps == D3DPCMPCAPS_NEVER)
		{
			caps_ok = false;
		}
		// DrawIndexPrimitive().
		if((caps.DevCaps & D3DDEVCAPS_DRAWPRIMTLVERTEX) == 0)
		{
			caps_ok = false;
		}
		// Hardware transformations and lighting.
		if((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0)
		{
			caps_ok = false;
		}
		// Alpha textures.
		if((caps.TextureCaps & D3DPTEXTURECAPS_ALPHA) == 0)
		{
			caps_ok = false;
		}
		// Non-power-of-two textures.
		if((caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL & D3DPTEXTURECAPS_POW2) != 0)
		{
			caps_ok = false;
		}
		// Non-square textures.
		if((caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0)
		{
			caps_ok = false;
		}
		return caps_ok;
	}


	// See function declaration for details.
	IDirect3D9* const GetDirect3DObject()
	{
		IDirect3D9* const d3d = Direct3DCreate9(D3D_SDK_VERSION);
		// Did the call fail?
		if(d3d == nullptr)
		{
			throw D3DError("avl::view::d3d::GetDirect3DObject", "Unable to obtain a pointer to a Direct3D9 object.", 0);
		}
		return d3d;
	}


	// See function declaration for details.
	IDirect3DDevice9* CreateDevice(IDirect3D9& d3d, HWND& window_handle, const D3DDisplayProfile& profile, D3DPRESENT_PARAMETERS& present_parameters)
	{
		// Set up the presentation parameters for the device.
		present_parameters.BackBufferWidth = profile.GetWidth();
		present_parameters.BackBufferHeight = profile.GetHeight();
		present_parameters.BackBufferFormat = profile.GetBackbufferFormat();
		present_parameters.BackBufferCount = 1;
		present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		present_parameters.MultiSampleQuality = 0;
		present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		present_parameters.hDeviceWindow = window_handle;
		present_parameters.Windowed = (profile.IsFullscreen() == true)? FALSE : TRUE;
		present_parameters.EnableAutoDepthStencil = TRUE;		// Let direct3d automatically handle the depth stencil.
		present_parameters.AutoDepthStencilFormat = D3DFMT_D16;	// Depth stencil will be 16 bits per pixel.
		present_parameters.Flags = 0;
		present_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		// Attempt to create the device.
		IDirect3DDevice9* device = nullptr;
		HRESULT result = d3d.CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device);
		// Check to see if the device was created successfully. If it wasn't, throw a
		// D3DError containing information on the failure.
		if (FAILED(result) == true)
		{
			throw D3DError("IDirect3D9::CreateDevice()", "avl::view::d3d::CreateDevice() -- Creation of the Direct3D device failed.", result);
		}
		ASSERT(device != nullptr);
		return device;
	}

	// See function declaration for details.
	const bool ResetDevice(IDirect3DDevice9& device, D3DPRESENT_PARAMETERS& present_parameters)
	{
		HRESULT result = device.Reset(&present_parameters);
		// Was the device reset?
		if(result == D3D_OK)
		{
			return true;
		}
		// Is the device still lost?
		else if(result == D3DERR_DEVICELOST)
		{
			return false;
		}
		// Was there an internal error?
		else
		{
			throw D3DError("IDirect3DDevice9::Reset()", "avl::view::d3d::ResetDevice() -- An error occurred while resetting the device.", result);
		}
	}





	// See function declaration for details.
	void CreateViewport(IDirect3DDevice9& device, const unsigned int& width, const unsigned int& height)
	{
		// Fill in the D3DVIEWPORT8 structure.
		D3DVIEWPORT9 viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		// Attempt to set the above structure as the device's viewport.
		HRESULT result = device.SetViewport(&viewport);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::SetViewport()", "avl::view::d3d::CreateViewport() -- Unable to set the viewport.", result);
		}
	}


	// See function declaration for details.
	void SetScreenScaling(IDirect3DDevice9& device, const float x_factor, const float y_factor)
	{
		// Scale x and y coordinates.
		D3DMATRIX matrix = {x_factor, 0.0f, 0.0f, 0.0f,
							0.0f, y_factor, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f};
		device.SetTransform(D3DTS_VIEW, &matrix);
	}


	// See function declaration for details.
	void ClearViewport(IDirect3DDevice9& device)
	{
		// Clear the viewport to solid black. If this fails, throw a D3DError with the error
		// code and description.
		HRESULT result = device.Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::Clear()", "avl::view::d3d::ClearViewport() -- Unable to clear the viewport.", result);
		}
	}


	// See function declaration for details.
	void CopyPixelDataToTexture(IDirect3DTexture9& destination, const unsigned char* const pixel_data,
									const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel)
	{
		ASSERT(pixel_data != nullptr);
		// If pixel_data is nullptr, throw an error describing the problem.
		if(pixel_data == nullptr)
		{
			throw utility::InvalidArgumentException("avl::view::d3d::CopyPixelDataToTexture()", "pixel_data", "Can not be null.");
		}
		// First access the texture's surface data.
		IDirect3DSurface9* surface = nullptr;
		HRESULT result = destination.GetSurfaceLevel(0, &surface);
		// If accessing the texture's surface data failed, throw a D3DError with the error code and a description.
		if (FAILED(result))
		{
			throw D3DError("IDirect3DTexture9::GetSurfaceLevel()", "avl::view::d3d::CopyPixelDataToTexture() -- Unable to access texture surface.", result);
		}
		// Delegate the task of copying the pixel data to the surface to CopyPixelDataToSurface().
		CopyPixelDataToSurface(*surface, pixel_data, width, height, bytes_per_pixel);
		// Release the surface.
		surface->Release();
	}


	// See function declaration for details.
	void CopyPixelDataToSurface(IDirect3DSurface9& destination, const unsigned char* const pixel_data,
													const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel)
	{
		ASSERT(pixel_data != nullptr);
		// If pixel_data is nullptr, throw an error describing the problem.
		if(pixel_data == nullptr)
		{
			throw utility::InvalidArgumentException("avl::view::d3d::CopyPixelDataToSurface()", "pixel_data", "Can not be null.");
		}
		// Temporarily holds return values.
		HRESULT result;
		// Lock the entire surface.
		D3DLOCKED_RECT rectangle;
		result = destination.LockRect(&rectangle, nullptr, 0);
		// If locking of the surface failed, throw a D3DError with the error code and a description.
		if(FAILED(result))
		{
			throw D3DError("IDirect3DSurface9::LockRect()", "avl::view::d3d::CopyPixelDataToSurface() -- Unable to lock surface.", result);
		}
		// Now copy pixel_data into the locked rect row by row while taking the pitch of the surface into account.
		// Illustration:
		//    |<-------1 row of pixel data------->|<---device-used padding--->|
		//    |<--------------------------   Pitch   ------------------------>|
		for(unsigned int row = 1; row <= height; ++row)
		{
			memcpy((unsigned char*)rectangle.pBits + rectangle.Pitch*(row-1), pixel_data + width * (row-1) * bytes_per_pixel, width * bytes_per_pixel);
		}
		// Unlock the surface.
		result = destination.UnlockRect();
		// If unable to unlock the surface, throw a D3DError with the error code and a description.
		if(FAILED(result))
		{
			throw D3DError("IDirect3DSurface9::UnlockRect()", "avl::view::d3d::CopyPixelDataToSurface() - Unable to unlock the surface.", result);
		}
	}


	// See function declaration for details.
	bool IsTextureFormatOk(IDirect3D9& d3d, D3DFORMAT& adapter_format, D3DFORMAT& format)
	{
		// Return true if the device supports the specified texture format.
		return d3d.CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapter_format, 0, D3DRTYPE_TEXTURE, format) == D3D_OK;
	}




	// See function declaration for details.
	IDirect3DTexture9* CreateTexture(IDirect3DDevice9& device, const unsigned int& width, const unsigned int& height, D3DFORMAT format)
	{
		// Temporarily stores the texture's address.
		IDirect3DTexture9* texture = nullptr;
		// Attempt to create the texture; if this fails, throw a D3DError with the error code and a
		// description of the problem.
		HRESULT result = device.CreateTexture(width, height, 1, 0, format, D3DPOOL_MANAGED, &texture, nullptr);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::CreateTexture()", "avl::view::d3d::CreateTexture() - Unable to create a new texture.", result);
		}
		ASSERT(texture != nullptr);
		// Return the address of the newly created texture.
		return texture;
	}




	// See function declaration for details.
	IDirect3DVertexBuffer9* CreateVertexBuffer(IDirect3DDevice9& device, const unsigned int& buffer_length)
	{
		// Attempt to create the vertex buffer.
		IDirect3DVertexBuffer9* vertex_buffer = nullptr;
		HRESULT result = device.CreateVertexBuffer(5 * buffer_length * sizeof(float), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertex_buffer, nullptr);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9()", "avl::view::d3d::CreateVertexBuffer() -- Unable to create a vertex buffer.", result);
		}
		ASSERT(vertex_buffer != nullptr);
		// Return the newly created vertex buffer.
		return vertex_buffer;
	}




	// See function declaration for details.
	IDirect3DIndexBuffer9* CreateIndexBuffer(IDirect3DDevice9& device, const unsigned int& buffer_length)
	{
		// 16-bit indices are 2 bytes each.
		unsigned int index_size = 2;
		// Attempt to create the index buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		IDirect3DIndexBuffer9* index_buffer = nullptr;
		HRESULT result = device.CreateIndexBuffer(buffer_length * index_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &index_buffer, nullptr);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::CreateIndexBuffer()", "avl::view::d3d::CreateIndexBuffer() -- Unable to create an index buffer.", result);
		}
		ASSERT(index_buffer != nullptr);
		// Return the newly created index buffer.
		return index_buffer;
	}




	// See function declaration for details.
	void FillVertexBuffer(IDirect3DVertexBuffer9& vertex_buffer, const std::vector<const float>& source)
	{
		// If size is 0, return.
		if(source.size() == 0)
		{
			return;
		}
		// If vertex_buffer can't hold all of the vertices, throw a RendererException with a description of the problem.
		D3DVERTEXBUFFER_DESC description;
		vertex_buffer.GetDesc(&description);
		if(description.Size < source.size() * sizeof(float))
		{
			throw RendererException("avl::view::d3d::D3DRenderer::FillVertexBuffer() -- The vertex buffer can't hold this much information!");
		}
		// Attempt to lock the vertex buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		BYTE* data = nullptr;
		HRESULT result = vertex_buffer.Lock(0, source.size() * sizeof(float), (void**)&data, D3DLOCK_DISCARD);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DVertexBuffer9::Lock()", "avl::view::d3d::FillVertexBuffer() -- Unable to lock the vertex buffer.", result);
		}
		// Copy the vertex information to the vertex buffer.
		memcpy(data, &source[0], source.size() * sizeof(float));
		// Unlock the vertex buffer.+
		data = nullptr;
		vertex_buffer.Unlock();
	}




	// See function declaration for details.
	void FillIndexBuffer(IDirect3DIndexBuffer9& index_buffer, const std::vector<const UINT16>& source)
	{
		// If size is 0, return.
		if(source.size() == 0)
		{
			return;
		}
		// If index_buffer can't hold all of the indices, throw a RendererException with a description of the problem.
		D3DINDEXBUFFER_DESC description;
		index_buffer.GetDesc(&description);
		if(description.Size < source.size() * sizeof(UINT16))
		{
			throw RendererException("avl::view::d3d::D3DRenderer::FillIndexBuffer() -- The index buffer can't hold this much information!");
		}
		// Attempt to lock the index buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		BYTE* data = nullptr;
		HRESULT result = index_buffer.Lock(0, source.size() * sizeof(UINT16), (void**)&data, D3DLOCK_DISCARD);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DIndexBuffer9::Lock()", "avl::view::d3d::BasicD3DRenderer::WriteIndicesToBuffer() -- Unable to lock the index buffer.", result);
		}
		// Copy the index information to the index buffer.
		memcpy(data, &source[0], source.size() * sizeof(UINT16));
		// Unlock the index buffer.
		data = nullptr;
		index_buffer.Unlock();
	}








	// Anonymous namespace.
	namespace
	{
		/** Calculates the squared sum of the difference in width and height between
		\a profile and \a width and \a height.
		@param profile The display profile with the resolution we're comparing to
		\a width and \a height.
		@param width The desired width.
		@param height The desired height.
		@return The squared sum of the difference between \a width and \a height, and
		\a profile.
		*/
		const unsigned int FindDifferenceFactor(const D3DDisplayProfile& profile, const unsigned int width, const unsigned int height)
		{
			const int width_difference = profile.GetWidth() - width;
			const int height_difference = profile.GetHeight() - height;
			return (unsigned int)(width_difference * width_difference) + (height_difference * height_difference);
		}



		/** Enumerates all compatible display modes supported by the \a d3d object and adds them to
		\a profiles.
		@param d3d [IN] The Direct3D interface to enumerate for.
		@param profiles [OUT] All compatible display modes.
		*/
		void EnumerateDisplayModes(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles)
		{
			// For each possible display format, get the number of adapter modes available using the
			// default adapter.
			for(unsigned int i = 0; i < D3DDisplayProfile::DISPLAY_FORMATS_LENGTH; ++i)
			{
				// Enumerate adapter modes on the current display mode.
				EnumerateAdapterModes(d3d, profiles, D3DDisplayProfile::DISPLAY_FORMATS[i]);
			}
		}



		/** Enumerates adapter modes using the \a display format.
		@param d3d [IN] The Direct3D interface to enumerate for.
		@param profiles [OUT] All compatible display modes.
		@param display The display format to enumerate adapter modes with.
		*/
		void EnumerateAdapterModes(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles, const D3DFORMAT& display)
		{
			HRESULT result;
			// Get the number of available modes using the current display format.
			const unsigned int adapter_modes = d3d.GetAdapterModeCount(D3DADAPTER_DEFAULT, display);
			// Enumerate each available adapter mode.
			for(unsigned int j = 0; j < adapter_modes; ++j)
			{
				// Enumerate the current adapter mode.
				D3DDISPLAYMODE display_mode;
				memset(&display_mode, 0, sizeof(D3DDISPLAYMODE));
				result = d3d.EnumAdapterModes(D3DADAPTER_DEFAULT, display, j, &display_mode);

				// If enumeration failed, throw a D3DError with the error code and a description of the problem.
				if(FAILED(result))
				{
					throw D3DError("IDirect3D9::EnumAdapterModes()", "avl::view::d3d::EnumerateAdapterModes() -- Unable to enumerate display mode.", result);
				}
				// Enumerate for fullscreen and windowed modes.
				EnumerateWindowedMode(d3d, profiles, display, display_mode, true);
				EnumerateWindowedMode(d3d, profiles, display, display_mode, false);
			}
		}


		/** Enumerates all compatible display modes using the \a display format, \a display_mode adapter mode, and with windowed mode indicated
		by \a is_fullscreen.
		@param d3d [IN] The Direct3D interface to enumerate for.
		@param profiles [OUT] All compatible display modes.
		@param display The display format to enumerate adapter modes with.
		@param display_mode The adapter mode to enumerate.
		@param is_fullscreen The windowed mode to enumerate.
		*/
		void EnumerateWindowedMode(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles, const D3DFORMAT& display, const D3DDISPLAYMODE& display_mode, const bool is_fullscreen)
		{
			// Obtain the resolution from the enumerated display mode.
			const unsigned int width = display_mode.Width;
			const unsigned int height = display_mode.Height;
			// For each possibly available backbuffer format, check to see if it is available with the
			// current display mode and fullscreen mode.
			for(unsigned int i = 0; i < D3DDisplayProfile::BACKBUFFER_FORMATS_LENGTH; ++i)
			{
				// Check to see if the current backbuffer format and display mode are compatible.
				VerifyModeCompatibility(d3d, profiles, display, D3DDisplayProfile::BACKBUFFER_FORMATS[i], is_fullscreen, width, height);				
			}
		}



		/** Verifies that the device mode indicated by \a display, \a backbuffer, \a is_fullscreen, \a width,
		and \a height is a valid device mode.
		@param d3d [IN] The Direct3D interface to enumerate for.
		@param profiles [OUT] All compatible display modes.
		@param display The display format to enumerate adapter modes with.
		@param is_fullscreen The windowed mode to enumerate.
		*/
		void VerifyModeCompatibility(IDirect3D9& d3d, std::vector<const D3DDisplayProfile>& profiles, const D3DFORMAT& display, const D3DFORMAT& backbuffer, const bool is_fullscreen, const unsigned int width, const unsigned int height)
		{
			HRESULT result = d3d.CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display, backbuffer, !is_fullscreen);
			// Are the display and backbuffer formats compatible?
			if(SUCCEEDED(result))
			{
				// Are 16-bit depth/stencil buffers supported?
				result = d3d.CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16);		
				if(SUCCEEDED(result))
				{
					// Is D3DMULTISAMPLE_NONE supported with this backbuffer format?
					DWORD quality_levels;
					result = d3d.CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, backbuffer, !is_fullscreen, D3DMULTISAMPLE_NONE, &quality_levels);
					if(SUCCEEDED(result))
					{
						// Does this display format allow for textures with format D3DFMT_A8R8G8B8?
						result = d3d.CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display, 0, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8);
						if(SUCCEEDED(result))
						{
							// Only add the element if there isn't an equivalent one already.
							const D3DDisplayProfile element(is_fullscreen, width, height, display, backbuffer);
							if(std::find(profiles.begin(), profiles.end(), element) == profiles.end())
							{
								profiles.insert(profiles.begin(), element);
							}
						}
					}
				}
			}
		}


		
	}




} // d3d
} // view
} // avl
