//component.hpp

#pragma once

#include "common.hpp"




struct InputAction
{
	InputAction() = default;

	void poll_input();

	Vector2 aim_direction(Vector2 player_position) const;
	Vector2 move_to_player(Vector2 target_pos, Vector2 own_pos) const;
	
	void rotate(float& rotation, Vector2 direction) const;

	Vector2 m_shot_end_pos = {};
	
};
struct TransformComp
{
	TransformComp() = default;

	void get_rec_on_screen();

	Vector2 m_position = {};
	Vector2 m_size = {};
	Vector2 m_center = (m_size * 0.5f);
	Rectangle m_destination = { m_position.x, m_position.y, m_size.x, m_size.y };
	float m_rotation = 90.0;

};
struct Sprite 
{
	Sprite() = default;

	void draw(const TransformComp& transform) const;

	Texture m_sprite_sheet = {};
	Rectangle m_source = {};
	Color m_tint = WHITE;

};
