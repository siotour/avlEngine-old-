/**********
 * Author: Sheldon Bachstein
 * Date: Dec 25, 2010
 * Description: See d3d renderer base.h for details.
 **********/

#include"d3d renderer base.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\renderer\renderer.h"
#include"..\..\..\utility\src\sprite\sprite.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"
#include"..\..\..\utility\src\assert\assert.h"
#include"..\d3d display profile\d3d display profile.h"
#include<queue>
#include<vector>
#include<algorithm>
#include<string>
#include<limits.h>
#include<cstdlib>
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>






namespace avl
{
namespace view
{
	// Attempts to find the closest fitting display profile matching the parameters.
	// If fullscreen is true, the resulting display profile will be fullscreen. If unable to find a
	// display profile that at least matches the user's fullscreen preference, will throw a
	// RendererException.
	const D3DDisplayProfile LeastSquaredDisplayProfile(const bool fullscreen, const int width, const int height)
	{
		// Retrieve all possible display profiles.
		const DisplayProfiles profiles = EnumerateDisplayProfiles();
		
		// Search through all of the display profiles.
		// Temp variables.
		unsigned int width_difference;
		unsigned int height_difference;
		unsigned int difference_factor;
		// Difference factor of the best matching profile.
		unsigned int lowest_difference_factor = UINT_MAX;
		// End of the profiles.
		DisplayProfiles::const_iterator end = profiles.end();
		// Points to the best-matching profile.
		DisplayProfiles::const_iterator index = end;
		for(DisplayProfiles::const_iterator i = profiles.begin(); i != end; ++i)
		{
			// We only care about display profiles matching the user's fullscreen preference.
			if(i->IsFullscreen() == fullscreen)
			{
				// Find the width difference.
				width_difference = abs((long)(i->GetWidth() - width));
				// Find the height difference.
				height_difference = abs((long)(i->GetHeight() - height));
				// Find the difference factor for this profile.
				difference_factor = width_difference * width_difference + height_difference * height_difference;
				// Is this profile better than the previous best?
				if(difference_factor < lowest_difference_factor)
				{
					// This is the new best profile.
					index = i;
					lowest_difference_factor = difference_factor;
				}
			}
		}

		// If index points to the end of display profiles, then no suitable display profile
		// was found.
		if(index == end)
		{
			throw RendererException("avl::view::D3DRendererBase::LeastSquaredDisplayProfile() -- Unable to find an appropriate display profile.");
		}
		// Otherwise we've found a good profile. Return it.
		return *index;
	}



	// This function attempts to enumerate the default display adapter, using the HAL device, for all
	// legal display profiles with multisampling disabled. Each display profile contains a width, height,
	// backbuffer format, depth/stencil buffer format, and is either windowed or fullscreen. See the file
	// direct3d display profile.h for details. The legal display profiles are returned in an std::set.
	// Throws: 
	DisplayProfiles EnumerateDisplayProfiles()
	{
		// The first step is to create the IDirect3D8 object.
		IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
		
		// Make sure that Direct3DCreate9() was successful. If not, throw a RendererException with a 
		// description of the problem.
		if(d3d == NULL)
		{
			throw RendererException("avl::view::D3DRendererBase::EnumerateDisplayProfiles() -- Unable to create the D3D9 object.");
		}


		// Valid display profiles will be stored here.
		std::vector<const D3DDisplayProfile> profiles;


		// Check to make sure that the HAL device on the default adapter has the required capabilities.
		// If not, return an empty vector.
		if(CheckDeviceCapabilities(d3d) == false)
		{
			return profiles;
		}



		// Temporary storage for profile data.
		bool is_fullscreen;
		int width;
		int height;
		D3DFORMAT display;
		D3DFORMAT backbuffer;

		// Temporary storage for return values from D3D calls.
		HRESULT result;

		// For each possible display format, get the number of adapter modes available using the
		// default adapter.
		for(unsigned int i = 0; i < D3DDisplayProfile::DISPLAY_FORMATS_LENGTH; ++i)
		{
			// Store the current display format.
			display = D3DDisplayProfile::DISPLAY_FORMATS[i];

			// Get the number of available modes using the current display format.
			const unsigned int adapter_modes = d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, display);

			// Enumerate each available adapter mode.
			for(unsigned int j = 0; j < adapter_modes; ++j)
			{
				// Enumerate the current adapter mode.
				D3DDISPLAYMODE display_mode;
				memset(&display_mode, 0, sizeof(D3DDISPLAYMODE));
				result = d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, display, j, &display_mode);

				// If enumeration failed, throw a D3DError with the error code and a description of the problem.
				if(FAILED(result))
				{
					throw D3DError("IDirect3D9::EnumAdapterModes()", "avl::view::D3DRendererBase::EnumerateDisplayProfiles() -- Unable to enumerate display mode.", result);
				}

				// Obtain the resolution from the enumerated display mode.
				width = display_mode.Width;
				height = display_mode.Height;

				// Now get the valid backbuffer formats using a HAL device both for windowed and
				// fullscreen modes. This loop will execute once for is_fullscreen == true and once
				// for is_fullscreen == false.
				is_fullscreen = true;
				do
				{
					// For each possibly available backbuffer format, check to see if it is available with the
					// current display mode.
					for(unsigned int k = 0; k < D3DDisplayProfile::BACKBUFFER_FORMATS_LENGTH; ++k)
					{
						// Check to see if the current backbuffer format is available for the current display
						// format.
						backbuffer = D3DDisplayProfile::BACKBUFFER_FORMATS[k];
						result = d3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display, backbuffer, !is_fullscreen);

						// If the display and backbuffer formats are compatible, continue on to see if 16-bit
						// depth/stencil buffers and no-multisampling are supported.
						if(SUCCEEDED(result))
						{
							// Are 16-bit depth/stencil buffers supported?
							result = d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16);
									
							if(SUCCEEDED(result))
							{
								// Is D3DMULTISAMPLE_NONE supported with this backbuffer format?
								DWORD quality_levels;
								result = d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, backbuffer, !is_fullscreen, D3DMULTISAMPLE_NONE, &quality_levels);

								if(SUCCEEDED(result))
								{
									// Does this display format allow for textures with format D3DFMT_A8R8G8B8?
									result = d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display, 0, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8);

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

					// Negate is_fullscreen for the next iteration.
					is_fullscreen = !is_fullscreen;
				} while(is_fullscreen != true);
			}
		}


		// Return the available display profiles.
		return profiles;
	}




	// Checks to make sure that the Direct3D device on the default adapter supports all of the functionality
	// required by the D3DRendererBase class.
	bool CheckDeviceCapabilities(IDirect3D9* const d3d)
	{
		ASSERT(d3d != NULL);

		// First of all, get the device capabilities. If this fails, throw a D3DError with the error code and
		// a description of the problem.
		D3DCAPS9 caps;
		HRESULT result = d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		if(FAILED(result))
		{
			throw D3DError("IDirect3D9::GetDeviceCaps()", "avl::view::D3DRendererBase::CheckDeviceCapabilities() -- Unable to get the device capabilities.", result);
		}


		// This remains true unless the device does not support required functionality.
		bool caps_ok = true;


		// Alpha testing.
		if(caps.AlphaCmpCaps == D3DPCMPCAPS_ALWAYS || caps.AlphaCmpCaps == D3DPCMPCAPS_NEVER)
		{
			caps_ok = false;
		}


		// Alpha blending.



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




	// D3DRendererBase class
	//
	//
	

	// Attempts to initialize a Direct3D device for the specified window and with the specified
	// width, height, and fullscreen/windowed mode.
	D3DRendererBase::D3DRendererBase(HWND window_handle, const D3DDisplayProfile& profile)
	: width(profile.GetWidth()), height(profile.GetHeight()), fullscreen(profile.IsFullscreen()), display_format(profile.GetDisplayFormat()), backbuffer_format(profile.GetBackbufferFormat()), window_handle(window_handle), d3d(NULL), device(NULL), is_device_ready(false)
	{
		// The first step is to create the IDirect3D8 object. If this fails, throw a
		// RendererException detailing the problem.
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(d3d == NULL)
		{
			throw RendererException("avl::view::D3DRendererBase::D3DRendererBase() -- Unable to create the Direct3D9 object.");
		}

		// Next, attempt to initialize the device.
		InitializeDevice();
		ASSERT(device != NULL);

		// Now attempt to create the viewport.
		CreateViewport();
	}





	// A basic destructor which releases the direct3d object and device.
	D3DRendererBase::~D3DRendererBase()
	{
		// Release the d3d device.
		if(device != NULL)
		{
			device->Release();
			device = NULL;
		}

		// Release the d3d object.
		if(d3d != NULL)
		{
			d3d->Release();
			d3d = NULL;
		}

		// Indicate that the device isn't ready to be used.
		is_device_ready = false;
	}





	// Attempts to reset the device if the device is lost. If successful, returns true. If not, returns false. Should
	// be called when the device is lost. If there is an internal error with the device, a D3DError will be thrown
	// with the error code and a description of the problem. If the device has just been lost, this function will
	// release all non-managed assets. Upon successfully resetting the device, this function will re-acquire all released
	// assets.
	bool D3DRendererBase::Reset()
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);
		// Check the device state.
		HRESULT result = device->TestCooperativeLevel();

		// If the device is ready, set is_device_ready to true and return true.
		if(result == D3D_OK)
		{
			is_device_ready = true;
			return true;
		}
		// If the device was just lost, release assets and set is_device_ready to false.
		else if(result == D3DERR_DEVICELOST)
		{
			// If the device was just lost, release all unmanaged assets and set is_device_ready
			// to false.
			if(is_device_ready == true)
			{
				ReleaseUnmanagedAssets();
				is_device_ready = false;
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

			HRESULT result2 = device->Reset(&present_parameters);
			// If resetting the device was successful...
			if(result2 == D3D_OK)
			{
				// If the device was previously lost, then re-acquire any unmanaged assets.
				if(is_device_ready == false)
				{
					AcquireUnmanagedAssets();
				}
				// Set is_device_ready to true and return true.
				is_device_ready = true;
				return true;
			}
			// If the device isn't ready to be reset, set is_device_ready to false and return false.
			else if(result2 == D3DERR_DEVICELOST)
			{
				is_device_ready = false;
				return false;
			}
			// Otherwise there was some sort of error. Throw a D3DError with the error code and a description
			// of the problem.
			else
			{
				throw D3DError("IDirect3DDevice9::Reset()", "avl::view::D3DRendererBase::ResetDevice() -- An error occurred while resetting the device.", result2);
			}
		}
		// Otherwise there was some sort of error (internal error?). Throw a D3DError with the error code
		// and a description of the problem.
		else
		{
			throw D3DError("IDirect3D9::TestCooperativeLevel()", "avl::view::D3DRendererBase::ResetDevice() -- An internal error occurred while checking the device's state.", result);
		}
	}



	// Attempts to clear the screen to black and the z-buffer to 1.0f.
	void D3DRendererBase::ClearViewport()
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);

		// Clear the viewport to solid black. If this fails, throw a D3DError with the error
		// code and description.
		HRESULT result = device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		if(FAILED(result))
		{
			throw D3DError("IDirect3D9::Clear()", "avl::view::D3DRendererBase::ClearViewport() -- Unable to clear the viewport.", result);
		}
	}




	// NOTE: This function delegates to CopyPixelDataToTexture(), so thrown exceptions may describe the problem as
	// originating there.
	// Attempts to copy pixel_data into the destination texture, taking into account the pitch of the destiantion
	// surface. destination must be a lockable Direct3D texture, and pixel_data must point to a block of pixel data
	// of size width*height*bytes_per_pixel bytes. width and height are the dimensions of the pixel data, and
	// bytes_per_pixel specifies the amount of data per pixel (which depends on the pixel format). If pixel_data
	// is NULL, will throw a RendererException describing the problem. If unable to lock or unlock the surface,
	// will throw a D3DError with the error code and a description of the problem.
	void D3DRendererBase::CopyPixelDataToTexture(IDirect3DTexture9& destination, const unsigned char* const pixel_data,
									const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel)
	{
		ASSERT(pixel_data != NULL);
		// If pixel_data is NULL, throw an error describing the problem.
		if(pixel_data == NULL)
		{
			throw RendererException("avl::view::D3DRendererBase::CopyPixelDataToTexture() -- NULL value passed for pixel_data.");
		}

		// First access the texture's surface data.
		IDirect3DSurface9* surface = NULL;
		HRESULT result = destination.GetSurfaceLevel(0, &surface);

		// If accessing the texture's surface data failed, throw a D3DError with the error code and a description.
		if (FAILED(result))
		{
			throw D3DError("IDirect3DTexture9::GetSurfaceLevel()", "avl::view::D3DRendererBase::CopyPixelDataToTexture() -- Unable to access texture surface.", result);
		}

		
		// Delegate the task of copying the pixel data to the surface to CopyPixelDataToSurface().
		CopyPixelDataToSurface(*surface, pixel_data, width, height, bytes_per_pixel);

		// Release the surface.
		surface->Release();
	}




	// Attempts to copy pixel_data into the destination surface, taking into account the pitch of destination.
	// destination must be a lockable Direct3D surface, and pixel_data must point to a block of pixel data
	// of size width*height*bytes_per_pixel bytes. width and height are the dimensions of the pixel data, and
	// bytes_per_pixel specifies the amount of data per pixel (which depends on the pixel format). If pixel_data
	// is NULL, will throw a RendererException describing the problem. If unable to lock or unlock the surface,
	// will throw a D3DError with the error code and a description of the problem.
	void D3DRendererBase::CopyPixelDataToSurface(IDirect3DSurface9& destination, const unsigned char* const pixel_data,
													const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel)
	{
		ASSERT(pixel_data != NULL);
		// If pixel_data is NULL, throw an error describing the problem.
		if(pixel_data == NULL)
		{
			throw RendererException("avl::view::D3DRendererBase::CopyPixelDataToSurface() -- NULL value passed for pixel_data.");
		}

		// Temporarily holds return values.
		HRESULT result;

		// Lock the entire surface.
		D3DLOCKED_RECT rectangle;
		result = destination.LockRect(&rectangle, NULL, 0);

		// If locking of the surface failed, throw a D3DError with the error code and a description.
		if(FAILED(result))
		{
			throw D3DError("IDirect3DSurface9::LockRect()", "avl::view::D3DRendererBase::CopyPixelDataToSurface() -- Unable to lock surface.", result);
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
			throw D3DError("IDirect3DSurface9::UnlockRect()", "avl::view::D3DRendererBase::CopyPixelDataToSurface() - Unable to unlock the surface.", result);
		}
	}




	// Checks to see if the device supports textures in the specified format.
	bool D3DRendererBase::IsTextureFormatOk(D3DFORMAT& format)
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);

		// Return true if the device supports the specified texture format.
		return d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, display_format, 0, D3DRTYPE_TEXTURE, format) == D3D_OK;
	}




	// Attempts to create a texture with the specified width, height, and format. You can check to see if the device supports
	// the creation of textures of a format by using D3DRendererBase::IsTextureFormatOk(). If creating the texture fails, will
	// throw a D3DError with the error code and a description of the error. Textures are created in the MANAGED direct3d pool and
	// with 1 level.
	IDirect3DTexture9* D3DRendererBase::CreateTexture(const unsigned int& width, const unsigned int& height, D3DFORMAT format)
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);

		// Temporarily stores the texture's address.
		IDirect3DTexture9* texture = NULL;

		// Attempt to create the texture; if this fails, throw a D3DError with the error code and a
		// description of the problem.
		// TODO: Note the format used below. ARGB doesn't work...
		HRESULT result = D3DRendererBase::device->CreateTexture(width, height, 1, 0, format, D3DPOOL_MANAGED, &texture, NULL);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::CreateTexture()", "avl::View::D3DRendererBAse::CreateTexture() - Unable to create a new texture.", result);
		}
		ASSERT(texture != NULL);

		// Return the address of the newly created texture.
		return texture;
	}




	// Attempts to create a vertex buffer of the specified length (in terms of number of vertices) and with a vertex format
	// consisting of coordinates (XYZ) and one set of texture coordinates. If this fails, will throw a D3DError with the
	// error code and a description of the problem. The created buffer is dynamic and write-only and in the default pool.
	IDirect3DVertexBuffer9* D3DRendererBase::CreateVertexBuffer(const unsigned int& buffer_length)
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);
		// Attempt to create the vertex buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		IDirect3DVertexBuffer9* vertex_buffer = NULL;
		HRESULT result = device->CreateVertexBuffer(5 * buffer_length * sizeof(float), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
																					D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &vertex_buffer, NULL);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9()", "avl::view::D3DRendererBase::CreateVertexBuffer() -- Unable to create a vertex buffer.", result);
		}
		
		ASSERT(vertex_buffer != NULL);

		// Return the newly created vertex buffer.
		return vertex_buffer;
	}




	// Attempts to create an index buffer of the specified length (in terms of number of indices) and with a 16 bit
	// index format. If this fails, will throw a D3DError with the error code and a description of the problem.
	// The created buffer is dynamic and write-only and in the default pool.
	IDirect3DIndexBuffer9* D3DRendererBase::CreateIndexBuffer(const unsigned int& buffer_length)
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);
		// 16-bit indices are 2 bytes each.
		unsigned int index_size = 2;

		// Attempt to create the index buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		IDirect3DIndexBuffer9* index_buffer = NULL;
		HRESULT result = device->CreateIndexBuffer(buffer_length * index_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
																				D3DFMT_INDEX16, D3DPOOL_DEFAULT, &index_buffer, NULL);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::CreateIndexBuffer()", "avl::view::D3DRendererBase::CreateIndexBuffer() -- Unable to create an index buffer.", result);
		}
		
		ASSERT(index_buffer != NULL);

		// Return the newly created index buffer.
		return index_buffer;
	}




	// Attempts to lock vertex_buffer and then copy size bytes from source to vertex_buffer.
	void D3DRendererBase::FillVertexBuffer(IDirect3DVertexBuffer9& vertex_buffer, const unsigned char* const source, const unsigned int size)
	{
		// If size is 0, return.
		if(size == 0)
		{
			return;
		}

		// If vertex_buffer can't hold all of the vertices, throw a RendererException with a description of the problem.
		D3DVERTEXBUFFER_DESC description;
		vertex_buffer.GetDesc(&description);
		if(description.Size < size)
		{
			throw RendererException("avl::view::D3DRenderer::FillVertexBuffer() -- The vertex buffer can't hold this much information!");
		}



		// Attempt to lock the vertex buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		BYTE* data = NULL;
		HRESULT result = vertex_buffer.Lock(0, size, (void**)&data, D3DLOCK_DISCARD);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DVertexBuffer9::Lock()", "avl::view::D3DRendererBase::FillVertexBuffer() -- Unable to lock the vertex buffer.", result);
		}


		// Copy the vertex information to the vertex buffer.
		memcpy(data, source, size);


		// Unlock the vertex buffer.
		data = NULL;
		vertex_buffer.Unlock();
	}




	// Attempts to lock index_buffer and then copy size bytes from source to vertex_buffer.
	void D3DRendererBase::FillIndexBuffer(IDirect3DIndexBuffer9& index_buffer, const unsigned char* const source, const unsigned int size)
	{
		// If size is 0, return.
		if(size == 0)
		{
			return;
		}

		// If index_buffer can't hold all of the indices, throw a RendererException with a description of the problem.
		D3DINDEXBUFFER_DESC description;
		index_buffer.GetDesc(&description);
		if(description.Size < size)
		{
			throw RendererException("avl::view::D3DRenderer::FillIndexBuffer() -- The index buffer can't hold this much information!");
		}

		// Attempt to lock the index buffer. If this fails, throw a D3DError with the error code and a description
		// of the problem.
		BYTE* data = NULL;
		HRESULT result = index_buffer.Lock(0, size, (void**)&data, D3DLOCK_DISCARD);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DIndexBuffer9::Lock()", "avl::view::BasicRenderer::WriteIndicesToBuffer() -- Unable to lock the index buffer.", result);
		}


		// Copy the index information to the index buffer.
		memcpy(data, source, size);


		// Unlock the index buffer.
		data = NULL;
		index_buffer.Unlock();
	}

	




	// Attempts to initialize the device. If this fails, will throw an exception detailing
	// the problem.
	void D3DRendererBase::InitializeDevice()
	{
		ASSERT(d3d != NULL);

		// Set up the presentation parameters for the device.

		present_parameters.BackBufferWidth = width;
		present_parameters.BackBufferHeight = height;
		// 32 bit pixel format, with 8 bits for alpha, red, green, and blue.
		present_parameters.BackBufferFormat = backbuffer_format;
		present_parameters.BackBufferCount = 1;
		present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		present_parameters.MultiSampleQuality = 0;
		present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		present_parameters.hDeviceWindow = window_handle;
		// If the user wants fullscreen, then it's not windowed. If user doesn't want
		// fullscreen, then windowed is true.
		present_parameters.Windowed = (fullscreen == true)? FALSE : TRUE;
		// Let direct3d automatically handle the depth stencil.
		present_parameters.EnableAutoDepthStencil = TRUE;
		// Depth stencil will be 16 bits per pixel.
		present_parameters.AutoDepthStencilFormat = D3DFMT_D16;
		present_parameters.Flags = 0;
		present_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


		// Attempt to create the device.
		HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device);

		// Check to see if the device was created successfully. If it wasn't, throw a
		// D3DError containing information on the failure.
		if (FAILED(result) == true)
		{
			throw D3DError("IDirect3D9::CreateDevice()", "avl::view::D3DRendererBase::InitializeDevice() -- Creation of the Direct3D device failed.", result);
		}

		ASSERT(device != NULL);
		// Now set is_device_ready to true to indicate that the device is ready for use.
		is_device_ready = true;
	}




	// Creates a basic viewport for the device. Uses the width and height supplied to the
	// constructor, with Z ranging from 0.0f to 1.0f and (0, 0) being in the top-left.
	void D3DRendererBase::CreateViewport()
	{
		ASSERT(d3d != NULL);
		ASSERT(device != NULL);

		// Fill in the D3DVIEWPORT8 structure.
		D3DVIEWPORT9 viewport;

		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;

		// Attempt to set the above structure as the device's viewport. If this fails, then
		// throw a avl::utility::D3DError with a description of the problem and the error
		// code.
		HRESULT result = device->SetViewport(&viewport);
		if(FAILED(result))
		{
			throw D3DError("IDirect3DDevice9::SetViewport()", "avl::view::D3DRendererBase::CreateViewport() -- Unable to set the viewport.", result);
		}
	}








	// D3DError Class:
	//
	//
	D3DError::D3DError(const std::string& d3d_function, const std::string& explanation, const HRESULT& error_code)
		: error_code(error_code)
	{
		description = "The Direct3D function ";
		description += d3d_function;
		description += " failed with error code ";
		description += error_code;
		description += ": ";
		description += explanation;
	}




	// Basic destructor.
	D3DError::~D3DError()
	{
	}




	// Copy constructor so that this class may be caught (using a try/catch block) by value.
	D3DError::D3DError(const D3DError& original)
	: RendererException(original), error_code(original.GetErrorCode())
	{
	}




	// Returns the error code as given by the failing Direct3D function.
	const HRESULT& D3DError::GetErrorCode() const
	{
		return error_code;
	}




}
}