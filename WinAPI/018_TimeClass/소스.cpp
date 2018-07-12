#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>


#include "TimeMgr.h"

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 600;

POINT	g_Pos	= {100, 100};

float	g_PosX = 100.0f;
float	g_PosY = 100.0f;

int		g_Speed = 100;

void Init(HWND _hWnd)
{
}
void Release()
{
}

void Update(float _elapseTime)
{
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		g_PosX += g_Speed  * _elapseTime;
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		g_PosY -= g_Speed * _elapseTime;
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		TIME.SetFPS(300);
	}
	else
	{
		TIME.SetFPS(60);
	}
}
void Render(HDC _hdc)
{
	TIME.Render(_hdc, 0, 0);

	RECT	rt = { -50, -50, 50, 50 };
	OffsetRect(&rt, long(g_PosX), long(g_PosY));

	Rectangle(_hdc, rt.left, rt.top, rt.right, rt.bottom);
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
		-1920, 0,					//	������ ���� ��ġ
		rt.right - rt.left, rt.bottom - rt.top,	//	������ ũ��
		nullptr,					//	�θ��������ڵ�
		nullptr,					//	�޴��ڵ�
		_hInst,						//	������ ���μ����ڵ�
		nullptr);					//	������...

	if (hWnd == nullptr)
	{
		return 0;
	}

	//	�ʱ�ȭ
	Init(hWnd);

	ShowWindow(hWnd, _CmdShow);

	//	3. �޽��� ����( ������ )
	MSG	msg = {};

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
			Update(TIME.Tick());

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

	//	����ó��
	Release();

	return msg.wParam;
}