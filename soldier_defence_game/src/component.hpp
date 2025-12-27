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
struct TextRenderer
{
	TextRenderer(Vector2 window_size) ;

	void start_screen_text();
	void playing_text(int player_health, bool first_bullet_shot);
	void lose_screen_text();
	void win_screen_text();

	Vector2 m_window_size;
	int m_text_borders = 0;
	Color m_text_colour = {};
	int m_text_size = 20;
	int m_text_size_title = 50;
	int m_text_size_slightly_bigger = 30;
};