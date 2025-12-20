#include "entity.hpp"

void Player::update(float dt, const InputAction& input)
{ 
	
	m_direction = input.aim_direction(m_transform.m_position);
	input.rotate_to_mouse(m_transform.m_rotation, m_direction);
	m_transform.get_rec_on_screen();

}

void Player::draw() const
{
	m_sprite.draw(m_transform);
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
void Bullet::update( float dt, const Player& player, const InputAction& input, Vector2 screen_size)
{

	constexpr float PIXELS_PER_SECOND = 200.0f;
	if (IsMouseButtonPressed(0))
		{
			m_transform.m_position = screen_size * 0.5;
			m_direction = input.aim_direction(player.m_transform.m_position);
			m_active = true;
		}
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