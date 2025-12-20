//game.cpp


#include "game.hpp"


bool Game::initialize() 
{
	

	m_screen_size = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };

	
	m_texture_cache.load("player", "assets/player.png");
	m_texture_cache.get("player", m_player.m_sprite.m_sprite_sheet);
	
	m_texture_cache.load("bullet", "assets/ball.png"); 
	m_texture_cache.get("bullet", m_bullet.m_sprite.m_sprite_sheet);

	m_texture_cache.load("enemy", "assets/enemy.png");
	m_texture_cache.get("enemy", m_enemy.m_sprite.m_sprite_sheet);

	
	
	m_player.m_sprite.m_source = { 0, 0, 20, 32 };
	m_player.m_transform.m_size = { 20.0f, 32.0f };
	m_player.m_transform.m_position = (m_screen_size - m_player.m_transform.m_size) * 0.5f;
	

	m_bullet.m_sprite.m_source = { 0, 0, 16, 16 };
	m_bullet.m_transform.m_size = { 16.0f, 16.0f };
	m_bullet.m_transform.m_position = (m_screen_size - m_bullet.m_transform.m_size) * 0.5f;

	m_enemy.m_sprite.m_source = { 0, 0, 32, 32 };
	m_enemy.m_transform.m_size = { 32.0f, 32.0f };
	m_enemy.m_transform.m_position = m_screen_size;

	return true;
}

void Game::shutdown()
{
	UnloadTexture(m_player.m_sprite.m_sprite_sheet);
	UnloadTexture(m_bullet.m_sprite.m_sprite_sheet);
	UnloadTexture(m_enemy.m_sprite.m_sprite_sheet);
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

	m_bullet.update(dt, m_player, m_input, m_screen_size);
	m_bullet.erase_outside_window(m_screen_size);

	m_enemy.update(dt, m_input, m_player);


}

void Game::draw()
{
	Color background_color = DARKBLUE;

	BeginDrawing();
	ClearBackground(background_color);
	
	m_bullet.draw();
	m_player.draw();
	m_enemy.draw();

	EndDrawing();
}


