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
	���� ��ġ, ���� �������� �簢���� 1�ʸ��� �������� ���ڴ�!!!!
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


//	���ϰ� ���� ���ؼ� ����ڰ� ���� �ѹ��� �����Ѵ�.
void MyRectangle(HDC _hdc, int _posX, int _posY, int _size, COLORREF _color)
{
	//	���ο� ĥ�ϱ� �� �����
	HBRUSH	brush		= CreateSolidBrush(_color);
	//	hdc �� ĥ�ϱ� �� �����ϱ�
	HBRUSH	oldBrush	= (HBRUSH)SelectObject(_hdc, brush);

	Rectangle(_hdc, 
		_posX - _size/2, 
		_posY - _size/2,
		_posX + _size/2,
		_posY + _size / 2);

	//	ĥ�ϱ� �� ����ġ
	SelectObject(_hdc, oldBrush);
	//	�����ߴ� �귯�� ����
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
		TIMER_BASEUPDATE,			//	�ش� Ÿ�̸���  ID ��ȣ
		1,							//	ȣ�� �ֱ� 1/1000 ����, ( �ִ�� 1/100 �� ����... )
		nullptr);					//	ȣ�� �Լ��ε�... nullptr �̸� WM_TIMER �޽��� �߻�

	SetTimer(
		hWnd,						//	� â�� Ÿ�̸��ΰ�?
		TIMER_CREATERECT,			//	�ش� Ÿ�̸���  ID ��ȣ
		1000,						//	ȣ�� �ֱ� 1/1000 ����, ( �ִ�� 1/100 �� ����... )
		nullptr);					//	ȣ�� �Լ��ε�... nullptr �̸� WM_TIMER �޽��� �߻�
	
	//	3. �޽��� ����( ������ )
	MSG	msg = {};

	while (GetMessage(&msg, nullptr, 0u, 0u))
	{
		DispatchMessage(&msg);
	}


	KillTimer(hWnd, TIMER_BASEUPDATE);
	KillTimer(hWnd, TIMER_CREATERECT);

	return msg.wParam;
}