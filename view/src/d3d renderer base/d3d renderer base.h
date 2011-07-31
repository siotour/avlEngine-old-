#ifndef __AVL_VIEW_D3D_RENDERER_BASE__
#define __AVL_VIEW_D3D_RENDERER_BASE__
/**********
 * Author: Sheldon Bachstein
 * Date: Dec 25, 2010
 * Description: Provides a partially implemented interface for a basic Direct3D renderer.
 * Implements the initialization of a Direct3D device and is meant to be used as a skeleton
 * on which to build customized Direct3D renderers. Implemented behavior is for a 32-bit
 * pixel format, but this can be customized by overloading the
 * avl::view::D3DRendererBase::InitilializeDevice() function. As long as
 * avl::view::D3DRendererBase::present_parameters is assigned the proper format, then
 * avl::D3DRendererBase::ResetDevice() should work fine. Additionally implements various
 * wrapper functions over Direct3D, such as for creating vertex buffers.
 **********/

  #include"..\..\..\utility\src\exception\exception.h"
// For window handles.
#include<Windows.h>
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>
#include<string>
#include<queue>
#include<vector>

namespace avl
{
// Forward declaration.
namespace utility{class TexturedVertex;}

namespace view
{
	// Forward declaration.
	class D3DDisplayProfile;




	// This function attempts to enumerate the default display adapter, using the HAL device, for all
	// legal display profiles with multisampling disabled. Each display profile contains a width, height,
	// display format, backbuffer format and is either windowed or fullscreen. See the
	// d3d display profile component for details. The legal display profiles are returned in an std::vector.
	// Throws: 
	const std::vector<D3DDisplayProfile> EnumerateDisplayProfiles();


	// Checks to make sure that the HAL Direct3D device on the default adaptersupports all of the
	// functionality required by the D3DRendererBase. Returns true if so, and false if not.
	bool CheckDeviceCapabilities(IDirect3D9* const d3d);



	// See the beginning of the file for details.
	class D3DRendererBase
	{
	public:
		// Constructors:
		// Attempts to initialize a Direct3D device for the specified window and with the specified display
		// profile.
		D3DRendererBase(HWND window_handle, const D3DDisplayProfile& profile);
		virtual ~D3DRendererBase();


		// Attempts to reset the device. If successful, returns true. If not, returns false. Should be called
		// when the device is lost.
		bool Reset();

	protected:
		// Attempts to clear the backbuffer to black and the z-buffer to 1.0f.
		void ClearViewport();
		// This function will be called when the device is lost; derived classes should implement it such that
		// it release all non-managed Direct3D assets. These should then be re-acquired in
		// AcquireUnmanagedAssets().
		virtual void ReleaseUnmanagedAssets() = 0;
		// This function will be called when the device is reset after having been lost. Derived classes should
		// implement it such that it re-acquries any non-managed Direct3D assets that were released in
		// ReleaseUnmanagedAssets().
		virtual void AcquireUnmanagedAssets() = 0;



		// Wrapper functions around common Direct3D functionality:
		//
		//


		// NOTE: This function delegates to CopyPixelDataToTexture(), so thrown exceptions may describe the problem as
		// originating there.
		// Attempts to copy pixel_data into the destination texture, taking into account the pitch of the destiantion
		// surface. destination must be a lockable Direct3D surface, and pixel_data must point to a block of pixel data
		// of size width*height*bytes_per_pixel bytes. width and height are the dimensions of the pixel data, and
		// bytes_per_pixel specifies the amount of data per pixel (which depends on the pixel format). If pixel_data
		// is NULL, will throw a utility::Exception describing the problem. If unable to lock or unlock the surface,
		// will throw a D3DError with the error code and a description of the problem.
		void CopyPixelDataToTexture(IDirect3DTexture9& destination, const unsigned char* const pixel_data,
									const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel);
		// Attempts to copy pixel_data into the destination surface, taking into account the pitch of destination.
		// destination must be a lockable Direct3D surface, and pixel_data must point to a block of pixel data
		// of size width*height*bytes_per_pixel bytes. width and height are the dimensions of the pixel data, and
		// bytes_per_pixel specifies the amount of data per pixel (which depends on the pixel format). If pixel_data
		// is NULL, will throw a utility::Exception describing the problem. If unable to lock or unlock the surface,
		// will throw a D3DError with the error code and a description of the problem.
		void CopyPixelDataToSurface(IDirect3DSurface9& destination, const unsigned char* const pixel_data,
										const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel);
		// Checks to see if the device supports textures in the specified format.
		bool IsTextureFormatOk(D3DFORMAT& format);
		// Attempts to create a texture with the specified width, height, and format. You can check to see if the device supports
		// the creation of textures of a format by using D3DRendererBase::IsTextureFormatOk(). If creating the texture fails, will
		// throw a D3DError with the error code and a description of the error. Textures are created in the MANAGED direct3d pool and
		// with 1 level.
		IDirect3DTexture9* CreateTexture(const unsigned int& width, const unsigned int& height, D3DFORMAT format);
		// Attempts to create a vertex buffer of the specified length (in terms of number of vertices) and with a vertex format
		// consisting of coordinates (XYZ) and one set of texture coordinates. If this fails, will throw a D3DError with the
		// error code and a description of the problem. The created buffer is dynamic and write-only and in the default pool.
		IDirect3DVertexBuffer9* CreateVertexBuffer(const unsigned int& buffer_length);
		// Attempts to create an index buffer of the specified length (in terms of number of indices) and with a 16 bit
		// index format. If this fails, will throw a D3DError with the error code and a description of the problem.
		// The created buffer is dynamic and write-only and in the default pool.
		IDirect3DIndexBuffer9* CreateIndexBuffer(const unsigned int& buffer_length);
		// Attempts to write the supplied vertices to the supplied vertex buffer. If an error occurs, will throw a D3DError
		// with the error code and a description of the problem. The previous information is discarded. The queue will
		// be left unmodified.
		void FillVertexBuffer(IDirect3DVertexBuffer9& vertex_buffer, std::queue<const utility::TexturedVertex* const> vertices);
		// Attempts to write the supplied indices to the supplied index buffer. If an error occurs, will throw a D3DError
		// with the error code and a description of the problem. The previous information is discarded. The queue will
		// be left unmodified.
		void FillIndexBuffer(IDirect3DIndexBuffer9& index_buffer, std::queue<const unsigned short> indices);




		// The present paramters for the device.
		D3DPRESENT_PARAMETERS present_parameters;

		// The width and height of the screen and viewport.
		const int width;
		const int height;
		// Fullscreen or not?
		const bool fullscreen;
		// The format of the direct3d device.
		const D3DFORMAT display_format;
		// The format of the backbuffer.
		const D3DFORMAT backbuffer_format;
		// A handle to the window which is being rendered to.
		const HWND window_handle;

		// The Direct3D8 object.
		IDirect3D9* d3d;

		// The Direct3D Device.
		IDirect3DDevice9* device;

		// Indicates whether or not the device is ready for rendering.
		bool is_device_ready;




	private:
		// Attempts to initialize the device. If this fails, will throw an exception detailing
		// the problem.
		void InitializeDevice();
		// Attempts to reset the device. Returns true if successful, and false if the device can't
		// yet be reset. If an error occurs, will throw a D3DError with the error code and a
		// description of the problem.
		bool ResetDevice();

		// Creates a basic viewport for the device. Uses the width and height supplied to the
		// constructor, with Z ranging from 0.0 to 1.0.
		void CreateViewport();

		// NOT IMPLEMENTED.
		D3DRendererBase(const D3DRendererBase&);
		const D3DRendererBase& operator=(const D3DRendererBase&);
	};




	

	// This is the basic exception type for renderers. All rendering-related exceptions
	// will be derived from this.
	class RendererException: public utility::Exception
	{
	public:
		// Takes a description of the problem. Should include a way of identifying where
		// the problem occurred (i.e. the full function name).
		RendererException(const std::string& description);
		RendererException(const RendererException& original);
		~RendererException();

	private:
		// NOT IMPLEMENTED.
		const RendererException& operator=(const RendererException& rhs);
	};




	
	
	
	
	
	// This exception type is thrown when one of the Direct3D functions fails. It
	// includes 
	class D3DError: public RendererException
	{
	public:
		// Constructors:
		// Takes a description of the error (which is suggested to contain the full
		// function name and what was being attempted when the error occured) and the
		// actual HRESULT error code returned by the failing function.
		D3DError(const std::string& description, const HRESULT& initial_error_code);
		~D3DError();
		// Copy constructor so that this may be caught by value.
		D3DError(const D3DError& original);

		// Returns the error code.
		const HRESULT& GetErrorCode() const;

	private:
		// The error code.
		const HRESULT error_code;

		// NOT IMPLEMENTED
		const D3DError& operator=(const D3DError&);
	};

} //avl
} //view
#endif // __AVL_VIEW_D3D_RENDERER_BASE__