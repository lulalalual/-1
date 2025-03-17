#pragma once
#include"tile.h"
#include"SDL.h"
#include"string"
#include"fstream"
#include"sstream"
#include"unordered_map"//��ϣ��
#include"route.h"

class Map
{
public:
	typedef std::unordered_map<int, Route>SpawnerRoutePool;//��ϣ��ļ�Ϊint,ֵΪRoute

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
			str_line = trim_str(str_line);//������β�հ��ַ��и�
			if (str_line.empty())
			{
				continue;
			}//�����ȡ���ǿհף�֤�������˿��У�����Ҫ����

			idx_x = -1, idx_y++;//�����������ۼ�
			tile_map_temp.emplace_back();//��ʱ��Ƭ����һ��

			std::string str_tile;
			std::stringstream str_stream(str_line);
			while (std::getline(str_stream, str_tile, ','))
			{
				idx_x++;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();
				load_tile_from_string(tile, str_tile);
			}//��һ�е�����������������
		}
		file.close();

		if (tile_map_temp.empty() || tile_map_temp[0].empty())
			return false;
		//�����ͼ�գ��Ǿͷ���false
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
		return tile_map[0].size();//���ص�һ�еĳ��ȣ���ʵ�ʹ�����
	}
	//�õ���ͼ�Ŀ��

	size_t get_height()const
	{
		return tile_map.size();
	}
	//�õ���ͼ�ĸ߶�

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
	//�������Ƭ�Ϸ��÷�����

private:
	TileMap tile_map;
	SDL_Point idx_home = { 0 };//����λ�������λ��
	SpawnerRoutePool spawner_route_pool;

private:
	std::string trim_str(const std::string& str)
	{
		//������һ��"   asda   "
		//���ս�ȡ��һ��"asda"��������ȡ��β�հ��ַ�

		size_t begin_idx = str.find_first_not_of(" \t");
		if (begin_idx == std::string::npos)
			return "";
		size_t end_idx = str.find_last_not_of(" \t");
		size_t idx_range = end_idx - begin_idx + 1;

		return str.substr(begin_idx, idx_range);
	}

	//�����������������˰��ַ����е����ݽ�����tile��������
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
				value = std::stoi(str_value);//��ȻҲ�п��ܲ������֣����׳��쳣
			}
			catch (const std::invalid_argument&)//�׳����Ϸ��Ĳ������쳣
			{
				value = -1;
			}
			values.push_back(value);
		}
		//��\�ָ�һ����Ԫ�����ڵ����֣��������ַ���ת������ֵ

		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
			//�������һ�����ֶ�û�У����ߵ�һ������С��0(����С��0�ǷǷ���)
		tile.decoration = (values.size() < 2) ? -1 : values[1];
			//���ֻ��һ�����֣�ȱ��װ�β�
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
			//����ٸ�������ߴ���������ִ��˵����
		tile.special_flag = (values.size() <= 3) ? -1 : values[3];
	}
	//�Ѵ���ĵ�Ԫ����ַ�������������ݷ�б�߷��������ó������浽values����
	
	//��ͼ��������ɹ���
	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				const Tile& tile = tile_map[y][x];
				if (tile.special_flag < 0)
					continue;//û�������־������
				if (tile.special_flag == 0)
				{
					idx_home.x = x;
					idx_home.y = y;
					//����������Ƿ��ӣ��Ϳ��԰ѷ��ӵ�ˮƽ����ֱ���긴�ƹ���
				}
				else//ɨ�赽��һ��ˢ�ֵ�
				{
					spawner_route_pool[tile.special_flag] = Route(tile_map, { x,y });
					//�Ͱ����ŵ�ˢ�ֵ��·����
				}
			}
		}
	}
};