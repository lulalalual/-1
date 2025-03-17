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
	//用枚举类更安全

	int terrian = 0;//地形层
	int decoration = -1;//装饰层，默认为-1
	int special_flag = -1;//特殊标志，默认为-1
	bool has_tower = false;//当前瓦片是否放置了防御塔
	Direction direction = Direction::None;
};
//瓦片图集类

typedef std::vector<std::vector<Tile>> TileMap;//这么一个二维数组