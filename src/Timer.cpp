#include "Timer.h"

/**
 * Timer implementation
 */

/**
 * Initializes variables.
 */
Timer::Timer() :
	startTicks(0),
	pausedTicks(0),
	paused(false),
	started(false)
{
}

/**
 * Start timer.
 */
void Timer::start()
{
	//Start the timer
	started = true;
	
	//Unpause the timer
	paused = false;

	//Get the current clock time
	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

/**
 * Stop timer.
 */
void Timer::stop()
{
	//Stop the timer
	started = false;

	//Unpause the timer
	paused = false;

	//Clear tick variables
	startTicks = 0;
	pausedTicks = 0;
}

/**
 * Pause timer.
 */
void Timer::pause()
{
	//If the timer is running and isn't already paused
	if (started && !paused)
	{
		//Pause the timer
		paused = true;

		//Calculate the paused ticks
		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

/**
 * Unpause timer.
 */
void Timer::unpause()
{
	//If the timer is running and paused
	if (started && paused)
	{
		//Unpause the timer
		paused = false;

		//Reset the starting ticks
		startTicks = SDL_GetTicks() - pausedTicks;

		//Reset the paused ticks
		pausedTicks = 0;
	}
}

/**
 * Gets the timer's time.
 * @return Uint32
 */
Uint32 Timer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (started)
	{
		//If the timer is paused
		if (paused)
		{
			//Return the number of ticks when timer was paused
			time = pausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - startTicks;
		}
	}
	
	return time;
}

/**
 * Checks if the timer is started.
 * @return bool
 */
bool Timer::isStarted()
{
	//Timer is running and paused or unpaused
	return started;
}

/**
 * Checks if the timer is paused.
 * @return bool
 */
bool Timer::isPaused()
{
	//Timer is running and paused
	return paused && started;
}