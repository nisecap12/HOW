#undef UNICODE
#include "Tile.h"
#include <windowsx.h>

Tile::Tile()
{
}

Tile::~Tile()
{
}

bool Tile::LoadTileImg(HWND _hWnd, int _width, int _height)
{
	if (_hWnd == nullptr || _width < 0 || _height < 0)
	{
		return false;
	}

	m_tileSize.cx = _width;
	m_tileSize.cy = _height;

	HDC hdc = GetDC(_hWnd);

	m_imgDC = CreateCompatibleDC(hdc);
	m_bit = (HBITMAP)LoadImage(nullptr, "rpgtile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_oldbit = SelectBitmap(m_imgDC, m_bit);

	ReleaseDC(_hWnd, hdc);

	return true;
}

void Tile::Render(HDC _destDC, int _posX, int _posY)
{
	switch (m_tileType)
	{
	case NONE:
	{
		switch (m_tileTerrain)
		{
		case GROUND:
		{
			BitBlt(_destDC, _posX, _posY, m_tileSize.cx, m_tileSize.cy, m_imgDC, 0, 32, SRCCOPY);
		}break;
		case HOLE:
		{
			BitBlt(_destDC, _posX, _posY, m_tileSize.cx, m_tileSize.cy, m_imgDC, 320, 320, SRCCOPY);
		}break;
		}
	}break;
	
	case START:
	{
		HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
		HBRUSH oldbrush = (HBRUSH)SelectBrush(_destDC, brush);
		Rectangle(_destDC,
			_posX, _posY,
			_posX + m_tileSize.cx, _posY + m_tileSize.cy);

		DeleteObject(SelectBrush(_destDC, oldbrush));
	}break;

	case END:
	{
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH oldbrush = (HBRUSH)SelectBrush(_destDC, brush);
		Rectangle(_destDC,
			_posX, _posY,
			_posX + m_tileSize.cx, _posY + m_tileSize.cy);

		DeleteObject(SelectBrush(_destDC, oldbrush));
	}break;
	}
	
	
}
