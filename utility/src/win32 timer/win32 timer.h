#ifndef __AVL_UTILITY_WIN32_TIMER__
#define __AVL_UTILITY_WIN32_TIMER__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 10, 2011
 * Description: Keeps track of the passage of time. Utilizes performance counters if specified
 * and if the machine supports it.
 **********/


#include"..\exception\exception.h"


namespace avl
{
namespace utility
{

	// See the beginning of the file for details.
	class Win32Timer
	{
	public:
		// Constructors:
		// Takes a bool specifying whether or not performance counters should be used. If
		// performance counters aren't supported but are requested, will throw a
		// avl::utility::PerformanceTimerUnsupported.
		Win32Timer(const bool& use_performance_counter);
		~Win32Timer();

		// Accessors:
		// Returns the amount of time passed since either creation or the last call to
		// Poll().
		const double Poll();

	private:
		// Polls the performance timer.
		const double PollPerformanceCounter();
		// Polls the non-performance timer.
		const double PollCounter();

		// Should we try to poll the performance timer?
		const bool use_performance_counter;
		// Stores the frequency of the timer per second.
		__int64 frequency;
		// Stores the counter's count at last poll.
		__int64 last_count;

	};




	// Signals that a performance counter is unavailable on the system.
	class PerformanceCounterUnsupported: public Exception
	{
	public:
		// Constructors:
		PerformanceCounterUnsupported();
		~PerformanceCounterUnsupported();
		// To allow for passing by value.
		PerformanceCounterUnsupported(const PerformanceCounterUnsupported& original);

	private:
		// NOT IMPLEMENTED.
		const PerformanceCounterUnsupported& operator=(const PerformanceCounterUnsupported&);
	};




} //avl
} //utility
#endif // __AVL_UTILITY_WIN32_TIMER__