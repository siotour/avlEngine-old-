/**********
 * Author: Sheldon Bachstein
 * Date: Dec 29, 2010
 * Description: Unit test for the settings file component. See settings file.h for details.
 * Expected output: 
 **********/


#include"settings file.h"
#include"..\exceptions\exceptions.h"
#include"..\assert\assert.h"
#include<iostream>
#include<string>

void TestSettingsFileComponent()
{
	using avl::utility::SettingsFile;
	try
	{
		SettingsFile file("Example.txt");

		std::cout << file.GetIntegerValue("integer_value_one") << "\n";

		std::cout << file.GetIntegerValue("negative_integer") << "\n";

		std::cout << file.GetStringValue("string_variable") << "\n";
	}
	catch (const avl::utility::Exception& error)
	{
		std::cout<< error.GetDescription() << std::endl;
	}
	system("pause");
}
