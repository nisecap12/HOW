#pragma once

class Time
{
	bool			m_isHighTimer		= false;			//	���ػ� �ϵ���� Ÿ�̸� ���� ����
	float			m_timeResolusion	= 0.0f;				//	�ð� �ػ� ( timeGetTime == 1/1000 �� )
	float			m_timeElapsed		= 0.0f;				//	��� �ð�
	__int64			m_lastTime			= 0;				//	���� �ð�
	__int64			m_nowTime			= 0;				//	���� �ð�

	unsigned long	m_FPS				= 0;				//	Frame Per Second
	unsigned long	m_FPSCount			= 0;				//	1�� ������ ī���ÿ�
	float			m_FPSTimeElapsed	= 0.0f;				//	FPS üũ�� 1�� ����ð� ������
	
	unsigned long	m_lockFPS			= 60;				//	�⺻ ���� FPS
	float			m_lockTime			= 1.0f / m_lockFPS;	//	1 �������� �ð�

	float			m_playTime			= 0.0f;

private:
	void	Initialize();
	void	Release();

public:
	Time();
	~Time();

	//	������ ����
	float Tick();

	//	FPS ���
	unsigned long	GetFPS()
	{
		return m_FPS;
	}
	//	FPS ����
	void	SetFPS(unsigned long _lockFPS)
	{
		if (_lockFPS == 0 || _lockFPS == m_lockFPS)
			return;

		m_lockFPS	= _lockFPS;
		m_lockTime	= 1.0f / m_lockFPS;
	}

	//	����ð� ���
	float			GetElapsedTime()
	{
		return m_timeElapsed;
	}
	//	�÷��� �ð� ���
	float			GetPlayTime()
	{
		return m_playTime;
	}
};

