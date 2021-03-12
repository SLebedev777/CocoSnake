#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "Snake_fwd.h"
#include <vector>
#include <iterator>

namespace NS_Snake
{
	class Snake
	{
	public:
		Snake(std::vector<DirectedSpritePtr>& parts, GameGridPtr grid,
			uint8_t speed, uint8_t accel, int max_health = 100, bool can_move_alone = false);
		~Snake() {}

		bool move(int up, int right);
		bool intersectsItself();
		bool addPart();
		void update();
		DirectedSprite& head() { return *(m_parts.front()); }
		DirectedSprite& tail() { return *(m_parts.back()); }
		DirectedSprite& neck() { return *(m_parts[1]); }

		void checkHealth()
		{
			if (m_health <= 0) { m_health = 0; m_alive = false; }
			if (m_health > m_maxHealth) { m_health = m_maxHealth; }
		}
		void addHealth(int h) { m_health += h; checkHealth(); }
		int getHealth() const { return m_health; }
		bool isAlive() const { return m_alive; }
		bool canMoveAlone() const { return m_canMoveAlone; }
		void setWrapAround(bool wrap_around) { m_wrapAround = wrap_around; }
		bool getWrapAround() const { return m_wrapAround; }
	private:
		std::vector<DirectedSpritePtr> m_parts;
		GameGridPtr m_grid;
		uint8_t m_speed;
		int m_maxHealth;
		int m_health;
		uint8_t m_currSpeed;
		uint8_t m_accel;
		bool m_alive;
		bool m_canMoveAlone;
		bool m_wrapAround;
		bool m_startedMoving;
	};
}

#endif  // __SNAKE_H__