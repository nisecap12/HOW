#undef UNICODE
#include <Windows.h>
#include <windowsx.h>


LPCSTR	g_winName	= "HOW-SAMPLEWINDOW";
const int WIDTH		= 800;
const int HEIGHT	= 600;

int	g_PosX = 100;

int g_MouseX = 0;
int g_MouseY = 0;

void Update()
{

}

void Render(HDC _hdc)
{
	{
		//	���ο� ĥ�ϱ� �� �����
		HBRUSH	brush = CreateSolidBrush(RGB(255, 0, 0));
		//	hdc �� ĥ�ϱ� �� �����ϱ�
		HBRUSH	oldBrush = (HBRUSH)SelectObject(_hdc, brush);

		Rectangle(
			_hdc,
			g_MouseX - 100,
			g_MouseY - 100,
			g_MouseX + 100,
			g_MouseY + 100);

		//	ĥ�ϱ� �� ����ġ
		SelectObject(_hdc, oldBrush);
		//	�����ߴ� �귯�� ����
		DeleteObject(brush);
	}
	
	{
		//	���ο� ĥ�ϱ� �� �����( GetStock �� ��� �ǵ������� ����� �ȵȴ�. )
		HBRUSH	brush		= (HBRUSH)GetStockObject(BLACK_BRUSH);
		//	hdc �� ĥ�ϱ� �� �����ϱ�
		HBRUSH	oldBrush	= (HBRUSH)SelectObject(_hdc, brush);

		Rectangle(_hdc, 100, 100, 200, 200);

		//	ĥ�ϱ� �� ����ġ
		SelectObject(_hdc, oldBrush);
	}
	

	{
		//	���ο� ĥ�ϱ� �� �����
		HBRUSH	brush		= CreateSolidBrush(RGB(0, 255, 0));
		//	hdc �� ĥ�ϱ� �� �����ϱ�
		HBRUSH	oldBrush	= (HBRUSH)SelectObject(_hdc, brush);

		HPEN	pen		= CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
		HPEN	oldPen	= (HPEN)SelectObject(_hdc, pen);

		Ellipse(_hdc, 100, 100, 200, 400);
		

		//	ĥ�ϱ� �� ����ġ
		SelectObject(_hdc, oldBrush);
		//	�����ߴ� �귯�� ����
		DeleteObject(brush);

		SelectObject(_hdc, oldPen);
		DeleteObject(pen);
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
		0, 0,					//	������ ���� ��ġ
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
		hWnd,		//	� â�� Ÿ�̸��ΰ�?
		0,			//	�ش� Ÿ�̸���  ID ��ȣ
		1,			//	ȣ�� �ֱ� 1/1000 ����, ( �ִ�� 1/100 �� ����... )
		nullptr);	//	ȣ�� �Լ��ε�... nullptr �̸� WM_TIMER �޽��� �߻�
	
	//	3. �޽��� ����( ������ )
	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		DispatchMessage(&msg);
	}


	KillTimer(hWnd, 0);

	return msg.wParam;
}