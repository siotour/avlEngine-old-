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
Unit test for the assert component. See "assert.h" for details.
@author Sheldon Bachstein
@date January 13, 2010
*/

#include"assert.h"
#include"..\exceptions\exceptions.h"
#include<iostream>


void TestAssertComponent()
{
	// This is how AssertionFailures are created to report a failed assertion.
	avl::utility::AssertVerifyFailure assertion_failure(__FILE__, __LINE__);
	// This is how you get the file name and line number of a failed assertion.
	std::cout<< assertion_failure.GetFile() << "\t" << assertion_failure.GetLine() << "\n";


	// This is how to catch assertion and verification failure exceptions.
	try
	{
		// Passes.
		ASSERT(2 == 2);
		// Passes.
		VERIFY(5 != 10);
		// Fails.
		VERIFY(2 + 2 == 5);
	}
	// Catch an assertion failure exception.
	catch (avl::utility::AssertVerifyFailure	n)
	{
		std::cout<< n.GetFile() << "\t" << n.GetLine() << "\n";
	}
	system("pause");
}