#include "Snake.h"
#include "DirectedSprite.h"

namespace NS_Snake
{
	Snake::Snake(std::vector<DirectedSpritePtr>& parts,
		uint8_t speed, uint8_t accel, uint8_t max_health) :
		m_speed(speed),
		m_accel(accel),
		m_maxHealth(max_health)
	{
		if (m_accel > m_speed)
			m_accel = m_speed;

		for (auto& part : parts)
		{
			m_parts.push_back(std::move(part));
		}
	}

	bool Snake::move(int up, int right)
	{
		if (!up && !right)
		{
			m_currSpeed = 0;
			return false;
		}
		m_currSpeed += m_accel;
		if (m_currSpeed < m_speed)
			return false;

		int head_shift_x = 0;
		int head_shift_y = 0;
		SPRITE_DIRECTION new_direction = SPRITE_DIRECTION::NONE;

		if (up != 0 && right == 0)
		{
			if ((up > 0 && head().getDirTo() == SPRITE_DIRECTION::DOWN) ||
				(up < 0 && head().getDirTo() == SPRITE_DIRECTION::UP))
				return false;
			head_shift_y += up * m_speed;
			new_direction = (up > 0) ? SPRITE_DIRECTION::UP : SPRITE_DIRECTION::DOWN;
		}
		if (up == 0 && right != 0)
		{
			if ((right > 0 && head().getDirTo() == SPRITE_DIRECTION::LEFT) ||
				(right < 0 && head().getDirTo() == SPRITE_DIRECTION::RIGHT))
				return false;
			head_shift_x += right * m_speed;
			new_direction = (right > 0) ? SPRITE_DIRECTION::RIGHT : SPRITE_DIRECTION::LEFT;
		}

		if (new_direction == SPRITE_DIRECTION::NONE)
			return false;

		// move all parts except head
		for (int i = m_parts.size() - 1; i >= 1; --i)
		{
			const auto& curr_part = m_parts[i];
			const auto& next_part = m_parts[i-1];
			curr_part->setPosition(next_part->getPosition());
			curr_part->setDirPair(next_part->getDirPair());
		}
		// kink neck
		m_parts[1]->setDirFrom(m_parts[1]->getDirTo());
		m_parts[1]->setDirTo(new_direction);
		// move head
		Point2d new_head_pos = Point2d(head().getPosition().x + head_shift_x, head().getPosition().y + head_shift_y);
		head().setPosition(new_head_pos);
		head().setDirPair({ new_direction, new_direction });

		m_currSpeed = 0;
		return true;
	}


	void Snake::update()
	{
		for (const auto& part : m_parts)
		{
			part->update();
		}
	}
}