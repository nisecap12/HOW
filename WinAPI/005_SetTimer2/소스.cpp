#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <list>


LPCSTR	g_winName	= "HOW-SAMPLEWINDOW";
const int WIN_WIDTH		= 800;
const int WIN_HEIGHT	= 600;

enum
{
	TIMER_BASEUPDATE,
	TIMER_CREATERECT
};

/*
	랜덤 위치, 랜덤 색상으로 사각형이 1초마다 생겼으면 좋겠다!!!!
*/
struct  SampleObject
{
	POINT		pos;
	int			size;
	COLORREF	color;
};

int g_MouseX = 0;
int g_MouseY = 0;

std::list<SampleObject>	 g_ObjList;


//	편하게 쓰기 위해서 사용자가 직접 한번더 포장한다.
void MyRectangle(HDC _hdc, int _posX, int _posY, int _size, COLORREF _color)
{
	//	새로운 칠하기 색 만들기
	HBRUSH	brush		= CreateSolidBrush(_color);
	//	hdc 에 칠하기 색 변경하기
	HBRUSH	oldBrush	= (HBRUSH)SelectObject(_hdc, brush);

	Rectangle(_hdc, 
		_posX - _size/2, 
		_posY - _size/2,
		_posX + _size/2,
		_posY + _size / 2);

	//	칠하기 색 원위치
	SelectObject(_hdc, oldBrush);
	//	생성했던 브러쉬 삭제
	DeleteObject(brush);
}
void MyRectangle(HDC _hdc, SampleObject _obj)
{
	MyRectangle(_hdc, _obj.pos.x, _obj.pos.y, _obj.size, _obj.color);
}

void Update()
{
}

void Render(HDC _hdc)
{
	for (auto &i : g_ObjList)
	{
		MyRectangle(_hdc, i);
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

		default:
			break;
		}

	}break;

	case WM_TIMER:
	{
		switch (_wParam)
		{
		case TIMER_BASEUPDATE:
			Update();
			InvalidateRect(_hWnd, nullptr, TRUE);
			break;

		case TIMER_CREATERECT:
		{
			SampleObject	New;

			New.pos.x	= rand() % WIN_WIDTH;
			New.pos.y	= rand() % WIN_HEIGHT;
			New.size	= rand() % 200 + 100;
			New.color	= RGB(rand() % 255, rand() % 255, rand() % 255);

			g_ObjList.push_back(New);
		}
		break;
		}
		
	}break;

	case WM_PAINT:
	{
		PAINTSTRUCT	ps;
		HDC			hdc = BeginPaint(_hWnd, &ps);

		Render(hdc);

		EndPaint(_hWnd, &ps);
	}break;

	case WM_MOUSEMOVE:
	{
		g_MouseX = GET_X_LPARAM(_lParam);
		g_MouseY = GET_Y_LPARAM(_lParam);
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

	ShowWindow(hWnd, _CmdShow);


	//	타이머
	SetTimer(
		hWnd,						//	어떤 창의 타이머인가?
		TIMER_BASEUPDATE,			//	해당 타이머의  ID 번호
		1,							//	호출 주기 1/1000 단위, ( 최대는 1/100 초 정도... )
		nullptr);					//	호출 함수인데... nullptr 이면 WM_TIMER 메시지 발생

	SetTimer(
		hWnd,						//	어떤 창의 타이머인가?
		TIMER_CREATERECT,			//	해당 타이머의  ID 번호
		1000,						//	호출 주기 1/1000 단위, ( 최대는 1/100 초 정도... )
		nullptr);					//	호출 함수인데... nullptr 이면 WM_TIMER 메시지 발생
	
	//	3. 메시지 루프( 고객센터 )
	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		DispatchMessage(&msg);
	}


	KillTimer(hWnd, TIMER_BASEUPDATE);
	KillTimer(hWnd, TIMER_CREATERECT);

	return msg.wParam;
}