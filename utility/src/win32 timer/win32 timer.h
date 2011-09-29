#ifndef __AVL_UTILITY_WIN32_TIMER__
#define __AVL_UTILITY_WIN32_TIMER__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 10, 2011
 * Description: Keeps track of the passage of time. Utilizes performance counters if
 * possible.
 **********/


namespace avl
{
namespace utility
{

	// See the beginning of the file for details.
	class Win32Timer
	{
	public:
		// Constructors:
		// Attempts to use performance counters if available. Use of performance counters
		// can be checked using IsPerformanceTimer().
		Win32Timer();
		~Win32Timer();

		// Accessors:
		// Returns true if the timer is using a performance counter, and false if not.
		const bool IsPerformanceTimer() const;
		// Returns the amount of time passed since either creation or the last call to
		// Poll().
		const double Poll();

	private:
		// Polls the performance timer.
		const double PollPerformanceCounter();
		// Polls the non-performance timer.
		const double PollCounter();

		// Should we try to poll the performance timer?
		bool use_performance_counter;
		// Stores the frequency of the timer per second.
		__int64 frequency;
		// Stores the counter's count at last poll.
		__int64 last_count;

		// NOT IMPLEMENTED.
		Win32Timer(const Win32Timer&);
		Win32Timer& operator=(const Win32Timer&);
	};



} //avl
} //utility
#endif // __AVL_UTILITY_WIN32_TIMER__