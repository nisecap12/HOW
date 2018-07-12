#undef UNICODE
#include <Windows.h>
#include <windowsx.h>

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 800;

HDC		g_ImgDC		= nullptr;
HBITMAP	g_ImgBit	= nullptr;
HBITMAP g_OldBit	= nullptr;

void	LoadImg(HWND _hWnd)
{
	HDC hdc = GetDC(_hWnd);
	
	g_ImgDC		= CreateCompatibleDC(hdc);
	g_ImgBit	= (HBITMAP)LoadImage(
		nullptr,			//	종속된 프로세스명 ( 파일로 불러올때는 nullptr )
		"Sample2.bmp",		//	파일명
		IMAGE_BITMAP,		//	이미지 유형( ICON, CURSOR, BITMAP )
		0, 0,			//	이미지의 크기( 0을 넣으면 이미지 본래의 크기 )
		LR_LOADFROMFILE);	//	파일에서 직접 불러온다는 옵션값
	g_OldBit	= SelectBitmap(g_ImgDC, g_ImgBit);

	ReleaseDC(_hWnd, hdc);
}

void	DeleteImg()
{
	if (g_ImgDC != nullptr)
	{
		DeleteObject(SelectBitmap(g_ImgDC, g_OldBit));
		DeleteDC(g_ImgDC);
	}
}

void Update()
{
}

void Render(HDC _hdc)
{
	if (g_ImgDC != nullptr)
	{
		BitBlt(_hdc, 0, 0, 300, 300, g_ImgDC, 0, 0, SRCCOPY);

		BitBlt(_hdc, 200, 0, 100, 100, g_ImgDC, 50, 0, SRCCOPY);

		BitBlt(_hdc, 350, 0, 100, 100, g_ImgDC, 0, 0, SRCCOPY);

		StretchBlt(_hdc, 0, 100, 200, 200, g_ImgDC, 0, 0, 100, 65, SRCCOPY);

		GdiTransparentBlt(_hdc, 0, 600, 200, 200, g_ImgDC, 0, 0, 240, 65, RGB(252, 0, 252));

		BLENDFUNCTION	bf = {};

		if (GetKeyState(VK_SPACE) & 0x8000)
			bf.SourceConstantAlpha = 255;
		else
			bf.SourceConstantAlpha = 120;

		GdiAlphaBlend(_hdc, 300, 600, 200, 200, g_ImgDC, 0, 0, 240, 65, bf);
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
	case WM_RBUTTONDOWN:
		LoadImg(_hWnd);
		break;

	case WM_KEYDOWN:
	{
		switch (_wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}break;

	case WM_TIMER:
	{
		Update();
		InvalidateRect(_hWnd, nullptr, FALSE);
	}break;

	case WM_PAINT:
	{
		PAINTSTRUCT	ps;
		HDC			hdc = BeginPaint(_hWnd, &ps);

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

		EndPaint(_hWnd, &ps);
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
		0,							//	해당 타이머의  ID 번호
		1,							//	호출 주기 1/1000 단위, ( 최대는 1/100 초 정도... )
		nullptr);					//	호출 함수인데... nullptr 이면 WM_TIMER 메시지 발생

	//	3. 메시지 루프( 고객센터 )
	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(hWnd, 0);

	//	이미지 삭제
	DeleteImg();

	return msg.wParam;
}