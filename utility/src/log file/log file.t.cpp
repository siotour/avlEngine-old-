/**********
 * Author: Sheldon Bachstein
 * Date: Jan 07, 2011
 * Description: Unit test for the log file component. See log file.h for details.
 * Expected output: 
 **********/

#include"log file.h"
#include"..\read write error\read write error.h"
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
	catch (const avl::utility::ReadWriteError&)
	{
		// If this exception is caught, it means that the program was unable to either
		// create or open the log file.
		std::cout << "A read/write error occured while attemping to create and/or use the log file (test log.txt).\n\n";
	}

	system("pause");
}