//waves.hpp

#pragma once

#include "entity.hpp"
#include "component.hpp"
#include "common.hpp"
#include "assets.hpp"

struct EnemyWave
{
	EnemyWave(int final_wave_number);
	
	void get_random_spawn_position(Vector2 screen_size);
	void enemy_spawn(TextureCache& texture_cache, Vector2 screen_size);
	void reset();

	int m_added_enemies_per_wave = 0;
	int m_max_number_of_waves = 1;
	int m_wave_number = 1;
	int m_number_of_enemies = 0;


	int m_type = 0;
	int m_x_or_y = 0;
	int m_up_or_down = 0;
	int m_left_or_right = 0;

	int m_enemy_walk_type = 0;


	Rectangle source = { };
	Vector2 size = { };
	Vector2 m_spawn_position = {};

	std::vector<Enemy> m_enemies;
	
};

struct CreateBullets
{
	CreateBullets() = default;

	bool random_bullet_sound(SoundCache& sound_cache);
	bool random_empty_sound(SoundCache& sound_cache);
	void bullet_spawn(TextureCache& texture_cache, SoundCache& sound_cache, Player player, float& time_since_last_bullet);
	void reset();

	int number_of_bullets_added = 0;
	int m_total_bullets = 0;
	float m_rotation = 0.0f;
	

	Rectangle m_source = { };
	Vector2 m_size = { };
	Vector2 m_spawn_position = {};

	Sound m_bullet_sound = {};
	int sound_number = 0;

	std::vector<Bullet> m_bullets;

};


struct ScoreBoard
{
	ScoreBoard() = default;

	void add_score(int score_increase);
	void draw(Vector2 window_size) const;
	void reset();
	int m_score = 0;
};

struct GamestateManager
{
	GamestateManager() = default;
	
	enum gamestate{start, playing, win, lose, exit};
	
	void change_gamestate(gamestate new_state );

	
	gamestate m_gamestate = start;
};
