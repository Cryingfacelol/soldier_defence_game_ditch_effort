//game.cpp


#include "game.hpp"


bool Game::initialize() 
{
	m_enemy_increase_per_wave = 5;
	m_window_size = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };
	m_text_colour = RAYWHITE;
	m_background_color = DARKGREEN;
	 
	m_texture_cache.load("player", "assets/player.png");
	m_texture_cache.get("player", m_player.m_sprite.m_sprite_sheet);


		//add sound
	//cooldown based on time for projectiles
	//add second type of enemy


	m_player.m_sprite.m_source = { 0, 0, 20, 32 };
	m_player.m_transform.m_size = { 20.0f, 32.0f };
	m_player.m_transform.m_position = (m_window_size * 0.5) - (m_player.m_transform.m_size * 0.5f);
	
	
	m_enemy_wave.enemy_spawn(m_texture_cache, m_enemy_increase_per_wave, m_window_size);

	return true;
}

void Game::shutdown()
{
	UnloadTexture(m_player.m_sprite.m_sprite_sheet);
	
	for_each(m_bullet_creator.m_bullets.begin(), m_bullet_creator.m_bullets.end(), [](auto& b) { UnloadTexture(b.m_sprite.m_sprite_sheet); });
	for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [](auto& e) { UnloadTexture(e.m_sprite.m_sprite_sheet); });
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
			m_bullet_creator.bullet_spawn(m_texture_cache, m_player.m_direction, m_player.m_transform.m_position, m_player.m_transform.m_size);
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
						if (!e.m_alive) { m_scoreboard.add_score(1); }
					});
			}

			std::for_each(m_enemy_wave.m_enemies.begin(), m_enemy_wave.m_enemies.end(), [=](auto& e) {e.reached_player(m_player); });

			std::erase_if(m_enemy_wave.m_enemies, [](auto& e) {if (e.m_alive == false) { return true; } else { return false; } });
		}

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
			m_enemy_wave.enemy_spawn(m_texture_cache, m_enemy_increase_per_wave, m_window_size);
		}
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