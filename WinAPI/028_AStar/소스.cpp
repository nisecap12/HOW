#include <iostream>
#include <list>
using namespace std;

#include <Windows.h>
#include <windowsx.h>

/*
		1. 시작사각형에서 검색된 인접사각형들을 열린목록에 넣습니다.

		2. 다음의 과정들을 반복합니다.

			a) 열린목록에서 가장 낮은 F 비용을 찾아 현재사각형으로 선택합니다.

			b) 이것을 열린목록에서 꺼내 닫힌목록으로 넣습니다.

			c) 현재 사각형에 인접한 8 개의 사각형에 대해?

				● 만약 인접한사각형이 갈수없는 것 이거나 그것이 닫힌목록상에 있다면 무시, 그렇지 않은것은 다음을 계속합니다.

				● 만약 이것이 열린목록에 있지 않다면, 이것을 열린목록에 추가하고. 이 사각형의 부모를 현재 사각형으로 만듭니다. 사각형의 F,G,H 비용을 기록.

				● 만약 이것이 이미 열린목록에 있다면, G비용을 이용하여 이 사각형이 더 나은가 알아보고, 그것의 G비용이 더 작으면 그것이 더 나은 길이라는 것을 의미하므로 부모 사각형을 그 (G비용이 더 작은)사각형으로 바꿉니다, 그리고 그 사각형의 G,F비용을 다시 계산합니다. 만약 여러분의 열린목록을 F비용으로 정렬하고 있다면 바뀐것에 따라서 열린목록을 다시 정렬해야합니다.

			d) 그만해야 할 때

				● 길을 찾는 중 목표사각형을 열린목록에 추가하였을때,

				● 열린목록이 비어있게 될 경우.

				(이때는 목표사각형을 찾는데 실패한것인데 이 경우 길이 없는경우입니다.)
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

//	POINT 자료형의 == 연산을 하기 위해서
bool	operator==(POINT _pos1, POINT _pos2)
{
	return (_pos1.x == _pos2.x && _pos1.y == _pos2.y);
}

struct	TileNode
{
	//	부모 노드
	TileNode	*parentNode = nullptr;

	//	타일 좌표
	POINT		pos = {};

	//	F,G,H
	int			F = 0;
	int			G = 0;
	int			H = 0;

	//	sort 사용을 위한 연산자 정의
	bool	operator < (TileNode &_right)
	{
		return F < _right.F;
	}
};

bool	FindPath_AStar(POINT _startPos, POINT _endPos, list<POINT> &_pathList)
{
	//	열린목록, 닫힌목록
	list<TileNode>	openList, closeList;

	//	시작타일을 열린목록에 넣는다.
	TileNode	info;
	info.pos = _startPos;

	openList.push_back(info);

	//	다음 과정을 반복
	while (true)
	{
		//   a) 열린목록에서 가장 낮은 F 비용을 찾아 현재사각형으로 선택합니다.
		//   b) 이것을 열린목록에서 꺼내 닫힌목록으로 넣습니다.
		closeList.push_back(openList.front());
		openList.pop_front();

		//	( 마지막으로 닫힌목록에 있는 사각형이 선택된 사각형이다. )
		TileNode	&selectNode = closeList.back();

		//	도착이야?
		if (selectNode.pos == _endPos)
		{
			_pathList.clear();

			//	경로 저장
			TileNode	*node = &selectNode;

			while (node != nullptr)
			{
				_pathList.push_front(node->pos);

				node = node->parentNode;
			}

			return true;
		}

		//	선택된 노드의 인접한 8개 검사
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				//	선택된 노드는 제외
				if (x == 0 && y == 0) continue;

				//	좌표설정
				POINT pos;
				pos.x = selectNode.pos.x + x;
				pos.y = selectNode.pos.y + y;

				//	이동불가(장외)
				if (pos.x < 0		||
					pos.y < 0		||
					pos.x >= width	||
					pos.y >= height)
					continue;

				//	이동불가(장애물)
				if (tileMap[width * pos.y + pos.x] == 0) 
					continue;

				//	닫힌 목록에 있는가?
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


				//	열린목록에 있는가?
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

				//	열린목록에 없을 때
				if (node == nullptr)
				{
					TileNode newNode;

					//	부모설정
					newNode.parentNode = &selectNode;

					//	F,G,H
					newNode.F = thisTimeF;
					newNode.G = thisTimeG;
					newNode.H = thisTimeH;

					//	pos 설정
					newNode.pos = pos;

					//	열린목록에 추가
					openList.push_back(newNode);
				}

				//	열린목록에 있을 때
				else
				{
					//	이번에 계산한 것이 더 효율적인가?
					if (node->G > thisTimeG)
					{
						//	부모를 이번 노드로 변경
						node->parentNode = &selectNode;

						//	F,G,H 를 재계산
						node->F = thisTimeF;
						node->G = thisTimeG;
						node->H = thisTimeH;
					}
				}
			}
		}

		//	열린목록이 비었다면?
		if (openList.empty() == true)	break;

		//	정렬
		openList.sort();
	}

	return false;
}

#pragma endregion


int main()
{
	return 0;
}