#undef UNICODE
#include <Windows.h>


LPCSTR	g_winName	= "HOW-SAMPLEWINDOW";
const int WIDTH		= 800;
const int HEIGHT	= 600;

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

	case WM_RBUTTONDOWN:
	{
		MessageBox(_hWnd, "�ϳ����� ���� ���°�?\n���¾��� ���� ���°�?", "�⼮60��", MB_YESNOCANCEL);
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
	RECT	rt			= { 0, 0, WIDTH, HEIGHT };

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
	
	//	3. �޽��� ����( ������ )
	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		DispatchMessage(&msg);
	}

	return msg.wParam;
}