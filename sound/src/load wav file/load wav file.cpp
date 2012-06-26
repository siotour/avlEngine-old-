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
Implementation for the load wav file component. See "load wav file.h" for details.
@author Sheldon Bachstein
@date Jun 23, 2012
*/

#include"load wav file.h"
#include"..\..\..\utility\src\file operations\file operations.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\assert\assert.h"
#include<vector>
#include<string>
#include<cstdint>

namespace avl
{
namespace sound
{
	// See method definitions for details.
	namespace
	{
		const std::string RIFF_ID = "RIFF";
		const std::string WAVE_ID = "WAVE";
		const std::string FMT_ID = "fmt ";
		const std::string DATA_ID = "data";
		const unsigned short WAVE_PCM = 1;

		struct RIFFChunk;

		const bool FindChunk(const std::vector<char>& data, RIFFChunk& chunk);

		struct RIFFChunk
		{
			std::string id;
			unsigned long size;
			std::size_t offset;
		};
	}


	// See function declaration for details.
	SoundSample LoadWAVFile(const std::string& file_name)
	{
		std::vector<char> file_data;
		utility::LoadFile(file_name, file_data);

		RIFFChunk chunk;
		chunk.offset = 0;
		chunk.size = 0;

		chunk.id = RIFF_ID;
		if(FindChunk(file_data, chunk) == false)
		{
			// throw utility::FileFormatException();
			throw utility::Exception("avl::sound::file::LoadWAVFile() -- File isn't a valid RIFF file.");
		}

		chunk.offset += 8;
		std::string file_type;
		file_type.append(&file_data[chunk.offset], 4);

		if(file_type != WAVE_ID)
		{
			// throw utility::FileFormatException();
			throw utility::Exception("avl::sound::file::LoadWAVFile() -- File isn't a WAVE file.");
		}

		chunk.offset += 4;
		chunk.id = FMT_ID;
		if(FindChunk(file_data, chunk) == false)
		{
			// throw utility::FileFormatException();
			throw utility::Exception("avl::sound::file::LoadWAVFile() -- File isn't a valid WAVE file.");
		}

		
		unsigned short wave_format = 0;

		chunk.offset += 8;
		memcpy(&wave_format, &file_data[chunk.offset], 2);

		if(wave_format != WAVE_PCM)
		{
			// throw utility::FileFormatException();
			throw utility::Exception("avl::sound::file::LoadWAVFile() -- File isn't in WAVE PCM format.");
		}

		

		unsigned short number_of_channels = 0;
		unsigned int frequency = 0;
		unsigned int bytes_per_sec = 0;
		unsigned short block_alignment = 0;
		unsigned short bit_depth = 0;
		
		
		chunk.offset += 2;
		memcpy(&number_of_channels, &file_data[chunk.offset], 2);

		chunk.offset += 2;
		memcpy(&frequency, &file_data[chunk.offset], 4);

		chunk.offset += 4;
		memcpy(&bytes_per_sec, &file_data[chunk.offset], 4);

		chunk.offset += 4;
		memcpy(&block_alignment, &file_data[chunk.offset], 2);

		chunk.offset += 2;
		memcpy(&bit_depth, &file_data[chunk.offset], 2);


		chunk.offset += 2;
		chunk.id = DATA_ID;
		if(FindChunk(file_data, chunk) == false)
		{
			// throw utility::FileFormatException();
			throw utility::Exception("avl::sound::file::LoadWAVFile() -- File isn't a valid WAVE file.");
		}

		// Check that the format information and size match up.

		char* audio_data = new char[chunk.size];

		if(audio_data == NULL)
		{
			throw utility::OutOfMemoryError();
		}

		memcpy(audio_data, &file_data[chunk.offset + 8], chunk.size);
		
		return SoundSample(bit_depth, frequency, number_of_channels, chunk.size, audio_data);
	}



	// Anonymous namespace.
	namespace
	{
		/**
		*/
		const bool FindChunk(const std::vector<char>& data, RIFFChunk& chunk)
		{
			ASSERT(chunk.offset <= data.size());
			std::size_t search_offset = chunk.offset;
			std::string search_id;
			unsigned long chunk_size = 0;

			while(search_offset < data.size() + 8)
			{
				search_id.append(&data[search_offset], 4);
				if(search_id == chunk.id)
				{
					chunk.size = 0;
					memcpy(&chunk.size, &data[search_offset + 4], 4);
					chunk.offset = search_offset;

					if(chunk.offset + chunk.size + 4 >= data.size())
					{
						break;
					}
					return true;
				}
				else
				{
					search_id.clear();
					search_offset += 4;
					memcpy(&chunk_size, &data[search_offset], 4);
					search_offset +=4;

					if(search_offset + chunk_size >= data.size())
					{
						break;
					}
					search_offset += chunk_size;
				}
			}
			chunk.id.clear();
			chunk.size = 0;
			chunk.offset = data.size() - 1;
			return false;
		}

	}


} // sound
} // avl
