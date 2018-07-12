#pragma once
#include <Windows.h>
#include <sstream>
#include "Tile.h"

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
	POINT m_clicked_pos;
	POINT m_mouse_pos;

	int m_flag;
	bool isClicked = false;
	std::stringstream m_s;
	Tile* m_tileArray;

private:
	TileManager() = default;
	~TileManager();
	void tileGenerate();

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
};

#define TILE_MGR TileManager::GetInstance()