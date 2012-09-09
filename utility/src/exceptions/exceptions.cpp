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
Implementation for the exceptions component. See "exceptions.h" for details.
@author Sheldon Bachstein
@date Dec 26, 2011
*/

#include"exceptions.h"
#include<string>


namespace avl
{
namespace utility
{
	//
	// Exception
	Exception::Exception(const std::string& description)
		: description(description)
	{
	}



	Exception::Exception(const Exception& original)
		: description(original.GetDescription())
	{
	}



	Exception::~Exception()
	{
	}



	const std::string& Exception::GetDescription() const
	{
		return description;
	}

	

	Exception::Exception()
	{
	}



	//
	// InvalidArgumentException
	InvalidArgumentException::InvalidArgumentException(const std::string& function, const std::string& parameter, const std::string& explanation)
	{
		description = function;
		description += " - An invalid argument was supplied for the parameter ";
		description += parameter;
		description += ": ";
		description += explanation;
	}



	InvalidArgumentException::InvalidArgumentException(const InvalidArgumentException& original)
		: Exception(original)
	{
	}



	InvalidArgumentException::~InvalidArgumentException()
	{
	}




	//
	// InvalidCallException
	InvalidCallException::InvalidCallException(const std::string& function, const std::string& explanation)
	{
		description = function;
		description += " - This function/method was called when it was invalid to do so: ";
		description += explanation;
	}



	InvalidCallException::InvalidCallException(const InvalidCallException& original)
		: Exception(original)
	{
	}



	InvalidCallException::~InvalidCallException()
	{
	}




	//
	// DivideByZeroError
	DivideByZeroError::DivideByZeroError(const std::string& function)
	{
		description = function;
		description += " - Unable to divide by zero.";
	}



	DivideByZeroError::DivideByZeroError(const DivideByZeroError& original)
		: Exception(original)
	{
	}



	DivideByZeroError::~DivideByZeroError()
	{
	}




	//
	// OutOfMemoryError
	OutOfMemoryError::OutOfMemoryError()
		: Exception("There is not enough memory available.")
	{
	}



	OutOfMemoryError::OutOfMemoryError(const OutOfMemoryError& original)
		: Exception(original)
	{
	}



	OutOfMemoryError::~OutOfMemoryError()
	{
	}




	//
	// IOException
	IOException::IOException(const std::string& description)
		: Exception(description)
	{
	}



	IOException::IOException(const IOException& original)
		: Exception(original)
	{
	}



	IOException::~IOException()
	{
	}



	IOException::IOException()
	{
	}




	//
	// FileIOException
	FileIOException::FileIOException()
	{
	}



	FileIOException::FileIOException(const FileIOException& original)
		: IOException(original)
	{
	}


	FileIOException::~FileIOException()
	{
	}




	//
	// FileNotFoundException
	FileNotFoundException::FileNotFoundException(const std::string& file_name)
	{
		description = "Unable to find the file ";
		description += file_name;
		description += '.';
	}



	FileNotFoundException::FileNotFoundException(const FileNotFoundException& original)
		: FileIOException(original)
	{
	}



	FileNotFoundException::~FileNotFoundException()
	{
	}




	//
	// FileReadException
	FileReadException::FileReadException(const std::string& file_name)
	{
		description = "An error occurred while reading from the file ";
		description += file_name;
		description += '.';
	}



	FileReadException::FileReadException(const FileReadException& original)
		: FileIOException(original)
	{
	}



	FileReadException::~FileReadException()
	{
	}



	//
	// FileWriteException
	FileWriteException::FileWriteException(const std::string& file_name)
	{
		description = "An error occurred while writing to the file ";
		description += file_name;
		description += '.';
	}



	FileWriteException::FileWriteException(const FileWriteException& original)
		: FileIOException(original)
	{
	}



	FileWriteException::~FileWriteException()
	{
	}



	//
	// FileFormatException
	FileFormatException::FileFormatException(const std::string& file_name)
	{
		description = "An error occurred while loading the file ";
		description += file_name;
		description += ": the file is malformatted.";
	}



	FileFormatException::FileFormatException(const FileFormatException& original)
		: FileIOException(original)
	{
	}



	FileFormatException::~FileFormatException()
	{
	}






} // utility
} // avl
