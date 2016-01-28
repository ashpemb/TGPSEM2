#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
	isGameLive = false;
	isGamePaused = false;
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

GameManager::~GameManager()
{
}