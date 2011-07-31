/**********
 * Author: Sheldon Bachstein
 * Date: Jan 10, 2011
 * Description: See win32 timer.h for details.
 **********/

#include"win32 timer.h"
#include"..\assert\assert.h"
#include<Windows.h>



namespace avl
{
namespace utility
{

	// Takes a bool specifying whether or not performance counters should be used. If performance
	// counters aren't supported but are requested, will throw a
	// PerformanceTimerUnsupported.
	Win32Timer::Win32Timer(const bool& use_performance_counter)
	: use_performance_counter(use_performance_counter)
	{
		// If the user specified the use of performance timers, attempt to query the
		// frequency now and poll the initial count.
		if (use_performance_counter == true)
		{
			// QueryPerformanceTimer returns a BOOL typedef, so comparing the result to 0 effectively
			// converts it to a bool and gets rid of a compiler warning about data-loss.
			bool performance_timer_supported = (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)) != 0);

			// If performance timers aren't supported, then throw a
			// PerformanceTimerUnsupported.
			if (performance_timer_supported != true)
			{
				throw PerformanceCounterUnsupported();
			}

			// Initialize last_count to the current count.
			VERIFY(QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&last_count)));
		}
		// If the user specified not to use performance timers, set up the frequency and poll the
		// initial count using timeGetTime().
		else
		{
			// The frequency for timeGetTime() is 1000.
			frequency = 1000;

			// Poll for the initial count and assign it to last_count.
			last_count = static_cast<__int64>(timeGetTime());
		}
	}




	// Basic destructor.
	Win32Timer::~Win32Timer()
	{
	}




	// Polling function. Returns the number of seconds which have passed since
	// the program was executed.
	const double Win32Timer::Poll()
	{
		// If the performance counter is being use, poll the performance counter. Otherwise
		// poll the standard counter.
		if (use_performance_counter == true)
		{
			return PollPerformanceCounter();
		}
		else
		{
			return PollCounter();
		}
	}




	// Polls the performance counter and calculates the number of seconds which have
	// passed since the last poll.
	const double Win32Timer::PollPerformanceCounter()
	{
		ASSERT(frequency != 0);
		// Stores the new count.
		__int64 new_count;
		// Stores the difference in counts between the last count and this count.
		__int64 difference;

		// Poll the performance counter for the current count.
		VERIFY(QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&new_count)));

		// If the count has not wrapped around, then simply subtract the last count
		// from the new count to find the difference.
		if (new_count >= last_count)
		{
			difference = new_count - last_count;
		}
		// If the count has wrapped around, then calculate the difference by
		// subtracting the last count from the maximum value, adding the new
		// count, and adding an addition one (to account for the value zero).
		else
		{
			difference = _I64_MAX - last_count + 1;
		}

		// Update the last count.
		last_count = new_count;

		// Calculate the number of seconds passed using the counter frequency, and
		// return it (as a double).
		return (double)difference/(double)frequency;
	}




	// Polls the standard counter and calculates the number of seconds which have
	// passed since the last poll.
	const double Win32Timer::PollCounter()
	{
		ASSERT(frequency != 0);
		// Stores the new count.
		__int64 new_count;
		// Stores the difference in counts between the last count and this count.
		__int64 difference;

		// Poll the system counter for the current count.
		new_count = static_cast<__int64>(timeGetTime());

		// If the count has not wrapped around, then simply subtract the last count
		// from the new count to find the difference.
		if (new_count >= last_count)
		{
			difference = new_count - last_count;
		}
		// If the count has wrapped around, then calculate the difference by
		// subtracting the last count from the maximum value, adding the new
		// count, and adding an addition one (to account for the value zero).
		else
		{
			difference = ULONG_MAX - last_count + 1;
		}

		// Update the last count.
		last_count = new_count;

		// Calculate the number of seconds passed using the counter frequency, and
		// return it (as a double).
		return (double)difference/(double)frequency;
	}




	// Basic constructor.
	PerformanceCounterUnsupported::PerformanceCounterUnsupported()
		: Exception("avl::utility::Win32Timer -- Performance counters were requested but aren't supported on this system.")
	{
	}




	// Basic destructor.
	PerformanceCounterUnsupported::~PerformanceCounterUnsupported()
	{
	}




	// Basic copy constructor (allows for passing by value).
	PerformanceCounterUnsupported::PerformanceCounterUnsupported(const PerformanceCounterUnsupported& original)
		: Exception(original)
	{
	}


}
}