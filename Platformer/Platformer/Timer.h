#pragma once
#include <SDL/SDL.h>

//Based on the SDL tutorial found here: http://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php

class Timer
{
public:
	Timer();
	~Timer();

	//Clock actions
	void start();
	void stop();
	void restart();
	void pause();
	void unpause();

	//Get the timer's time
	Uint32 getTicks();

	//Check the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

