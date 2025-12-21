//game.cpp


#include "game.hpp"


bool Game::initialize() 
{
	

	m_screen_size = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };

	
	m_texture_cache.load("player", "assets/player.png");
	m_texture_cache.get("player", m_player.m_sprite.m_sprite_sheet);
	

	//create gamestate manager
	//add sound
	//spawn enemies over time
	
	m_player.m_sprite.m_source = { 0, 0, 20, 32 };
	m_player.m_transform.m_size = { 20.0f, 32.0f };
	m_player.m_transform.m_position = (m_screen_size *0.5) - (m_player.m_transform.m_size * 0.5f);
	
	m_wave_one.enemy_spawn(m_texture_cache, 30, m_screen_size);

	return true;
}

void Game::shutdown()
{
	UnloadTexture(m_player.m_sprite.m_sprite_sheet);
	
	for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [](auto& b) { UnloadTexture(b.m_sprite.m_sprite_sheet); });
	for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [](auto& e) { UnloadTexture(e.m_sprite.m_sprite_sheet); });
}

bool Game::is_running() const
{

	return m_running;
}

void Game::poll_input()
{
	m_input.poll_input();
}

void Game::update(float dt)
{
	m_player.update(dt, m_input );

	if (IsMouseButtonPressed(0)) 
	{ 
		m_bullet_creator.bullet_spawn(m_texture_cache, m_player.m_direction, m_player.m_transform.m_position, m_player.m_transform.m_size); 
	}
	
	if (!m_bullet_creator.m_bullets.empty()) 
	{
		std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.update(dt, m_screen_size); });
		std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.erase_outside_window(m_screen_size); });

		std::erase_if(m_bullet_creator.m_bullets, [=](auto& b) {if (b.m_active == false) { return true; } else { return false; }});
	}

	if (!m_wave_one.m_enemies.empty()) {
		std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [=](auto& e) {e.update(dt, m_input, m_player); });

		if (!m_bullet_creator.m_bullets.empty()) 
		{
			std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [=](auto& e) //checks all the enemies with all the bullets, collision detection and scoring
			{
				std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [&e](auto b)
					{
						if (e.m_alive) {
							e.is_hit(b);
						}
					});
				if (!e.m_alive) { m_scoreboard.add_score(1); }
			});
		}

		std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [=](auto& e) {e.reached_player(m_player); });

		std::erase_if(m_wave_one.m_enemies, [](auto& e) {if (e.m_alive == false) { return true; } else { return false; } });
	}
}

void Game::draw()
{
	Color background_color = DARKBLUE;

	BeginDrawing();
	ClearBackground(background_color);

	std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.draw(); });
	m_player.draw();
	
	std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [](auto& e) {e.draw(); });
	
	m_scoreboard.draw();

	EndDrawing();
}