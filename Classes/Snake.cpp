#include "Snake.h"
#include "DirectedSprite.h"
#include "GameGrid.h"
#include <map>

namespace NS_Snake
{
	Snake::Snake(std::vector<DirectedSpritePtr>& parts, GameGridPtr grid,
		uint8_t speed, uint8_t accel, int max_health, bool can_move_alone) :
		m_speed(speed),
		m_accel(accel),
		m_maxHealth(max_health),
		m_health(max_health),
		m_alive(true),
		m_grid(grid),
		m_canMoveAlone(can_move_alone),
		m_wrapAround(false)
	{
		if (max_health <= 0)
		{
			throw std::invalid_argument("Max health must be positive");
		}

		if (m_accel > m_speed)
			m_accel = m_speed;

		if (parts.size() < 3)
		{
			throw std::invalid_argument("At least 3 parts needed for snake - head, 1 body part and tail");
		}

		for (auto& part : parts)
		{
			if (!m_grid->occupyCell(m_grid->xyToCell(part->getPosition())))
			{
				throw std::runtime_error("Failed to create snake. Cell occupied.");
			}

		}

		for (auto& part : parts)
		{
			m_parts.push_back(std::move(part));
		}
	}

	bool Snake::move(int up, int right)
	{
		if (!up && !right)
		{
			if (m_canMoveAlone)
			{
				switch (head().getDirTo())
				{
				case SPRITE_DIRECTION::UP:
					up = 1;
					break;
				case SPRITE_DIRECTION::DOWN:
					up = -1;
					break;
				case SPRITE_DIRECTION::LEFT:
					right = -1;
					break;
				case SPRITE_DIRECTION::RIGHT:
					right = 1;
					break;
				default: break;
				}
			}
			else
			{
				m_currSpeed = 0;
				return false;
			}
		}
		m_currSpeed += m_accel;
		if (m_currSpeed < m_speed)
			return false;

		int head_shift_x = 0;
		int head_shift_y = 0;
		SPRITE_DIRECTION new_direction = SPRITE_DIRECTION::NONE;

		// handle single arrow pressed
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
		// handle combo when 2 arrows are pressed simultaneously
		if (up != 0 && right != 0)
		{
			// cases when 1 arrow is the same as snake head direction
			if ((head().getDirTo() == SPRITE_DIRECTION::UP && up > 0) || 
				(head().getDirTo() == SPRITE_DIRECTION::DOWN && up < 0))
			{
				if (!move(0, right))
					move(up, 0);
			}
			else if ((head().getDirTo() == SPRITE_DIRECTION::LEFT && right < 0) || 
				(head().getDirTo() == SPRITE_DIRECTION::RIGHT && right > 0))
			{
				if (!move(up, 0))
					move(0, right);
			}
			// cases when 1 arrow is the opposite to snake head direction
			else if ((head().getDirTo() == SPRITE_DIRECTION::UP && up < 0) ||
				(head().getDirTo() == SPRITE_DIRECTION::DOWN && up > 0))
			{
				move(0, right);
			}
			else if ((head().getDirTo() == SPRITE_DIRECTION::LEFT && right > 0) ||
				(head().getDirTo() == SPRITE_DIRECTION::RIGHT && right < 0))
			{
				move(up, 0);
			}
			else
			{
				return false;
			}
		}

		if (new_direction == SPRITE_DIRECTION::NONE)
			return false;

		// get new possible head position
		Point2d poss_new_head_pos = Point2d(head().getPosition().x + head_shift_x, head().getPosition().y + head_shift_y);
		auto poss_new_head_cell = m_grid->xyToCell(poss_new_head_pos);
		
		// wrap around mode
		if (m_wrapAround)
		{			
			if (poss_new_head_pos.x < m_grid->getOrigin().x)
				poss_new_head_cell.cix = m_grid->getNumCellsX() - 1;
			else if (poss_new_head_pos.x > m_grid->getOrigin().x + m_grid->getWidth())
				poss_new_head_cell.cix = 0;

			if (poss_new_head_pos.y < m_grid->getOrigin().y)
				poss_new_head_cell.ciy = m_grid->getNumCellsY() - 1;
			else if (poss_new_head_pos.y > m_grid->getOrigin().y + m_grid->getHeight())
				poss_new_head_cell.ciy = 0;

		}
		
		// detect walls collision
		if (m_grid->isCellOccupied(poss_new_head_cell) && m_grid->getCellType(poss_new_head_cell) == NS_Snake::GameGrid::CellType::WALL)
			return false;

		// make successful movement

		for (auto& part : m_parts)
			m_grid->releaseCell(m_grid->xyToCell((part)->getPosition()));

		// move all parts except head
		for (int i = m_parts.size() - 1; i >= 1; --i)
		{
			const auto& curr_part = m_parts[i];
			const auto& next_part = m_parts[i-1];
			curr_part->setPosition(next_part->getPosition());
			curr_part->setDirPair(next_part->getDirPair());
		}
		// kink neck
		neck().setDirFrom(neck().getDirTo());
		neck().setDirTo(new_direction);
		// move head
		Point2d new_head_pos = m_grid->cellToXyCenter(poss_new_head_cell);
		head().setPosition(new_head_pos);
		head().setDirPair({ new_direction, new_direction });

		for (auto& part : m_parts)
			m_grid->occupyCell(m_grid->xyToCell((part)->getPosition()));

		m_currSpeed = 0;
		return true;
	}


	bool Snake::addPart()
	{
		DirectedSpritePtr new_part = std::make_unique<DirectedSprite>(neck().getTable());
		DirectedSpritePtr new_tail = std::make_unique<DirectedSprite>(tail().getTable());
		new_part->setPosition(tail().getPosition());
		new_part->setDirPair({tail().getDirTo(), tail().getDirTo() });
		auto cc_layer = tail().getSprite()->getParent();
		cc_layer->addChild(new_part->getSprite());
		new_part->update();

		std::map<SPRITE_DIRECTION, std::pair<int, int>> tail_cell_shifts = {
			{SPRITE_DIRECTION::UP, {0, -1}},
			{SPRITE_DIRECTION::DOWN, {0, 1}},
			{SPRITE_DIRECTION::LEFT, {1, 0}},
			{SPRITE_DIRECTION::RIGHT, {-1, 0}},
		};

		Point2d tail_old_pos = tail().getPosition();
		GameGrid::Cell tail_old_cell = m_grid->xyToCell(tail_old_pos);

		std::vector<SPRITE_DIRECTION> try_directions;
		try_directions.push_back(tail().getDirTo());
		for (const auto& it : tail_cell_shifts)
		{
			auto dir = it.first;
			if (dir == tail().getDirTo() || dir == oppositeDir(tail().getDirTo()))
				continue;
			try_directions.push_back(dir);
		}

		bool new_tail_pos_found = false;
		for (auto& dir : try_directions)
		{
			auto shift = tail_cell_shifts[dir];
			GameGrid::Cell tail_new_cell(tail_old_cell.cix + shift.first, tail_old_cell.ciy + shift.second);
			if (!m_grid->contains(tail_new_cell))
				continue;
			if (m_grid->isCellOccupied(tail_new_cell))
				continue;
			m_grid->occupyCell(tail_new_cell);
			Point2d tail_new_pos = m_grid->cellToXyCenter(tail_new_cell);
			new_tail->setPosition(tail_new_pos);
			new_tail->setDirPair({ dir, dir });
			new_tail->update();
			cc_layer->addChild(new_tail->getSprite());
			new_tail_pos_found = true;
			break;
		}
		// failed to find free grid cell for new tail.
		// doing simple - just not enlarging snake at all
		if (!new_tail_pos_found)
			return false;

		new_part->setDirFrom(new_tail->getDirTo());

		m_parts.pop_back();
		m_parts.push_back(std::move(new_part));
		m_parts.push_back(std::move(new_tail));

		return true;
	}

	void Snake::update()
	{
		for (const auto& part : m_parts)
		{
			part->update();
		}
	}

	bool Snake::intersectsItself()
	{
		auto head_cell = m_grid->xyToCell(head().getPosition());
		for (size_t i = 1; i < m_parts.size(); i++)
		{
			auto part_cell = m_grid->xyToCell(m_parts[i]->getPosition());
			if (part_cell == head_cell)
				return true;
		}
		return false;
	}
}