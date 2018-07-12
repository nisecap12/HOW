#pragma once
#include <Windows.h>
#pragma comment (lib, "winmm.lib")

class Timer
{
private:
	int FPS;
	DWORD prevTime;
	int beginPeriod;

public:
	Timer();
	~Timer();

	void SetFPS(int _fps);
	int GetFPS();
	void SetTimerPeriod(int _period);
	DWORD Sleep();
	DWORD Sleep(int _millSec);
};

