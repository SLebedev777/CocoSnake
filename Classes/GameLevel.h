#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__

#include <vector>

class GameLevel
{
public:
	GameLevel(int number):
		m_Number(number)
	{}

	int getNumber() { return m_Number; }

private:
	int m_Number;
};


typedef std::vector<GameLevel*> GameLevelsArray;


class GameLevelManager
{
public:
	static GameLevelManager* getInstance();

	void setGameLevelsArray(GameLevelsArray& levels);
	GameLevel* getCurrLevel() const;
	bool moveToNextLevel();
	void rewind() { m_currLevelNumber = 0; }
	size_t getCurrLevelNumber() const { return m_currLevelNumber; }
	size_t getNumLevels() const { return m_levels.size(); }

private:
	GameLevelManager();
	~GameLevelManager();

	static GameLevelManager* m_pGameLevelManagerInstance;
	GameLevelsArray m_levels;
	size_t m_currLevelNumber;
	GameLevel* m_pCurrLevel;
};

#endif // __GAMELEVEL_H__