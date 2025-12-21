//game.cpp


#include "game.hpp"


bool Game::initialize() 
{
	

	m_screen_size = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };

	
	m_texture_cache.load("player", "assets/player.png");
	m_texture_cache.get("player", m_player.m_sprite.m_sprite_sheet);
	
	m_texture_cache.load("bullet", "assets/bullet.png"); 
	m_texture_cache.get("bullet", m_bullet.m_sprite.m_sprite_sheet);

	
	//make a specified initialize
	//create vectors of enemies and bullets
	//move collision detection out to the game loop
	
	m_player.m_sprite.m_source = { 0, 0, 20, 32 };
	m_player.m_transform.m_size = { 20.0f, 32.0f };
	m_player.m_transform.m_position = (m_screen_size - m_player.m_transform.m_size) * 0.5f;
	

	m_bullet.m_sprite.m_source = { 0, 0, 8, 8 };
	m_bullet.m_transform.m_size = { 8.0f, 8.0f };
	m_bullet.m_transform.m_position = (m_screen_size - m_bullet.m_transform.m_size) * 0.5f;

	m_wave_one.enemy_spawn(m_texture_cache, 30, m_screen_size);

	return true;
}

void Game::shutdown()
{
	UnloadTexture(m_player.m_sprite.m_sprite_sheet);
	UnloadTexture(m_bullet.m_sprite.m_sprite_sheet);
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

	m_bullet.update(dt, m_player, m_input, m_screen_size);
	m_bullet.erase_outside_window(m_screen_size);

	std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [=](auto& e) {e.update(dt, m_input, m_player ); });
	std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [=](auto& e) {e.is_hit(m_bullet);} );
	std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [=](auto& e) {e.reached_player(m_player); });
	std::erase_if(m_wave_one.m_enemies, [](auto& e) {
		if (e.m_alive == false) { return true; }
		else { return false; }
		} );
}

void Game::draw()
{
	Color background_color = DARKBLUE;

	BeginDrawing();
	ClearBackground(background_color);

	m_bullet.draw();
	m_player.draw();

	std::for_each(m_wave_one.m_enemies.begin(), m_wave_one.m_enemies.end(), [](auto& e) {e.draw(); });

	EndDrawing();
}