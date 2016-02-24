#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
	isGameLive = false;
	isGamePaused = false;
	isGameMuted = false;
	mil = 0;
	sec = 0;
	min = 0;
}

GameManager* GameManager::sharedGameManager()
{
	if (instance == NULL)
	{
		instance = new GameManager();
	}

	return instance;
}

void GameManager::setIsGameLive(bool live) 
{
	isGameLive = live;
}

bool GameManager::getIsGameLive()
{
	return isGameLive;
}

void GameManager::setIsGamePaused(bool paused)
{
	isGamePaused = paused;
}

bool GameManager::getIsGamePaused()
{
	return isGamePaused;
}

void GameManager::setIsGameMuted(bool muted)
{
	isGameMuted = muted;
}

bool GameManager::getIsGameMuted()
{
	return isGameMuted;
}

void GameManager::startLevelTimer()
{
	mil = 0;
	sec = 0;
	min = 0;
}

void GameManager::setCurrentLevel(int level)
{
	this->level = level;
}

int GameManager::getCurrentLevel()
{
	return level;
}

void GameManager::updateLevelTimer()
{
	mil++;

	if (mil > 1000)
	{
		sec++;
		mil = 0;
	}

	if (sec >= 60)
	{
		min++;
		sec = sec - 60;
	}


}

int GameManager::getTimer()
{
	return 0;
}


int GameManager::getMil()
{
	return mil;
}

int GameManager::getSec()
{
	return sec;
}


int GameManager::getMin()
{
	return min;
}


GameManager::~GameManager()
{
}