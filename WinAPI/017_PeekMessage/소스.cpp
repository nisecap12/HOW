#undef UNICODE
#include <Windows.h>
#include <windowsx.h>
#include <sstream>

//	timeGetTime �Լ��� ����ϱ� ���� ���̺귯�� �߰�
#pragma comment( lib, "winmm.lib" )

LPCSTR		g_winName		= "HOW-SAMPLEWINDOW";

const int WIN_WIDTH			= 800;
const int WIN_HEIGHT		= 600;

POINT	g_Pos = {100, 100};
int		g_Count = 0;

int		g_SetFPS		= 60;
int		g_FPS			= 0;
int		g_FPSTime		= 0;

//	���� �ð�
DWORD	g_LastTime = 0;

void Init(HWND _hWnd)
{
	//	timeGetTime ���е� ����
	//	�Ű����� 1�� �ǹ̴� ������ 1/1000�� ������ �����ϰ� �����ϵ���
	timeBeginPeriod(1);

	g_LastTime = timeGetTime();
}
void Release()
{
	//	timeGetTime ���е� ����
	timeEndPeriod(1);
}

void Update(DWORD _elapseTime)
{
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		g_Pos.x++;
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		g_Pos.x--;
	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		g_SetFPS = 500;
	}
	else
	{
		g_SetFPS = 60;
	}
}
void Render(HDC _hdc)
{
	std::stringstream	text;
	text << "ī��Ʈ : " << g_Count;
	TextOut(_hdc, g_Pos.x, g_Pos.y, text.str().c_str(), text.str().length());

	//	���ڿ� �ʱ�ȭ text = "";
	text.str("");
	text << "FPS : " << g_FPS;
	TextOut(_hdc, 100, 130, text.str().c_str(), text.str().length());
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
			DWORD	elapseTime;

			//	��� �ð���ŭ ���
			do
			{
				DWORD	nowTime = timeGetTime();
				elapseTime = nowTime - g_LastTime;

			} while (elapseTime < (1000.0f / g_SetFPS));

			//	��� �ð� ������ �����ϰ�
			g_FPSTime	+= elapseTime;
			g_LastTime	+= elapseTime;
			g_Count++;

			//	1�� �� ����� ������ ����
			if (g_FPSTime > 1000)
			{
				g_FPS = g_Count;
				g_Count = 0;

				g_FPSTime -= 1000;
			}

			//	�޽����� �߻����� ���� ���
			Update(elapseTime);

#pragma region RENDER
			HDC			hdc = GetDC(hWnd);

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

			ReleaseDC(hWnd, hdc);

#pragma endregion
		}
	}

	//	����ó��
	Release();

	return msg.wParam;
}