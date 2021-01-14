#include <stdexcept>
#include <cassert>
#include "GameLevel.h"


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