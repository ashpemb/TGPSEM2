#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class GameManager
{
private:
	GameManager();
	static GameManager* instance;

	bool isGameLive;
	bool isGamePaused;
	int time;
public:

	~GameManager();
	static GameManager* sharedGameManager();

	void setIsGameLive(bool);
	bool getIsGameLive();
	void setIsGamePaused(bool);
	bool getIsGamePaused();
	void startLevelTimer();
	void updateLevelTimer();
	int getTimer();
};
