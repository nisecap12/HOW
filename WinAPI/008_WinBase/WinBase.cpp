#undef UNICODE
#include "WinBase.h"

WinBase	*g_pMainWindow = nullptr;

//	�ݹ��Լ� : �����찡 ��� ȣ�����ش�.
LRESULT	CALLBACK MyWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return g_pMainWindow->WndProc(_hWnd, _message, _wParam, _lParam);
}
VOID WinBase::RegiWinClass()
{
	//	1. ������ Ŭ���� ���� �� ���
	WNDCLASS	wndClass = {};

	//	�����Է�
	wndClass.lpszClassName	= m_ClassName.c_str();
	wndClass.hInstance		= m_hInst;
	wndClass.lpfnWndProc	= MyWndProc;							//	�Լ�������

	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	//	����
	wndClass.hCursor		= LoadCursor(nullptr, IDC_ARROW);		//	Ŀ�����
	wndClass.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);	//	������

	RegisterClass(&wndClass);
}
BOOL WinBase::MakingWindow()
{
	g_pMainWindow = this;

	//	���� ������ ũ�� ���ϱ�
	int		wsOption = WS_OVERLAPPEDWINDOW;
	RECT	rt = { 0, 0, m_WinSize.cx, m_WinSize.cy };

	//	������ �⺻ �ɼǿ� ���� ���� �⺻â ũ��
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. ������ ���� �� ȭ�� ����
	m_hWnd = CreateWindow(
		m_ClassName.c_str(),		//	���� ������Ŭ���� �̸�
		m_WinName.c_str(),			//	������ Ÿ��Ʋ
		wsOption,					//	������ �⺻ �ɼ�( ĸ�ǹ�, �ִ�ȭ��ư, �ּҹ�ư. )
		-1920, 0,					//	������ ���� ��ġ
		rt.right - rt.left, rt.bottom - rt.top,	//	������ ũ��
		nullptr,					//	�θ��������ڵ�
		nullptr,					//	�޴��ڵ�
		m_hInst,					//	������ ���μ����ڵ�
		nullptr);					//	������...

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
	//	Ÿ�̸�
	SetTimer(
		m_hWnd,		//	� â�� Ÿ�̸��ΰ�?
		0,			//	�ش� Ÿ�̸���  ID ��ȣ
		1,			//	ȣ�� �ֱ� 1/1000 ����, ( �ִ�� 1/100 �� ����... )
		nullptr);	//	ȣ�� �Լ��ε�... nullptr �̸� WM_TIMER �޽��� �߻�

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

	//	1. ������ Ŭ���� ���� �Է� �� ���
	RegiWinClass();

	//	2. ������ ����
	if (MakingWindow() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}
