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
	bool isGameMuted;
	bool isObjectBeingTouched;
	int level;
	int time;
	int mil;
	int sec;
	int min;

	

	
public:

	~GameManager();
	static GameManager* sharedGameManager();

	void setIsGameLive(bool);
	bool getIsGameLive();
	void setIsGamePaused(bool);
	bool getIsGamePaused();
	void setIsGameMuted(bool);
	bool getIsGameMuted();
	void setIsObjectTouched(bool);
	bool getIsObjectTouched();
	void setCurrentLevel(int level);
	int getCurrentLevel();
	void startLevelTimer();
	void updateLevelTimer();
	int getTimer();
	int getMil();
	int getSec();
	int getMin();
};
