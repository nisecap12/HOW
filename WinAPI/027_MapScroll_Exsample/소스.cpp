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
	//	���� ���� ����
	int		cellWidth	= g_BGSize.cx / g_tileSize + 1;
	int		cellHeight	= g_BGSize.cy / g_tileSize + 1;

	//	���� Ÿ�� ��ǥ
	int		startPosX	= g_CamPos.x / g_tileSize;
	int		startPosY	= g_CamPos.y / g_tileSize;

	//	�ܰ���
	Rectangle(_hdc,
		g_tileStartPos.x,
		g_tileStartPos.y,
		g_tileStartPos.x + g_BGSize.cx,
		g_tileStartPos.y + g_BGSize.cy );

	//	����
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

	//	����
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

	//	��ǥ
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

	//	�ʱ�ȭ
	Init(hWnd);

	ShowWindow(hWnd, _CmdShow);

	//	3. �޽��� ����( ������ )
	MSG	msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			//	Ż������
			if (msg.message == WM_QUIT) break;

			//	�޽����� �߻��� ���
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(TIME.Tick());

			ImgClass	back(hWnd, WIN_WIDTH, WIN_HEIGHT);
			//	���ο� DC�� �̹����� �׸���.
			Render(back.GetImgDC());
			//	ȭ�� hdc �� ����� �̹����� �׸���.
			back.Render(hWnd);
		}
	}

	//	����ó��
	Release();

	return msg.wParam;
}