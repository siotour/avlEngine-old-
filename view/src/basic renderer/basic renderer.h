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
#include<vector>
#include<queue>
#include<d3d9.h>

namespace avl
{
// Forward declaration.
namespace utility{class TexturedQuad;}

namespace view
{
	// See the beginning of the file for details.
	class BasicRenderer: public D3DRendererBase
	{
	public:
		// Vector of pointers to TexturedQuads. Each set of TexturedQuads with a common texture should be
		// grouped into one of these.
		typedef std::vector<const utility::TexturedQuad* const> VectorOfTexQuads;
		// VectorOfTexQuads should be grouped with their corresponding texture IDs in these.
		typedef std::map<unsigned int, VectorOfTexQuads> TexIDToVectorOfTexQuads;



		// Constructors:
		// Attempts to initialize a Direct3D device for the specified window and with the specified
		// display profile.
		BasicRenderer(HWND window_handle, const D3DDisplayProfile& profile);
		// Destroys the Direct3D device and releases all assets.
		~BasicRenderer();

		// Mutators:
		// Attempts to create a texture with the specified data. Returns the texture handle, which can be used
		// to specify which texture to use when rendering primitives.
		const unsigned int AddTexture(const unsigned int& width, const unsigned int& height, const BYTE* const pixel_data);
		// Releases the texture associated with the specified handle and renders the handle useless. Don't try to
		// draw with textures that have been deleted or else <TODO>.
		void DeleteTexture(const unsigned int& texture_handle);
		// Renders a series of textured quads.
		void RenderTexturedQuads(TexIDToVectorOfTexQuads& quad_vectors);

	private:
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