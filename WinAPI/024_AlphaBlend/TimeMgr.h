#pragma once

#include "TemplateSingletonA.h"
#include <Windows.h>

//	���漱��
//	�̷� �̸��� Ŭ������ �ִ�. ( �Լ��� ������ Ÿ�԰� ���� �ǹ� )
//	��� ������ �ش� Ŭ���� �̸��� �����͸� ����µ� ���
//	����(��ü) ������ �Ұ��� �ϴ�.
class Time;

class TimeMgr 
	: public TemplateSingletonA<TimeMgr>
{
	friend TemplateSingletonA;
private:
	Time	*m_timer = nullptr;

private:
	TimeMgr();
	~TimeMgr();

public:
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