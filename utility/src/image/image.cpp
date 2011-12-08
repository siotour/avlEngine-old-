/**********
 * Author: Sheldon Bachstein
 * Date: Jul 24, 2011
 * Description: See image.h for details.
 **********/

#include"image.h"
#include"..\exception\exception.h"
#include"..\assert\assert.h"
#include<memory>
#include<fstream>



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
							unsigned short& pixel_depth, bool& contains_alpha_channel, unsigned char*& pixel_data)
	{
		using namespace std;
		
		try
		{
			// Attempt to open the file specified. If this fails, return false.
			ifstream file(file_name.c_str(), ios_base::in | ios_base::binary);
			if(file.good() == false)
			{
				return false;
			}

			// Get the file size.
			file.seekg(0, ios_base::end);
			unsigned long file_size = (unsigned long)file.tellg();
			file.seekg(0, ios_base::beg);

			// If unable to allocate memory for file_data, return false.
			unsigned char* file_data = new unsigned char[file_size];
			if(file_data == NULL)
			{
				file.close();
				return false;
			}

			// Copy the file contents to memory for safer reading.
			file.read((char*)file_data, file_size);

			// Done with the file -- close it.
			file.close();



			//
			// Now load the header information:

			// If the image has a color map, return false.
			if(file_data[1] != 0)
			{
				return false;
			}

			// Store the file's encoding type.
			unsigned char encoding = file_data[2];
			// The only acceptable encodings are raw RGB and run length encoded RGB. Return false for
			// any other encoding.
			if(encoding != 2 && encoding != 10)
			{
				return false;
			}

			// Get the width and height.
			unsigned short x1;
			unsigned short x2;
			unsigned short y1;
			unsigned short y2;
			memcpy(&x1,&file_data[8],2);
			memcpy(&y1,&file_data[10],2);
			memcpy(&x2,&file_data[12],2);
			memcpy(&y2,&file_data[14],2);
 
			width = (x2 - x1);
			height = (y2 - y1);
		
			// If the width or height are less than 1, return false.
			if(width < 1 || height < 1)
			{
				return false;
			}

			// Retrieve the pixel depth.
			pixel_depth = file_data[16] / 8;

			// Calculate the size of the image data.
			unsigned long image_size = width * height * pixel_depth;

			// Check to see if the image is flipped either horizontall, vertically, or both.
			bool flipped_horizontally = false;
			bool flipped_vertically = false;
			if(file_data[17] & 16)
			{
				flipped_horizontally = true;
			}
			if(file_data[17] & 32)
			{
				flipped_vertically = true;
			}

			// If the image is interleaved, return false.
			if(file_data[17] > 32)
			{
				return false;
			}

			// If the pixel depth is 4 (32 bits), the image has an alpha channel.
			if(pixel_depth == 4)
			{
				contains_alpha_channel = true;
			}
			else
			{
				contains_alpha_channel = false;
			}

			// If there are any color map entries, return false.
			unsigned short color_map_entries;
			memcpy(&color_map_entries, &file_data[4], 2);
			if(color_map_entries != 0)
			{
				return false;
			}


			// Done with the header. Allocate memory for the pixel data.
			pixel_data = new unsigned char[image_size];
			if(pixel_data == NULL)
			{
				return false;
			}

			// Now copy the pixel data based on the encoding.
			unsigned short offset = file_data[0] + 18;
			switch(encoding)
			{
			case 2:
				// Raw RGB.
				memcpy(pixel_data, &file_data[offset], image_size);
				break;
			case 10:
				// RLE RGB.

				// Points to the current run-length chunk/pixel data. Start from the beginning of the image data.
				unsigned char* current;
				current = &file_data[offset];
				// Index in bytes to the beginning of the current run-length chunk/pixel data.
				unsigned long index;
				index = 0;
				// The run length of each encoding chunk
				unsigned char run_length;
				// Loop counter.
				unsigned char i;

				// Unpack the encoded pixel data.
				while(index < image_size)
				{
					// Is this section encoded?
					if(*current & 0x80)
					{
						// Get the run length.
						run_length = *current - 127;
						// Scoot up past the run-length chunk to the pixel data.
						++current;
						// For the length of this run-length, put that many copies of the current
						// pixel into pixel_data
						for(i = 0; i < run_length; ++i, index += pixel_depth)
						{
							memcpy(&pixel_data[index], current, pixel_depth);
						}
						// Scoot up to the next run-length chunk.
						current += pixel_depth;
					}
					// This section isn't encoded.
					else
					{
						// Figure out how many pixels are in this unencoded run.
						run_length = *current + 1;
						// Scoot up the pixel data.
						++current;
						// For each pixel in the unencoded run, copy it to the pixel data.
						for(i = 0; i < run_length; ++i, current += pixel_depth, index += pixel_depth)
						{
							memcpy(&pixel_data[index], current, pixel_depth);
						}
					}
				}
				break;
			default: // Return false if the encoding is unsupported.
				return false;
				break;
			}
		

		
			// Does the image need to be flipped horizontally?
			if(flipped_horizontally == true)
			{
				// Temporary storage.
				unsigned short upper_line = 1;
				unsigned short lower_line = height;
				unsigned char* temp = new unsigned char[width * pixel_depth];
				// If unable to get the memory, return false.
				if(temp == NULL)
				{
					delete[] pixel_data;
					delete[] file_data;
					return false;
				}
			
				// Flip the upper lines with their corresponding lower lines like this:
				//		-->================
				//		|  ================
				// Swap	|  ================
				//		|  ================
				//		-->================
				while(upper_line < lower_line)
				{
					// Copy the upper line to temp.
					memcpy(temp, &pixel_data[(upper_line - 1) * width * pixel_depth], width * pixel_depth);
					// Copy the lower line to the upper line.
					memcpy(&pixel_data[(upper_line - 1) * width * pixel_depth], &pixel_data[(lower_line - 1) * width * pixel_depth], width * pixel_depth);
					// Copy the old upper line to the lower line.
					memcpy(&pixel_data[(lower_line - 1) * width * pixel_depth], temp, width * pixel_depth);

					// Move the upper line down, and the lower line up.
					++upper_line;
					--lower_line;
				}
				// Clean up.
				delete[] temp;
			}
			// Does the image need to be flipped vertically?
			if(flipped_vertically == true)
			{
				// Temporary storage.
				unsigned short left_pixel;
				unsigned short right_pixel;
				unsigned char* temp = new unsigned char[pixel_depth];
				// If unable to get the memory, return false.
				if(temp == NULL)
				{
					delete[] pixel_data;
					delete[] file_data;
					return false;
				}

				// Swap right-most pixels with left-most pixels on each line, like this:
				//
				//		=================
				//		=================
				//		=================
				//		  ^           ^
				//		  |___________|
				//			  Swap
				for(unsigned short line = 0; line < height; ++line)
				{
					// Start from the outside, then move inwards.
					left_pixel = 0;
					right_pixel = width - 1;
					while(left_pixel < right_pixel)
					{
						// Copy the left pixel to temp.
						memcpy(temp, &pixel_data[(line * width + left_pixel) * pixel_depth], pixel_depth);
						// Copy the right pixel to left pixel.
						memcpy(&pixel_data[(line * width + left_pixel) * pixel_depth], &pixel_data[(line * width + right_pixel) * pixel_depth], pixel_depth);
						// Copy the old left pixel to right pixel.
						memcpy(&pixel_data[(line * width + right_pixel) * pixel_depth], temp, pixel_depth);
					
						// Move each pixel index inwards.
						++left_pixel;
						--right_pixel;
					}
				}
				// Clean up.
				delete[] temp;
			}

			// Done with the file data now.
			delete[] file_data;

			// Return success.
			return true;


			}
		// If an exception occurred while reading from the file, simply return false.
		catch(...)
		{
			return false;
		}
	}





	// Image class
	//
	//


	// Full-spec constructor: initial_width and initial_height are the resolution of the image,
	// initial_pixel_depth is the image's pixel depth in bytes, initial_alpha should be true if the image
	// contains an alpha channel and false otherwise, and initial_pixel_data should point to the image's
	// pixel data in little-endian ordering.
	Image::Image(const unsigned int& width, const unsigned int& height,
			const unsigned short& pixel_depth, const bool& alpha, unsigned char* pixel_data)
		: width(width), height(height), pixel_depth(pixel_depth), contains_alpha_channel(alpha), pixel_data(pixel_data)
	{
		ASSERT(width != 0 && height != 0 && pixel_depth != 0 && pixel_data != NULL);
	}




	// Attempts to load an image given only a file name. The filename must have the extension of a currently
	// implemented image file format, and the data contained in the file must match that file format (i.e. you
	// will run into problems if you simply rename a .PNG file to .TGA and then try loading it). If an error
	// occurs while reading from the file, if the file's formatting is not supported, or if an unsupported
	// file extension is supplied, the created Image will have 0 width, height, pixel depth, and the image
	// data pointer will be NULL. See the image-loading functions for each individual image file format to
	// see what additional restrictions are imposed for each file format.
	Image::Image(const std::string& file_name)
	{
		// Figure out the file extension by moving backwards from the end of the string to the first period.
		// Then store the remainder of the string from there in lowercase.
		std::string extension;
		std::string::const_iterator begin = file_name.begin();
		for(std::string::const_iterator i = --file_name.end(); i != begin; --i)
		{
			if(*i == '.')
			{
				extension.assign(++i, file_name.end());
				// Convert the extension to lowercase.
				for(std::string::iterator j = extension.begin(); j != extension.end(); ++j)
				{
					*j = tolower(*j);
				}
				break;
			}
		}

		// Stores the success status of loading the image.
		bool status;

		// If the extension is a valid format, use the appropriate loading function.
		if(extension == "tga")
		{
			status = LoadImageTGA(file_name, width, height, pixel_depth, contains_alpha_channel, pixel_data);
		}
		else
		{
			status = false;
		}

		// If unable to load the image, initialize all values to 0.
		if(status == false)
		{
			width = 0;
			height = 0;
			pixel_depth = 0;
			contains_alpha_channel = false;
			pixel_data = NULL;
		}
	}




	// Basic destructor. Deletes the image data from the heap.
	Image::~Image()
	{
		if(pixel_data != NULL)
		{
			delete[] pixel_data;
			pixel_data = NULL;
		}
	}




	// Returns the image's width.
	const unsigned int Image::GetWidth() const
	{
		return width;
	}




	// Returns the image's height.
	const unsigned int Image::GetHeight() const
	{
		return height;
	}




	// Returns the image's pixel depth in bytes.
	const unsigned short Image::GetPixelDepth() const
	{
		return pixel_depth;
	}




	// Returns true if the image contains an alpha channel, and false otherwise.
	const bool Image::ContainsAlphaChannel() const
	{
		return contains_alpha_channel;
	}




	// Returns a pointer to the image's pixel data. This allows for modification of the pixel data.
	// Note that the pixel data is arranged in little-endian order.
	unsigned char* const Image::GetPixelData() const
	{
		return pixel_data;
	}




}
}