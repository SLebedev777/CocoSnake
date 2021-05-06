#include "GameLevel.h"
#include <stdexcept>
#include <cassert>

GameLevel::GameLevel(
	int number,
	size_t num_cells_x,
	size_t num_cells_y,
	NS_Snake::FoodTable& food_table,
	std::string level_name,
	uint16_t snake_starting_length,
	uint16_t num_starting_food,
	uint16_t num_starting_walls,
	uint16_t score_needed,
	uint16_t max_time,
	float spawn_food_interval
) :
	m_number(number),
	m_numCellsX(num_cells_x),
	m_numCellsY(num_cells_y),
	m_foodTable(food_table),
	m_levelName(level_name),
	m_snakeStartingLength(snake_starting_length),
	m_numStartingFood(num_starting_food),
	m_numStartingWalls(num_starting_walls),
	m_scoreNeeded(score_needed),
	m_maxTime(max_time),
	m_spawnFoodInterval(spawn_food_interval)
{
	// check case when some food has limited lifetime, but spawn food interval is set to default.
	// (spawn new food only when snake eats some existing food).
	// in this case, all generated food can disappear finally, and no new food will be generated, which is weird.
	if (m_spawnFoodInterval == 0.0f)
	{
		if (std::any_of(m_foodTable.staticFoodTable.begin(), m_foodTable.staticFoodTable.end(),
			[](std::pair<StaticFoodType, StaticFoodDescription> item) {return item.second.lifetime != -1; })
			&&
			std::any_of(m_foodTable.movingFoodTable.begin(), m_foodTable.movingFoodTable.end(),
				[](std::pair<MovingFoodType, MovingFoodDescription> item) {return item.second.lifetime != -1; })
			)
		{
			throw std::invalid_argument("Level #" + std::to_string(m_number) + "error." +
				"If spawn food interval is 0, all food must have no lifetime limits.");
		}
	}
}


GameLevelManager::GameLevelManager() :
	m_currLevelNumber(0)
{}


GameLevelManager& GameLevelManager::getInstance()
{
	static GameLevelManager instance;
	return instance;
}

void GameLevelManager::setGameLevelsArray(GameLevelsArray& levels)
{
	if (!levels.empty())
	{
		m_levels.clear();
		for (auto& level : levels)
			m_levels.push_back(level);
		rewind();
	}
	else
	{
		throw std::invalid_argument("GameLevelsArray cannot be empty");
	}
}

GameLevel& GameLevelManager::getCurrLevel() const
{
	assert(!m_levels.empty());
	return *m_levels[m_currLevelNumber];
}

bool GameLevelManager::moveToNextLevel()
{
	if (m_currLevelNumber < getNumLevels() - 1)
	{
		m_currLevelNumber++;
		return true;
	}
	return false;
}