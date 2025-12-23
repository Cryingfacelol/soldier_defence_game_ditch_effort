//game.hpp


#pragma once
#include "common.hpp"
#include "entity.hpp"
#include "component.hpp"
#include "assets.hpp"
#include "waves.hpp"

class Game 
{
public:
	Game() = default;
	
	bool initialize();
	void shutdown();

	bool is_running() const;
	void poll_input();
	void update(float dt);
	void draw();

private:

	bool m_running = true;
	Vector2 m_window_size = {};
	int m_enemy_increase_per_wave = 0;
	int m_player_health = 5;
	int m_number_of_waves = 5;
	Color m_text_colour = {};
	Color m_background_color = {};
	int m_text_borders = 20;


	TextureCache m_texture_cache;

	SoundCache m_sound_cache;
	ScoreBoard m_scoreboard;

	InputAction m_input = {};

	Player m_player = {m_player_health};

	EnemyWave m_enemy_wave = { m_number_of_waves};
	CreateBullets m_bullet_creator;
	GamestateManager m_gamestate_manager;
	
	
};
