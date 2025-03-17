#pragma once
#include"tile.h"
#include"SDL.h"
#include"string"
#include"fstream"
#include"sstream"
#include"unordered_map"//哈希表
#include"route.h"

class Map
{
public:
	typedef std::unordered_map<int, Route>SpawnerRoutePool;//哈希表的键为int,值为Route

public:
	Map() = default;
	~Map() = default;

	bool load(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.good()) return false;

		TileMap tile_map_temp;

		int idx_x = -1, idx_y = -1;

		std::string str_line;
		while (std::getline(file, str_line))
		{
			str_line = trim_str(str_line);//进行首尾空白字符切割
			if (str_line.empty())
			{
				continue;
			}//如果截取到是空白，证明遇到了空行，所以要跳过

			idx_x = -1, idx_y++;//行索引进行累加
			tile_map_temp.emplace_back();//临时瓦片新增一行

			std::string str_tile;
			std::stringstream str_stream(str_line);
			while (std::getline(str_stream, str_tile, ','))
			{
				idx_x++;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();
				load_tile_from_string(tile, str_tile);
			}//在一行的内容里依次增加列
		}
		file.close();

		if (tile_map_temp.empty() || tile_map_temp[0].empty())
			return false;
		//如果地图空，那就返回false
		tile_map = tile_map_temp;
		generate_map_cache();
		return true;
	}
	size_t get_width()const
	{
		if (tile_map.empty())
		{
			return 0;
		}
		return tile_map[0].size();//返回第一行的长度，其实就代表宽度
	}
	//得到地图的宽度

	size_t get_height()const
	{
		return tile_map.size();
	}
	//得到地图的高度

	const TileMap& get_tile_map() const
	{
		return tile_map;
	}

	const SDL_Point& get_idx_home() const
	{
		return idx_home;
	}

	const SpawnerRoutePool& get_idx_spawner_pool() const
	{
		return spawner_route_pool;
	}

	void place_tower(const SDL_Point& idx_tile)
	{
		tile_map[idx_tile.y][idx_tile.x].has_tower = true;
	}
	//在这个瓦片上放置防御塔

private:
	TileMap tile_map;
	SDL_Point idx_home = { 0 };//防守位点的索引位置
	SpawnerRoutePool spawner_route_pool;

private:
	std::string trim_str(const std::string& str)
	{
		//当传入一个"   asda   "
		//最终截取到一个"asda"，用来截取首尾空白字符

		size_t begin_idx = str.find_first_not_of(" \t");
		if (begin_idx == std::string::npos)
			return "";
		size_t end_idx = str.find_last_not_of(" \t");
		size_t idx_range = end_idx - begin_idx + 1;

		return str.substr(begin_idx, idx_range);
	}

	//以下这个函数就完成了把字符串中的内容解析到tile对象中了
	void load_tile_from_string(Tile& tile, const std::string& str)
	{
		std::string str_tidy = trim_str(str);
		std::string str_value;
		std::vector<int>values;
		std::stringstream str_stream(str_tidy);

		while (std::getline(str_stream, str_value, '\\'))
		{
			int value;
			try
			{
				value = std::stoi(str_value);//当然也有可能不是数字，就抛出异常
			}
			catch (const std::invalid_argument&)//抛出不合法的参数的异常
			{
				value = -1;
			}
			values.push_back(value);
		}
		//用\分割一个单元格以内的数字，把数字字符串转换成数值

		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
			//如果里面一个数字都没有，或者第一个数字小于0(地形小于0是非法的)
		tile.decoration = (values.size() < 2) ? -1 : values[1];
			//如果只有一个数字，缺少装饰层
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
			//如果少个方向或者代表方向的数字错了的情况
		tile.special_flag = (values.size() <= 3) ? -1 : values[3];
	}
	//把传入的单元格的字符串，把里面根据反斜线风格的数字拿出来，存到values里来
	
	//地图缓存的生成工作
	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				const Tile& tile = tile_map[y][x];
				if (tile.special_flag < 0)
					continue;//没有特殊标志就跳过
				if (tile.special_flag == 0)
				{
					idx_home.x = x;
					idx_home.y = y;
					//等于零代表是房子，就可以把房子的水平和竖直坐标复制过来
				}
				else//扫描到了一个刷怪点
				{
					spawner_route_pool[tile.special_flag] = Route(tile_map, { x,y });
					//就把他放到刷怪点的路径池
				}
			}
		}
	}
};