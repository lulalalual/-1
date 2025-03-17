#pragma once

#include"tile.h"

#include"SDL.h"
#include"vector"

class Route
{
public:
	typedef std::vector<SDL_Point>IdxList;//存储单元格在地图上的索引,即路线每一个点的索引

public:
	Route() = default;

	//传入一个map并且传入起始位点时，就可以生成一条可行的道路
	Route(const TileMap& map, const SDL_Point& idx_origin)//第二个参数是从哪个初始点开始寻路
	{
		size_t width_map = map[0].size();
		size_t height_map = map.size();
		SDL_Point idx_next = idx_origin;//从起始点开始寻路

		while (true)
		{
			if (idx_next.x >= width_map || idx_next.y >= height_map)//路径超出地图边界
				break;

			//这个循环可能会卡死，因为玩家设置的路径可能是循环,所以要检测是否重复
			if (check_duplicate_idx(idx_next))
				break;
			else
				idx_list.push_back(idx_next);

			bool is_next_dir_exist = true;//检查一下有没有下一个方向标
			const Tile& tile = map[idx_next.y][idx_next.x];
			
			if (tile.special_flag == 0)
				break;//如果到房屋，那就停
			//否则根据方向去推进下一个点

			switch (tile.direction)
			{
			case Tile::Direction::Up:
				idx_next.y--;
				break;
			case Tile::Direction::Down:
				idx_next.y++;
				break;
			case Tile::Direction::Left:
				idx_next.x--;
				break;
			case Tile::Direction::Right:
				idx_next.x++;
				break;
			default:
				is_next_dir_exist = false;
				break;
			}

			//如果下一个路标不存在
			if (!is_next_dir_exist)
				break;

			//跳出循环：遇见重复单元格，遇到房屋，走到头了，超出边界
		}
	}
	~Route() = default;

	const IdxList& get_idx_list() const
	{
		return idx_list;//返回常量引用
	}

private:
	IdxList idx_list;

private:
	//检查路径是否已经出现在idxlist里面,防止出现重复的索引点
	bool check_duplicate_idx(const SDL_Point& target_idx)
	{
		for (const SDL_Point& idx : idx_list)
		{
			if (idx.x == target_idx.x && idx.y == target_idx.y)
				return true;
		}
		return false;
	}
};

//寻路实现