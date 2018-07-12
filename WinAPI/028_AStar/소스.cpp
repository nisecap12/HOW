#include <iostream>
#include <list>
using namespace std;

#include <Windows.h>
#include <windowsx.h>

/*
		1. ���ۻ簢������ �˻��� �����簢������ ������Ͽ� �ֽ��ϴ�.

		2. ������ �������� �ݺ��մϴ�.

			a) ������Ͽ��� ���� ���� F ����� ã�� ����簢������ �����մϴ�.

			b) �̰��� ������Ͽ��� ���� ����������� �ֽ��ϴ�.

			c) ���� �簢���� ������ 8 ���� �簢���� ����?

				�� ���� �����ѻ簢���� �������� �� �̰ų� �װ��� ������ϻ� �ִٸ� ����, �׷��� �������� ������ ����մϴ�.

				�� ���� �̰��� ������Ͽ� ���� �ʴٸ�, �̰��� ������Ͽ� �߰��ϰ�. �� �簢���� �θ� ���� �簢������ ����ϴ�. �簢���� F,G,H ����� ���.

				�� ���� �̰��� �̹� ������Ͽ� �ִٸ�, G����� �̿��Ͽ� �� �簢���� �� ������ �˾ƺ���, �װ��� G����� �� ������ �װ��� �� ���� ���̶�� ���� �ǹ��ϹǷ� �θ� �簢���� �� (G����� �� ����)�簢������ �ٲߴϴ�, �׸��� �� �簢���� G,F����� �ٽ� ����մϴ�. ���� �������� ��������� F������� �����ϰ� �ִٸ� �ٲ�Ϳ� ���� ��������� �ٽ� �����ؾ��մϴ�.

			d) �׸��ؾ� �� ��

				�� ���� ã�� �� ��ǥ�簢���� ������Ͽ� �߰��Ͽ�����,

				�� ��������� ����ְ� �� ���.

				(�̶��� ��ǥ�簢���� ã�µ� �����Ѱ��ε� �� ��� ���� ���°���Դϴ�.)
*/

// 20x20
int tileMap[] =
{
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
int width	= 20;
int height	= 20;

#pragma region AStar

POINT		startPos	= {};
POINT		endPos		= {};
list<POINT>	pathList;

//	POINT �ڷ����� == ������ �ϱ� ���ؼ�
bool	operator==(POINT _pos1, POINT _pos2)
{
	return (_pos1.x == _pos2.x && _pos1.y == _pos2.y);
}

struct	TileNode
{
	//	�θ� ���
	TileNode	*parentNode = nullptr;

	//	Ÿ�� ��ǥ
	POINT		pos = {};

	//	F,G,H
	int			F = 0;
	int			G = 0;
	int			H = 0;

	//	sort ����� ���� ������ ����
	bool	operator < (TileNode &_right)
	{
		return F < _right.F;
	}
};

bool	FindPath_AStar(POINT _startPos, POINT _endPos, list<POINT> &_pathList)
{
	//	�������, �������
	list<TileNode>	openList, closeList;

	//	����Ÿ���� ������Ͽ� �ִ´�.
	TileNode	info;
	info.pos = _startPos;

	openList.push_back(info);

	//	���� ������ �ݺ�
	while (true)
	{
		//   a) ������Ͽ��� ���� ���� F ����� ã�� ����簢������ �����մϴ�.
		//   b) �̰��� ������Ͽ��� ���� ����������� �ֽ��ϴ�.
		closeList.push_back(openList.front());
		openList.pop_front();

		//	( ���������� ������Ͽ� �ִ� �簢���� ���õ� �簢���̴�. )
		TileNode	&selectNode = closeList.back();

		//	�����̾�?
		if (selectNode.pos == _endPos)
		{
			_pathList.clear();

			//	��� ����
			TileNode	*node = &selectNode;

			while (node != nullptr)
			{
				_pathList.push_front(node->pos);

				node = node->parentNode;
			}

			return true;
		}

		//	���õ� ����� ������ 8�� �˻�
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				//	���õ� ���� ����
				if (x == 0 && y == 0) continue;

				//	��ǥ����
				POINT pos;
				pos.x = selectNode.pos.x + x;
				pos.y = selectNode.pos.y + y;

				//	�̵��Ұ�(���)
				if (pos.x < 0		||
					pos.y < 0		||
					pos.x >= width	||
					pos.y >= height)
					continue;

				//	�̵��Ұ�(��ֹ�)
				if (tileMap[width * pos.y + pos.x] == 0) 
					continue;

				//	���� ��Ͽ� �ִ°�?
				bool	isCloseList = false;

				for (auto &i : closeList)
				{
					if (i.pos == pos)
					{
						isCloseList = true;
						break;
					}
				}
				if (isCloseList == true) continue;


				//	������Ͽ� �ִ°�?
				TileNode	*node = nullptr;

				for (auto &i : openList)
				{
					if (i.pos == pos)
					{
						node = &i;
						break;
					}
				}

				////////////////////////////////////////////

				int thisTimeG = selectNode.G + ((x*y == 0) ? 10 : 14);
				int thisTimeH = (abs(pos.x - endPos.x) + abs(pos.y - endPos.y)) * 10;
				int thisTimeF = thisTimeG + thisTimeH;

				//	������Ͽ� ���� ��
				if (node == nullptr)
				{
					TileNode newNode;

					//	�θ���
					newNode.parentNode = &selectNode;

					//	F,G,H
					newNode.F = thisTimeF;
					newNode.G = thisTimeG;
					newNode.H = thisTimeH;

					//	pos ����
					newNode.pos = pos;

					//	������Ͽ� �߰�
					openList.push_back(newNode);
				}

				//	������Ͽ� ���� ��
				else
				{
					//	�̹��� ����� ���� �� ȿ�����ΰ�?
					if (node->G > thisTimeG)
					{
						//	�θ� �̹� ���� ����
						node->parentNode = &selectNode;

						//	F,G,H �� ����
						node->F = thisTimeF;
						node->G = thisTimeG;
						node->H = thisTimeH;
					}
				}
			}
		}

		//	��������� ����ٸ�?
		if (openList.empty() == true)	break;

		//	����
		openList.sort();
	}

	return false;
}

#pragma endregion


int main()
{
	return 0;
}