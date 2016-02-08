#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CCFileUtils.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

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

	void storeHighscoreToFile(int level, int star, std::string time);
	int getStarFromFile(int level);
	std::string getTimeFromFile(int level);
};
