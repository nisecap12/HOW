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
		nullptr,			//	���ӵ� ���μ����� ( ���Ϸ� �ҷ��ö��� nullptr )
		"Sample2.bmp",		//	���ϸ�
		IMAGE_BITMAP,		//	�̹��� ����( ICON, CURSOR, BITMAP )
		0, 0,			//	�̹����� ũ��( 0�� ������ �̹��� ������ ũ�� )
		LR_LOADFROMFILE);	//	���Ͽ��� ���� �ҷ��´ٴ� �ɼǰ�
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

//	�ݹ��Լ� : �����찡 ��� ȣ�����ش�.
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

	ShowWindow(hWnd, _CmdShow);


	//	Ÿ�̸�
	SetTimer(
		hWnd,						//	� â�� Ÿ�̸��ΰ�?
		0,							//	�ش� Ÿ�̸���  ID ��ȣ
		1,							//	ȣ�� �ֱ� 1/1000 ����, ( �ִ�� 1/100 �� ����... )
		nullptr);					//	ȣ�� �Լ��ε�... nullptr �̸� WM_TIMER �޽��� �߻�

	//	3. �޽��� ����( ������ )
	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(hWnd, 0);

	//	�̹��� ����
	DeleteImg();

	return msg.wParam;
}