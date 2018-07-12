#undef UNICODE
#include "ImageAnimation.h"

ImageAnimation::ImageAnimation()
{
}

ImageAnimation::ImageAnimation(vector<string> _array)
{
	for (auto a : _array)
	{
		paths.push_back(a);
	}
	m_arraySize = _array.size();
}


ImageAnimation::~ImageAnimation()
{
	paths.clear();
	m_DC = nullptr;
	m_Bitmaps.clear();
}

void ImageAnimation::SetPaths(vector<string> _array)
{
	for (auto a : _array)
	{
		paths.push_back(a);
	}
	m_arraySize = _array.size();
}

bool ImageAnimation::CreateAnimation(HDC _hdc)
{
	if (m_Bitmaps.size() == 0)
	{
		m_DC = CreateCompatibleDC(_hdc);
		for (auto p : paths)
		{
			m_Bitmaps.push_back((HBITMAP)LoadImage(m_hInst, p.c_str(), m_ImageType, 0, 0, m_ImageOption));
		}
		return false;
	}
	else
	{
		return true;
	}
}

void ImageAnimation::ShowImage(HDC _hdc)
{
	if (m_currentImg >= m_arraySize)
	{
		m_currentImg = 0;
	}
	SelectBitmap(m_DC, m_Bitmaps[m_currentImg++]);
	BitBlt(_hdc, m_ImgPos.x, m_ImgPos.y, 300, 300, m_DC, 0, 0, SRCCOPY);
}

POINT ImageAnimation::GetPosition()
{
	return m_ImgPos;
}

void ImageAnimation::SetPosition(int _x, int _y)
{
	m_ImgPos.x = _x;
	m_ImgPos.y = _y;
}
