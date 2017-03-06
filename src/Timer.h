#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

/**
 * Timer is a class that is basically a timer.
 */
class Timer
{
public:
	/**
     * Initializes variables.
     */
	Timer();

	//The various clock actions

    /**
     * Start timer.
     */
	void start();
	
    /**
     * Stop timer.
     */
    void stop();
	
    /**
     * Pause timer.
     */
    void pause();
	
    /**
     * Unpause timer.
     */
    void unpause();

	/**
     * Gets the timer's time.
     * @return Uint32
     */
	Uint32 getTicks();

	/**
     * Checks if the timer is started.
     * @return bool
     */
	bool isStarted();
	
    /**
     * Checks if the timer is paused.
     * @return bool
     */
    bool isPaused();

private:
	/**
     * The clock time when the timer started.
     */
	Uint32 startTicks;

	/**
     * The ticks stored when the timer was paused.
     */
	Uint32 pausedTicks;

	/**
     * Is timer paused.
     */
	bool paused;
	
    /**
     * Is timer started.
     */
    bool started;
};

#endif // TIMER_H