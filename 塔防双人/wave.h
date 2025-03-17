#pragma once
#include"enemy_type.h"
#include"vector"

struct Wave
{
	struct SpawnEvent
	{
		double interval = 0;
		int spawn_point = 1;
		EnemyType enemy_type = EnemyType::Slim;
	};
	//有关生成的项目

	double rewards = 0;
	double interval = 0;
	std::vector<SpawnEvent> spawn_event_list;

};
//波次