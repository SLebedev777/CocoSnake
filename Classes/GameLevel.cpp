#include "GameLevel.h"

GameLevelManager* GameLevelManager::m_pGameLevelManagerInstance = nullptr;

GameLevelManager::GameLevelManager() :
	m_currLevelNumber(0),
	m_pCurrLevel(nullptr)
{}

GameLevelManager::~GameLevelManager()
{}


GameLevelManager* GameLevelManager::getInstance()
{
	if (!m_pGameLevelManagerInstance)
	{
		m_pGameLevelManagerInstance = new GameLevelManager();
	}
	return m_pGameLevelManagerInstance;
}

void GameLevelManager::setGameLevelsArray(GameLevelsArray& levels)
{
	if (!levels.empty())
	{
		m_levels = levels;
		rewind();
	}
	else
	{
		throw;
	}
}

GameLevel* GameLevelManager::getCurrLevel() const
{
	if (m_levels.empty())
		return nullptr;

	return m_levels[m_currLevelNumber];
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