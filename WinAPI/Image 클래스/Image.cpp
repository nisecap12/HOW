#undef UNICODE
#include "Image.h"

Image::~Image()
{
	path = nullptr;
	m_DC = nullptr;
	m_Bitmap = nullptr;
}

void Image::ModifyPath(const char * _path)
{
	int len = strlen(_path);
	path = new char[len];
	strcpy_s(path, len, _path);
}

void Image::SetPosition(int x, int y)
{
	m_ImgPos.x = x;
	m_ImgPos.y = y;
}

POINT Image::GetPosition()
{
	return m_ImgPos;
}

void Image::CreateImage(HDC _hdc)
{
	m_DC = CreateCompatibleDC(_hdc);
	m_Bitmap = (HBITMAP)LoadImage(m_hInst, path, m_ImageType, 0, 0, m_ImageOption);
}

bool Image::ShowImage(HDC _hdc)
{
	HBITMAP tempBit;
	tempBit = SelectBitmap(m_DC, m_Bitmap);
	if (BitBlt(_hdc, m_ImgPos.x, m_ImgPos.y, 300, 300, m_DC, 0, 0, SRCCOPY))
	{
		DeleteObject(SelectBitmap(m_DC, tempBit));
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

Image::Image(const char * _path)
{
	if (path == nullptr)
	{
		int len = strlen(_path);
		path = new char[len];
		strcpy_s(path, len + 1, _path);
	}
}
