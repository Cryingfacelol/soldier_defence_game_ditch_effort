// waves.cpp

#include "waves.hpp"


void EnemyWave::enemy_spawn(TextureCache& texture_cache, int number_of_enemies, Vector2 screen_size)
{
	Rectangle source = { 0, 0, 32, 32 };
	Vector2 size = { 32.0f, 32.0f };
	Vector2 spawn_position = {};
	int x_or_y = 1;
	int up_or_down = 1;
	int left_or_right = 1;
	texture_cache.load("enemy", "assets/enemy.png");

	for (int i = 0; i < number_of_enemies; i++) {

		x_or_y = GetRandomValue(1, 2);
		up_or_down = GetRandomValue(1, 2);
		left_or_right = GetRandomValue(1, 2);

		if (x_or_y == 1) {
			spawn_position.x = (float)GetRandomValue(0, int(screen_size.x));

			if (spawn_position.x < 0 || spawn_position.x > screen_size.x || spawn_position.x == 0 || spawn_position.x == screen_size.x)
			{
				spawn_position.y = (float)GetRandomValue(0, int(screen_size.y));
			}
			if (screen_size.x > spawn_position.x && spawn_position.x > 0)
			{
				if (up_or_down == 1) 
				{
				spawn_position.y = 0.0f;
				}
				if (up_or_down == 2) 
				{
					spawn_position.y = screen_size.y;
				}
			}
		}
		if (x_or_y == 2) {
			spawn_position.y = (float)GetRandomValue(0, int(screen_size.y));

			if (spawn_position.y < 0 || spawn_position.y > screen_size.y || spawn_position.y == 0 || spawn_position.y == screen_size.y)
			{
				spawn_position.x = (float)GetRandomValue(0, int(screen_size.x));
			}
			if (screen_size.y > spawn_position.y && spawn_position.y > 0)
			{
				if (left_or_right == 1)
				{
					spawn_position.x = 0.0f;
				}
				if (left_or_right == 2)
				{
					spawn_position.x = screen_size.x;
				}
			}
		}
		

		m_enemies.push_back({
			source,
			size,
			spawn_position
			});

		texture_cache.get("enemy", m_enemies[i].m_sprite.m_sprite_sheet);
	}

}
