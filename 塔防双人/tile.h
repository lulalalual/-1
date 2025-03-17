#pragma once
#include"vector"

#define SIZE_TILE 48

struct Tile
{
	enum class Direction
	{
		None = 0,
		Up,
		Down,
		Left,
		Right
	};
	//��ö�������ȫ

	int terrian = 0;//���β�
	int decoration = -1;//װ�β㣬Ĭ��Ϊ-1
	int special_flag = -1;//�����־��Ĭ��Ϊ-1
	bool has_tower = false;//��ǰ��Ƭ�Ƿ�����˷�����
	Direction direction = Direction::None;
};
//��Ƭͼ����

typedef std::vector<std::vector<Tile>> TileMap;//��ôһ����ά����