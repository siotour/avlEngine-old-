#ifndef __AVL_UTILITY_LOG_FILE__
#define __AVL_UTILITY_LOG_FILE__
/**
@file
Used for logging messages to a file in human-readable format.
@author Sheldon Bachstein
@date Jan 07, 2011
*/

#include<string>
#include<fstream>


namespace avl
{
namespace utility
{
	/** Provides a simple interface for logging messages to a file in a consistent,
	human-readable format. Includes an urgency rating and time stamp for messages.
	*/
	class LogFile
	{
	public:
		/** Takes the name of a file which will either be created or appended to.
		Delegates the initialization of the file to the virtual function LogFile::OpenFile().
		@param file_name Name of the file to log messages to.
		@throws FileWriteException If an error occurs while attempting to access \a file_name.
		*/
		LogFile(const std::string& file_name);
		/** Basic destructor. Closes the file stream.*/
		virtual ~LogFile();

		/** Logs a formatted message to LogFile::file.
		Delegates the work to the virtual functions LogFile::WriteMessageHeader() and LogFile::WriteMessage().
		\n The format of the message is:\n
			[**  ]02/12/1990 @ 11:51:04AM---This is a message of urgency 2.
		@param urgency The urgency of the message, from 1 to 4. Values less than 1 will be considered 1 and
		values greater than 4 will be considered 4, with 1 being the least urgent and 4 being the most urgent.
		@param message The message to be logged.
		@throws FileWriteException If an error occurs while attempting to access the file.
		*/
		virtual const LogFile& operator()(const short& urgency, const std::string& message);

	private:
		/** Attempts to write a message header to LogFile::file.
		@param urgency The urgency of the message to follow this header.
		@throws FileWriteException If an error occurs while attempting to write to \ref file.
		*/
		virtual void WriteMessageHeader(const short& urgency);
		/** Attempts to write \a message to LogFile::file.
		@param message The message to be written.
		@throws FileWriteException If an error occurs while attempting to write to \ref file.
		*/
		virtual void WriteMessage(const std::string& message);

		/// The name of the log file.
		const std::string file_name;
		
		/// The file which information will be logged to.
		std::ofstream file;

		/// NOT IMPLEMENTED.
		LogFile(const LogFile&);
		/// NOT IMPLEMENTED.
		const LogFile& operator=(const LogFile&);
	};




} //avl
} //utility
#endif // __AVL_UTILITY_LOG_FILE__