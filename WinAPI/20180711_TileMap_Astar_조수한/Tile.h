#pragma once
#include <Windows.h>

enum TileTerrain 
{
	GROUND,
	HOLE,
};

enum TileType
{
	NONE,
	START,
	END,
	ROUTE,
};

class Tile
{
private:
	HDC m_imgDC;
	HBITMAP m_bit, m_oldbit;
	int m_tileType = NONE;
	int m_tileTerrain = GROUND;
	POINT m_pos;
	SIZE m_tileSize;

public:
	Tile();
	~Tile();

	bool LoadTileImg(HWND _hWnd, int _width = 0, int _height = 0);
	void Render(HDC _destDC, int _posX, int _posY);
	void SetPos(int _x, int _y);
	POINT GetPos()
	{
		return m_pos;
	}
	void SetTerrain(int _type)
	{
		m_tileTerrain = _type;
	}
	int GetTerrain()
	{
		return m_tileTerrain;
	}
	void ChangeTerrain();
	void SetTileType(int _type)
	{
		m_tileType = _type;
	}
};