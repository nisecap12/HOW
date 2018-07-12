#pragma once
#include <Windows.h>
#include <sstream>
#include <list>
#include "Tile.h"

struct Node
{
	int F = 0;
	int G = 0;
	int H = 0;
	POINT pos = {};
	Node* parent = nullptr;

	bool operator < (Node &_right)
	{
		return F < _right.F;
	}
};

class TileManager
{
private:
	HWND m_hWnd;
	int m_unit_size;
	int m_vert_number;
	int m_hori_number;
	int m_x_max;
	int m_y_max;

	SIZE m_camera_size;
	SIZE m_map_size;

	POINT m_upper_left_pos;
	POINT m_camera_pos;
	POINT m_mouse_pos;
	POINT m_start_pos = {};
	POINT m_end_pos = {};

	int m_flag;
	bool isStartCheck = false;
	bool isEndCheck = false;
	std::stringstream m_s;
	Tile* m_tileArray;
	std::list<POINT> pathList;

private:
	TileManager() = default;
	~TileManager();
	void tileGenerate();
	void DrawRoute(bool _isClear);

public:
	static TileManager& GetInstance()
	{
		static TileManager instance;
		return instance;
	}
	void Init(HWND _hWnd, SIZE _cameraSize, int _unitSize = 50, int _vertNum = 10, int _horiNum = 10);
	void setVerticalNumber(int _num)
	{
		m_vert_number = _num;
		m_y_max = m_unit_size * _num;
	}
	void setHorizonNumber(int _num)
	{
		m_hori_number = _num;
		m_x_max = m_unit_size * _num;
	}

	POINT* getUpperLeft();
	void Render(HDC _hdc);
	void DrawGrid(HDC _hdc);
	void DrawCheckPoint(HDC _hdc);
	void TileClicked(POINT _mousePos);
	void TileChange(POINT _mousePos);
	void SetCameraPos(POINT _camPos)
	{
		m_camera_pos = _camPos;
	}
	void SetCameraSize(SIZE _camera)
	{
		m_camera_size = _camera;
	}
	void SetMousePos(POINT _mousePos)
	{
		m_mouse_pos = _mousePos;
	}
	void GoRight(int _offset);
	void GoLeft(int _offset);
	void GoUp(int _offset);
	void GoDown(int _offset);
	void ChangeMode();

	bool FindAStarRoute();
};

#define TILE_MGR TileManager::GetInstance()