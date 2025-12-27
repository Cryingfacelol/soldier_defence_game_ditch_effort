//game.cpp


#include "game.hpp"


bool Game::initialize() 
{
	m_window_size = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };
	m_background_color = DARKGREEN;
	m_text_renderer.m_window_size = m_window_size;

	
	m_assets_loader.load_textures(m_texture_cache);
	m_assets_loader.load_sounds(m_sound_cache);

	PlayMusicStream(m_assets_loader.m_background_music);
	PauseMusicStream(m_assets_loader.m_background_music);
	
	
	m_texture_cache.get("player", m_player.m_sprite.m_sprite_sheet);

	m_player.m_sprite.m_source = { 0, 0, 20, 32 };
	m_player.m_transform.m_size = { 20.0f, 32.0f };
	m_player.m_transform.m_position = (m_window_size * 0.5) - (m_player.m_transform.m_size * 0.5f);
	
	m_enemy_wave.enemy_spawn(m_texture_cache, m_window_size);

	return true;
}

void Game::shutdown()
{
	UnloadTexture(m_player.m_sprite.m_sprite_sheet);
	
	m_texture_cache.unload_all();
	m_sound_cache.unload_all();
	UnloadMusicStream(m_assets_loader.m_background_music);
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
	UpdateMusicStream(m_assets_loader.m_background_music);

	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::start)
	{
		if (IsKeyPressed(KEY_S)) { 
			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::playing); 
		ResumeMusicStream(m_assets_loader.m_background_music);
		}
		if (IsKeyPressed(KEY_Q)) { m_running = false; }
	}

	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::playing)
	{
		m_player.update(dt, m_input);

		if (IsMouseButtonPressed(0))
		{
			m_bullet_creator.bullet_spawn(m_texture_cache, m_sound_cache, m_player, m_time_since_last_bullet);
		}

		if (!m_bullet_creator.m_bullets.empty())
		{
			std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.update(dt, m_window_size); });
			std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.erase_outside_window(m_window_size); });

			std::erase_if(m_bullet_creator.m_bullets, [=](auto& b) {if (b.m_active == false) { return true; } else { return false; }});
		}

		if (!m_enemy_wave.m_enemies.empty()) {

			std::for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [=](auto& e) {e.update(dt, m_input, m_player); });

			if (!m_bullet_creator.m_bullets.empty())
			{
				std::for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [=](auto& e) //checks all the enemies with all the bullets, collision detection and scoring
					{
						std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [&e](auto b)
							{
								if (e.m_alive) {
									e.is_hit(b);
								}
							});
						if (!e.m_alive) { m_scoreboard.add_score(m_value_of_one_enemy); e.play_death_sound(m_sound_cache); }
					});
			}

			std::for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [=](auto& e) 
			{
				if (e.reached_player(m_player)) 
				{ 
					m_player.play_hit_sound(m_sound_cache); 
					m_player.m_hit = true; 

				}
			});

			std::erase_if(m_enemy_wave.m_enemies, [](auto& e) {if (e.m_alive == false) { return true; } else { return false; } });
		}
		m_player.tint_change_hit(dt);

		if (m_player.m_health <= 0) 
		{ 

			m_enemy_wave.reset();
			m_bullet_creator.reset();
			m_player.reset(m_player_health);

			PauseMusicStream(m_assets_loader.m_background_music);

			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::lose); 
		}

		if (m_enemy_wave.m_wave_number >= m_enemy_wave.m_max_number_of_waves && m_enemy_wave.m_enemies.empty()) 
		{

			m_enemy_wave.reset();
			m_bullet_creator.reset();
			m_player.reset(m_player_health);

			PauseMusicStream(m_assets_loader.m_background_music);

			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::win); 
		}
		
		if (m_enemy_wave.m_enemies.empty()) 
		{
			m_enemy_wave.enemy_spawn(m_texture_cache, m_window_size);
		}
		m_time_since_last_bullet += dt;
	}

	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::win)
	{
		
		if (IsKeyPressed(KEY_R)) 
		{ 
			m_scoreboard.reset();
			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::playing); 
			ResumeMusicStream(m_assets_loader.m_background_music);
		}
		if (IsKeyPressed(KEY_Q)) { m_running = false; }
	}

	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::lose) 
	{

		if (IsKeyPressed(KEY_R)) 
		{ 
			m_scoreboard.reset();
			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::playing); 
			ResumeMusicStream(m_assets_loader.m_background_music);
		}
		if (IsKeyPressed(KEY_Q)) { m_running = false; }
	}

	

}

void Game::draw()
{

	BeginDrawing();

	ClearBackground(m_background_color);
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::start) 
	{
		m_text_renderer.start_screen_text();
	}
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::playing) 
	{
		std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.draw(); });
		m_player.draw();
	
		std::for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [](auto& e) {e.draw(); });

		m_text_renderer.playing_text(m_player.m_health, m_bullet_creator.m_first_bullet_shot);
	}
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::win) 
	{
		m_text_renderer.win_screen_text();
	}
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::lose) 
	{
		m_text_renderer.lose_screen_text();
	}

	
	
	m_scoreboard.draw(m_window_size);

	EndDrawing();
}