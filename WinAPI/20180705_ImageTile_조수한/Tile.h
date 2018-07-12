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
};

class Tile
{
private:
	HDC m_imgDC;
	HBITMAP m_bit, m_oldbit;
	int m_tileType = NONE;
	int m_tileTerrain = GROUND;
	SIZE m_tileSize;

public:
	Tile();
	~Tile();

	bool LoadTileImg(HWND _hWnd, int _width = 0, int _height = 0);
	void Render(HDC _destDC, int _posX, int _posY);
	void SetTile(int _type)
	{
		m_tileTerrain = _type;
	}
	int GetTile()
	{
		return m_tileTerrain;
	}
	void ChangeTile()
	{
		switch (m_tileTerrain)
		{
		case GROUND:
			m_tileTerrain = HOLE;
			break;
		case HOLE:
			m_tileTerrain = GROUND;
			break;
		}
	}
	void SetTileType(int _type)
	{
		m_tileType = _type;
	}
};