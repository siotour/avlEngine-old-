/**********
 * Author: Sheldon Bachstein
 * Date: Jan 09, 2011
 * Description: See read write error.h for details.
 **********/

#include"read write error.h"
#include<string>



namespace avl
{
namespace utility
{


	// Basic constructor.
	ReadWriteError::ReadWriteError()
		: Exception("avl::utility::ReadWriteError -- An error occurred while reading from or writing to a file.")
	{
	}




	// Basic constructor. file_name should be the name of the file with which the exception occurred.
	ReadWriteError::ReadWriteError(const std::string& file_name)
		: Exception("avl::utility::ReadWriteError -- An error occurred while reading from or writing to the file " + file_name + ".")
	{
	}




	// Basic destructor.
	ReadWriteError::~ReadWriteError()
	{
	}




	// Basic copy constructor. Allows for catching by value.
	ReadWriteError::ReadWriteError(const avl::utility::ReadWriteError& original)
		: Exception(original)
	{
	}




}
}