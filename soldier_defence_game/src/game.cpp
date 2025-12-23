//game.cpp


#include "game.hpp"


bool Game::initialize() 
{
	m_window_size = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };
	m_text_colour = RAYWHITE;
	m_background_color = DARKGREEN;
	 
	m_texture_cache.load("player", "assets/player.png");
	m_texture_cache.get("player", m_player.m_sprite.m_sprite_sheet);

	m_sound_cache.load("player_hit", "assets/player_hit_001.ogg");
	m_sound_cache.load("player_dead", "assets/player_dead.ogg"); 

	m_sound_cache.load("bullet_leaves_gun_001", "assets/bullet_leaves_gun_001.ogg");
	m_sound_cache.load("bullet_leaves_gun_002", "assets/bullet_leaves_gun_002.ogg");
	m_sound_cache.load("bullet_leaves_gun_003", "assets/bullet_leaves_gun_003.ogg");

	m_sound_cache.load("gun_empty_001", "assets/gun_empty_001.ogg");
	m_sound_cache.load("gun_empty_002", "assets/gun_empty_002.ogg");
	m_sound_cache.load("gun_empty_003", "assets/gun_empty_003.ogg");

	m_sound_cache.load("enemy_hit_001", "assets/enemy_hit.ogg");
	m_sound_cache.load("enemy_hit_002", "assets/enemy_hit_002.ogg");
	m_sound_cache.load("enemy_hit_003", "assets/enemy_hit_003.ogg");
	m_sound_cache.load("enemy_hit_004", "assets/enemy_hit_004.ogg");
	m_sound_cache.load("enemy_hit_005", "assets/enemy_hit_005.ogg");




	//add sound
	// bullet spawn,  enemy dead, win, lose, (background music?)
	// add colour change to player when hit
	// OPTIONAL:
	// make a place to put all the screen rendering?




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
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::start)
	{
		if (IsKeyPressed(KEY_S)) { m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::playing); }
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
						if (!e.m_alive) { m_scoreboard.add_score(1); e.play_death_sound(m_sound_cache); }
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


			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::lose); 
		}

		if (m_enemy_wave.m_wave_number >= m_enemy_wave.m_max_number_of_waves && m_enemy_wave.m_enemies.empty()) 
		{

			m_enemy_wave.reset();
			m_bullet_creator.reset();
			m_player.reset(m_player_health);


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
		}
		if (IsKeyPressed(KEY_Q)) { m_running = false; }
	}

	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::lose) 
	{

		if (IsKeyPressed(KEY_R)) 
		{ 
			m_scoreboard.reset();
			m_gamestate_manager.change_gamestate(m_gamestate_manager.gamestate::playing); 
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
		DrawText("Ditch Effort", m_text_borders, int(m_window_size.y * 0.5)- (2*m_text_borders), 50, m_text_colour);
		DrawText("Press S to begin your last stand", m_text_borders, int(m_window_size.y * 0.5)+ m_text_borders, 20, m_text_colour);
		DrawText("Press Q to QUIT", m_text_borders, int(m_window_size.y * 0.5)+(2 * m_text_borders), 20, m_text_colour);

	}
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::playing) 
	{
		std::for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [=](auto& b) {b.draw(); });
		m_player.draw();
	
		std::for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [](auto& e) {e.draw(); });

		DrawText(TextFormat("Health: %i", m_player.m_health), m_text_borders, 3*m_text_borders, 40, m_text_colour);
		if(m_scoreboard.m_score == 0 ) { DrawText("LEFT CLICK TO SHOOT", int(m_window_size.x * 0.5)- (2*m_text_borders), int(m_window_size.y )- (2*m_text_borders), 30, m_text_colour); }
	}
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::win) 
	{
		DrawText("You have been rescued by comrades, congratulations!", m_text_borders, int(m_window_size.y * 0.5), 20, m_text_colour);
		DrawText("Press R to restart", m_text_borders, int(m_window_size.y * 0.5) + m_text_borders, 20, m_text_colour);
		DrawText("Press Q to QUIT", m_text_borders, int(m_window_size.y * 0.5) + (2 * m_text_borders), 20, m_text_colour);
	}
	if (m_gamestate_manager.m_gamestate == m_gamestate_manager.gamestate::lose) 
	{
		DrawText("You have died. At least you took as many as possible down with you...", m_text_borders, int(m_window_size.y * 0.5), 20, m_text_colour);
		DrawText("Press R to restart", m_text_borders, int(m_window_size.y * 0.5) + m_text_borders, 20, m_text_colour);
		DrawText("Press Q to QUIT", m_text_borders, int(m_window_size.y * 0.5)+(2*m_text_borders), 20, m_text_colour);
	}

	
	
	m_scoreboard.draw(m_window_size);

	EndDrawing();
}