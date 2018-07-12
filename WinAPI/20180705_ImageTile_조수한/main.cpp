#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>

#include "TimeMgr.h"
#include "TileManager.h"

LPCSTR		g_winName = "HOW-SAMPLEWINDOW";

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int TILE_UNIT_SIZE = 50;
const int TILE_W_MAX = 15;
const int TILE_H_MAX = 15;

POINT	g_Pos;

float	g_PosX = 100.0f;
float	g_PosY = 100.0f;

int		g_Speed = 100;
int		g_camera_speed = 10;

void Init(HWND _hWnd)
{
	SIZE camSize = {WIN_WIDTH/2, WIN_HEIGHT/2};
	TILE_MGR.Init(_hWnd, camSize, TILE_UNIT_SIZE, TILE_W_MAX, TILE_H_MAX);
	TILE_MGR.getUpperLeft()->x = 50;
	TILE_MGR.getUpperLeft()->y = 50;
}
void Release()
{
}

void Update(float _elapseTime)
{
	
}
void Render(HDC _hdc)
{
	//TIME.Render(_hdc, 0, 0);
	TILE_MGR.Render(_hdc);
	TILE_MGR.DrawGrid(_hdc);
	TILE_MGR.DrawCheckPoint(_hdc);
}

//	콜백함수 : 윈도우가 대신 호출해준다.
LRESULT	CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_LBUTTONDOWN:
	{
		g_Pos.x = GET_X_LPARAM(_lParam);
		g_Pos.y = GET_Y_LPARAM(_lParam);
		TILE_MGR.TileClicked(g_Pos);
	}break;
	case WM_RBUTTONDOWN:
	{
		g_Pos.x = GET_X_LPARAM(_lParam);
		g_Pos.y = GET_Y_LPARAM(_lParam);
		TILE_MGR.TileChange(g_Pos);
	}break;
	case WM_MOUSEMOVE:
	{
		g_Pos.x = GET_X_LPARAM(_lParam);
		g_Pos.y = GET_Y_LPARAM(_lParam);
		TILE_MGR.SetMousePos(g_Pos);
	}break;
	case WM_KEYDOWN:
	{
		switch (_wParam)
		{
		case VK_RIGHT:
		{
			TILE_MGR.GoRight(g_camera_speed);
		}break;
		case VK_LEFT:
		{
			TILE_MGR.GoLeft(g_camera_speed);
		}break;
		case VK_UP:
		{
			TILE_MGR.GoUp(g_camera_speed);
		}break;
		case VK_DOWN:
		{
			TILE_MGR.GoDown(g_camera_speed);
		}break;
		case VK_SPACE:
		{
			TILE_MGR.ChangeMode();
		}break;
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
	HINSTANCE	_hInst,			//	프로세스 핸들
	HINSTANCE	_hPrevInst,		//	이전 프로세스 핸들( 현재 안씀 )
	LPSTR		_CmdParam,		//	명령줄 인수( char* )
	int			_CmdShow)		//	기본창 상태
{
	LPCSTR	g_className = "HOW";

	//	1. 윈도우 클래스 정의 및 등록
	WNDCLASS	wndClass = {};

	//	정보입력
	wndClass.lpszClassName = g_className;
	wndClass.hInstance = _hInst;
	wndClass.lpfnWndProc = WndProc;	//	함수포인터

	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//	배경색
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);		//	커서모양
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//	아이콘

	RegisterClass(&wndClass);

	//	실제 윈도우 크기 구하기
	int		wsOption = WS_OVERLAPPEDWINDOW;
	RECT	rt = { 0, 0, WIN_WIDTH, WIN_HEIGHT };

	//	윈도우 기본 옵션에 따른 실제 기본창 크기
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. 윈도우 생성 및 화면 띄우기
	HWND hWnd = CreateWindow(
		g_className, g_winName, wsOption, 100, 100,					
		rt.right - rt.left, rt.bottom - rt.top,	
		nullptr, nullptr, _hInst, nullptr);					

	if (hWnd == nullptr)
	{
		return 0;
	}

	//	초기화
	Init(hWnd);
	
	ShowWindow(hWnd, _CmdShow);

	//	3. 메시지 루프( 고객센터 )
	MSG	msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			//	탈출조건
			if (msg.message == WM_QUIT) break;

			//	메시지가 발생한 경우
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(TIME.Tick());

#pragma region RENDER
			HDC			hdc = GetDC(hWnd);

			HDC	backDC = CreateCompatibleDC(hdc);
			HBITMAP hBit = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
			HBITMAP hOldBit = SelectBitmap(backDC, hBit);

			RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
			FillRect(backDC, &rc, GetStockBrush(WHITE_BRUSH));

			Render(backDC);

			BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, backDC, 0, 0,SRCCOPY);

			DeleteObject(SelectBitmap(backDC, hOldBit));
			DeleteDC(backDC);

			ReleaseDC(hWnd, hdc);

#pragma endregion
		}
	}

	//	해제처리
	Release();

	return msg.wParam;
}