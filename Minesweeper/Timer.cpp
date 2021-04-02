#include "Timer.h"


Timer::Timer()
{
	bPaused = true;
	runTime = 0;
	mC.restart();
}

void Timer::restart()
{
	mC.restart();
	runTime = 0;
	bPaused = false;
}

void Timer::start()
{
	if (bPaused) {
		mC.restart();
	}
	bPaused = false;
}

void Timer::pause()
{
	if (!bPaused) {
		runTime += mC.getElapsedTime().asSeconds();
	}
	bPaused = true;
}

float Timer::getElapsedTime()
{
	if (!bPaused)
	{
		return runTime + mC.getElapsedTime().asSeconds();
	}
	return runTime;
}

Timer::~Timer()
{
}
