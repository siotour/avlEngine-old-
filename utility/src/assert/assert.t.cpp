/**********
 * Author: Sheldon Bachstein
 * Date: January 13, 2010
 * Description: Unit test of the assert component. See assert.h for details on the component.
 * Expected Output:
 *					assert.t.cpp	19
 *					assert.t.cpp	24
 *					assert.t.cpp	36
 **********/

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