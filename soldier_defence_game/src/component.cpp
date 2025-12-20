//component.cpp

#include "component.hpp"
#include "game.hpp"




void InputAction::poll_input()
{
	m_shot_end_pos = GetMousePosition();
}



Vector2 InputAction::aim_direction(Vector2 player_position) const
{
	Vector2 direction = {};
	Vector2 distance = {};

	distance = m_shot_end_pos - player_position;
	direction = Vector2Normalize(distance);

	return direction;
}

void InputAction::rotate(float& rotation, Vector2 direction) const
{
	rotation = 90.0f + atan2f(direction.y, direction.x) * RAD2DEG;
}

Vector2 InputAction::move_to_player(Vector2 target_pos, Vector2 own_pos) const
{
	Vector2 direction = {};
	Vector2 distance = {};

	distance = target_pos - own_pos;
	direction = Vector2Normalize(distance);

	return direction;
}


void Sprite::draw(const TransformComp& transform) const
{
	DrawTexturePro(m_sprite_sheet, m_source, transform.m_destination, transform.m_center, transform.m_rotation, m_tint);
	
}

void TransformComp::get_rec_on_screen()
{
	m_center = (m_size * 0.5f);
	m_destination = { m_position.x, m_position.y, m_size.x, m_size.y };
}
