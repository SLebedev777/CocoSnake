#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "Snake_fwd.h"
#include <vector>

namespace NS_Snake
{
	class Snake
	{
	public:
		Snake(std::vector<DirectedSpritePtr>& parts, 
			uint8_t speed, uint8_t max_health = 100);
		~Snake() {}

		bool move(int up, int right);
		void checkIntersectItself() {}
		void checkHealth() {}
		void addPart() {}
		void update();
		DirectedSprite& head() { return *(m_parts.front()); }
		DirectedSprite& tail() { return *(m_parts.back()); }

	private:
		std::vector<DirectedSpritePtr> m_parts;
		uint8_t m_speed;
		uint8_t m_maxHealth;
	};
}

#endif  // __SNAKE_H__