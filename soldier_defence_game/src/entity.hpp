//entity.hpp

#pragma once

#include "common.hpp"
#include "component.hpp"


struct Player 
{
	Player(int health);

	void update(float dt, InputAction& input);
	void draw() const;
	void reset(int initial_health);

	TransformComp m_transform;
	Sprite m_sprite;
	int m_health = 1;

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
	Enemy(Rectangle source, Vector2 size, Vector2 position);
	
	enum enemy_walk_type{straight, around};
	

	void set_enemy_type();

	void update(float dt, InputAction& input, Player& player);
	void draw() const;
	void reached_player(Player& player);
	void is_hit(Bullet& bullet);

	
	enemy_walk_type m_enemy_walk_type = { straight };
	int m_type = 0;

	TransformComp m_transform;
	Sprite m_sprite;
	
	bool m_alive = true;

	Vector2 m_direction = {};
	Vector2 m_perpendicular_direction = {};
	Vector2 m_final_direction = {};
	

};