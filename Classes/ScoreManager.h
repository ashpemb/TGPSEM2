#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CCFileUtils.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GameManager.h"

using namespace cocos2d;

class ScoreManager
{
private:
	ScoreManager();
	static ScoreManager* instance;

	// Methods
	std::string getFilePath();
public:
	~ScoreManager();
	static ScoreManager* sharedScoreManager();

	void resetScore();

	void compareScoreToHighscore();

	void storeHighscoreToFile(int level, int star, int minutes, int seconds, int miliseconds);
	int getStarFromFile(int level);
	std::string getMinutesFromFile(int level);
	std::string getSecondsFromFile(int level);
	std::string getMilisecondsFromFile(int level);

	int getDefaultStarRating(int level);
};
