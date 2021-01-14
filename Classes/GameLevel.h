#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__

#include <vector>
#include <string>
#include <memory>


class GameLevel
{
public:
	GameLevel(
		int number,
		/* FoodTable* food_table, */
		std::string level_name = "", 
		uint16_t snake_starting_length = 3, 
		uint16_t num_starting_food = 10,
		uint16_t num_starting_walls = 5,
		uint16_t score_needed = 10,
		uint16_t max_time = -1
		):
		m_number(number),
		/*m_pFoodTable(food_table),*/
		m_levelName(level_name),
		m_snakeStartingLength(snake_starting_length),
    	m_numStartingFood(num_starting_food),
	    m_numStartingWalls(num_starting_walls),
	    m_scoreNeeded(score_needed),
	    m_maxTime(max_time)
	{}

	int getNumber() { return m_number; }
	uint16_t getSnakeStartingLength() { return m_snakeStartingLength; }
	uint16_t getNumStartingFood() { return m_numStartingFood; }
	uint16_t getNumStartingWalls() { return m_numStartingWalls; }
	uint16_t getScoreNeeded() { return m_scoreNeeded; }
	uint16_t getMaxTime() { return m_maxTime; }

	~GameLevel() {}


private:
	/* FoodTable* m_pFoodTable; */
	int m_number;
	std::string m_levelName;
	uint16_t m_snakeStartingLength;
	uint16_t m_numStartingFood;
	uint16_t m_numStartingWalls;
	uint16_t m_scoreNeeded;
	uint16_t m_maxTime;
	/* uint8_t m_gameSpeedCoeff; */
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