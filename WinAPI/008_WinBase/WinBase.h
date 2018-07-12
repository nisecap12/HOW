#pragma once

#include <Windows.h>
#include <string>

class WinBase
{
	std::string		m_WinName;
	std::string		m_ClassName;
	SIZE			m_WinSize;

	HINSTANCE		m_hInst;
	HWND			m_hWnd;

private:
	//	1. 윈도우클래스 정보입력 및 등록
	VOID	RegiWinClass();
	//	2. 윈도우 생성
	BOOL	MakingWindow();

	//	메시지 프로시저
	LRESULT	WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	//	friend
	friend	LRESULT	CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);


	//	Update
	VOID	Update();
	//	Render
	VOID	Render(HDC _hdc);

public:
	BOOL	NewWindow(HINSTANCE _hInst, int _width, int _height, LPCSTR _winName);

	//	3. 메시지 루프
	int		MessageLoop();
};

