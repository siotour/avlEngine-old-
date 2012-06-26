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
Implementation for the file operations component. See "file operations.h" for details.
@author Sheldon Bachstein
@date Jun 22, 2012
*/

#include"file operations.h"
#include"..\exceptions\exceptions.h"
#include<string>
#include<vector>
#include<fstream>


namespace avl
{
namespace utility
{

	// See function declaration for details.
	const bool FileExists(const std::string& file_name)
	{
		std::ifstream file;
		try
		{
			// Don't throw any exceptions.
			file.exceptions(std::ios::goodbit);
			file.open(file_name);
			const bool open = file.is_open();
			file.close();
			return open;
		}
		catch(...)
		{
			file.close();
			throw FileReadException(file_name);
		}
	}

	// See function declaration for details.
	const std::streamoff FileSize(const std::string& file_name)
	{
		std::ifstream file;
		try
		{
			// Don't throw any exceptions.
			file.exceptions(std::ios::goodbit);
			file.open(file_name, std::ios::in | std::ios::binary);
			// File not found?
			if(file.fail() == true)
			{
				file.close();
				throw FileNotFoundException(file_name);
			}
			// Seek to the end of the file and report the offset.
			file.seekg(0, std::ios_base::end);
			const std::streamoff file_size = file.tellg();
			file.close();
			return file_size;
		}
		catch(...)
		{
			file.close();
			throw FileReadException(file_name);
		}
	}

	// See function declaration for details.
	void LoadFile(const std::string& file_name, std::vector<char>& file_data)
	{
		std::ifstream file;
		try
		{
			// Don't throw any exceptions.
			file.exceptions(std::ios::goodbit);
			file.open(file_name, std::ios::in | std::ios::binary);
			// Any problems?
			if(file.fail() == true)
			{
				file.close();
				throw FileNotFoundException(file_name);
			}
			file.seekg(0, std::ios_base::end);
			const std::streamoff file_size = file.tellg();
			// The file is empty.
			if(file_size < 1)
			{
				return;
			}
			file.seekg(0, std::ios_base::beg);
			// Reserve and initialize space for the entire file.
			file_data.reserve(static_cast<const unsigned int>(file_size));
			file_data.insert(file_data.end(), static_cast<const unsigned int>(file_size), 0);
			// Read in the file.
			file.read(&file_data[0], file_size);
			// Was an error encountered while reading data?
			if(file.bad() == true)
			{
				throw FileReadException(file_name);
			}
			file.close();
		}
		catch(const std::bad_alloc&)
		{
			file.close();
			throw OutOfMemoryError();
		}
		catch(...)
		{
			file.close();
			throw FileReadException(file_name);
		}
	}

	// See function declaration for details.
	void WriteFile(const std::string& file_name, const std::vector<const char>& file_data)
	{
		// If file_data is empty then we don't need to do anything.
		if(file_data.size() < 1)
		{
			return;
		}
		std::ofstream file;
		try
		{
			// Don't throw any exceptions.
			file.exceptions(std::ios::goodbit);
			file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
			// Any problems?
			if(file.fail() == true)
			{
				file.close();
				throw FileNotFoundException(file_name);
			}
			// Write the data to the file.
			file.write(&file_data[0], file_data.size());
			// Was an error encountered while writing data?
			if(file.bad() == true)
			{
				throw FileWriteException(file_name);
			}
			file.close();
		}
		catch(const std::bad_alloc&)
		{
			file.close();
			throw OutOfMemoryError();
		}
		catch(...)
		{
			file.close();
			throw FileWriteException(file_name);
		}
	}

	// See function declaration for details.
	void WriteFile(const std::string& file_name, const std::vector<char>& file_data)
	{
		std::ofstream file;
		try
		{
			// Don't throw any exceptions.
			file.exceptions(std::ios::goodbit);
			file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
			// Any problems?
			if(file.fail() == true)
			{
				file.close();
				throw FileNotFoundException(file_name);
			}
			// Write the data to the file.
			file.write(&file_data[0], file_data.size());
			// Was an error encountered while writing data?
			if(file.bad() == true)
			{
				throw FileWriteException(file_name);
			}
			file.close();
		}
		catch(const std::bad_alloc&)
		{
			file.close();
			throw OutOfMemoryError();
		}
		catch(...)
		{
			file.close();
			throw FileWriteException(file_name);
		}
	}

	// See function declaration for details.
	void WriteFile(const std::string& file_name, const std::string& file_data)
	{
		std::ofstream file;
		try
		{
			// Don't throw any exceptions.
			file.exceptions(std::ios::goodbit);
			file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
			// Any problems?
			if(file.fail() == true)
			{
				file.close();
				throw FileNotFoundException(file_name);
			}
			// Write the data to the file.
			file.write(file_data.c_str(), file_data.size());
			// Was an error encountered while writing data?
			if(file.bad() == true)
			{
				throw FileWriteException(file_name);
			}
			file.close();
		}
		catch(const std::bad_alloc&)
		{
			file.close();
			throw OutOfMemoryError();
		}
		catch(...)
		{
			file.close();
			throw FileWriteException(file_name);
		}
	}


} // utility
} // avl
