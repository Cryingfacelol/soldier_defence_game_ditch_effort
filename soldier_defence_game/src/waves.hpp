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

struct CreateBullets
{
	CreateBullets() = default;

	void bullet_spawn(TextureCache& texture_cache, Vector2 direction, Vector2 player_position, Vector2 player_size);

	int number_of_bullets_added = 1;
	int m_max_number_of_bullets = 5;

	std::vector<Bullet> m_bullets;

};


struct ScoreBoard
{
	ScoreBoard() = default;

	void add_score(int score_increase);
	void draw() const;
	int m_score = 0;
};