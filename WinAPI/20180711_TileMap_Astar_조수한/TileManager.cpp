#undef UNICODE
#include "TileManager.h"
#include <windowsx.h>
#include <sstream>

bool operator==(POINT _pos1, POINT _pos2)
{
	return (_pos1.x == _pos2.x && _pos1.y == _pos2.y);
}

int CalcIndex(POINT _pos, int _column)
{
	return _pos.x + (_pos.y * _column);
}

int CalcDistance(POINT _curPos, POINT _endPos)
{
	return abs(_endPos.x - _curPos.x) + abs(_endPos.y - _curPos.y);
}

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
			m_tileArray[i].SetTerrain(TileTerrain::GROUND);
		}break;
		case TileTerrain::HOLE:
		{
			m_tileArray[i].SetTerrain(TileTerrain::HOLE);
		}break;
		}
		
		m_tileArray[i].SetPos((i % m_hori_number),(i / m_hori_number));
	}
}

void TileManager::DrawRoute(bool _isClear)
{
	for (auto i : pathList)
	{
		if (i == pathList.front() || i == pathList.back()) continue;

		if (_isClear)
		{
			m_tileArray[CalcIndex(i, m_hori_number)].SetTileType(NONE);
		}
		else
		{
			m_tileArray[CalcIndex(i, m_hori_number)].SetTileType(ROUTE);
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

			if (winPosX < -m_unit_size || winPosX >= m_camera_size.cx)
			{
				continue;
			}

			int winPosY = m_unit_size * (startPosY + y) - m_camera_pos.y;

			if (winPosY < -m_unit_size || winPosY >= m_camera_size.cy)
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

			int index = (startPosX + x) + (startPosY + y) * m_hori_number;
			std::stringstream text;
			text << "(" << m_tileArray[index].GetPos().x << "," << m_tileArray[index].GetPos().y << ")";
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
				if (m_tileArray[xIndex + (yIndex * m_hori_number)].GetTerrain() != TileTerrain::HOLE)
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

	if (m_tileArray[xIndex + (yIndex * m_hori_number)].GetTerrain() != TileTerrain::HOLE)
	{
		m_s.str("");
		m_s << "clicked : " << xIndex << " , " << yIndex;

		switch (m_flag)
		{
		case TileType::START :
		{
			
			m_tileArray[m_start_pos.x + (m_start_pos.y * m_hori_number)].SetTileType(TileType::NONE);
			m_start_pos = { xIndex, yIndex };
			m_tileArray[xIndex + (yIndex * m_hori_number)].SetTileType(m_flag);
			isStartCheck = true;

			if(isEndCheck) m_flag = TileType::NONE;
			m_flag = TileType::END;
		}break;
		case TileType::END :
		{
			m_tileArray[m_end_pos.x + (m_end_pos.y * m_hori_number)].SetTileType(TileType::NONE);
			m_end_pos = { xIndex, yIndex };
			m_tileArray[xIndex + (yIndex * m_hori_number)].SetTileType(m_flag);
			isEndCheck = true;

			m_flag = TileType::NONE;
		}break;
		}
	}
}

void TileManager::TileChange(POINT _mousePos)
{
	POINT pt = { m_camera_pos.x + (_mousePos.x - m_upper_left_pos.x),
		m_camera_pos.y + (_mousePos.y - m_upper_left_pos.y) };

	int xIndex = pt.x / m_unit_size;
	int yIndex = pt.y / m_unit_size;

	m_tileArray[xIndex + (yIndex * m_hori_number)].ChangeTerrain();
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

bool TileManager::FindAStarRoute()
{
	std::list<Node> openList, closeList;

	Node start;
	start.pos = m_start_pos;

	openList.push_back(start);

	while (true)
	{
		closeList.push_back(openList.front());
		openList.pop_front();

		Node &curTile = closeList.back();

		if (curTile.pos == m_end_pos)
		{
			DrawRoute(true);
			pathList.clear();

			Node* node = &curTile;

			while (node != nullptr)
			{
				pathList.push_front(node->pos);
				node = node->parent;
			}

			DrawRoute(false);
			return true;
		}

		for (int y = -1; y < 2; y++)
		{
			for (int x = -1; x < 2; x++)
			{
				if (x == 0 && y == 0) continue;

				POINT pos;
				pos.x = curTile.pos.x + x;
				pos.y = curTile.pos.y + y;

				if (pos.x < 0 || pos.y < 0 || pos.x >= m_hori_number || pos.y >= m_vert_number) continue;

				if (m_tileArray[CalcIndex(pos, m_hori_number)].GetTerrain() == TileTerrain::HOLE) continue;

				bool isCloseList = false;

				for (auto &i : closeList)
				{
					if (i.pos == pos)
					{
						isCloseList = true;
						break;
					}
				}
				if (isCloseList == true) continue;

				Node* node = nullptr;

				for (auto &i : openList)
				{
					if (i.pos == pos)
					{
						node = &i;
						break;
					}
				}

				int curG = curTile.G + ((x*y == 0) ? 10 : 14);
				int curH = CalcDistance(pos, m_end_pos);
				int curF = curG + curH;

				if (node == nullptr)
				{
					Node newNode;

					newNode.parent = &curTile;
					newNode.F = curF;
					newNode.G = curG;
					newNode.H = curH;
					newNode.pos = pos;

					openList.push_back(newNode);
				}
				else
				{
					if (node->G > curG)
					{
						node->parent = &curTile;

						node->F = curF;
						node->G = curG;
						node->H = curH;
					}
				}
			}
		}

		if (openList.empty() == true) break;

		openList.sort();
	}

	return false;
}
