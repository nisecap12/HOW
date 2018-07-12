#undef UNICODE
#include <Windows.h>
#include <windowsx.h>

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 600;

void Update()
{
}

void Render(HDC _hdc)
{
	//	1. �簢�� �׸���
	Rectangle(_hdc, 100, 100, 200, 200);

	HBRUSH hBrush		= GetStockBrush(BLACK_BRUSH);
	HBRUSH hOldBrush	= SelectBrush(_hdc, hBrush);

	//	2. �� �׸���
	Ellipse(_hdc, 300, 300, 500, 500);

	SelectBrush(_hdc, hOldBrush);



	hBrush		= CreateSolidBrush(RGB(255, 0, 0));
	hOldBrush	= SelectBrush(_hdc, hBrush);

	Rectangle(_hdc, 500, 100, 600, 200);

	DeleteObject(SelectBrush(_hdc, hOldBrush));
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

	return msg.wParam;
}