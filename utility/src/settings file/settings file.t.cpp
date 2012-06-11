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
Unit test for the settings file component. See "settings file.h" for details.
@author Sheldon Bachstein
@date December 29, 2010
*/


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
