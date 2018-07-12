#pragma once
#include <Windows.h>
#include <windowsx.h>

class Image
{
private:
	char			*path			= nullptr;
	HINSTANCE		m_hInst			= nullptr;
	unsigned int	m_ImageType		= IMAGE_BITMAP;
	unsigned int	m_ImageOption	= LR_LOADFROMFILE;
	POINT			m_ImgPos		= {};
	HDC				m_DC			= nullptr;
	HBITMAP			m_Bitmap		= nullptr;

public:
	Image(const char *_path);
	~Image();

	void ModifyPath(const char *_path);
	void SetPosition(int x, int y);
	POINT GetPosition();
	void CreateImage(HDC _hdc);
	bool ShowImage(HDC _hdc);
};

