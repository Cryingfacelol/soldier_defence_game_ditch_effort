//waves.hpp

#pragma once

#include "entity.hpp"
#include "component.hpp"
#include "common.hpp"
#include "assets.hpp"

struct EnemyWave
{
	EnemyWave() = default;

	void enemy_spawn(TextureCache& texture_cache, int number_of_enemies, Vector2 screen_size);

	int m_killed = 0;
	std::vector<Enemy> m_enemies;
};