#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <string.h>
#include <vector>
using namespace std;

class ImageAnimation
{
private:
	vector<string>	paths;
	HINSTANCE		m_hInst = nullptr;
	unsigned int	m_ImageType = IMAGE_BITMAP;
	unsigned int	m_ImageOption = LR_LOADFROMFILE;
	POINT			m_ImgPos = {};
	HDC				m_DC = nullptr;
	vector<HBITMAP>	m_Bitmaps;
	int				m_currentImg = 0;
	int				m_arraySize = 0;

public:
	ImageAnimation();
	ImageAnimation(vector<string> _array);
	~ImageAnimation();

	void SetPaths(vector<string> _array);
	bool CreateAnimation(HDC _hdc);
	void ShowImage(HDC _hdc);
	POINT GetPosition();
	void SetPosition(int _x, int _y);
};

