/**********
 * Author: Sheldon Bachstein
 * Date: Dec 29, 2010
 * Description: Unit test for the settings file component. See settings file.h for details.
 * Expected output: 
 **********/


#include"settings file.h"
#include"..\read write error\read write error.h"
#include<iostream>

void TestSettingsFileComponent()
{
	try
	{
		avl::utility::SettingsFile file("blah.txt");
	}
	catch (avl::utility::ReadWriteError)
	{
		std::cout<< "ERROR: UNABLE TO READ FILE.";
	}
	catch (const avl::utility::SyntaxError error)
	{
		std::cout<< "ERROR: SYNTAX INCORRECT. PROBLEM: ";
		switch(error.GetErrorType())
		{
		case avl::utility::SyntaxError::BAD_VALUE:
			std::cout<< "BAD VALUE";
			break;
		case avl::utility::SyntaxError::BAD_VARIABLE_NAME:
			std::cout<< "BAD VARIABLE NAME";
			break;
		case avl::utility::SyntaxError::OUT_OF_RANGE_INTEGER:
			std::cout<< "INTEGER VALUE OUT OF BOUNDS";
			break;
		}
		std::cout<< " ON LINE " << error.GetLineNumber();
	}
	system("pause");
}
