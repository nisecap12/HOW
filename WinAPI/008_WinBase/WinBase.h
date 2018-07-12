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
	//	1. ������Ŭ���� �����Է� �� ���
	VOID	RegiWinClass();
	//	2. ������ ����
	BOOL	MakingWindow();

	//	�޽��� ���ν���
	LRESULT	WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	//	friend
	friend	LRESULT	CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);


	//	Update
	VOID	Update();
	//	Render
	VOID	Render(HDC _hdc);

public:
	BOOL	NewWindow(HINSTANCE _hInst, int _width, int _height, LPCSTR _winName);

	//	3. �޽��� ����
	int		MessageLoop();
};

