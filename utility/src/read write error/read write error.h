#ifndef __AVL_UTILITY_READ_WRITE_ERROR__
#define __AVL_UTILITY_READ_WRITE_ERROR__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 09, 2011
 * Description: Provides an interface for reporting that there was an unexpected problem
 * while attemping to read from or write to a file.
 **********/

#include"..\exception\exception.h"
#include<string>



namespace avl
{
namespace utility
{
	// See the beginning of the file for a description.
	class ReadWriteError: public avl::utility::Exception
	{
	public:
		// Constructors:
		ReadWriteError();
		// file_name should be the name of the file with which the exception occurred.
		ReadWriteError(const std::string& file_name);
		~ReadWriteError();
		// Copy constructor allows for this object to be passed/caught by value.
		ReadWriteError(const avl::utility::ReadWriteError& original);
	private:
		// NOT IMPLEMENTED
		const ReadWriteError& operator=(const ReadWriteError&);
	};




} //avl
} //utility
#endif // __AVL_UTILITY_READ_WRITE_ERROR__