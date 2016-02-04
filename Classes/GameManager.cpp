#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
	isGameLive = false;
	isGamePaused = false;
	time = 0;
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

void GameManager::startLevelTimer()
{
	time = 0;
}

void GameManager::updateLevelTimer()
{
	time++;
}

int GameManager::getTimer()
{
	return time;
}

GameManager::~GameManager()
{
}