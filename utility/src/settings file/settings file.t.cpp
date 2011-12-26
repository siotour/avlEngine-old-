/**********
 * Author: Sheldon Bachstein
 * Date: Dec 29, 2010
 * Description: Unit test for the settings file component. See settings file.h for details.
 * Expected output: 
 **********/


#include"settings file.h"
#include"..\read write error\read write error.h"
#include"..\assert\assert.h"
#include<iostream>
#include<string>

void TestSettingsFileComponent()
{
	using avl::utility::SettingsFile;
	try
	{
		SettingsFile file("Example.txt");
		std::string string_value;
		long int_value;

		VERIFY(file.GetIntegerValue("integer_value_one", int_value));
		std::cout << int_value << "\n";
		VERIFY(file.GetIntegerValue("negative_integer", int_value));
		std::cout << int_value << "\n";
		VERIFY(file.GetStringValue("string_variable", string_value));
		std::cout << string_value << "\n";
	}
	catch (const avl::utility::Exception& error)
	{
		std::cout<< error.GetDescription() << std::endl;
	}
	system("pause");
}
