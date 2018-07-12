#pragma once

#include "TemplateSingletonA.h"
#include <Windows.h>

//	전방선언
//	이런 이름의 클래스가 있다. ( 함수의 프로토 타입과 같은 의미 )
//	사용 목적은 해당 클래스 이름의 포인터를 만드는데 사용
//	변수(객체) 선언은 불가능 하다.
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
	//	프레임 유지
	float Tick();
	//	FPS 설정
	unsigned long	GetFPS();
	void			SetFPS(unsigned long _lockFPS);

	//	경과시간 얻기
	float			GetElapsedTime();
	//	플레이 시간 얻기
	float			GetPlayTime();
	//	화면 FPS 출력
	void			Render(HDC _hdc, int _x, int _y);
};

#define		TIME	TimeMgr::GetInstance()