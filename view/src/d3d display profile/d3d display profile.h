#ifndef __AVL_VIEW_D3D_DISPLAY_PROFILE__
#define __AVL_VIEW_D3D_DISPLAY_PROFILE__
/**********
 * Author: Sheldon Bachstein
 * Date: Jul 07, 2011
 * Description: The D3DDisplayProfile class is a structure designed to store D3D Device display format information; this
 * includes: width/height (resolution), backbuffer format, and depth/stencil buffer format. This structure does not take into
 * account the adapter, device type, or multisampling type; instead, these are assumed to be (respectively): D3DADAPTER_DEFAULT,
 * D3DDEVTYPE_HAL, D3DMULTISAMPLE_NONE. This class is intended to be used internally to enumerate the possible different display
 * formats, and then given to the user so that they may choose an appropriate display format.
 **********/



#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>



namespace avl
{
namespace view
{
	// See the beginning of the file for details.
	class D3DDisplayProfile
	{
	public:
		// Constructor. Requires a width, height, fullscreen boolean, back buffer format, and a
		// depth/stencil format.
		D3DDisplayProfile(const bool fullscreen, const unsigned int initial_width, const unsigned int initial_height,
									const D3DFORMAT initial_display_format, const D3DFORMAT initial_backbuffer_format);
		// Copy constructor.
		D3DDisplayProfile(const D3DDisplayProfile& original);
		// Basic destructor.
		~D3DDisplayProfile();

		// Accessors:
		// Returns true if the profile is fullscreen and false if it's windowed.
		const bool IsFullscreen() const;
		// Returns the width.
		const unsigned int GetWidth() const;
		// Returns the height.
		const unsigned int GetHeight() const;
		// Returns the backbuffer format.
		// Returns the display format.
		const D3DFORMAT GetDisplayFormat() const;
		const D3DFORMAT GetBackbufferFormat() const;

		// Assignment operator. This is required to allow for storing these in a vector.
		D3DDisplayProfile& operator=(const D3DDisplayProfile& rhs);
		// Equality operator. Equality is based on the fullscreen mode, width, height, display
		// format, and backbuffer format being the same.
		bool operator==(const D3DDisplayProfile& rhs) const;


		// Contains all of the backbuffer formats that may be used.
		static const D3DFORMAT BACKBUFFER_FORMATS[];
		// The length of the backbuffer format array.
		static const unsigned int BACKBUFFER_FORMATS_LENGTH;
		// Contains all of the display formats that may be used.
		static const D3DFORMAT DISPLAY_FORMATS[];
		// The length of the display format array.
		static const unsigned int DISPLAY_FORMATS_LENGTH;


	private:
		// Is this profile windowed or fullscreen?
		bool is_fullscreen;
		// Width and height.
		unsigned int width;
		unsigned int height;
		// The display format.
		D3DFORMAT display_format;
		// Backbuffer format.
		D3DFORMAT backbuffer_format;

	};

} //avl
} //view
#endif // __AVL_VIEW_D3D_DISPLAY_PROFILE__