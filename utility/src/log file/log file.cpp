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
Implementation for the log file component. See "log file.h" for details.
@author Sheldon Bachstein
@date January 07, 2011
*/

#include"log file.h"
#include"..\exceptions\exceptions.h"
#include"..\assert\assert.h"
#include<string>
#include<ctime>




namespace avl
{
namespace utility
{

	// Takes the name of a file to use for logging. If the file doesn't exist, it will be created.
	// If it does exist, it will be appended to. If an error occurs while attempting to open the
	// file, a FileWriteException will be thrown.
	LogFile::LogFile(const std::string& file_name)
		: file_name(file_name)
	{
		// First, tell the file not to throw any exceptions.
		file.exceptions(std::ios_base::goodbit);
		// Attempt to open the file for writing in append mode.
		file.open(file_name.c_str(), std::ios_base::out | std::ios_base::app);

		// Check to make sure that the file was successfully opened. If not, throw a
		// FileWriteException.
		if (file.good() != true)
		{
			throw FileWriteException(file_name);
		}
	}




	// Destructor. Closes the output file.
	LogFile::~LogFile()
	{
		// Close the log file.
		file.close();
	}




	// Used to log information to the file specified upon creation. The first
	// parameter defines the urgency of the message, from 1 to 4 (values less than
	// 1 will be considered 1 and values greater than 4 will be considered 4) with
	// 1 being the least urgent and 4 being the most urgent. Delegates the work to
	// the virtual functions LogFile::WriteMessageHeader() and LogFile::WriteMessage().
	// Will throw a FileWriteException if an error occurs while attempting to
	// access the file.
	const LogFile& LogFile::operator()(const short& urgency, const std::string& message)
	{
		// If the file isn't ready for writing, throw a FileWriteException.
		if (file.good() != true)
		{
			throw FileWriteException(file_name);
		}

		// Write the message header.
		this->WriteMessageHeader(urgency);
		// Write the message.
		this->WriteMessage(message);

		// Make sure that the file is still in a writable state. If not, throw a
		// FileWriteException.
		if (file.good() != true)
		{
			throw FileWriteException(file_name);
		}

		// Return the original object.
		return *this;
	}




	// Attempts to write a message header including the message's urgency (in the
	// form of 1-4 asterisks), the date, and the time. If there is a problem writing
	// to the file, will throw a FileWriteException.
	void LogFile::WriteMessageHeader(const short& urgency)
	{
		// If the file isn't read for writing, throw a FileWriteException.
		if (file.good() != true)
		{
			throw FileWriteException(file_name);
		}

		// Contains the message header as it is composed.
		std::string header;

		// First compose the urgency indicator, which consists of 1-4 asterisks
		// inside of brackets. If urgency is less than 1, write 1 asterisk. If urgency
		// is greater than 4, write 4 asterisks.

		// First up, start with the opening bracket and at least one asterisk, then print
		// from 0 to 3 more asterisks, depending on the value of urgency.
		header = "[*";
		for (short x = 2; x <= urgency; ++x)
		{
			header += '*';
		}

		// Now add in a space for each star that is missing to keep the formatting
		// consistent.
		for (short x = 4 - urgency; x >= 1; --x)
		{
			header += ' ';
		}

		// Now close the brackets.
		header += ']';



		// Now compose the date and time and add that to the header.

		// Temporarily stores the date and time.
		char date_and_time[30];
		// Stores the raw time.
		time_t raw_time;
		// Stores the formatted time structure.
		struct tm time_info;

		// Get the raw time.
		time(&raw_time);
		// Get the formatted time structure in local time.
		VERIFY(localtime_s(&time_info, &raw_time) == 0);
		// Format the time and date into a string.
		strftime(date_and_time, 30, "%m/%d/%y @ %I:%M:%S%p---", &time_info);

		// Add the date and time to the header.
		header += date_and_time;

		// Write the header to the file.
		file << header;
	}




	// Writes the message to the log file without any special formatting. Will throw a
	// FileWriteException if there is a problem writing to the file.
	void LogFile::WriteMessage(const std::string& message)
	{
		// If the file isn't ready for writing, throw a FileWriteException.
		if (file.good() != true)
		{
			throw FileWriteException(file_name);
		}

		// Write the message to the file.
		file << message;
		// Write a trailing newline to the file.
		file << '\n';
	}


}
}