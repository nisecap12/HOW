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

//	콜백함수 : 윈도우가 대신 호출해준다.
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
	HINSTANCE	_hInst,			//	프로세스 핸들
	HINSTANCE	_hPrevInst,		//	이전 프로세스 핸들( 현재 안씀 )
	LPSTR		_CmdParam,		//	명령줄 인수( char* )
	int			_CmdShow)		//	기본창 상태
{
	LPCSTR	g_className		= "HOW";

	//	1. 윈도우 클래스 정의 및 등록
	WNDCLASS	wndClass	= {};

	//	정보입력
	wndClass.lpszClassName	= g_className;
	wndClass.hInstance		= _hInst;
	wndClass.lpfnWndProc	= WndProc;	//	함수포인터

	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	//	배경색
	wndClass.hCursor		= LoadCursor(nullptr, IDC_ARROW);		//	커서모양
	wndClass.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);	//	아이콘

	//wndClass.style			= CS_HREDRAW | CS_VREDRAW;			//	비트플래그

	RegisterClass(&wndClass);

	//	실제 윈도우 크기 구하기
	int		wsOption	= WS_OVERLAPPEDWINDOW;
	RECT	rt			= { 0, 0, WIN_WIDTH, WIN_HEIGHT };

	//	윈도우 기본 옵션에 따른 실제 기본창 크기
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. 윈도우 생성 및 화면 띄우기
	HWND hWnd = CreateWindow(
		g_className,				//	위의 윈도우클래스 이름
		g_winName,					//	윈도우 타이틀
		wsOption,					//	윈도우 기본 옵션( 캡션바, 최대화버튼, 최소버튼. )
		900, 0,					//	윈도우 시작 위치
		rt.right - rt.left, rt.bottom - rt.top,	//	윈도우 크기
		nullptr,					//	부모윈도우핸들
		nullptr,					//	메뉴핸들
		_hInst,						//	연결할 프로세스핸들
		nullptr);					//	무쓸모...

	if (hWnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hWnd, _CmdShow);

	//	3. 메시지 루프( 고객센터 )
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
			//메시지가 발생한 경우
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//메시지가 발생하지 않은 경우
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

			//	기존DC 와 동일한 DC를 새로 생성
			HDC	backDC = CreateCompatibleDC(hdc);
			//	화면크기만함 HBITMAP 생성
			HBITMAP hBit = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
			//	새로운 DC에 HBITMAP 장착
			HBITMAP hOldBit = SelectBitmap(backDC, hBit);
			//	기본 배경색 칠하기
			RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
			FillRect(backDC, &rc, GetStockBrush(WHITE_BRUSH));

			//	새로운 DC에 이미지를 그린다.
			Render(backDC);
			std::stringstream s;
			s << CntSec << " fps";
			TextOut(backDC, 500, 520, s.str().c_str(), s.str().length());

			//	기존 DC에 새로운 DC의 정보를 복사한다.
			BitBlt(
				hdc,						//	대상 DC
				0, 0,						//	대상의 시작 위치
				WIN_WIDTH, WIN_HEIGHT,		//	대상의 크기
				backDC,						//	원본 DC
				0, 0,						//	원본 시작 위치
				SRCCOPY);					//	복사 옵션

											//	새로만들었던 HBITMAP, HDC 를 삭제한다.
			DeleteObject(SelectBitmap(backDC, hOldBit));
			DeleteDC(backDC);
			ReleaseDC(hWnd, hdc);
			
		}
	}

	KillTimer(hWnd, 0);

	return msg.wParam;
}