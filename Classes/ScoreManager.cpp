#include "ScoreManager.h"

ScoreManager* ScoreManager::instance = NULL;

ScoreManager::ScoreManager()
{
	setHighscore(getHighscoreFromFile());
	resetScore();
}

ScoreManager* ScoreManager::sharedScoreManager()
{
	if (instance == NULL)
	{
		instance = new ScoreManager();
	}

	return instance;
}

void ScoreManager::addToScore(float point)
{
	score += point;
}

void ScoreManager::subtractFromScore(float point)
{
	score -= point;
}

float ScoreManager::getScore()
{
	return score;
}

void ScoreManager::resetScore()
{
	score = 0;
}

void ScoreManager::compareScoreToHighscore()
{
	if (score > highScore)
	{
		setHighscore(score);
	}
}

void ScoreManager::setHighscore(int newScore)
{
	highScore = newScore;
	storeHighscoreToFile(highScore);
}

int ScoreManager::getHighscore()
{
	return highScore;
}

void ScoreManager::storeHighscoreToFile(int highScore)
{
	bool tagFound = false;

	// Insert code to save highscore to file
	std::string path = getFilePath();
	std::string fileContents = "";

	std::ifstream infile(path);
	std::string currLine;

	if (infile.good()) {
		while (std::getline(infile, currLine)) {
			if (currLine.find("<highscore>") == true) {
				tagFound = true;
			}
			fileContents += currLine + "\n";
		}

		if (!tagFound) {
			int indexTagEnd = fileContents.find("</data>");
			std::string insertString = "\t<highscore>" + StringUtils::format("%d", highScore) + "</highscore>\n";

			std::string firstHalf = "";
			std::string secondHalf = "";

			for (int i = 0; i < indexTagEnd; i++) {
				firstHalf += fileContents[i];
			}
			for (int i2 = indexTagEnd; i2 < fileContents.size(); i2++) {
				secondHalf += fileContents[i2];
			}

			fileContents = firstHalf + insertString + secondHalf;
		}
		else {
			int indexTagStart = fileContents.find("<highscore>");
			int indexTagEnd = fileContents.find("</highscore>");

			std::string firstHalf = "";
			std::string secondHalf = "";

			for (int i = 0; i < (indexTagStart + 11); i++) {
				firstHalf += fileContents[i];
			}
			for (int i2 = indexTagEnd; i2 < fileContents.size(); i2++) {
				secondHalf += fileContents[i2];
			}

			fileContents = firstHalf + StringUtils::format("%d", highScore) + secondHalf;
		}

		std::fstream output;
		output.open(path);

		output << fileContents;

		output.close();
	}
	else {
		FILE *fp = fopen(path.c_str(), "w");

		if (!fp) {
			CCLOG("Cannot create file in %s", path.c_str());
			return;
		}

		std::string value = StringUtils::format("<?xml version=\"1.0\" encoding=\"UTF - 8\"?>");
		value += StringUtils::format("\n<!DOCTYPE SB76-PBS-ABHS PUBLIC>");
		value += StringUtils::format("\n<data>");
		value += StringUtils::format("\n\t<highscore>%d</highscore>", highScore);
		value += StringUtils::format("\n</data>");

		fputs(value.c_str(), fp);
		fclose(fp);
	}
}

int ScoreManager::getHighscoreFromFile()
{
	bool tagFound = false;

	int highscore = 0;
	std::string tempHighScore = "";

	// Insert code to get highscore from file
	std::string path = getFilePath();

	std::ifstream infile(path);
	std::string currLine;

	while (std::getline(infile, currLine)) {
		if (!tagFound) {
			int found = currLine.find("<highscore>");
			if (found != -1) {
				tagFound = true;

				if (tagFound) {
					int indexScoreStart = currLine.find(">");
					int indexScoreEnd = currLine.find("<", indexScoreStart + 1);

					for (int i = indexScoreStart + 1; i < indexScoreEnd; i++) {
						tempHighScore += currLine.at(i);
					}

					highScore = strtod(tempHighScore.c_str(), nullptr);
				}
			}
		}
	}

	// No tag found, set highscore to 0
	if (!tagFound) {
		highScore = 0;
	}

	return highScore;
}

std::string ScoreManager::getFilePath()
{
	std::string path = "";

	// testing
	std::string writableDir = CCFileUtils::getInstance()->getWritablePath();

	path = writableDir + "\highscoredata.xml";

	return path;
}

ScoreManager::~ScoreManager()
{
}