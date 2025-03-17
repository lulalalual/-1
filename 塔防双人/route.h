#pragma once

#include"tile.h"

#include"SDL.h"
#include"vector"

class Route
{
public:
	typedef std::vector<SDL_Point>IdxList;//�洢��Ԫ���ڵ�ͼ�ϵ�����,��·��ÿһ���������

public:
	Route() = default;

	//����һ��map���Ҵ�����ʼλ��ʱ���Ϳ�������һ�����еĵ�·
	Route(const TileMap& map, const SDL_Point& idx_origin)//�ڶ��������Ǵ��ĸ���ʼ�㿪ʼѰ·
	{
		size_t width_map = map[0].size();
		size_t height_map = map.size();
		SDL_Point idx_next = idx_origin;//����ʼ�㿪ʼѰ·

		while (true)
		{
			if (idx_next.x >= width_map || idx_next.y >= height_map)//·��������ͼ�߽�
				break;

			//���ѭ�����ܻῨ������Ϊ������õ�·��������ѭ��,����Ҫ����Ƿ��ظ�
			if (check_duplicate_idx(idx_next))
				break;
			else
				idx_list.push_back(idx_next);

			bool is_next_dir_exist = true;//���һ����û����һ�������
			const Tile& tile = map[idx_next.y][idx_next.x];
			
			if (tile.special_flag == 0)
				break;//��������ݣ��Ǿ�ͣ
			//������ݷ���ȥ�ƽ���һ����

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

			//�����һ��·�겻����
			if (!is_next_dir_exist)
				break;

			//����ѭ���������ظ���Ԫ���������ݣ��ߵ�ͷ�ˣ������߽�
		}
	}
	~Route() = default;

	const IdxList& get_idx_list() const
	{
		return idx_list;//���س�������
	}

private:
	IdxList idx_list;

private:
	//���·���Ƿ��Ѿ�������idxlist����,��ֹ�����ظ���������
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

//Ѱ·ʵ��