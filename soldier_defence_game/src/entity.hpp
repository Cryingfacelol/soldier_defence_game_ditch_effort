//entity.hpp

#pragma once

#include "common.hpp"
#include "entity.hpp"
#include "component.hpp"


struct Player 
{
	Player() = default;

	void update(float dt, const InputAction& input);
	void draw() const;

	TransformComp m_transform;
	Sprite m_sprite;

	Vector2 m_direction = {};
};

struct Bullet
{
	Bullet() = default;

	void erase_outside_window(Vector2 screen_size); //only moves it back to center for now, not sure how to erase will have to check
	void update(float dt, const Player& player, const InputAction& input, Vector2 screen_size);
	void draw() const;

	TransformComp m_transform;
	Sprite m_sprite;
	Vector2 m_direction = {};
	bool m_active = true;
};