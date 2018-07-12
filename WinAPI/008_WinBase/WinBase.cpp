#undef UNICODE
#include "WinBase.h"

WinBase	*g_pMainWindow = nullptr;

//	콜백함수 : 윈도우가 대신 호출해준다.
LRESULT	CALLBACK MyWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return g_pMainWindow->WndProc(_hWnd, _message, _wParam, _lParam);
}
VOID WinBase::RegiWinClass()
{
	//	1. 윈도우 클래스 정의 및 등록
	WNDCLASS	wndClass = {};

	//	정보입력
	wndClass.lpszClassName	= m_ClassName.c_str();
	wndClass.hInstance		= m_hInst;
	wndClass.lpfnWndProc	= MyWndProc;							//	함수포인터

	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	//	배경색
	wndClass.hCursor		= LoadCursor(nullptr, IDC_ARROW);		//	커서모양
	wndClass.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);	//	아이콘

	RegisterClass(&wndClass);
}
BOOL WinBase::MakingWindow()
{
	g_pMainWindow = this;

	//	실제 윈도우 크기 구하기
	int		wsOption = WS_OVERLAPPEDWINDOW;
	RECT	rt = { 0, 0, m_WinSize.cx, m_WinSize.cy };

	//	윈도우 기본 옵션에 따른 실제 기본창 크기
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. 윈도우 생성 및 화면 띄우기
	m_hWnd = CreateWindow(
		m_ClassName.c_str(),		//	위의 윈도우클래스 이름
		m_WinName.c_str(),			//	윈도우 타이틀
		wsOption,					//	윈도우 기본 옵션( 캡션바, 최대화버튼, 최소버튼. )
		-1920, 0,					//	윈도우 시작 위치
		rt.right - rt.left, rt.bottom - rt.top,	//	윈도우 크기
		nullptr,					//	부모윈도우핸들
		nullptr,					//	메뉴핸들
		m_hInst,					//	연결할 프로세스핸들
		nullptr);					//	무쓸모...

	if (m_hWnd == nullptr)
	{
		g_pMainWindow = nullptr;
		return FALSE;
	}

	ShowWindow(m_hWnd, SW_SHOW);

	return TRUE;
}

LRESULT WinBase::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
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
		Update();
		InvalidateRect(_hWnd, nullptr, TRUE);
	}break;

	case WM_PAINT:
	{
		PAINTSTRUCT	ps;
		HDC			hdc = BeginPaint(_hWnd, &ps);

		Render(hdc);

		EndPaint(_hWnd, &ps);
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;

	}

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}

VOID WinBase::Update()
{
	return VOID();
}

VOID WinBase::Render(HDC _hdc)
{
	return VOID();
}


int WinBase::MessageLoop()
{
	//	타이머
	SetTimer(
		m_hWnd,		//	어떤 창의 타이머인가?
		0,			//	해당 타이머의  ID 번호
		1,			//	호출 주기 1/1000 단위, ( 최대는 1/100 초 정도... )
		nullptr);	//	호출 함수인데... nullptr 이면 WM_TIMER 메시지 발생

	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		DispatchMessage(&msg);
	}

	KillTimer(m_hWnd, 0);

	return 0;
}
BOOL WinBase::NewWindow(HINSTANCE _hInst, int _width, int _height, LPCSTR _winName)
{
	m_hInst			= _hInst;

	m_WinSize.cx	= _width;
	m_WinSize.cy	= _height;

	m_ClassName		=	_winName;
	m_WinName		=	_winName;

	//	1. 윈도우 클래스 정보 입력 및 등록
	RegiWinClass();

	//	2. 윈도우 생성
	if (MakingWindow() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}
