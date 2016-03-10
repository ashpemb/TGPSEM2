#include "ScoreManager.h"

ScoreManager* ScoreManager::instance = NULL;

ScoreManager::ScoreManager()
{
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

void ScoreManager::resetScore()
{
	// TODO
}

void ScoreManager::compareScoreToHighscore()
{
	// TODO
}

void ScoreManager::storeHighscoreToFile(int level, int star, int time)
{
	bool levelTagFound = false;
	bool starTagFound = false;
	bool timeTagFound = false;

	// Insert code to save highscore to file
	std::string path = getFilePath();
	std::string fileContents = "";

	std::ifstream infile(path);
	std::string currLine;

	if (infile.good()) {
		// Keep looping through the file to find the level tag
		while (std::getline(infile, currLine)) {
			if (currLine.find("<level_" + StringUtils::format("%d", level) + ">") == true) {
				levelTagFound = true;
			}
			fileContents += currLine + "\n";
		}

		if (!levelTagFound) {
			int indexTagEnd = fileContents.find("</data>");
			std::string insertString = "\t<level_" + StringUtils::format("%d", level) + ">\n";
			insertString += "\t\t<star>" + StringUtils::format("%d", star) + "</star>\n";
			insertString += "\t\t<time>" + StringUtils::format("%d", time) + "</time>\n";
			insertString += "\t</level_" + StringUtils::format("%d", level) + ">\n";

			std::string firstHalf = "";
			std::string secondHalf = "";

			for (int i = 0; i < indexTagEnd; i++) {
				firstHalf += fileContents[i];
			}
			for (unsigned int i2 = indexTagEnd; i2 < fileContents.size(); i2++) {
				secondHalf += fileContents[i2];
			}

			fileContents = firstHalf + insertString + secondHalf;
		}
		else {
			int indexTagStart = fileContents.find("<level_" + StringUtils::format("%d", level) + ">");
			int indexTagEnd = fileContents.find("</level_" + StringUtils::format("%d", level) + ">");
						
			std::string firstHalf = "";
			std::string secondHalf = "";

			std::string levelTagString = "<level_" + StringUtils::format("%d", level) + ">";

			// Add entire document string up to the end of the level tag
			for (unsigned int i = 0; i < (indexTagStart + levelTagString.length()); i++) {
				firstHalf += fileContents[i];
			}

			fileContents = firstHalf;

			// Now add the other tags
			fileContents += "\n\t\t<star>" + StringUtils::format("%d", star) + "</star>";
			fileContents += "\n\t\t<time>" + StringUtils::format("%d", time) + "</time>";
			fileContents += "\n\t";

			for (unsigned int i2 = indexTagEnd; i2 < fileContents.size(); i2++) {
				secondHalf += fileContents[i2];
			}

			fileContents += secondHalf;
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
		value += StringUtils::format("\n\t<level_%d>", level);
		value += StringUtils::format("\n\t\t<star>%d</star>", star);
		value += StringUtils::format("\n\t\t<time>") + StringUtils::format("%d", time) + StringUtils::format("</time>");
		value += StringUtils::format("\n\t</level_%d>", level);
		value += StringUtils::format("\n</data>");

		fputs(value.c_str(), fp);
		fclose(fp);
	}
}

int ScoreManager::getStarFromFile(int level)
{
	bool levelTagFound = false;
	bool starTagFound = false;

	int starRating = 0;
	//std::string tempHighScore = "";

	// Insert code to get highscore from file
	std::string path = getFilePath();

	std::ifstream infile(path);
	std::string currLine;

	while (std::getline(infile, currLine)) {
		if (!starTagFound) {
			if (!levelTagFound) {
				int found = currLine.find("<level_" + StringUtils::format("%d", level) + ">");
				if (found != -1) {
					// Level tag has been found
					levelTagFound = true;
				}
			}
			// Check if level tag has been found, otherwise these checks are not worth doing
			else {
				int found = currLine.find("<star>");

				if (found != -1) {
					// Star tag has been found
					starTagFound = true;

					int indexScoreStart = currLine.find(">");
					int indexScoreEnd = currLine.find("<", indexScoreStart + 1);

					starRating = currLine.at(indexScoreStart + 1);
				}
			}
		}
	}

	return starRating;
}

std::string ScoreManager::getTimeFromFile(int level)
{
	bool levelTagFound = false;
	bool timeTagFound = false;

	//int starRating = 0;
	std::string time = "No Recorded Time";

	// Insert code to get highscore from file
	std::string path = getFilePath();

	std::ifstream infile(path);
	std::string currLine;

	while (std::getline(infile, currLine)) {
		if (!timeTagFound) {
			if (!levelTagFound) {
				int found = currLine.find("<level_" + StringUtils::format("%d", level) + ">");
				if (found != -1) {
					// Level tag has been found
					levelTagFound = true;
				}
			}
			// Check if level tag has been found, otherwise these checks are not worth doing
			else {
				int found = currLine.find("<time>");

				if (found != -1) {
					// Time tag has been found
					timeTagFound = true;

					int indexScoreStart = currLine.find(">");
					int indexScoreEnd = currLine.find("<", indexScoreStart + 1);

					time = "";

					for (int i = indexScoreStart + 1; i < indexScoreEnd; i++) {
						time += currLine.at(i);
					}
				}
			}
		}
	}

	return time;
}

std::string ScoreManager::getFilePath()
{
	std::string path = "";

	// testing
	std::string writableDir = FileUtils::getInstance()->getWritablePath();

	path = writableDir + "\\highscoredata.xml";

	return path;
}

int ScoreManager::getDefaultStarRating(int level)
{
	int star1 = -1;
	int star2 = -1;
	int star3 = -1;

	std::string path = "res/LevelData/Level_" + StringUtils::format("%d", level) + ".xml";

	std::ifstream infile(path);
	std::string currLine;

	while (std::getline(infile, currLine)) {
		if (star1 < 0) {
			if (star2 < 0) {
				if (star3 < 0) {
					int indexScoreStart = currLine.find("<3Stars>");
					int indexScoreEnd = currLine.find("</3Stars>", indexScoreStart + 1);

					if (indexScoreStart > 0) {
						std::string tempString = "";

						for (int i = indexScoreStart + 8; i < indexScoreEnd; i++) {
							tempString += currLine.at(i);
						}

						star3 = std::stoi(tempString);
					}
				}
				else {
					int indexScoreStart = currLine.find("<2Stars>");
					int indexScoreEnd = currLine.find("</2Stars>", indexScoreStart + 1);

					if (indexScoreStart > 0) {
						std::string tempString = "";

						for (int i = indexScoreStart + 8; i < indexScoreEnd; i++) {
							tempString += currLine.at(i);
						}

						star2 = std::stoi(tempString);
					}
				}
			}
			else {
				int indexScoreStart = currLine.find("<1Stars>");
				int indexScoreEnd = currLine.find("</1Stars>", indexScoreStart + 1);

				if (indexScoreStart > 0) {
					std::string tempString = "";

					for (int i = indexScoreStart + 8; i < indexScoreEnd; i++) {
						tempString += currLine.at(i);
					}

					star1 = std::stoi(tempString);
				}
			}
		}
		else {
			break;
		}
	}

	// Have a look at the time
	int time = (GameManager::sharedGameManager()->getMin() * 60) + GameManager::sharedGameManager()->getSec();

	// Check if time can award 3 stars
	if (time < star3) {
		storeHighscoreToFile(level, 3, time);
		return 3;
	}
	else if (time == star3) {
		if (GameManager::sharedGameManager()->getMil() == 0) {
			storeHighscoreToFile(level, 3, time);
			return 3;
		}
		else {
			storeHighscoreToFile(level, 2, time);
			return 2;
		}
	}
	// Check if time can award 2 stars
	else if (time < star2) {
		storeHighscoreToFile(level, 2, time);
		return 2;
	}
	else if (time == star2) {
		if (GameManager::sharedGameManager()->getMil() == 0) {
			storeHighscoreToFile(level, 2, time);
			return 2;
		}
		else {
			storeHighscoreToFile(level, 1, time);
			return 1;
		}
	}
	// Check if time can award 1 star
	else if (time < star1) {
		storeHighscoreToFile(level, 1, time);
		return 1;
	}
	else if (time == star1) {
		if (GameManager::sharedGameManager()->getMil() == 0) {
			storeHighscoreToFile(level, 1, time);
			return 1;
		}
		else {
			storeHighscoreToFile(level, 0, time);
			return 0;
		}
	}

	storeHighscoreToFile(level, 0, time);
	return 0;
}

ScoreManager::~ScoreManager()
{
}