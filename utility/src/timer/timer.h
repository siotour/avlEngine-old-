#pragma once
#ifndef AVL_UTILITY_TIMER__
#define AVL_UTILITY_TIMER__
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
Used for keeping track of the passage of time.
@author Sheldon Bachstein
@date Jan 10, 2011
*/


namespace avl
{
namespace utility
{

	/**
	Keeps track of the passage of time. Uses performance counters if available.
	*/
	class Timer
	{
	public:
		/** Basic constructor. Attempts to use performance counters if available.
		*/
		Timer();
		/** Basic destructor.*/
		~Timer();

		/** Returns the counter precision in hertz.
		@return The precision of this timer.
		*/
		const __int64 GetPrecision() const;
		/** Resets the timer's point of reference and returns the amount of time
		elapsed since either the last call to Reset or since creation if Reset hasn't
		been called. Note that time elapsed between calls to Timer::Pause() and
		Timer::Unpause() isn't counted.
		@return The amount of time passed since either the last call to Reset or since
		creation if Reset hasn't been called.
		*/
		const double Reset();
		/** Returns the amount of time elapsed since either the last call to Reset or
		since creation if Reset hasn't been called. Note that time elapsed between calls to
		Timer::Pause() and Timer::Unpause() isn't counted.
		@return The amount of time passed since either the last call to Reset or since
		creation if Reset hasn't been called.
		*/
		const double Elapsed() const;

		/** Pauses the timer so that elapsed time is not tracked.
		*/
		void Pause();

		/** Unpauses the timer so that elapsed time is tracked.
		*/
		void Unpause();

	private:
		/** Polls the performance timer.
		@return Units of time passed since \ref last_count.
		*/
		const __int64 PollPerformanceCounter() const;
		/** Polls the non-performance timer.
		@return Units of time passed since \ref last_count.
		*/
		const __int64 PollCounter() const;
		/** Resets the timer to refer to the current time as a reference for
		calculating elapsed time.
		@param new_count The new time measurement to refer to.
		*/
		void ResetCounter(const __int64 new_count);
		/** Calculates the amount of time elapsed between \ref last_count and
		\a current_count.
		@param current_count The measurement of time to measure up to from
		\ref last_count.
		@return Seconds passed between \ref last_count and \a current_count.
		*/
		const double CalculateTimeSplit(const __int64 current_count) const;

		/// Are we paused?
		bool paused;
		/// Are performance counters used?
		bool use_performance_counter;
		/// Stores the number of counter ticks per second.
		__int64 frequency;
		/// Stores the counter's count at last poll.
		__int64 last_count;

		/// NOT IMPLEMENTED.
		Timer(const Timer&);
		/// NOT IMPLEMENTED.
		Timer& operator=(const Timer&);
	};



} //avl
} //utility
#endif // AVL_UTILITY_TIMER__