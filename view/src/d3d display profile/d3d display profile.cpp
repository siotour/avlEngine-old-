/**********
 * Author: Sheldon Bachstein
 * Date: Jul 07, 2011
 * Description: See d3d display profile.h for details.
 **********/

#include"d3d display profile.h"
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>




namespace avl
{
namespace view
{
	// Contains all of the backbuffer formats that may be used.
	const D3DFORMAT D3DDisplayProfile::BACKBUFFER_FORMATS[] = {
						    D3DFMT_A1R5G5B5,
							D3DFMT_A2R10G10B10,
							D3DFMT_A8R8G8B8,
							D3DFMT_R5G6B5,
							D3DFMT_X1R5G5B5,
							D3DFMT_X8R8G8B8 };


	
	// The length of the backbuffer formats array.
	const unsigned int D3DDisplayProfile::BACKBUFFER_FORMATS_LENGTH = 6;



	// Contains all of the display formats that may be used.
	const D3DFORMAT D3DDisplayProfile::DISPLAY_FORMATS[] = {
							D3DFMT_A2R10G10B10,
							D3DFMT_R5G6B5,
							D3DFMT_X1R5G5B5,
							D3DFMT_X8R8G8B8 };


	
	// The length of the display formats array.
	const unsigned int D3DDisplayProfile::DISPLAY_FORMATS_LENGTH = 4;




	// Constructor. Requires a width, height, fullscreen boolean, back buffer format, and a
	// depth/stencil format.
	D3DDisplayProfile::D3DDisplayProfile(const bool fullscreen, const unsigned int initial_width, const unsigned int initial_height,
										 const D3DFORMAT initial_display_format, const D3DFORMAT initial_backbuffer_format)
		: is_fullscreen(fullscreen), width(initial_width), height(initial_height), display_format(initial_display_format), backbuffer_format(initial_backbuffer_format)
	{
	}




	// Copy constructor.
	D3DDisplayProfile::D3DDisplayProfile(const D3DDisplayProfile& original)
		: is_fullscreen(original.IsFullscreen()), width(original.GetWidth()), height(original.GetHeight()), display_format(original.GetDisplayFormat()), backbuffer_format(original.GetBackbufferFormat())
	{
	}





	// Basic destructor.
	D3DDisplayProfile::~D3DDisplayProfile()
	{
	}




	// Returns true if the profile is fullscreen and false if it's windowed.
	const bool D3DDisplayProfile::IsFullscreen() const
	{
		return is_fullscreen;
	}




	// Returns the width.
	const unsigned int D3DDisplayProfile::GetWidth() const
	{
		return width;
	}




	// Returns the height.
	const unsigned int D3DDisplayProfile::GetHeight() const
	{
		return height;
	}




	// Returns the display format.
	const D3DFORMAT D3DDisplayProfile::GetDisplayFormat() const
	{
		return display_format;
	}




	// Returns the backbuffer format.
	const D3DFORMAT D3DDisplayProfile::GetBackbufferFormat() const
	{
		return backbuffer_format;
	}




	// Assignment operator; required to store these objects in a vector.
	D3DDisplayProfile& D3DDisplayProfile::operator=(const D3DDisplayProfile& rhs)
	{
		is_fullscreen = rhs.IsFullscreen();
		width = rhs.GetWidth();
		height = rhs.GetHeight();
		display_format = rhs.GetDisplayFormat();
		backbuffer_format = rhs.GetBackbufferFormat();

		return *this;
	}




	// Equality operator. Equality is based on the fullscreen mode, width, height, backbuffer format, and
	// the depth/stencil buffer format being the same.
	bool D3DDisplayProfile::operator==(const D3DDisplayProfile& rhs) const
	{
		return is_fullscreen == rhs.IsFullscreen() && width == rhs.GetWidth() && height == rhs.GetHeight() && display_format == rhs.GetDisplayFormat() && backbuffer_format == rhs.GetBackbufferFormat();
	}



}
}