//waves.hpp

#pragma once

#include "entity.hpp"
#include "component.hpp"
#include "common.hpp"
#include "assets.hpp"

struct EnemyWave
{
	EnemyWave(int final_wave_number);

	void enemy_spawn(TextureCache& texture_cache, int number_of_enemies, Vector2 screen_size);

	int m_max_number_of_waves = 1;
	int m_wave_number = 1;
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
	void draw(Vector2 window_size) const;
	int m_score = 0;
};

struct GamestateManager
{
	GamestateManager() = default;
	
	enum gamestate{start, playing, win, lose};
	
	void change_gamestate(gamestate new_state );

	
	gamestate m_gamestate = start;
};