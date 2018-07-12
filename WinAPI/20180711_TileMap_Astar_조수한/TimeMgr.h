#pragma once

#include <Windows.h>

class Time;

class TimeMgr
{
private:
	Time	*m_timer = nullptr;

private:
	TimeMgr();
	~TimeMgr();

public:
	static TimeMgr & GetInstance()
	{
		static TimeMgr instance;
		return instance;
	}

	//	������ ����
	float Tick();
	//	FPS ����
	unsigned long	GetFPS();
	void			SetFPS(unsigned long _lockFPS);

	//	����ð� ���
	float			GetElapsedTime();
	//	�÷��� �ð� ���
	float			GetPlayTime();
	//	ȭ�� FPS ���
	void			Render(HDC _hdc, int _x, int _y);
};

#define		TIME	TimeMgr::GetInstance()