//entity.hpp

#pragma once

#include "common.hpp"
#include "component.hpp"


struct Player 
{
	Player() = default;

	void update(float dt, InputAction& input);
	void draw() const;

	TransformComp m_transform;
	Sprite m_sprite;

	Vector2 m_direction = {};
};

struct Bullet
{
	Bullet() = default;

	void erase_outside_window(Vector2 screen_size); //only moves it back to center for now, not sure how to erase will have to check
	void update(float dt, Player& player, InputAction& input, Vector2 screen_size);
	void draw() const;

	TransformComp m_transform;
	Sprite m_sprite;
	Vector2 m_direction = {};
	bool m_active = false;
};

struct Enemy
{
	Enemy(Rectangle source, Vector2 size, Vector2 position);

	void update(float dt, InputAction& input, Player& player);
	void draw() const;
	void reached_player(Player& player);
	void is_hit(Bullet& bullet);
	

	TransformComp m_transform;
	Sprite m_sprite;
	Vector2 m_direction = {};
	bool m_alive = true;

};