#ifndef __AVL_UTILITY_IMAGE__
#define __AVL_UTILITY_IMAGE__
/**********
 * Author: Sheldon Bachstein
 * Date: Jul 24, 2011
 * Description: Defines a data structure representing an image, which includes the image data, width, height,
 * the presence of an alpha channel, and the pixel depth. Additionally implements functions for loading
 * various image file formats.
 * Note: Currently only supports 24-bit (non-alpha) and 32-bit (alpha) images.
 * Currently implemented file formats: .TGA
 **********/


#include<string>


namespace avl
{
namespace utility
{
	// This function attempts to load image data from a file in the .TGA image file format. If an error occurs while
	// reading from the file, if the file doesn't exist, or if the file isn't properly formatted, false is returned
	// and no guarantees are made about any of the parameters except that pixel_data won't point to a new chunk of
	// memory (in other words, you don't need to delete pixel_data if this function fails). If the function succeeds,
	// true will be returned and width, height, pixel_depth, contains_alpha_channel, and pixel_data will contain
	// the image's data. You are responsible for delete[]ing pixel_data when you're done with it.
	// Note that only color images without a color map are supported.
	// Parameters:
	//		[IN]	file_name - name of the TGA file to load the image data from.
	//		[OUT]	width - width of the image.
	//		[OUT]	height - height of the image.
	//		[OUT]	pixel_depth - number of bytes per pixel.
	//		[OUT]	contains_alpha_channel - whether or not the image has an alpha channel.
	//		[OUT]	pixel_data - pointer to the image's pixel data.
	bool LoadImageTGA(const std::string& file_name, unsigned int& width, unsigned int& height,
							unsigned short& pixel_depth, bool& contains_alpha_channel, unsigned char*& pixel_data);



	// See the beginning of the file for details.
	class Image
	{
	public:
		// Full-spec constructor: initial_width and initial_height are the resolution of the image,
		// initial_pixel_depth is the image's pixel depth in bytes, initial_alpha should be true if the image
		// contains an alpha channel and false otherwise, and initial_pixel_data should point to the image's
		// pixel data in little-endian ordering.
		Image(const unsigned int& initial_width, const unsigned int& initial_height,
				const unsigned short& initial_pixel_depth, const bool& initial_alpha, unsigned char* initial_pixel_data);
		// Attempts to load an image given only a file name. The filename must have the extension of a currently
		// implemented image file format, and the data contained in the file must match that file format (i.e. you
		// will run into problems if you simply rename a .PNG file to .TGA and then try loading it). If an error
		// occurs while reading from the file, if the file's formatting is not supported, or if an unsupported
		// file extension is supplied, the created Image will have 0 width, height, pixel depth, and the image
		// data pointer will be NULL. See the image-loading functions for each individual image file format to
		// see what additional restrictions are imposed for each file format.
		Image(const std::string& file_name);
		// Basic destructor. Deletes the image data from the heap.
		~Image();

		// Accessors:
		// Returns the image's width.
		const unsigned int GetWidth() const;
		// Returns the image's height.
		const unsigned int GetHeight() const;
		// Returns the image's pixel depth in bytes.
		const unsigned short GetPixelDepth() const;
		// Returns true if the image contains an alpha channel, and false otherwise.
		const bool ContainsAlphaChannel() const;
		// Returns a pointer to the image's pixel data. This allows for modification of the pixel data.
		// Note that the pixel data is arranged in little-endian order.
		unsigned char* const GetPixelData() const;

	private:
		// The image's width and height.
		unsigned int width;
		unsigned int height;
		// The image's pixel depth in bytes.
		unsigned short pixel_depth;
		// True if the image contains an alpha channel.
		bool contains_alpha_channel;
		// The image's pixel data.
		unsigned char* pixel_data;
	};



} //avl
} //utility
#endif // __AVL_UTILITY_IMAGE__