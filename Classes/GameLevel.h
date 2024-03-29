#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__

#include "IFood.h"
#include <vector>
#include <string>
#include <memory>

using namespace NS_Snake;

class GameLevel
{
public:
	GameLevel(
		int number,
		size_t num_cells_x,
		size_t num_cells_y,
		NS_Snake::FoodTable& food_table,
		std::string level_name = "",
		uint16_t snake_starting_length = 3,
		uint16_t num_starting_food = 10,
		uint16_t num_starting_walls = 5,
		uint16_t score_needed = 10,
		uint16_t max_time = -1,
		float spawn_food_interval = 0.0f,
		uint8_t snake_accel = 4
	);

	size_t getNumCellsX() const { return m_numCellsX; }
	size_t getNumCellsY() const { return m_numCellsY; }
	NS_Snake::FoodTable getFoodTable() const { return m_foodTable; }
	int getNumber() const { return m_number; }
	uint16_t getSnakeStartingLength() const { return m_snakeStartingLength; }
	uint16_t getNumStartingFood() const { return m_numStartingFood; }
	uint16_t getNumStartingWalls() const { return m_numStartingWalls; }
	uint16_t getScoreNeeded() const { return m_scoreNeeded; }
	uint16_t getMaxTime() const { return m_maxTime; }
	float getSpawnFoodInterval() const { return m_spawnFoodInterval; }
	uint8_t getSnakeAccel() const { return m_snakeAccel; }

	~GameLevel() {}


private:
	NS_Snake::FoodTable m_foodTable;
	int m_number;
	size_t m_numCellsX;
	size_t m_numCellsY;
	std::string m_levelName;
	uint16_t m_snakeStartingLength;
	uint16_t m_numStartingFood;
	uint16_t m_numStartingWalls;
	uint16_t m_scoreNeeded;
	uint16_t m_maxTime;
	float m_spawnFoodInterval;
	uint8_t m_snakeAccel;
};

typedef std::shared_ptr<GameLevel> GameLevelPtr;
typedef std::vector<GameLevelPtr> GameLevelsArray;


class GameLevelManager
{
public:
	static GameLevelManager& getInstance();
	~GameLevelManager() {}

	void setGameLevelsArray(GameLevelsArray& levels);
	GameLevel& getCurrLevel() const;
	bool moveToNextLevel();
	void rewind() { m_currLevelNumber = 0; }
	size_t getCurrLevelNumber() const { return m_currLevelNumber; }
	size_t getNumLevels() const { return m_levels.size(); }

private:
	GameLevelManager();

	GameLevelsArray m_levels;
	size_t m_currLevelNumber;
};

#endif // __GAMELEVEL_H__