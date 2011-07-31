#ifndef __AVL_UTILITY_LOG_FILE__
#define __AVL_UTILITY_LOG_FILE__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 07, 2011
 * Description: Provides an interface for logging information to a file in a human-readable
 * and consistently-formatted way, and implements a basic logging format which includes
 * the date, time, and urgency of logged messages.
 **********/

#include<string>
#include<fstream>


namespace avl
{
namespace utility
{
	// See the beginning of the file for details.
	class LogFile
	{
	public:
		// Constructors:
		// Takes the name of a file which will either be created or appended to.
		// Delegates the initialization of the file to the virtual function
		// LogFile::OpenFile(). Will throw a avl::utility::ReadWriteError if an
		// error occurs while attempting to access the file.
		LogFile(const std::string& file_name);
		~LogFile();

		// Mutators:
		// Used to log information to the file specified upon creation. The first
		// parameter defines the urgency of the message, from 1 to 4 (values less
		// than 1 will be considered 1 and values greater than 4 will be considered
		// 4) with 1 being the least urgent and 4 being the most urgent. Delegates
		// the work to the virtual functions LogFile::WriteMessageHeader() and
		// LogFile::WriteMessage(). Will throw a avl::utility::ReadWriteError if
		// an error occurs while attempting to access the file.
		virtual const LogFile& operator()(const short& urgency, const std::string& message);

	private:
		// Attempts to write a message header to the file. Will throw a
		// avl::utility::ReadWriteError if an error occurs while attempting to
		// access the file.
		virtual void WriteMessageHeader(const short& urgency);
		// Attempts to write the message to the file. Will throw a
		// avl::utility::ReadWriteError if an error occurs while attempting to
		// access the file.
		virtual void WriteMessage(const std::string& message);

		// The file which information will be logged to.
		std::ofstream file;

		// NOT IMPLEMENTED.
		LogFile(const LogFile&);
		const LogFile& operator=(const LogFile&);
	};




} //avl
} //utility
#endif // __AVL_UTILITY_LOG_FILE__