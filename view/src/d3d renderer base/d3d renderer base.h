#ifndef __AVL_VIEW_D3D_RENDERER_BASE__
#define __AVL_VIEW_D3D_RENDERER_BASE__
/**
@file
Defines the \ref avl::view::D3DRendererBase class.
@author Sheldon Bachstein
@date Dec 25, 2010
@todo Everything in this file needs to be refactored into smaller, more manageable chunks.
@todo Should this class inherit from Renderer, or should D3D renderers just contain one?
*/

#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\renderer\renderer.h"
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
namespace view
{
	// Forward declaration.
	class D3DDisplayProfile;

	/** Used to return a set of \ref avl::view::D3DDisplayProfile objects.*/
	typedef const std::vector<const D3DDisplayProfile> DisplayProfiles;



	/** Attempts to find the closest fitting display profile matching the parameters.
	@post If a match is found, then it will be fullscreen if and only if \a fullscreen
	is true.
	@param fullscreen Do you want a fullscreen or windowed profile?
	@param width The desired width.
	@param height The desired height.
	@throws RendererException If unable to find an appropriate display profile.
	@throws D3DError If enumeration of a display mode fails.
	*/
	const D3DDisplayProfile LeastSquaredDisplayProfile(const bool fullscreen, const int width, const int height);


	/** This function attempts to enumerate the default display adapter, using the HAL
	device, for all legal display profiles with multisampling disabled.
	@return A vector containing all legal display profiles.
	@throws RendererException If unable to create the D3D object.
	@throws D3DError If enumeration of a display mode fails.
	@sa avl::view::D3DDisplayProfile
	@todo Refactor this function into multiple anonymous functions.
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
	@return True if the HAL device supports the required functionality, and false if not.
	@throws D3DError If unable to create the D3D object.
	@todo This function needs to be refactored.
	*/
	bool CheckDeviceCapabilities(IDirect3D9* const d3d);



	/**
	Provides a partially implemented interface for a basic Direct3D renderer.
	Implements the initialization of a Direct3D device and is meant to be used as a skeleton
	on which to build customized Direct3D renderers. Additionally implements various
	wrapper functions over Direct3D, such as for creating vertex buffers.\p
	Implemented behavior is for a 32-bit pixel format, but this can be customized by
	overloading the \ref avl::view::D3DRendererBase::InitializeDevice function. As long as
	\ref avl::view::D3DRendererBase::present_parameters is assigned the proper format, then
	\ref avl::view::D3DRendererBase::Reset should work fine.
	*/
	class D3DRendererBase: public Renderer
	{
	public:
		/** Attempts to initialize a Direct3D device for the window with handle \a window_handle
		using the parameters detailed in \a profile.
		@param window_handle The handle of the window to which the device is initialized.
		@param profile Details the parameters of the device.
		@throws RendererException If unable to create the D3D object.
		@throws D3DError If unable to create the Direct3D device.
		@throws D3DError If unable to set the viewport.
		*/
		D3DRendererBase(HWND window_handle, const D3DDisplayProfile& profile);

		/** Releases the D3D object and the Direct3D device, and sets the renderer to a
		not-ready state.
		*/
		virtual ~D3DRendererBase();


		/** Attempts to reset the device. Should be called when the device is lost.
		@note This function makes calls to the pure-virtual functions
		\ref ReleaseUnmanagedAssets and \ref AcquireUnmanagedAssets.
		@return True if the device is successfully reset, and false if not.
		@throws D3DError If an error occurs while resetting the device.
		@throws D3DError If an internal Direct3D error occurs.
		@todo This function needs to be cleaned up and probably refactored.
		*/
		bool Reset();

	protected:
		/** Attempts to clear the backbuffer to black and the z-buffer to 1.0f.
		@throws D3DError If unable to clear the viewport.
		*/
		void ClearViewport();

		/** This function will be called when the device is lost.
		@attention Derived classes should implement this method such that it release all non-managed Direct3D
		assets. These should then be re-acquired in the \ref AcquireUnmanagedAssets method.
		*/
		virtual void ReleaseUnmanagedAssets() = 0;

		/** This function will be called when the device is reset after having been lost.
		@attention Derived classes should implement this method such that it re-acquries any non-managed
		Direct3D assets that were released in the \ref ReleaseUnmanagedAssets method.
		*/
		virtual void AcquireUnmanagedAssets() = 0;



		// Wrapper functions around common Direct3D functionality:
		//
		//


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
		@throws D3DError If \a pixel_data is \c NULL.
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
		@throws D3DError If \a pixel_data is \c NULL.
		@throws D3DError If unable to lock or unlock \a destination.
		*/
		void CopyPixelDataToSurface(IDirect3DSurface9& destination, const unsigned char* const pixel_data,
										const unsigned int& width, const unsigned int& height, const unsigned int& bytes_per_pixel);
		
		/** Checks to see if the device supports textures in the specified format.
		@param format The texture format to check whether or not the device supports.
		@return True if the device supports textures of the format \a format, and false if not.
		*/
		bool IsTextureFormatOk(D3DFORMAT& format);
		
		/** Attempts to create a texture with the specified width, height, and format.
		Textures are created in the \c MANAGED Direct3D pool and with 1 level.
		@note You can check to see if the device supports the creation of textures of a format by using
		\ref avl::view::D3DRendererBase::IsTextureFormatOk().
		@sa avl::view::D3DRendererBase::IsTextureFormatOk()
		@throws D3DError If the creation of the texture fails.
		*/
		IDirect3DTexture9* CreateTexture(const unsigned int& width, const unsigned int& height, D3DFORMAT format);
		
		/** Attempts to create a vertex buffer capable of storing \a buffer_length vertices. The expected vertex format
		consists of spatial coordinates (XYZ) and one set of texture coordinates. The created buffer is dynamic and
		write-only and in the default pool.
		@return The newly created vertex buffer.
		@throws D3DError If unable to create the buffer.
		*/
		IDirect3DVertexBuffer9* CreateVertexBuffer(const unsigned int& buffer_length);
		
		/** Attempts to create an index buffer capable of storing \a buffer_length 16-bit indices.
		The created buffer is dynamic and write-only and in the default pool.
		@return The newly created index buffer.
		@throws D3DError If unable to create the index buffer.
		*/
		IDirect3DIndexBuffer9* CreateIndexBuffer(const unsigned int& buffer_length);
		
		/** Attempts to lock \a vertex_buffer and then copy \a size bytes from \a source to
		\a vertex_buffer.
		@param vertex_buffer The destination vertex buffer.
		@param source The source of data to be copied to \a vertex_buffer.
		@param size The number of bytes to copy from \a source to \a vertex_buffer.
		@throws RendererException If \a vertex_buffer can't hold \a size bytes.
		@throws D3DError If unable to lock \a vertex_buffer.
		*/
		void FillVertexBuffer(IDirect3DVertexBuffer9& vertex_buffer, const unsigned char* const source, const unsigned int size);
		
		/** Attempts to lock \a index_buffer and then copy \a size bytes from \a source to
		\a vertex_buffer.
		@param index_buffer The destination index buffer.
		@param source The source of data to be copied to \a index_buffer.
		@param size The number of bytes to copy from \a source to \a index_buffer.
		@throws RendererException If \a index_buffer can't hold \a size bytes.
		@throws D3DError If unable to lock \a index_buffer.
		*/
		void FillIndexBuffer(IDirect3DIndexBuffer9& index_buffer, const unsigned char* const source, const unsigned int size);




		/// The present paramters for the device.
		D3DPRESENT_PARAMETERS present_parameters;

		/// The width of the backbuffer and viewport in pixels.
		const int width;
		/// The height of the backbuffer and viewport in pixels.
		const int height;
		/// Is the window for this device fullscreen or not?
		const bool fullscreen;
		/// The format of the direct3d device.
		const D3DFORMAT display_format;
		/// The format of the backbuffer.
		const D3DFORMAT backbuffer_format;
		/// A handle to the window which is being rendered to.
		const HWND window_handle;

		/// The Direct3D8 object.
		IDirect3D9* d3d;

		/// The Direct3D device.
		IDirect3DDevice9* device;

		/// Indicates whether or not the device is ready for rendering.
		bool is_device_ready;




	private:
		/** Attempts to create the Direct3D device. Uses a 16-bit depth/stencil buffer, and allows
		Direct3D to maintain it.
		@throws D3DError If unable to create the device.
		*/
		void InitializeDevice();

		
		/** Attmpts to reset the Direct3D9 device.
		@return True if the device was successfully reset, and false if unable to reset the device.
		@throws D3DError If an error occurs while resetting the device.
		*/
		bool PerformDeviceReset();


		/** Creates a basic viewport for the device. Uses the width and height supplied to the
		constructor, with Z ranging from 0.0 to 1.0.
		@throws D3DError If unable to set the viewport.
		*/
		void CreateViewport();

		/// NOT IMPLEMENTED.
		D3DRendererBase(const D3DRendererBase&);
		/// NOT IMPLEMENTED.
		const D3DRendererBase& operator=(const D3DRendererBase&);
	};




	

	
	
	
	
	/** This exception type is thrown when one of the Direct3D functions fail. In addition
	to a description of the problem, it records the error code returned from the function.
	*/
	class D3DError: public RendererException
	{
	public:
		/** Full-spec constructor.
		@param d3d_function The name of the Direct3D function which failed.
		@param explanation A description of the problem which has occurred.
		@param error_code The actual error code returned from calling \a d3d_function.
		function.
		*/
		D3DError(const std::string& d3d_function, const std::string& explanation, const HRESULT& error_code);
		/** Basic destructor.*/
		~D3DError();
		/** Copy constructor.
		@param original The object being copied.
		*/
		D3DError(const D3DError& original);

		/** Returns the failed error code.
		@return The error code returned from the problematic Direct3D function.
		*/
		const HRESULT& GetErrorCode() const;

	private:
		/// The error code.
		const HRESULT error_code;

		/// NOT IMPLEMENTED.
		const D3DError& operator=(const D3DError&);
	};

} //avl
} //view
#endif // __AVL_VIEW_D3D_RENDERER_BASE__