#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>
#include "Image.h"
#include "ImageAnimation.h"
#include "Timer.h"

#pragma comment(lib, "winmm.lib")

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 800;

Image ImgArray[2] = { Image("Sample_0.bmp"), Image("Sample2.bmp") };
ImageAnimation anim;
int Cnt = 0;

void Update()
{
	
}

void Render(HDC _hdc)
{
	for (auto a : ImgArray)
	{
		a.CreateImage(_hdc);
		a.ShowImage(_hdc);
	}
	
	if (anim.CreateAnimation(_hdc))
	{
		anim.ShowImage(_hdc);
	}
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
		900, 0,					//	������ ���� ��ġ
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

	ImgArray[0].SetPosition(0, 0);
	ImgArray[1].SetPosition(0, 300);
	vector<string> animPath;
	for (int i = 0; i < 4; i++)
	{
		stringstream s;
		s << "Sample_" << i << ".bmp";
		animPath.push_back(s.str().c_str());
	}
	anim.SetPaths(animPath);
	anim.SetPosition(500, 0);

	Timer timer;
	int FPSTime = 0;
	int CntSec = 0;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//�޽����� �߻��� ���
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�޽����� �߻����� ���� ���
			DWORD elapseTime = timer.Sleep();
		
			if (FPSTime >= 1000)
			{
				CntSec = Cnt;
				FPSTime = 0;
				Cnt = 0;
			}
			else
			{
				FPSTime += elapseTime;
				Cnt++;
			}

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
			std::stringstream s;
			s << CntSec << " fps";
			TextOut(backDC, 500, 520, s.str().c_str(), s.str().length());

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
			
		}
	}

	KillTimer(hWnd, 0);

	return msg.wParam;
}