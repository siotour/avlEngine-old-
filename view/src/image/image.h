#pragma once
#ifndef AVL_VIEW_IMAGE__
#define AVL_VIEW_IMAGE__
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
Used for loading graphical image data, especially from a file.
@par Currently supported file formats:
@li *.TGA
@attention Currently only supports \b 24-bit (non-alpha) and \b 32-bit (alpha) images.
@author Sheldon Bachstein
@date Jul 24, 2011
@todo The file-loading capabilities of this component need to be refactored into a
separate component.
*/


#include<string>


namespace avl
{
namespace view
{
	/** Attempts to load image data from a file in the .TGA image file format.
	@pre \a file_name is the name of a color image file (not color-mapped) in .TGA format.
	@post If true is returned, then \a width, \a height, \a pixel_depth, \a contains_alpha, and \a pixel_data will
	contain information about the file \a file_name; you are responsible for deleting \a pixel_data when you no longer
	need the image data.\n If false is returned, then there was a problem when reading
	from the file \a file_name, the file doesn't exist, or the file was improperly formatted; if this is the case,
	then the only gaurantees are that \a file_name is unchanged and \a pixel_data does not need to be deleted.
	@param file_name [IN] Name of the TGA file to load the image data from.
	@param width [OUT] Width of the image.
	@param height [OUT] Height of the image.
	@param pixel_depth [OUT] Number of bytes per pixel.
	@param contains_alpha_channel [OUT]Whether or not the image has an alpha channel.
	@param pixel_data [OUT] Pointer to the image's pixel data.
	@todo This function definition is absolutely monolothic -- nearly 300 lines. It needs to be refactored into utility
	functions and probably all of them put into an anonymous namespace. Either that or put all image-loading functions
	(and utility functions) into their own namespace.
	*/
	bool LoadImageTGA(const std::string& file_name, unsigned int& width, unsigned int& height,
							unsigned short& pixel_depth, bool& contains_alpha_channel, unsigned char*& pixel_data);



	/**
	Contains the data necessary to display a graphical image.
	*/
	class Image
	{
	public:
		/** Full-spec constructor.
		@param width The width of the image in pixel.
		@param height The height of the image in pixels.
		@param pixel_depth The image's pixel depth in bytes.
		@param alpha Does the image contain an alpha channel?
		@param pixel_data Pointer to the image's pixel data.
		*/
		Image(const unsigned int& width, const unsigned int& height,
				const unsigned short& pixel_depth, const bool& alpha, unsigned char* pixel_data);
		/** Attempts to load an image given only a file name. \a file_name must have the extension of a currently
		implemented image file format, and the data contained in the file must match that file format (i.e. you
		will run into problems if you simply rename a .PNG file to .TGA and then try loading it).
		@post If an error occurs while reading from the file, if the file's formatting is not supported, or if an
		unsupported file extension is supplied, the created Image will have 0 \ref width, \ref height, \ref pixel_depth, and
		\ref pixel_data pointer will be \c NULL.\n
		See the image-loading functions for each individual image file format to
		see what additional restrictions are imposed for each file format.
		*/
		Image(const std::string& file_name);
		/** Basic destructor. Deletes \ref pixel_data.*/
		~Image();

		/** Returns the image's width.
		@return The value of \ref width.
		*/
		const unsigned int GetWidth() const;
		/** Returns the image's height.
		@return The value of \ref height.
		*/
		const unsigned int GetHeight() const;
		/** Returns the image's pixel depth in bytes.
		@return The value of \ref pixel_depth.
		*/
		const unsigned short GetPixelDepth() const;
		/** Does this image have an alpha channel?
		@return True if the image has an alpha channel, and false if not.
		*/
		const bool ContainsAlphaChannel() const;
		/** Gets the image's pixel data. Allows for modification of the pixel data.
		@return \ref pixel_data.
		@attention Pixel data is arranged in little-endian order.
		*/
		unsigned char* const GetPixelData() const;

	private:
		/// The image's width in pixels.
		unsigned int width;
		/// The image's height in pixels.
		unsigned int height;
		/// The image's pixel depth in bytes.
		unsigned short pixel_depth;
		/// True if the image contains an alpha channel.
		bool contains_alpha_channel;
		/// The image's pixel data.
		unsigned char* pixel_data;
	};



} // view
} // avl
#endif // AVL_VIEW_IMAGE__