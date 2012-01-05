#ifndef __AVL_VIEW_BASIC_RENDERER__
#define __AVL_VIEW_BASIC_RENDERER__
/**
@file
Defines the \ref avl::view::BasicRenderer class.
@author Sheldon Bachstein
@date Jan 13, 2011
@todo There's a memory leak while using this class. Enable maximum debugging in the DirectX9
control panel for details.
*/

#include"..\d3d renderer base\d3d renderer base.h"
// Necessary since the typedef utility::Sprite::TextureHandle is used.
#include"..\..\..\utility\src\sprite\sprite.h"
#include<map>
#include<utility>
#include<d3d9.h>

namespace avl
{
// Forward declaration.
namespace utility{class Image;}

namespace view
{
	/**
	Builds on the functionality of \ref avl::view::D3DRendererBase to implement a basic
	2-D renderer capable of rendering basic textured rectangles.
	@todo The methods for this class desperately need to be refactored into smaller,
	cleaner chunks.
	@todo It seems very sloppy that sprites are rendered using a non-const container of
	non-const pointers to sprites... Modify this interface (and the rendering code) to
	instead use a const reference to a container of pointers to const sprites.
	*/
	class BasicRenderer: public D3DRendererBase
	{
	public:
		/** Attempts to initialize a Direct3D device to render to the window represented by
		\a window_handle using the display profile \a profile.
		@param window_handle The handle to the window to render to.
		@param profile The display profile to initialize the device with.
		@throws RendererException If unable to create the D3D object.
		@throws D3DError If unable to create the Direct3D device.
		@throws D3DError If unable to set the viewport.
		*/
		BasicRenderer(HWND window_handle, const D3DDisplayProfile& profile);
		/** Destroys the Direct3D device and releases all assets.
		*/
		~BasicRenderer();

		
		/** Attempts to create a texture for \a image.
		@sa avl::view::BasicRenderer::DeleteTexture
		@sa avl::view::BasicRenderer::RenderSprites
		@param image The image data used to create the texture.
		@return A handle to the created texture.
		@throws D3DError If unable to create the texture.
		*/
		const utility::Sprite::TextureHandle AddTexture(const utility::Image& image);

		/** Releases the texture associated with the texture handle \a texture_handle.
		If \a texture_handle is not associated with a texture, then nothing happens.
		@warning Don't try to render sprites using a deleted texture handle.
		@param texture_handle The handle to the texture to be deleted.
		*/
		void DeleteTexture(const utility::Sprite::TextureHandle& texture_handle);

		/** Renders a series of sprites.
		@attention \a sprites may very well be modified during this operation (though
		no actual sprites will be modified). If you need a copy of \a sprites after
		the call to this method, then make that copy before this method call.
		@post \a sprites may well not contain the same (or any) sprites. Make sure to
		keep your own separate copy if you need \a sprites for something else.
		@param sprites The sprites to be rendered.
		@throws RendererException If \a sprites contains one or more NULL pointers.
		@throws D3DError If there is an error while using the device.
		*/
		void RenderSprites(utility::Sprite::SpriteList& sprites);

	private:
		/** Checks to see whether or not an image contains semi-transparent pixels. Semi-transparent pixels
		have an alpha value that is neither the minimum nor the maximum.
		@pre The pixels are in 32-bit RGBA format.
		@param size The number of pixels in \a pixel_data.
		@param pixel_data The pixel data for the image.
		@returns True if \a pixel_data contains semi-transparent pixels, and false if not.
		*/
		bool IsImagePartiallyTransparent(const unsigned int size, const BYTE* const pixel_data);


		/** Releases the index buffer and vertex buffer. This is called when the device is lost.
		*/
		void ReleaseUnmanagedAssets();

		/** Creates the index buffer and vertex buffer. This is called upon initialization and when the
		device is reset after having been lost.
		@throws D3DError If unable to create either the vertex or index buffers.
		*/
		void AcquireUnmanagedAssets();


		/// Used to store the type of vertex format used for sprites.
		const unsigned int vertex_format;

		/// Used to store the number of bytes per pixel.
		const unsigned int bytes_per_pixel;

		/// Maps texture handles to texture objects.
		typedef std::map<const utility::Sprite::TextureHandle, IDirect3DTexture9*> TexHandleToTex;

		/// Maintains a map of valid texture handles and their associated textures.
		TexHandleToTex textures;

		/// Used to assign a unique texture handle to each created texture. This imposes the restriction that it
		///is highly inadvisable to attempt to load more than UINT_MAX/2 (defined in limits.h) textures throughout a program.
		unsigned int next_texture_handle;

		
		/// The size of the vertex buffer and index buffer, in terms of the objects being stored in them.
		const unsigned int buffer_length;

		/// The vertex buffer.
		IDirect3DVertexBuffer9* vertex_buffer;

		/// The index buffer.
		IDirect3DIndexBuffer9* index_buffer;


		/// NOT IMPLEMENTED.
		BasicRenderer(const BasicRenderer&);
		/// NOT IMPLEMENTED.
		const BasicRenderer& operator=(const BasicRenderer&);
	};



} //avl
} //view
#endif // __AVL_VIEW_BASIC_RENDERER__