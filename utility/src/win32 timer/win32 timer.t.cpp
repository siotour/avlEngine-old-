/**********
 * Author: Sheldon Bachstein
 * Date: Jan 10, 2011
 * Description: Unit test for the win32 timer component. See win32 timer.h for details.
 * Expected output: 
 **********/

#include"win32 timer.h"
#include<iostream>



void TestWin32TimerComponent()
{
	try
	{
		// Create a low-precision timer.
		avl::utility::Win32Timer standard_timer(false);
		// Create a high-precision timer.
		avl::utility::Win32Timer performance_timer(true);

		// Calling Poll() will return the amount of time which has passed since the last call
		// to Poll(). Note that timers are initially Polled upon creation.
		std::cout << "It has been " << standard_timer.Poll() << " seconds since the standard-"
			"precision timer was created, and " << performance_timer.Poll() << " seconds since "
			"the high-precision timer was created.\n\n";

		// This is how you would wait a specified amount of time using timers.
		std::cout << "I'm now going to count down from 20 seconds!\n20...\n";
		// Every second, count down from 19.
		double accumulator = 0.0f;
		for(int count = 19; count >= 0; accumulator += performance_timer.Poll())
		{
			if (accumulator >= 1.0f)
			{
				std::cout<< count << "...\n";
				--count;
				accumulator -= 1.0f;
			}
		}
	}
	catch(const avl::utility::PerformanceCounterUnsupported&)
	{
		// If this exception is caught, then it signals that the system does not support high-
		// precision timers.
		std::cout<< "Ahh! Performance counters aren't supported on this sytem -- use a "
			"standard timer instead!\n\n";
	}

	std::cout<< "\nGoodbye!\n";

	system("pause");
}