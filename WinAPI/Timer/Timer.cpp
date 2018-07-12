#include "Timer.h"

Timer::Timer()
{
	FPS = 60;
	beginPeriod = 1;
	timeBeginPeriod(beginPeriod);
}

Timer::~Timer()
{
	timeEndPeriod(beginPeriod);
}

void Timer::SetFPS(int _fps)
{
	FPS = _fps;
}

int Timer::GetFPS()
{
	return FPS;
}

void Timer::SetTimerPeriod(int _period)
{
	beginPeriod = _period;
	timeBeginPeriod(beginPeriod);
}

DWORD Timer::Sleep()
{
	prevTime = timeGetTime();
	DWORD curTime;
	DWORD elapseTime = 0u;
	do
	{
		curTime = timeGetTime();
		elapseTime = curTime - prevTime;
	} while (elapseTime < (1000.0f/FPS));

	return elapseTime;
}

DWORD Timer::Sleep(int _millSec)
{
	prevTime = timeGetTime();
	DWORD curTime;
	DWORD elapseTime = 0u;
	do
	{
		curTime = timeGetTime();
		elapseTime = curTime - prevTime;
	} while (elapseTime < _millSec);

	return elapseTime;
}
