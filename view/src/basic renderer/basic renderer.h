#ifndef __AVL_VIEW_BASIC_RENDERER__
#define __AVL_VIEW_BASIC_RENDERER__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 13, 2011
 * Description: Builds on the functionality of D3DRendererBase to implement a basic 2-D renderer
 * capable of rendering basic textured rectangles.
 * TODO: Implement batch rendering using vertex and index buffers.
 **********/

#include"..\d3d renderer base\d3d renderer base.h"
#include<map>
#include<list>
#include<utility>
#include<d3d9.h>

namespace avl
{
// Forward declaration.
namespace utility{class Sprite; class Image;}

namespace view
{
	// See the beginning of the file for details.
	class BasicRenderer: public D3DRendererBase
	{
	public:
		// A utility::Sprite and its corresponding TextureID.
		typedef std::pair<const utility::Sprite* const, const unsigned int> SpriteAndTexture;
		// A list of SpriteAndTexture pairs. Rendering is done through these structures.
		typedef std::list<SpriteAndTexture> SpriteAndTextureList;


		// Constructors:
		// Attempts to initialize a Direct3D device for the specified window and with the specified
		// display profile.
		BasicRenderer(HWND window_handle, const D3DDisplayProfile& profile);
		// Destroys the Direct3D device and releases all assets.
		~BasicRenderer();

		// Mutators:
		// Attempts to create a texture with the specified data. Returns the texture handle, which can be used
		// to specify which texture to use when rendering primitives.
		const unsigned int AddTexture(const utility::Image& image);
		// Releases the texture associated with the specified handle and renders the handle useless. Don't try to
		// draw with textures that have been deleted or else <TODO>.
		void DeleteTexture(const unsigned int& texture_handle);
		// Renders a series of sprites.
		void RenderSprites(SpriteAndTextureList sprites_and_textures);

	private:
		// Utility functions:
		// Iterates through the pixel data for an image with an alpha channel, checking for semi-transparency.
		// If semi-transparency is found, true is returned. Otherwise false. The image data is assumed to be
		// 32-bits per pixel in RGBA format. size is the number of pixels in the image, and pixel_data is a
		// pointer to the pixel data.
		bool IsImagePartiallyTransparent(const unsigned int size, const BYTE* const pixel_data);
		// Initializes the Direct3D device's state to what is required for this object to render with it. This
		// includes creating the vertex and index buffers, turning lighting off, and configuring the device
		// to use a fixed function vertex shader.
		void Initialize();
		// Releases the index buffer and vertex buffer. This is called when the device is lost.
		void ReleaseUnmanagedAssets();
		// Creates the index buffer and vertex buffer. This is called upon initialization and when the
		// device is reset after having been lost.
		void AcquireUnmanagedAssets();


		// Used to store the type of vertex format used for textures.
		const unsigned int vertex_format;
		// Used to store the number of bytes per pixel.
		const unsigned int bytes_per_pixel;

		// Texture map.
		typedef std::map<const unsigned int, IDirect3DTexture9*> TexIDToTex;
		// Maintains a map of valid texture handles and their associated textures.
		TexIDToTex textures;

		// Used to assign a unique texture handle to each created texture. This imposes the restriction that it
		// is highly inadvisable to attempt to load more than UINT_MAX (defined in limits.h) textures throughout a program.
		unsigned int next_texture_handle;

		
		// The size of the vertex buffer and index buffer, in terms of the objects being stored in them.
		const unsigned int buffer_length;
		// Holds the vertex buffer.
		IDirect3DVertexBuffer9* vertex_buffer;
		// Holds the index buffer.
		IDirect3DIndexBuffer9* index_buffer;


		// NOT IMPLEMENTED:
		BasicRenderer(const BasicRenderer&);
		const BasicRenderer& operator=(const BasicRenderer&);
	};



} //avl
} //view
#endif // __AVL_VIEW_BASIC_RENDERER__