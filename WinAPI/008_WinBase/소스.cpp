#undef UNICODE
#include <Windows.h>
#include "WinBase.h"

int APIENTRY WinMain(
	HINSTANCE	_hInst,			//	���μ��� �ڵ�
	HINSTANCE	_hPrevInst,		//	���� ���μ��� �ڵ�( ���� �Ⱦ� )
	LPSTR		_CmdParam,		//	����� �μ�( char* )
	int			_CmdShow)		//	�⺻â ����
{
	WinBase	main;

	if (main.NewWindow(_hInst, 400, 300, "SampleBase") == FALSE)
	{
		return 0;
	}

	return main.MessageLoop();
}