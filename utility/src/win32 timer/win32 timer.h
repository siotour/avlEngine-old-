#ifndef __AVL_UTILITY_WIN32_TIMER__
#define __AVL_UTILITY_WIN32_TIMER__
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
	class Win32Timer
	{
	public:
		/** Basic constructor. Attempts to use performance counters if available.
		*/
		Win32Timer();
		/** Basic destructor.*/
		~Win32Timer();

		/** Are performance counters being used?
		@return True if performance counters are used, and false if not.
		*/
		const bool IsPerformanceTimer() const;
		/**
		@return The amount of time passed, in seconds, since either creation or the last call
		to \ref Poll().
		*/
		const double Poll();

	private:
		/** Polls the performance timer.
		@return Seconds passed since the last poll of the performance counter.
		*/
		const double PollPerformanceCounter();
		/** Polls the non-performance timer.
		@return Seconds passed since the last poll of the non-performance counter.
		*/
		const double PollCounter();

		/// Are performance counters used?
		bool use_performance_counter;
		/// Stores the number of counter ticks per second.
		__int64 frequency;
		/// Stores the counter's count at last poll.
		__int64 last_count;

		/// NOT IMPLEMENTED.
		Win32Timer(const Win32Timer&);
		/// NOT IMPLEMENTED.
		Win32Timer& operator=(const Win32Timer&);
	};



} //avl
} //utility
#endif // __AVL_UTILITY_WIN32_TIMER__