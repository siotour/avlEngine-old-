#pragma once
#ifndef AVL_UTILITY_EXCEPTIONS__
#define AVL_UTILITY_EXCEPTIONS__
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
Defines the basic exception types used throughout the avl Library.
@author Sheldon Bachstein
@date Dec 26, 2011
*/

#include<string>


namespace avl
{
namespace utility
{
	/**
	Base class for all avl Library exceptions.
	*/
	class Exception
	{
	public:
		/** Public constructor; requires a description.
		@param description A description of the exception which occurred.
		*/
		Exception(const std::string& description);
		/** Copy constructor.
		@param original The object being copied.
		*/
		Exception(const Exception& original);
		/* Basic desctructor.*/
		virtual ~Exception();

		/** Returns a description of why this object was thrown.
		@return \ref description.
		*/
		const std::string& GetDescription() const;

	protected:
		/* Used by derived classes who can't immediately supply a description.
		@post \ref description is left empty.
		*/
		Exception();

		/// A description of why this object was thrown.
		std::string description;

		/// NOT IMPLEMENTED.
		Exception& operator=(const Exception&);

	};



	/**
	Indicates that one of the arguments supplied to a function is not a valid argument.
	*/
	class InvalidArgumentException: public Exception
	{
	public:
		/** Full-spec constructor.
		@param function The full scope and name of the function which received an invalid argument.
		@param parameter The name of the parameter which received an invalid argument.
		@param explanation An explanation of why the argument is invalid or the possible 
		*/
		InvalidArgumentException(const std::string& function, const std::string& parameter, const std::string& explanation);
		/** Copy constructor.
		@param original The object being copied.
		*/
		InvalidArgumentException(const InvalidArgumentException& original);
		/** Basic destructor.*/
		~InvalidArgumentException();
	private:
		/// Not implemented.
		InvalidArgumentException& operator=(const InvalidArgumentException&);
	};



	/**
	Indicates that a function/method was called when it was invalid to call that function/method.
	*/
	class InvalidCallException: public Exception
	{
	public:
		/** Full-spec constructor.
		@param function The full scope and name of the function which was invalidly called.
		@param explanation An explanation of why calling \a function was invalid.
		*/
		InvalidCallException(const std::string& function, const std::string& explanation);
		/** Copy constructor.
		@param original The object being copied.
		*/
		InvalidCallException(const InvalidCallException& original);
		/** Basic destructor.*/
		~InvalidCallException();
	private:
		/// Not implemented.
		InvalidCallException& operator=(const InvalidCallException&);
	};



	/**
	Indicates that an attempt was made to divide by zero.
	*/
	class DivideByZeroError: public Exception
	{
	public:
		/** Full-spec constructor.
		@param function The full scope and name of the function in which the division
		was to take place.
		*/
		DivideByZeroError(const std::string& function);
		/** Copy constructor.
		@param original The object being copied.
		*/
		DivideByZeroError(const DivideByZeroError& original);
		/** Basic destructor.*/
		~DivideByZeroError();
	private:
		/// Not implemented.
		DivideByZeroError& operator=(const DivideByZeroError&);
	};

		

	/**
	Represents the fact that the application has run out of memory and can't allocate required space.
	@todo Change this class so that it requires a string explaining where the exception was thrown
	from.
	*/
	class OutOfMemoryError: public Exception
	{
	public:
		/** Basic constructor.*/
		OutOfMemoryError();
		/** Copy constructor.
		@param original The object being copied.
		*/
		OutOfMemoryError(const OutOfMemoryError& original);
		/** Basic destructor.*/
		~OutOfMemoryError();
	private:
		/// Not implemented.
		OutOfMemoryError& operator=(const OutOfMemoryError&);
	};



	/**
	Represents a failure while attempting to perform an input/output operation.
	*/
	class IOException: public Exception
	{
	public:
		/** Basic constructor.
		@param description A description of the IO problem which occurred.
		*/
		IOException(const std::string& description);
		/** Copy constructor.
		@param original The object being copied.
		*/
		IOException(const IOException& original);
		/** Basic destructor.*/
		virtual ~IOException();
	protected:
		/** A no-spec constructor. Used by derived classes who can't initially provide
		a description.
		@post \ref description will be left empty.
		*/
		IOException();
	private:
		/// Not implemented.
		IOException& operator=(const IOException&);
	};



	/**
	Indicates that an error occurred while perform an input or output operation
	on a file.
	*/
	class FileIOException: public IOException
	{
	public:
		/** Basic constructor.*/
		FileIOException();
		/** Copy constructor.
		@param original The object being copied.
		*/
		FileIOException(const FileIOException& original);
		/** Basic destructor.*/
		virtual ~FileIOException();
	private:
		/// Not implemented.
		FileIOException& operator=(const FileIOException&);
	};


	/**
	Indicates that a file which was expected to exist could not be found.
	*/
	class FileNotFoundException: public FileIOException
	{
	public:
		/** Basic constructor.
		@param file_name The name of the file which could not be found.
		*/
		FileNotFoundException(const std::string& file_name);
		/** Copy constructor.
		@param original The object being copied.
		*/
		FileNotFoundException(const FileNotFoundException& original);
		/** Basic destructor.
		*/
		~FileNotFoundException();
	private:
		/// Not implemented.
		FileNotFoundException& operator=(const FileNotFoundException&);
	};



	/**
	Indicates that an error occurred while reading from a file.
	*/
	class FileReadException: public FileIOException
	{
	public:
		/** Basic constructor.
		@param file_name The name of the file which triggered the read error.
		*/
		FileReadException(const std::string& file_name);
		/** Copy constructor.
		@param original The object being copied.
		*/
		FileReadException(const FileReadException& original);
		/** Basic destructor.
		*/
		~FileReadException();
	private:
		/// Not implemented.
		FileReadException& operator=(const FileReadException&);
	};



	/**
	Indicates that an error occurred while writing to a file.
	*/
	class FileWriteException: public FileIOException
	{
	public:
		/** Basic constructor.
		@param file_name The name of the file which triggered the write error.
		*/
		FileWriteException(const std::string& file_name);
		/** Copy constructor.
		@param original The object being copied.
		*/
		FileWriteException(const FileWriteException& original);
		/** Basic destructor.
		*/
		~FileWriteException();
	private:
		/// Not implemented.
		FileWriteException& operator=(const FileWriteException&);
	};



	/**
	Indicates that a file is corrupt, incomplete, or malformatted.
	*/
	class FileFormatException: public FileIOException
	{
	public:
		/** Basic constructor.
		@param file_name The name of the file which is malformatted.
		*/
		FileFormatException(const std::string& file_name);
		/** Copy constructor.
		@param original The object being copied.
		*/
		FileFormatException(const FileFormatException& original);
		/** Basic destructor.
		*/
		~FileFormatException();
	private:
		/// Not implemented.
		FileFormatException& operator=(const FileFormatException&);

	};




} // utility
} // avl
#endif // AVL_UTILITY_EXCEPTIONS__
