#ifndef _TIMER_
#define _TIMER_

#include <thread>
#include <functional>

using namespace std;

namespace SNY
{
class Timer
{
public:
	template <class callable, class... arguments>
	Timer(int afterMilliSeconds, bool async, callable &&f, arguments &&... args)
	{
		function<typename result_of<callable(arguments...)>::type()> task(bind(forward<callable>(f), forward<arguments>(args)...));

		if (async)
		{
			thread([afterMilliSeconds, task]() {
				this_thread::sleep_for(chrono::milliseconds(afterMilliSeconds));
				task();
			})
				.detach();
		}
		else
		{
			this_thread::sleep_for(chrono::milliseconds(afterMilliSeconds));
			task();
		}
	}
};
} // namespace SNY

#endif
