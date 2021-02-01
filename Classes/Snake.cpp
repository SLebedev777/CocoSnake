#include "Snake.h"
#include "DirectedSprite.h"
#include "GameGrid.h"
#include <map>

namespace NS_Snake
{
	Snake::Snake(std::vector<DirectedSpritePtr>& parts, GameGridPtr grid,
		uint8_t speed, uint8_t accel, uint8_t max_health) :
		m_speed(speed),
		m_accel(accel),
		m_maxHealth(max_health),
		m_grid(grid)
	{
		if (m_accel > m_speed)
			m_accel = m_speed;

		if (parts.size() < 3)
		{
			throw std::invalid_argument("At least 3 parts needed for snake - head, 1 body part and tail");
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
		Point2d new_head_pos = Point2d(head().getPosition().x + head_shift_x, head().getPosition().y + head_shift_y);
		head().setPosition(new_head_pos);
		head().setDirPair({ new_direction, new_direction });

		for (auto& part : m_parts)
			m_grid->occupyCell(m_grid->xyToCell((part)->getPosition()));

		m_currSpeed = 0;
		return true;
	}


	void Snake::addPart()
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
			break;
		}
		new_part->setDirFrom(new_tail->getDirTo());

		m_parts.pop_back();
		// TODO: FIXME: rare case: when lots of food, no appropriate free cell is found for grown new tail, 
		// loop over tail shifts exits and new_tail is not added to CC layer, but it's added to parts vector!
		m_parts.push_back(std::move(new_part));
		m_parts.push_back(std::move(new_tail));
	}

	void Snake::update()
	{
		for (const auto& part : m_parts)
		{
			part->update();
		}
	}
}