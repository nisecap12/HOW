#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>
#include <iomanip>

#include "TimeMgr.h"
#include "ImgMgr.h"

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 600;

POINT	g_ClickPos	= {};
POINT	g_CamPos	= {};
SIZE	g_BGSize	= {};
SIZE	g_MapSize	= {3000, 3000};

////////////////////////////////////////////////////////
POINT	g_tileStartPos	= {100, 200};
int		g_tileSize		= 100;

////////////////////////////////////////////////////////

void Init(HWND _hWnd)
{
	g_BGSize.cx = WIN_WIDTH		/ 2;
	g_BGSize.cy = WIN_HEIGHT	/ 2;
}
void Release()
{
}

void Update(float _elapseTime)
{
	if (GetKeyState(VK_ADD) & 0x8000)
	{
		if(g_tileSize < 200)		g_tileSize++;
	}
	if (GetKeyState(VK_SUBTRACT) & 0x8000)
	{
		if(g_tileSize > 100)		g_tileSize--;
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		g_CamPos.x-=10;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		g_CamPos.x+=10;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		g_CamPos.y-=10;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		g_CamPos.y+=10;
	}
	if (g_CamPos.x < 0)								g_CamPos.x = 0;
	if (g_CamPos.x + g_BGSize.cx > g_MapSize.cx)	g_CamPos.x = g_MapSize.cx - g_BGSize.cx;
	if (g_CamPos.y < 0)								g_CamPos.y = 0;
	if (g_CamPos.y + g_BGSize.cy > g_MapSize.cy)	g_CamPos.y = g_MapSize.cy - g_BGSize.cy;
}
void Render(HDC _hdc)
{
	//	가로 세로 개수
	int		cellWidth	= g_BGSize.cx / g_tileSize + 1;
	int		cellHeight	= g_BGSize.cy / g_tileSize + 1;

	//	시작 타일 좌표
	int		startPosX	= g_CamPos.x / g_tileSize;
	int		startPosY	= g_CamPos.y / g_tileSize;

	//	외각선
	Rectangle(_hdc,
		g_tileStartPos.x,
		g_tileStartPos.y,
		g_tileStartPos.x + g_BGSize.cx,
		g_tileStartPos.y + g_BGSize.cy );

	//	세로
	for (int x = 0; x <= cellWidth; x++)
	{
		int winPosX = g_tileSize * (startPosX + x) - g_CamPos.x;

		if (winPosX < 0 ||	
			winPosX > g_BGSize.cx)
		{
			continue;
		}

		winPosX += g_tileStartPos.x;

		MoveToEx(_hdc,	winPosX, g_tileStartPos.y, nullptr);
		LineTo(_hdc,	winPosX, g_tileStartPos.y + g_BGSize.cy);
	}

	//	가로
	for (int y = 0; y <= cellHeight; y++)
	{
		int winPosY = g_tileSize * (startPosY + y) - g_CamPos.y;

		if (winPosY < 0 ||
			winPosY > g_BGSize.cy)
		{
			continue;
		}

		winPosY += g_tileStartPos.y;

		MoveToEx(_hdc,	g_tileStartPos.x				,winPosY, nullptr);
		LineTo(_hdc,	g_tileStartPos.x + g_BGSize.cx	,winPosY);
	}

	//	좌표
	for (int y = 0; y < cellHeight; y++)
	{
		for (int x = 0; x < cellWidth; x++)
		{
			int winPosX = g_tileSize * (startPosX + x) - g_CamPos.x;

			if (winPosX < 0 ||	winPosX >= g_BGSize.cx)
			{
				continue;
			}

			int winPosY = g_tileSize * (startPosY + y) - g_CamPos.y;

			if (winPosY < 0 ||	winPosY >= g_BGSize.cy)
			{
				continue;
			}

			winPosX += g_tileStartPos.x;
			winPosY += g_tileStartPos.y;

			std::stringstream	ss;
			ss << "[" << std::setw(3) << startPosX + x << ", " << std::setw(3) << startPosY + y << "]";
			TextOutA(_hdc, winPosX + 5, winPosY + 5, ss.str().c_str(), ss.str().length());
		}

		
	}

	std::stringstream	ss;
	ss << "Cam( " << std::setw(4) << g_CamPos.x << ", " << std::setw(4) << g_CamPos.y << ")";
	TextOutA(_hdc, 0, 40, ss.str().c_str(), ss.str().length());

	ss.str("");
	ss << "Click( " << std::setw(4) << g_ClickPos.x << ", " << std::setw(4) << g_ClickPos.y << ")";
	TextOutA(_hdc, 0, 60, ss.str().c_str(), ss.str().length());

	TIME.Render(_hdc, 0, 0);
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

		default:
			break;
		}
	}break;

	case WM_RBUTTONDOWN:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(_lParam);
		pt.y = GET_Y_LPARAM(_lParam);

		pt.x += g_CamPos.x;
		pt.y += g_CamPos.y;

		pt.x -= g_tileStartPos.x;
		pt.y -= g_tileStartPos.y;

		g_ClickPos.x = pt.x / g_tileSize;
		g_ClickPos.y = pt.y / g_tileSize;
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
		-1920, 0,					//	윈도우 시작 위치
		rt.right - rt.left, rt.bottom - rt.top,	//	윈도우 크기
		nullptr,					//	부모윈도우핸들
		nullptr,					//	메뉴핸들
		_hInst,						//	연결할 프로세스핸들
		nullptr);					//	무쓸모...

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

			ImgClass	back(hWnd, WIN_WIDTH, WIN_HEIGHT);
			//	새로운 DC에 이미지를 그린다.
			Render(back.GetImgDC());
			//	화면 hdc 에 백버퍼 이미지를 그린다.
			back.Render(hWnd);
		}
	}

	//	해제처리
	Release();

	return msg.wParam;
}