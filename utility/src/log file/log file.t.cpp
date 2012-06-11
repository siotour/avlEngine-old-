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
Unit test for the log file component. See "log file.h" for details.
@author Sheldon Bachstein
@date January 07, 2011
*/

#include"log file.h"
#include"..\exceptions\exceptions.h"
#include<iostream>

void TestLogFileComponent()
{
	try
	{
		// Create a log file to log to.
		std::cout << "Attemping to create the log file...\n";
		avl::utility::LogFile log("test log.txt");

		std::cout << "Attemping to use the log file...\n";
		// Log a mundane message.
		log(1, "Log file created.");
		// Log a critical message.
		log(4, "A fake critical error has occured! Run!!!!");
	}
	catch (const avl::utility::FileIOException& e)
	{
		// If this exception is caught, it means that the program was unable to either
		// create or open the log file.
		std::cout << e.GetDescription() << std::endl;
	}

	system("pause");
}