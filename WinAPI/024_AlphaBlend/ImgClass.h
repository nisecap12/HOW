#pragma once

#include <Windows.h>

class ImgClass
{
private:
	HDC			m_imgDC;
	HBITMAP		m_bit, m_oldBit;
	SIZE		m_size;

	BOOL		m_isTrans;
	COLORREF	m_colorkey;

	INT			m_opacity;

public:
	ImgClass() = default;
	ImgClass(HWND _hWnd, int _width, int _height);
	ImgClass(HDC _hdc, int _width, int _height);
	~ImgClass();

	//	생성
	bool	CreateImg(HWND _hWnd, int _width, int _height);
	bool	CreateImg(HDC _hdc, int _width, int _height);
	bool	LoadImg(HWND _hWnd, LPCSTR _path, int _width = 0, int _height = 0);

	//	설정
	void	SetTrans(BOOL _isTrans = TRUE, COLORREF _colorkey = RGB(255, 0, 255));
	void	SetOpacity(INT _opacity);
	INT		GetOpacity()
	{
		return m_opacity;
	}

	//	랜더
	void	Render(HDC _destDC, int _posX = 0, int _posY = 0, int _width = 0, int _height = 0 );
	void	Render(HWND _hWnd);

	//	수정
	void	Clear(COLORREF _color = RGB(255,255,255) );

	//	DC 얻기
	HDC		GetImgDC()
	{
		return m_imgDC;
	}
};

