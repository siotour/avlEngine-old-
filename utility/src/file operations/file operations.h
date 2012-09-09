#pragma once
#ifndef AVL_UTILITY_FILE_OPERATIONS__
#define AVL_UTILITY_FILE_OPERATIONS__
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
Provides basic functions for reading from and writing to files.
@author Sheldon Bachstein
@date Jun 22, 2012
*/

#include<string>
#include<vector>
#include<fstream>

namespace avl
{
namespace utility
{
	/** Checks to see if the file named \a file_name exists.
	@param file_name The name of the file to check.
	@return True if the file exists, and false if not.
	*/
	const bool FileExists(const std::string& file_name);

	/** Checks the file size of the file named \a file_name.
	@param file_name The name of the file to check.
	@return The size of the file named \a file_name in bytes.
	@throws OutOfMemoryError If we run out of memory.
	@throws FileNotFoundException If the file doesn't exist.
	*/
	const std::streamoff FileSize(const std::string& file_name);

	/** Reads the file named \a file_name to \a file_data.
	@param file_name The name of the file to read from.
	@param file_data Will be used to store the data from the file named
	\a file_name.
	@throws OutOfMemoryError If we run out of memory.
	@throws FileNotFoundException If the file doesn't exist.
	@throws FileReadException If an error occurs while reading from the file.
	*/
	void LoadFile(const std::string& file_name, std::vector<char>& file_data);

	/** Writes the data from \a file_data to the file named \a file_name.
	@post If \a file_data is empty, then no file will be created or
	modified.
	@warning This procedure will write over any existing file named
	\a file_name.
	@param file_name The name of the file to write to.
	@param file_data The data to write.
	@throws OutOfMemoryError If we run out of memory.
	@throws FileNotFoundException If the file doesn't exist.
	@throws FileReadException If an error occurs while writing to the file.
	*/
	void WriteFile(const std::string& file_name, const std::vector<const char>& file_data);

	/** Writes the data from \a file_data to the file named \a file_name.
	@post If \a file_data is empty, then no file will be created or
	modified.
	@warning This procedure will write over any existing file named
	\a file_name.
	@param file_name The name of the file to write to.
	@param file_data The data to write.
	@throws OutOfMemoryError If we run out of memory.
	@throws FileNotFoundException If the file doesn't exist.
	@throws FileReadException If an error occurs while writing to the file.
	*/
	void WriteFile(const std::string& file_name, const std::vector<char>& file_data);

	/** Writes \a file_data to the file named \a file_name.
	@post If \a file_data is empty, then no file will be created or
	modified.
	@warning This procedure will write over any existing file named
	\a file_name.
	@param file_name The name of the file to write to.
	@param file_data The data to write.
	@throws OutOfMemoryError If we run out of memory.
	@throws FileNotFoundException If the file doesn't exist.
	@throws FileReadException If an error occurs while writing to the file.
	*/
	void WriteFile(const std::string& file_name, const std::string& file_data);



} // utility
} // avl
#endif // AVL_UTILITY_FILE_OPERATIONS__
