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
Implementation for the timer component. See "timer.h" for details.
@author Sheldon Bachstein
@date January 10, 2011
*/

#include"timer.h"
#include"..\assert\assert.h"
#include<Windows.h>



namespace avl
{
namespace utility
{

	// See method declaration for details.
	Timer::Timer()
	{
		// Are performance counters supported?
		LARGE_INTEGER temp_count;
		use_performance_counter = (QueryPerformanceFrequency(&temp_count) != 0);
		// Synchronize this timer with the current system time.
		if(use_performance_counter == true)
		{
			frequency = temp_count.QuadPart;
			ResetCounter(PollPerformanceCounter());
		}
		else
		{
			frequency = 1000;
			ResetCounter(PollCounter());
		}
	}




	// See method declaration for details.
	Timer::~Timer()
	{
	}




	// See method declaration for details.
	const __int64 Timer::GetPrecision() const
	{
		return frequency;
	}




	// See method declaration for details.
	const double Timer::Reset()
	{
		// Get the time elapsed.
		__int64 current_count;
		double elapsed_time;
		// Reset the last counter.
		if (use_performance_counter == true)
		{
			current_count = PollPerformanceCounter();
		}
		else
		{
			current_count = PollCounter();
		}
		// Calculate the elapsed time and reset the counter.
		elapsed_time = CalculateTimeSplit(current_count);
		ResetCounter(current_count);
		// Return the elapsed time.
		return elapsed_time;
	}




	// See method declaration for details.
	const double Timer::Elapsed() const
	{
		// If the performance counter is being use, poll the performance counter. Otherwise
		// poll the standard counter.
		if (use_performance_counter == true)
		{
			return CalculateTimeSplit(PollPerformanceCounter());
		}
		else
		{
			return CalculateTimeSplit(PollCounter());
		}
	}




	// See method declaration for details.
	const __int64 Timer::PollPerformanceCounter() const
	{
		ASSERT(frequency != 0);
		// Temp. storage for the LARGE_INTEGER structure.
		LARGE_INTEGER temp_count;
		// Poll the performance counter for the current count.
		VERIFY(QueryPerformanceCounter(&temp_count));
		// Return the current count as an int64.
		return temp_count.QuadPart;
	}





	// See method declaration for details.
	const __int64 Timer::PollCounter() const
	{
		return timeGetTime();
	}




	// See method declaration for details.
	void Timer::ResetCounter(const __int64 new_count)
	{
		last_count = new_count;
	}



	// See method declaration for details.
	const double Timer::CalculateTimeSplit(const __int64 current_count) const
	{
		// Time unit difference.
		__int64 difference;
		// Has the count wrapped around?
		if (current_count >= last_count)
		{
			difference = current_count - last_count;
		}
		else
		{
			difference = ULONG_MAX - last_count + 1;
		}
		return (double)difference/(double)frequency;
	}




}
}