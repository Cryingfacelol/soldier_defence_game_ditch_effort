// waves.cpp

#include "waves.hpp"


EnemyWave::EnemyWave(int final_wave_number)
{
	m_max_number_of_waves = final_wave_number;
}

void EnemyWave::get_random_spawn_position(Vector2 screen_size)
{

	//this section spawns enemies from all edges evenly(thats why half rendomize the x value first and half randomize the y value first)

	m_x_or_y = GetRandomValue(1, 2);
	m_up_or_down = GetRandomValue(1, 2);
	m_left_or_right = GetRandomValue(1, 2);

	if (m_x_or_y == 1) {
		m_spawn_position.x = (float)GetRandomValue(0, int(screen_size.x));

		if (m_spawn_position.x < 0 || m_spawn_position.x > screen_size.x || m_spawn_position.x == 0 || m_spawn_position.x == screen_size.x)
		{
			m_spawn_position.y = (float)GetRandomValue(0, int(screen_size.y));
		}
		if (screen_size.x > m_spawn_position.x && m_spawn_position.x > 0)
		{
			if (m_up_or_down == 1)
			{
				m_spawn_position.y = 0.0f;
			}
			if (m_up_or_down == 2)
			{
				m_spawn_position.y = screen_size.y;
			}
		}
	}

	if (m_x_or_y == 2) {
		m_spawn_position.y = (float)GetRandomValue(0, int(screen_size.y));

		if (m_spawn_position.y < 0 || m_spawn_position.y > screen_size.y || m_spawn_position.y == 0 || m_spawn_position.y == screen_size.y)
		{
			m_spawn_position.x = (float)GetRandomValue(0, int(screen_size.x));
		}
		if (screen_size.y > m_spawn_position.y && m_spawn_position.y > 0)
		{
			if (m_left_or_right == 1)
			{
				m_spawn_position.x = 0.0f;
			}
			if (m_left_or_right == 2)
			{
				m_spawn_position.x = screen_size.x;
			}
		}
	}

}

void EnemyWave::enemy_spawn(TextureCache& texture_cache, Vector2 screen_size)
{
	source = { 0, 0, 32, 32 };
	size = { 32.0f, 32.0f };

	m_added_enemies_per_wave = 5;
	m_max_number_of_waves = 5;

	m_type = 1;
	m_x_or_y = 1;
	m_up_or_down = 1;
	m_left_or_right = 1;
	

	m_number_of_enemies += m_added_enemies_per_wave;

	for (int i = 0; i < m_number_of_enemies; i++) {
		m_type = GetRandomValue(1, m_wave_number * 2);
		
		if (m_type >= m_wave_number ) { m_enemy_walk_type = 1; }
		if (m_type <  m_wave_number ) { m_enemy_walk_type = 2; }
				
		get_random_spawn_position(screen_size);

		m_enemies.push_back({
			source,
			size,
			m_spawn_position,
			m_enemy_walk_type
			});

		texture_cache.get("enemy", m_enemies[i].m_sprite.m_sprite_sheet);
	}
	m_wave_number++;


}

void EnemyWave::reset()
{
	m_wave_number = 1;
	m_enemies.clear();
	m_number_of_enemies = 0;
}

void ScoreBoard::add_score(int score_increase)
{
	m_score += score_increase;

}

void ScoreBoard::draw(Vector2 window_size) const
{
	DrawText(TextFormat("Score: %i", m_score), m_text_borders, m_text_borders, m_text_size*2, RAYWHITE);

}
void ScoreBoard::reset()
{
	m_score = 0;

}

bool CreateBullets::random_bullet_sound(SoundCache& sound_cache)
{
	sound_number = GetRandomValue(1, 3);
	if (sound_number == 1) { 
		return sound_cache.get("bullet_leaves_gun_001", m_bullet_sound);
	}
	if (sound_number == 2) { 
		return sound_cache.get("bullet_leaves_gun_002", m_bullet_sound);
	}
	else { 
		return sound_cache.get("bullet_leaves_gun_003", m_bullet_sound);
	}
}

bool CreateBullets::random_empty_sound(SoundCache& sound_cache)
{
	sound_number = GetRandomValue(1, 3);
	if (sound_number == 1) { 
		return sound_cache.get("gun_empty_001", m_bullet_sound);
	}
	if (sound_number == 2) { 
		return sound_cache.get("gun_empty_002", m_bullet_sound);
	}
	else { 
		return sound_cache.get("gun_empty_003", m_bullet_sound);
	}



}

void CreateBullets::bullet_spawn(TextureCache& texture_cache, SoundCache& sound_cache, Player player, float& time_since_last_bullet)
{
	m_source = { 0, 0, 8, 8 };
	m_size = { 8.0f, 8.0f };
	m_spawn_position.x = player.m_transform.m_position.x + (player.m_transform.m_size.x*0.5f) - (m_size.x * 0.5f );
	m_spawn_position.y = player.m_transform.m_position.y - (m_size.y * 0.5f);


	number_of_bullets_added = 1;
	m_total_bullets = (int)m_bullets.size() + number_of_bullets_added;
	m_rotation = 90.0f;

	

	if (time_since_last_bullet >= 0.25) 
	{
		m_first_bullet_shot = true;

		

		if (random_bullet_sound(sound_cache))
		{
			SetSoundVolume(m_bullet_sound, 0.5f);
			PlaySound(m_bullet_sound);
		}
		for (int i = (int)m_bullets.size(); i < m_total_bullets; i++) 
		{
			m_bullets.push_back({ player.m_direction, m_spawn_position, m_rotation, m_size, m_source });

			texture_cache.get("bullet", m_bullets[i].m_sprite.m_sprite_sheet);
		}

		time_since_last_bullet = 0;
	}
	else 
	{
		if (random_empty_sound(sound_cache))
		{
			PlaySound(m_bullet_sound);
		}

	}

}
void CreateBullets::reset()
{
	m_bullets.clear();
}


void GamestateManager::change_gamestate( gamestate new_state)
{
	if (m_gamestate == start && new_state == playing){m_gamestate = new_state;}

	if (m_gamestate == playing && new_state != start) { m_gamestate = new_state; }

	if (m_gamestate == lose || m_gamestate == win) { if (new_state == playing) { m_gamestate = new_state; } }


}
