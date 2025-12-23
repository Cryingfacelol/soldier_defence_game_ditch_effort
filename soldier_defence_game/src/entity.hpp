//entity.hpp

#pragma once

#include "common.hpp"
#include "component.hpp"
#include "assets.hpp"


struct Player 
{
	Player(int health);

	void update(float dt, InputAction& input);
	void draw() const;
	void reset(int initial_health);
	void tint_change_hit(float dt);
	bool get_hit_sound(SoundCache& sound_cache);
	void play_hit_sound(SoundCache& sound_cache);

	TransformComp m_transform;
	Sprite m_sprite;
	int m_health = 1;
	bool m_hit = false;
	float m_timer = 0;
	float m_old_time = 0;
	Sound m_sound = {};

	Vector2 m_direction = {};
};

struct Bullet
{
	Bullet(Vector2 direction, Vector2 player_position, float rotation, Vector2 size, Rectangle source);

	void erase_outside_window(Vector2 screen_size); //only moves it back to center for now, not sure how to erase will have to check
	void update(float dt, Vector2 screen_size);
	void draw() const;

	TransformComp m_transform;
	Sprite m_sprite;
	Vector2 m_direction = {};
	bool m_active = true;
};

struct Enemy
{
	Enemy(Rectangle source, Vector2 size, Vector2 position, int enemy_walk_type);
	
	

	void update(float dt, InputAction& input, Player& player);
	void draw() const;
	bool reached_player(Player& player);
	void is_hit(Bullet& bullet);
	bool get_death_sound(SoundCache& sound_cache);
	void play_death_sound(SoundCache& sound_cache);
	
	int m_enemy_walk_type = 0;
	int m_sound_number = 0;


	TransformComp m_transform;
	Sprite m_sprite;
	
	bool m_alive = true;

	Vector2 m_direction = {};
	Vector2 m_perpendicular_direction = {};
	Vector2 m_final_direction = {};
	
	Sound m_death_sound = {};
};