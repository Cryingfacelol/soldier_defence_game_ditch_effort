//component.cpp

#include "component.hpp"




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

TextRenderer::TextRenderer(Vector2 window_size)
{
	m_window_size = window_size;
	m_text_borders = 20;
	m_text_colour = RAYWHITE;
}

void TextRenderer::start_screen_text()
{
	DrawText("Ditch Effort", m_text_borders, int(m_window_size.y * 0.5) - (2 * m_text_borders), 50, m_text_colour);
	DrawText("Press S to begin your last stand", m_text_borders, int(m_window_size.y * 0.5) + m_text_borders, 20, m_text_colour);
	DrawText("Press Q to QUIT", m_text_borders, int(m_window_size.y * 0.5) + (2 * m_text_borders), 20, m_text_colour);

}

void TextRenderer::playing_text(int player_health, bool first_bullet_shot)
{
	DrawText(TextFormat("Health: %i", player_health), m_text_borders, 3 * m_text_borders, 40, m_text_colour);
	if (!first_bullet_shot) { DrawText("LEFT CLICK TO SHOOT", int(m_window_size.x * 0.5) - (2 * m_text_borders), int(m_window_size.y) - (2 * m_text_borders), 30, m_text_colour); }

}

void TextRenderer::lose_screen_text()
{
	DrawText("You have died. At least you took as many as possible down with you...", m_text_borders, int(m_window_size.y * 0.5), 20, m_text_colour);
	DrawText("Press R to restart", m_text_borders, int(m_window_size.y * 0.5) + m_text_borders, 20, m_text_colour);
	DrawText("Press Q to QUIT", m_text_borders, int(m_window_size.y * 0.5) + (2 * m_text_borders), 20, m_text_colour);

}

void TextRenderer::win_screen_text()
{
	DrawText("You have been rescued by comrades, congratulations!", m_text_borders, int(m_window_size.y * 0.5), 20, m_text_colour);
	DrawText("Press R to restart", m_text_borders, int(m_window_size.y * 0.5) + m_text_borders, 20, m_text_colour);
	DrawText("Press Q to QUIT", m_text_borders, int(m_window_size.y * 0.5) + (2 * m_text_borders), 20, m_text_colour);

}
