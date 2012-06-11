#pragma once
#ifndef AVL_VIEW_D3D_D3D_DISPLAY_PROFILE__
#define AVL_VIEW_D3D_D3D_DISPLAY_PROFILE__
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
Defines the \ref avl::view::d3d::D3DDisplayProfile class.
@author Sheldon Bachstein
@date Jul 07, 2011
*/



#include<vector>
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

	// Forward declaration.
	class D3DDisplayProfile;


	/** Used to return a set of \ref avl::view::d3d::D3DDisplayProfile objects.*/
	typedef const std::vector<const D3DDisplayProfile> DisplayProfiles;


	/**
	A structure designed to store D3D Device display format information. This includes:
	width/height (resolution), backbuffer format, and depth/stencil buffer format. This structure does not take into
	account the adapter, device type, or multisampling type; instead, these are assumed to be (respectively): \c D3DADAPTER_DEFAULT,
	\c D3DDEVTYPE_HAL, \c D3DMULTISAMPLE_NONE. This class is intended to be used internally to enumerate the possible different display
	formats, and then be given to the user so that they may choose an appropriate display format.
	@todo The public static arrays and the public static array lengths are very ugly. There must be a better way to do this.
	*/
	class D3DDisplayProfile
	{
	public:
		/**
		Full-spec constructor.
		@param fullscreen Is this profile fullscreen or windowed?
		@param width The width of the profile in pixels.
		@param height The height of the profile in pixels.
		@param display_format The pixel format of the profile display.
		@param backbuffer_format The pixel format of the profile backbuffer.
		*/
		D3DDisplayProfile(const bool fullscreen, const unsigned int width, const unsigned int height,
									const D3DFORMAT display_format, const D3DFORMAT backbuffer_format);
		/** Copy constructor.
		@param original The object being copied.
		*/
		D3DDisplayProfile(const D3DDisplayProfile& original);
		/** Basic destructor.*/
		~D3DDisplayProfile();


		/** Is this profile fullscreen?
		@return True if the the profile is fullscreen, and false if it's windowed.
		*/
		const bool IsFullscreen() const;

		/** Returns the width.
		@return The width of the profile in pixels.
		*/
		const unsigned int GetWidth() const;

		/** Returns the height.
		@return The height of the profile in pixels.
		*/
		const unsigned int GetHeight() const;

		/** Returns the display format.
		@return The format of the profile display.
		*/
		const D3DFORMAT GetDisplayFormat() const;

		/** Returns the backbuffer format.
		@return The format of the profile backbuffer.
		*/
		const D3DFORMAT GetBackbufferFormat() const;

		/** Makes this object into a copy of \a rhs.
		@note This is required to allow for storing these in a vector.
		@param rhs The object to which this one is being assigned.
		@return A reference to this object after the assignment takes place.
		*/
		D3DDisplayProfile& operator=(const D3DDisplayProfile& rhs);

		/** Compares this object to \a rhs for equality. Equality is based on the fullscreen
		mode, width, height, display format, and backbuffer format being the same.
		@param rhs The object against which this one is being compared.
		@return True if this object is equal to \a rhs, and false if not.
		*/
		bool operator==(const D3DDisplayProfile& rhs) const;


		/// Contains all of the backbuffer formats that may be used.
		static const D3DFORMAT BACKBUFFER_FORMATS[];
		/// The length of the backbuffer format array.
		static const unsigned int BACKBUFFER_FORMATS_LENGTH;
		/// Contains all of the display formats that may be used.
		static const D3DFORMAT DISPLAY_FORMATS[];
		/// The length of the display format array.
		static const unsigned int DISPLAY_FORMATS_LENGTH;


	private:
		/// Is this profile windowed or fullscreen?
		bool is_fullscreen;
		/// Width and height.
		unsigned int width;
		unsigned int height;
		/// The display format.
		D3DFORMAT display_format;
		/// Backbuffer format.
		D3DFORMAT backbuffer_format;

	};

} // d3d
} // avl
} // view
#endif // AVL_VIEW_D3D_D3D_DISPLAY_PROFILE__