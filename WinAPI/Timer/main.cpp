#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>
#include "Timer.h"

//	timeGetTime �Լ��� ����ϱ� ���� ���̺귯�� �߰�
#pragma comment( lib, "winmm.lib" )

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 600;

POINT	g_Pos = {100, 100};
int		g_Count = 0;
int		g_totalSec = 0;
double	g_distancePerSecX = 0.5;
Timer timer;

void Update(DWORD _elapseTime)
{
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		g_Pos.x += (g_distancePerSecX * _elapseTime);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		g_Pos.x -= (g_distancePerSecX * _elapseTime);
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		timer.SetFPS(60);
	
	}
	else
	{
		timer.SetFPS(200);
		
	}
}

void Render(HDC _hdc)
{
	std::stringstream	text;
	text << g_totalSec << "��";
	TextOut(_hdc, g_Pos.x, g_Pos.y, text.str().c_str(), text.str().length());
	text.str("");
	text << "FPS : " << g_Count;
	TextOut(_hdc, 100, 130, text.str().c_str(), text.str().length());
	text.str("");
	text << "�ʴ� �̵��Ÿ� : " << g_distancePerSecX;
	TextOut(_hdc, 100, 70, text.str().c_str(), text.str().length());
}

//	�ݹ��Լ� : �����찡 ��� ȣ�����ش�.
LRESULT	CALLBACK WndProc(
	HWND		_hWnd, 
	UINT		_message, 
	WPARAM		_wParam, 
	LPARAM		_lParam)
{
	switch (_message)
	{
	case WM_KEYDOWN:
	{
		switch (_wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		default:
			break;
		}
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;

	}

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}


int APIENTRY WinMain(
	HINSTANCE	_hInst,			//	���μ��� �ڵ�
	HINSTANCE	_hPrevInst,		//	���� ���μ��� �ڵ�( ���� �Ⱦ� )
	LPSTR		_CmdParam,		//	����� �μ�( char* )
	int			_CmdShow)		//	�⺻â ����
{
	LPCSTR	g_className		= "HOW";

	//	1. ������ Ŭ���� ���� �� ���
	WNDCLASS	wndClass	= {};

	//	�����Է�
	wndClass.lpszClassName	= g_className;
	wndClass.hInstance		= _hInst;
	wndClass.lpfnWndProc	= WndProc;	//	�Լ�������

	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	//	����
	wndClass.hCursor		= LoadCursor(nullptr, IDC_ARROW);		//	Ŀ�����
	wndClass.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);	//	������

	//wndClass.style			= CS_HREDRAW | CS_VREDRAW;			//	��Ʈ�÷���

	RegisterClass(&wndClass);

	//	���� ������ ũ�� ���ϱ�
	int		wsOption	= WS_OVERLAPPEDWINDOW;
	RECT	rt			= { 0, 0, WIN_WIDTH, WIN_HEIGHT };

	//	������ �⺻ �ɼǿ� ���� ���� �⺻â ũ��
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. ������ ���� �� ȭ�� ����
	HWND hWnd = CreateWindow(
		g_className,				//	���� ������Ŭ���� �̸�
		g_winName,					//	������ Ÿ��Ʋ
		wsOption,					//	������ �⺻ �ɼ�( ĸ�ǹ�, �ִ�ȭ��ư, �ּҹ�ư. )
		0, 0,					//	������ ���� ��ġ
		rt.right - rt.left, rt.bottom - rt.top,	//	������ ũ��
		nullptr,					//	�θ��������ڵ�
		nullptr,					//	�޴��ڵ�
		_hInst,						//	������ ���μ����ڵ�
		nullptr);					//	������...

	if (hWnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hWnd, _CmdShow);

	//	3. �޽��� ����( ������ )
	MSG	msg = {};

	int FPSTime = 0;
	int Cnt = 0;
	timer.SetFPS(60);

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			//	Ż������
			if (msg.message == WM_QUIT) break;

			//	�޽����� �߻��� ���
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD elapseTime = timer.Sleep();

			if (FPSTime >= 1000)
			{
				g_Count = Cnt;
				FPSTime = 0;
				Cnt = 0;
				g_totalSec++;
			}
			else
			{
				FPSTime += elapseTime;
				Cnt++;
			}

			//	�޽����� �߻����� ���� ���
			Update(elapseTime);

#pragma region RENDER
			HDC			hdc = GetDC(hWnd);

			//	����DC �� ������ DC�� ���� ����
			HDC	backDC = CreateCompatibleDC(hdc);
			//	ȭ��ũ�⸸�� HBITMAP ����
			HBITMAP hBit = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
			//	���ο� DC�� HBITMAP ����
			HBITMAP hOldBit = SelectBitmap(backDC, hBit);
			//	�⺻ ���� ĥ�ϱ�
			RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
			FillRect(backDC, &rc, GetStockBrush(WHITE_BRUSH));

			//	���ο� DC�� �̹����� �׸���.
			Render(backDC);

			//	���� DC�� ���ο� DC�� ������ �����Ѵ�.
			BitBlt(
				hdc,						//	��� DC
				0, 0,						//	����� ���� ��ġ
				WIN_WIDTH, WIN_HEIGHT,		//	����� ũ��
				backDC,						//	���� DC
				0, 0,						//	���� ���� ��ġ
				SRCCOPY);					//	���� �ɼ�

											//	���θ������ HBITMAP, HDC �� �����Ѵ�.
			DeleteObject(SelectBitmap(backDC, hOldBit));
			DeleteDC(backDC);

			ReleaseDC(hWnd, hdc);

#pragma endregion
		}
	}

	return msg.wParam;
}