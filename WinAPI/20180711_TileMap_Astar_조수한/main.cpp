#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>

#include "TimeMgr.h"
#include "TileManager.h"

LPCSTR		g_winName = "HOW-SAMPLEWINDOW";

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int TILE_UNIT_SIZE = 50;
const int TILE_W_MAX = 15;
const int TILE_H_MAX = 15;

POINT	g_Pos;

float	g_PosX = 100.0f;
float	g_PosY = 100.0f;

int		g_Speed = 100;
int		g_camera_speed = 10;
SIZE	g_camSize;

void Init(HWND _hWnd)
{
	g_camSize = {WIN_WIDTH/2, WIN_HEIGHT/2};
	TILE_MGR.Init(_hWnd, g_camSize, TILE_UNIT_SIZE, TILE_W_MAX, TILE_H_MAX);
	TILE_MGR.getUpperLeft()->x = 50;
	TILE_MGR.getUpperLeft()->y = 50;
}
void Release()
{
}

void Update(float _elapseTime)
{
	
}
void Render(HDC _hdc)
{
	//TIME.Render(_hdc, 0, 0);
	TILE_MGR.Render(_hdc);
	TILE_MGR.DrawGrid(_hdc);
	TILE_MGR.DrawCheckPoint(_hdc);

}

//	�ݹ��Լ� : �����찡 ��� ȣ�����ش�.
LRESULT	CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_LBUTTONDOWN:
	{
		g_Pos.x = GET_X_LPARAM(_lParam);
		g_Pos.y = GET_Y_LPARAM(_lParam);
		TILE_MGR.TileClicked(g_Pos);
	}break;
	case WM_RBUTTONDOWN:
	{
		g_Pos.x = GET_X_LPARAM(_lParam);
		g_Pos.y = GET_Y_LPARAM(_lParam);
		TILE_MGR.TileChange(g_Pos);
	}break;
	case WM_MOUSEMOVE:
	{
		g_Pos.x = GET_X_LPARAM(_lParam);
		g_Pos.y = GET_Y_LPARAM(_lParam);
		TILE_MGR.SetMousePos(g_Pos);
	}break;
	case WM_KEYDOWN:
	{
		switch (_wParam)
		{
		case VK_RIGHT:
		{
			TILE_MGR.GoRight(g_camera_speed);
		}break;
		case VK_LEFT:
		{
			TILE_MGR.GoLeft(g_camera_speed);
		}break;
		case VK_UP:
		{
			TILE_MGR.GoUp(g_camera_speed);
		}break;
		case VK_DOWN:
		{
			TILE_MGR.GoDown(g_camera_speed);
		}break;
		case VK_SPACE:
		{
			TILE_MGR.ChangeMode();
		}break;
		case VK_RETURN:
		{
			TILE_MGR.FindAStarRoute();
		}break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		default:
			break;
		}
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
	LPCSTR	g_className = "HOW";

	//	1. ������ Ŭ���� ���� �� ���
	WNDCLASS	wndClass = {};

	//	�����Է�
	wndClass.lpszClassName = g_className;
	wndClass.hInstance = _hInst;
	wndClass.lpfnWndProc = WndProc;	//	�Լ�������

	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//	����
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);		//	Ŀ�����
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//	������

	RegisterClass(&wndClass);

	//	���� ������ ũ�� ���ϱ�
	int		wsOption = WS_OVERLAPPEDWINDOW;
	RECT	rt = { 0, 0, WIN_WIDTH, WIN_HEIGHT };

	//	������ �⺻ �ɼǿ� ���� ���� �⺻â ũ��
	AdjustWindowRect(&rt, wsOption, FALSE);

	//	2. ������ ���� �� ȭ�� ����
	HWND hWnd = CreateWindow(
		g_className, g_winName, wsOption, 100, 100,					
		rt.right - rt.left, rt.bottom - rt.top,	
		nullptr, nullptr, _hInst, nullptr);					

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

#pragma region RENDER
			HDC			hdc = GetDC(hWnd);

			HDC	backDC = CreateCompatibleDC(hdc);
			HBITMAP hBit = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
			HBITMAP hOldBit = SelectBitmap(backDC, hBit);

			RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
			FillRect(backDC, &rc, GetStockBrush(WHITE_BRUSH));

			Render(backDC);

			BitBlt(hdc,
				TILE_MGR.getUpperLeft()->x, 
				TILE_MGR.getUpperLeft()->y,
				g_camSize.cx + 1, 
				g_camSize.cy + 1, 
				backDC,
				TILE_MGR.getUpperLeft()->x,
				TILE_MGR.getUpperLeft()->y,
				SRCCOPY);

			DeleteObject(SelectBitmap(backDC, hOldBit));
			DeleteDC(backDC);

			ReleaseDC(hWnd, hdc);

#pragma endregion
		}
	}

	//	����ó��
	Release();

	return msg.wParam;
}