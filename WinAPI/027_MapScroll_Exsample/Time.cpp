#include "Time.h"

#include <Windows.h>
#pragma comment(lib, "winmm.lib")

Time::Time()
{
	Initialize();
}
Time::~Time()
{
	Release();
}

void Time::Initialize()
{
	LARGE_INTEGER	periodRrequency = {};
	m_isHighTimer = QueryPerformanceFrequency(&periodRrequency);

	if (m_isHighTimer == true)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lastTime);
		m_timeResolusion = 1.0f / periodRrequency.QuadPart;
	}
	else
	{
		//	�ػ� ����
		timeBeginPeriod(1);

		m_lastTime = timeGetTime();
		m_timeResolusion = 0.001f;
	}
}

void Time::Release()
{
	timeEndPeriod(1);
}

float Time::Tick()
{
	do
	{
		//	���� �ð�
		if( m_isHighTimer )
			QueryPerformanceCounter((LARGE_INTEGER*)&m_nowTime);
		else
			m_nowTime		= timeGetTime();

		//	��� �ð� ���
		m_timeElapsed	= (m_nowTime - m_lastTime) * m_timeResolusion;

	} while (m_timeElapsed <= m_lockTime);

	//	������ �ð� ����
	m_lastTime			= m_nowTime;
	//	FPSCount ����
	m_FPSCount++;
	//	FPS ����ð� ����
	m_FPSTimeElapsed	+= m_timeElapsed;
	//	�÷��� �ð� ����
	m_playTime			+= m_timeElapsed;

	//	1�� ����� ���� �� ����
	if (m_FPSTimeElapsed > 1.0f)
	{
		m_FPS				= m_FPSCount;
		m_FPSCount			= 0;
		m_FPSTimeElapsed	-= 1.0f;
	}

	return m_timeElapsed;
}
