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










} // utility
} // avl
