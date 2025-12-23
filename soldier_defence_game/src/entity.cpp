#include "entity.hpp"

Player::Player(int health)
{
	m_health = health;
}

void Player::update(float dt, InputAction& input)
{ 
	
	m_direction = input.aim_direction(m_transform.m_position);
	input.rotate(m_transform.m_rotation, m_direction);
	m_transform.get_rec_on_screen();

}

void Player::draw() const
{
	m_sprite.draw(m_transform);
}
void Player::reset(int initial_health)
{
	m_health = initial_health;
}
void Player::tint_change_hit(float dt)
{
	m_timer += 1000 *dt;
	if (m_hit) {
		
		if (m_timer - m_old_time > 1) 
		{
			m_sprite.m_tint.b -= 1;
			m_sprite.m_tint.g -= 1;
			m_old_time = m_timer;
		}
		
		if (m_sprite.m_tint.b == 0 || m_sprite.m_tint.g == 0)
		{
			m_hit = false;
		}
	}
	if (!m_hit) 
	{
		if (m_sprite.m_tint.b < 255 || m_sprite.m_tint.g < 255)
		{
			m_sprite.m_tint.b += 1;
			m_sprite.m_tint.g += 1;
			m_old_time = m_timer;
		}
	}
}
bool Player::get_hit_sound(SoundCache& sound_cache)
{
	if (m_health == 0) 
	{
		return sound_cache.get("player_dead", m_sound);
	}
	else { return sound_cache.get("player_hit", m_sound); }
}
void Player::play_hit_sound(SoundCache& sound_cache)
{
	if (get_hit_sound(sound_cache))
	{
		if (m_health != 0) { SetSoundPitch(m_sound, 0.75f); }
		PlaySound(m_sound);
	}

}
Bullet::Bullet(Vector2 direction, Vector2 player_position, float rotation, Vector2 size, Rectangle source)
{
	m_direction = direction;
	m_transform.m_position = player_position;
	m_transform.m_rotation = rotation;
	m_transform.m_size = size;
	m_sprite.m_source = source;
}
void Bullet::erase_outside_window(Vector2 screen_size) { //just reset to center for now
	if (m_transform.m_position.x > screen_size.x ||
		m_transform.m_position.y > screen_size.y ||
		m_transform.m_position.x < 0 ||
		m_transform.m_position.y < 0) 
	{
		m_transform.m_position = screen_size * 0.5f;
		m_active = false;
	}
}
void Bullet::update( float dt, Vector2 screen_size)
{

	constexpr float PIXELS_PER_SECOND = 200.0f; //speed
	
	if (m_active) 
	{
		m_transform.m_position += m_direction * PIXELS_PER_SECOND * dt;
		
	}
	if (!m_active) 
	{
		m_transform.m_position = screen_size * 0.5;
	}
	m_transform.get_rec_on_screen();
	
}

void Bullet::draw() const
{
	if (m_active) { m_sprite.draw(m_transform); }
}


void Enemy::update(float dt, InputAction& input, Player& player)
{
	constexpr float PIXELS_PER_SECOND = 50.0f; //speed
	float m_speed_in = 1.0f ;
	float m_speed_around = 0.99f ;
	

	if (m_alive){
		
		m_direction = input.move_to_player(player.m_transform.m_position, m_transform.m_position);
		input.rotate(m_transform.m_rotation, m_direction);

		if(m_enemy_walk_type == 1){m_transform.m_position += m_direction * PIXELS_PER_SECOND * dt;}
		
		if (m_enemy_walk_type == 2) //second type of enemy that circles in
		{ 
			m_perpendicular_direction = { -m_direction.y, m_direction.x };
			m_final_direction = Vector2Normalize((m_direction * m_speed_in) + (m_perpendicular_direction * m_speed_around));

			m_transform.m_position += m_final_direction * 2 *PIXELS_PER_SECOND * dt;
		}


	}

	m_transform.get_rec_on_screen();
	

}

void Enemy::draw() const
{
	if (m_alive) { m_sprite.draw(m_transform); }
}

bool Enemy::reached_player(Player& player)
{
	if (CheckCollisionRecs(m_transform.m_destination, player.m_transform.m_destination)) 
	{ 
		m_alive = false;
		player.m_health--;
		
	}
	return CheckCollisionRecs(m_transform.m_destination, player.m_transform.m_destination);
}

void Enemy::is_hit(Bullet& bullet)
{
	if (CheckCollisionRecs(m_transform.m_destination, bullet.m_transform.m_destination)) 
	{ 
		m_alive = false; 
	}
}

bool Enemy::get_death_sound(SoundCache& sound_cache)
{
	m_sound_number = GetRandomValue(1, 5);
	
	if (m_sound_number == 1) {
		return sound_cache.get("enemy_hit_001", m_death_sound);
	}
	if (m_sound_number == 2) {
		return sound_cache.get("enemy_hit_002", m_death_sound);
	}
	if (m_sound_number == 3) {
		return sound_cache.get("enemy_hit_003", m_death_sound);
	}
	if (m_sound_number == 4) {
		return sound_cache.get("enemy_hit_004", m_death_sound);
	}
	else {
		return sound_cache.get("enemy_hit_005", m_death_sound);
	}
	
}

void Enemy::play_death_sound(SoundCache& sound_cache)
{
	if (get_death_sound(sound_cache)) { PlaySound(m_death_sound); }
}

Enemy::Enemy(Rectangle source, Vector2 size, Vector2 position, int enemy_walk_type)
{
	m_sprite.m_source = source;
	m_transform.m_size = size;
	m_transform.m_position = position;
	m_enemy_walk_type = enemy_walk_type;
	
}
