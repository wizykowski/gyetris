#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <sys/time.h>

///
class Timer
{
public:
	Timer();

	/// Sets a starting point for the timer.
	/// If called multiple times, will restart.
	void start();

	/// Temporarily stops the timer.
	void pause();

	/// Restarts the timer if it was paused.
	void unpause();

	/// Tells if the timer's still running (hasn't called stop())
	bool isRunning();

	/// Tells if the timer's paused.
	bool isPaused();

	/// Returns the whole timer's difference in milisseconds.
	//	@note If the timer's not started, will return 0.
    long long int delta_us();

	/// Returns the milisseconds part of the timer's difference.
    long long int delta_ms();

	/// Returns the seconds part of the timer's difference.
    long long int delta_s();

protected:
    long long int startMark;
    long long int stopMark;
    long long int pausedMark;

	bool running;
	bool paused;
};

#endif /* TIMER_H_DEFINED */

