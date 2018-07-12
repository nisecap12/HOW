#undef UNICODE
#include "TileManager.h"
#include <windowsx.h>
#include <sstream>

TileManager::~TileManager()
{
}

void TileManager::Init(HWND _hWnd, SIZE _cameraSize, int _unitSize, int _vertNum, int _horiNum)
{
	srand(GetTickCount());
	m_hWnd = _hWnd;
	m_camera_size = _cameraSize;
	m_map_size = {(_unitSize *_horiNum), (_unitSize * _vertNum)};
	m_unit_size = _unitSize;
	m_vert_number = _vertNum;
	m_hori_number = _horiNum;
	m_x_max = _unitSize * _horiNum;
	m_y_max = _unitSize * _vertNum;
	m_flag = false;
	tileGenerate();
}

void TileManager::tileGenerate()
{
	int size = m_vert_number * m_hori_number;
	int *tileArray = new int[size];

	for (int i = 0; i < size; i++)
	{
		*(tileArray + i) = rand() % 2;
	}

	m_tileArray = new Tile[size];
	
	for (int i = 0; i < size; i++)
	{
		m_tileArray[i].LoadTileImg(m_hWnd, m_unit_size, m_unit_size);
		switch (*(tileArray + i))
		{
		case TileTerrain::GROUND:
		{
			m_tileArray[i].SetTile(TileTerrain::GROUND);
		}break;
		case TileTerrain::HOLE:
		{
			m_tileArray[i].SetTile(TileTerrain::HOLE);
		}break;
		}
		
	}
}

POINT* TileManager::getUpperLeft()
{
	return &m_upper_left_pos;
}

void TileManager::Render(HDC _hdc)
{
	int		cellWidth = m_camera_size.cx / m_unit_size + 1;
	int		cellHeight = m_camera_size.cy / m_unit_size + 1;

	int		startPosX = m_camera_pos.x / m_unit_size;
	int		startPosY = m_camera_pos.y / m_unit_size;

	for (int y = 0; y < m_hori_number; y++)
	{
		for (int x = 0; x < m_vert_number; x++)
		{
			int winPosX = m_unit_size * (startPosX + x) - m_camera_pos.x;

			if (winPosX < 0 || winPosX >= m_camera_size.cx)
			{
				continue;
			}

			int winPosY = m_unit_size * (startPosY + y) - m_camera_pos.y;

			if (winPosY < 0 || winPosY >= m_camera_size.cy)
			{
				continue;
			}

			winPosX += m_upper_left_pos.x;
			winPosY += m_upper_left_pos.y;

			m_tileArray[(startPosX + x) + ((startPosY + y)*m_hori_number)].Render(_hdc, winPosX, winPosY);
		}
	}
}

void TileManager::DrawGrid(HDC _hdc)
{
	int		cellWidth = m_camera_size.cx / m_unit_size + 1;
	int		cellHeight = m_camera_size.cy / m_unit_size + 1;

	int		startPosX = m_camera_pos.x / m_unit_size;
	int		startPosY = m_camera_pos.y / m_unit_size;

	int posX = m_upper_left_pos.x;
	int posY = m_upper_left_pos.y;
	MoveToEx(_hdc, posX, posY, nullptr);
	LineTo(_hdc, posX, posY + m_camera_size.cy);
	LineTo(_hdc, posX + m_camera_size.cx, posY + m_camera_size.cy);
	LineTo(_hdc, posX + m_camera_size.cx, posY);
	LineTo(_hdc, posX, posY);

	//세로선 그리기

	for (int x = 0; x <= m_hori_number; x++)
	{
		int winPosX = m_unit_size * (startPosX + x) - m_camera_pos.x;

		if (winPosX < 0 || winPosX > m_camera_size.cx)
		{
			continue;
		}

		winPosX += m_upper_left_pos.x;

		MoveToEx(_hdc, winPosX, m_upper_left_pos.y, nullptr);
		LineTo(_hdc, winPosX, m_upper_left_pos.y + m_camera_size.cy);
	}

	//가로선 그리기
	for (int y = 0; y <= cellHeight; y++)
	{
		int winPosY = m_unit_size * (startPosY + y) - m_camera_pos.y;

		if (winPosY < 0 || winPosY > m_camera_size.cy)
		{
			continue;
		}

		winPosY += m_upper_left_pos.y;

		MoveToEx(_hdc, m_upper_left_pos.x, winPosY, nullptr);
		LineTo(_hdc, m_upper_left_pos.x + m_camera_size.cx, winPosY);
	}

	

	//좌표출력
	for (int y = 0; y < cellHeight; y++)
	{
		for (int x = 0; x < cellWidth; x++)
		{
			int winPosX = m_unit_size * (startPosX + x) - m_camera_pos.x;

			if (winPosX < 0 || winPosX >= m_camera_size.cx)
			{
				continue;
			}

			int winPosY = m_unit_size * (startPosY + y) - m_camera_pos.y;

			if (winPosY < 0 || winPosY >= m_camera_size.cy)
			{
				continue;
			}

			winPosX += m_upper_left_pos.x;
			winPosY += m_upper_left_pos.y;

			std::stringstream text;
			text << "(" << startPosX + x << "," << startPosY + y << ")";
			TextOut(_hdc,
				winPosX + 5, winPosY + 5,
				text.str().c_str(),
				text.str().length());
		}
	}

	// 클릭된 타일 색칠하기
	/*if (isClicked)
	{
		int posX = ((m_clicked_pos.x - startPosX) * m_unit_size) + m_upper_left_pos.x;
		int posY = ((m_clicked_pos.y - startPosY) * m_unit_size) + m_upper_left_pos.y;
		
		if (posX > 0 && posX <= m_camera_size.cx)
		{
			if (posY > 0 && posY <= m_camera_size.cy)
			{
				HPEN newpen = CreatePen(BS_SOLID, 3, RGB(255, 0, 0));
				HPEN oldpen = (HPEN)SelectObject(_hdc, newpen);

				MoveToEx(_hdc, posX, posY, nullptr);
				LineTo(_hdc, posX, posY + m_unit_size);
				LineTo(_hdc, posX + m_unit_size, posY + m_unit_size);
				LineTo(_hdc, posX + m_unit_size, posY);
				LineTo(_hdc, posX, posY);

				SelectObject(_hdc, oldpen);
				DeleteObject(newpen);
			}
		}
	}*/

	//클릭한 타일 출력
	TextOut(_hdc, 0, 0, m_s.str().c_str(), m_s.str().length());
}

void TileManager::DrawCheckPoint(HDC _hdc)
{
	if (m_flag != TileType::NONE)
	{
		int posX = m_mouse_pos.x / m_unit_size * m_unit_size;
		int posY = m_mouse_pos.y / m_unit_size * m_unit_size;

		POINT pt = { m_camera_pos.x + (m_mouse_pos.x - m_upper_left_pos.x),
			m_camera_pos.y + (m_mouse_pos.y - m_upper_left_pos.y) };

		int xIndex = pt.x / m_unit_size;
		int yIndex = pt.y / m_unit_size;

		if (posX >= m_upper_left_pos.x && posX < m_camera_size.cx + m_upper_left_pos.x)
		{
			if (posY >= m_upper_left_pos.y && posY < m_camera_size.cy + m_upper_left_pos.y)
			{
				if (m_tileArray[xIndex + (yIndex * m_hori_number)].GetTile() != TileTerrain::HOLE)
				{
					switch (m_flag)
					{
					case TileType::START:
					{
						HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
						HBRUSH oldbrush = (HBRUSH)SelectBrush(_hdc, brush);
						Rectangle(_hdc,
							posX, posY,
							posX + m_unit_size, posY + m_unit_size);

						DeleteObject(SelectBrush(_hdc, oldbrush));
					}break;
					case TileType::END:
					{
						HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
						HBRUSH oldbrush = (HBRUSH)SelectBrush(_hdc, brush);
						Rectangle(_hdc,
							posX, posY,
							posX + m_unit_size, posY + m_unit_size);

						DeleteObject(SelectBrush(_hdc, oldbrush));
					}break;
					}
				}
			}
		}
	}
}

void TileManager::TileClicked(POINT _mousePos)
{
	POINT pt = { m_camera_pos.x + (_mousePos.x - m_upper_left_pos.x),
		m_camera_pos.y + (_mousePos.y - m_upper_left_pos.y) };

	int xIndex = pt.x / m_unit_size;
	int yIndex = pt.y / m_unit_size;

	if (m_tileArray[xIndex + (yIndex * m_hori_number)].GetTile() != TileTerrain::HOLE)
	{
		m_s.str("");
		m_s << "clicked : " << xIndex << " , " << yIndex;

		m_tileArray[xIndex + (yIndex * m_hori_number)].SetTileType(m_flag);
	}
}

void TileManager::TileChange(POINT _mousePos)
{
	POINT pt = { m_camera_pos.x + (_mousePos.x - m_upper_left_pos.x),
		m_camera_pos.y + (_mousePos.y - m_upper_left_pos.y) };

	int xIndex = pt.x / m_unit_size;
	int yIndex = pt.y / m_unit_size;

	m_tileArray[xIndex + (yIndex * m_hori_number)].ChangeTile();
}

void TileManager::GoRight(int _offset)
{
	m_camera_pos.x += _offset;

	if (m_camera_pos.x + m_camera_size.cx > m_map_size.cx )
	{
		m_camera_pos.x = m_map_size.cx - m_camera_size.cx;
	}
}

void TileManager::GoLeft(int _offset)
{
	m_camera_pos.x -= _offset;

	if (m_camera_pos.x < 0)
	{
		m_camera_pos.x = 0;
	}
}

void TileManager::GoUp(int _offset)
{
	m_camera_pos.y -= _offset;

	if (m_camera_pos.y < 0)
	{
		m_camera_pos.y = 0;
	}
}

void TileManager::GoDown(int _offset)
{
	m_camera_pos.y += _offset;

	if (m_camera_pos.y + m_camera_size.cy > m_map_size.cy)
	{
		m_camera_pos.y = m_map_size.cy - m_camera_size.cy;
	}
}

void TileManager::ChangeMode()
{
	switch (m_flag)
	{
	case TileType::NONE:
		m_flag = TileType::START;
		break;
	case TileType::START:
		m_flag = TileType::END;
		break;
	case TileType::END:
		m_flag = TileType::NONE;
		break;
	}
}
