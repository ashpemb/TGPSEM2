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

	// score
	float score;
	int highScore;

	// Methods
	void setHighscore(int highScore);
	void storeHighscoreToFile(int highScore);
	int getHighscoreFromFile();
public:
	~ScoreManager();
	static ScoreManager* sharedScoreManager();

	void addToScore(float point);
	void subtractFromScore(float point);
	float getScore();
	void resetScore();

	void compareScoreToHighscore();
	int getHighscore();
	std::string getFilePath();
};
