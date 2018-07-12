#undef UNICODE
#include <Windows.h>
#include "WinBase.h"

int APIENTRY WinMain(
	HINSTANCE	_hInst,			//	프로세스 핸들
	HINSTANCE	_hPrevInst,		//	이전 프로세스 핸들( 현재 안씀 )
	LPSTR		_CmdParam,		//	명령줄 인수( char* )
	int			_CmdShow)		//	기본창 상태
{
	WinBase	main;

	if (main.NewWindow(_hInst, 400, 300, "SampleBase") == FALSE)
	{
		return 0;
	}

	return main.MessageLoop();
}