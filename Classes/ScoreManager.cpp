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

void ScoreManager::storeHighscoreToFile(int level, int star, int minutes, int seconds, int miliseconds)
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
			insertString += "\t\t<minutes>" + StringUtils::format("%d", minutes) + "</minutes>\n";
			insertString += "\t\t<seconds>" + StringUtils::format("%d", seconds) + "</seconds>\n";
			insertString += "\t\t<miliseconds>" + StringUtils::format("%d", miliseconds) + "</miliseconds>\n";
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
			for (int i = 0; i < (int)(indexTagStart + levelTagString.length()); i++) {
				firstHalf += fileContents[i];
			}

			fileContents = firstHalf;

			// Now add the other tags
			fileContents += "\n\t\t<star>" + StringUtils::format("%d", star) + "</star>\n";
			fileContents += "\t\t<minutes>" + StringUtils::format("%d", minutes) + "</minutes>\n";
			fileContents += "\t\t<seconds>" + StringUtils::format("%d", seconds) + "</seconds>\n";
			fileContents += "\t\t<miliseconds>" + StringUtils::format("%d", miliseconds) + "</miliseconds>";
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
		value += StringUtils::format("\n\t\t<minutes>%d</minutes>", minutes);
		value += StringUtils::format("\n\t\t<seconds>%d</seconds>", seconds);
		value += StringUtils::format("\n\t\t<miliseconds>%d</miliseconds>", miliseconds);
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

					std::string temp = "";
					temp += currLine.at(indexScoreStart + 1);

					starRating = strtod(temp.c_str(), nullptr);
				}
			}
		}
	}

	return starRating;
}

std::string ScoreManager::getMinutesFromFile(int level)
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
				int found = currLine.find("<minutes>");

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

std::string ScoreManager::getSecondsFromFile(int level)
{
	bool levelTagFound = false;
	bool timeTagFound = false;

	//int starRating = 0;
	std::string time = "";

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
				int found = currLine.find("<seconds>");

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

std::string ScoreManager::getMilisecondsFromFile(int level)
{
	bool levelTagFound = false;
	bool timeTagFound = false;

	//int starRating = 0;
	std::string time = "";

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
				int found = currLine.find("<miliseconds>");

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

						star3 = strtod(tempString.c_str(), nullptr);
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

						star2 = strtod(tempString.c_str(), nullptr);
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

					star1 = strtod(tempString.c_str(), nullptr);
				}
			}
		}
		else {
			break;
		}
	}

	// Have a look at the time
	int minutes = GameManager::sharedGameManager()->getMin();
	int seconds = GameManager::sharedGameManager()->getSec();
	int miliseconds = GameManager::sharedGameManager()->getMil();
	int time = (GameManager::sharedGameManager()->getMin() * 60) + GameManager::sharedGameManager()->getSec();

	// Check if score is lower than previous saved
	bool higherScore = false;

	// Check if time can award 3 stars
	if (time < star3) {
		if (getStarFromFile(level) < 3) {
			storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
			return 3;
		}
		else if (getStarFromFile(level) == 3) {
			if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
				// stored score has higher minute value, this is a new highscore
				storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
				return 3;
			}
			else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
				// Same minute value, check seconds
				if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
					// stored score has higher second value, this is a new highscore
					storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
					return 3;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
					// Same second value, check miliseconds
					if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
						// stored score has higher milisecond value, this is a new highscore
						storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
						return 3;
					}
					else {
						higherScore = true;
						return 3;
					}
				}
				else {
					higherScore = true;
					return 3;
				}
			}
			else {
				higherScore = true;
				return 3;
			}
		}
		else {
			higherScore = true;
			return 3;
		}
	}
	else if (time == star3) {
		if (GameManager::sharedGameManager()->getMil() == 0) {
			if (getStarFromFile(level) < 3) {
				storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
				return 3;
			}
			else if (getStarFromFile(level) == 3) {
				if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
					// stored score has higher minute value, this is a new highscore
					storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
					return 3;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
					// Same minute value, check seconds
					if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
						// stored score has higher second value, this is a new highscore
						storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
						return 3;
					}
					else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
						// Same second value, check miliseconds
						if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
							// stored score has higher milisecond value, this is a new highscore
							storeHighscoreToFile(level, 3, minutes, seconds, miliseconds);
							return 3;
						}
						else {
							higherScore = true;
							return 3;
						}
					}
				}
				else {
					higherScore = true;
					return 3;
				}
			}
			else {
				higherScore = true;
				return 3;
			}
		}
		else {
			if (getStarFromFile(level) < 2) {
				storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
				return 2;
			}
			else if (getStarFromFile(level) == 2) {
				if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
					// stored score has higher minute value, this is a new highscore
					storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
					return 2;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
					// Same minute value, check seconds
					if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
						// stored score has higher second value, this is a new highscore
						storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
						return 2;
					}
					else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
						// Same second value, check miliseconds
						if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
							// stored score has higher milisecond value, this is a new highscore
							storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
							return 2;
						}
						else {
							higherScore = true;
							return 2;
						}
					}
					else {
						higherScore = true;
						return 2;
					}
				}
				else {
					higherScore = true;
					return 2;
				}
			}
			else {
				higherScore = true;
				return 2;
			}
		}
	}
	// Check if time can award 2 stars
	else if (time < star2) {
		if (getStarFromFile(level) < 2) {
			storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
			return 2;
		}
		else if (getStarFromFile(level) == 2) {
			if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
				// stored score has higher minute value, this is a new highscore
				storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
				return 2;
			}
			else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
				// Same minute value, check seconds
				if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
					// stored score has higher second value, this is a new highscore
					storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
					return 2;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
					// Same second value, check miliseconds
					if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
						// stored score has higher milisecond value, this is a new highscore
						storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
						return 2;
					}
					else {
						higherScore = true;
						return 2;
					}
				}
				else {
					higherScore = true;
					return 2;
				}
			}
			else {
				higherScore = true;
				return 2;
			}
		}
		else {
			higherScore = true;
			return 2;
		}
	}
	else if (time == star2) {
		if (GameManager::sharedGameManager()->getMil() == 0) {
			if (getStarFromFile(level) < 2) {
				storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
				return 2;
			}
			else if (getStarFromFile(level) == 2) {
				if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
					// stored score has higher minute value, this is a new highscore
					storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
					return 2;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
					// Same minute value, check seconds
					if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
						// stored score has higher second value, this is a new highscore
						storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
						return 2;
					}
					else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
						// Same second value, check miliseconds
						if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
							// stored score has higher milisecond value, this is a new highscore
							storeHighscoreToFile(level, 2, minutes, seconds, miliseconds);
							return 2;
						}
						else {
							higherScore = true;
							return 2;
						}
					}
					else {
						higherScore = true;
						return 2;
					}
				}
				else {
					higherScore = true;
					return 2;
				}
			}
			else {
				higherScore = true;
				return 2;
			}
		}
		else {
			if (getStarFromFile(level) < 1) {
				storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
				return 1;
			}
			else if (getStarFromFile(level) == 1) {
				if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
					// stored score has higher minute value, this is a new highscore
					storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
					return 1;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
					// Same minute value, check seconds
					if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
						// stored score has higher second value, this is a new highscore
						storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
						return 1;
					}
					else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
						// Same second value, check miliseconds
						if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
							// stored score has higher milisecond value, this is a new highscore
							storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
							return 1;
						}
						else {
							higherScore = true;
							return 1;
						}
					}
					else {
						higherScore = true;
						return 1;
					}
				}
				else {
					higherScore = true;
					return 1;
				}
			}
			else {
				higherScore = true;
				return 1;
			}
		}
	}
	// Check if time can award 1 star
	else if (time < star1) {
		if (getStarFromFile(level) < 1) {
			storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
			return 1;
		}
		else if (getStarFromFile(level) == 1) {
			if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
				// stored score has higher minute value, this is a new highscore
				storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
				return 1;
			}
			else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
				// Same minute value, check seconds
				if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
					// stored score has higher second value, this is a new highscore
					storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
					return 1;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
					// Same second value, check miliseconds
					if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
						// stored score has higher milisecond value, this is a new highscore
						storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
						return 1;
					}
					else {
						higherScore = true;
						return 1;
					}
				}
				else {
					higherScore = true;
					return 1;
				}
			}
			else {
				higherScore = true;
				return 1;
			}
		}
		else {
			higherScore = true;
			return 1;
		}
	}
	else if (time == star1) {
		if (GameManager::sharedGameManager()->getMil() == 0) {
			if (getStarFromFile(level) < 1) {
				storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
				return 1;
			}
			else if (getStarFromFile(level) == 1) {
				if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
					// stored score has higher minute value, this is a new highscore
					storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
					return 1;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
					// Same minute value, check seconds
					if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
						// stored score has higher second value, this is a new highscore
						storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
						return 1;
					}
					else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
						// Same second value, check miliseconds
						if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
							// stored score has higher milisecond value, this is a new highscore
							storeHighscoreToFile(level, 1, minutes, seconds, miliseconds);
							return 1;
						}
						else {
							higherScore = true;
							return 1;
						}
					}
					else {
						higherScore = true;
						return 1;
					}
				}
				else {
					higherScore = true;
					return 1;
				}
			}
			else {
				higherScore = true;
				return 1;
			}
		}
		else {
			if (strtod(getMinutesFromFile(level).c_str(), nullptr) > minutes) {
				// stored score has higher minute value, this is a new highscore
				storeHighscoreToFile(level, 0, minutes, seconds, miliseconds);
				return 0;
			}
			else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == minutes) {
				// Same minute value, check seconds
				if (strtod(getSecondsFromFile(level).c_str(), nullptr) > seconds) {
					// stored score has higher second value, this is a new highscore
					storeHighscoreToFile(level, 0, minutes, seconds, miliseconds);
					return 0;
				}
				else if (strtod(getMinutesFromFile(level).c_str(), nullptr) == seconds) {
					// Same second value, check miliseconds
					if (strtod(getMilisecondsFromFile(level).c_str(), nullptr) > miliseconds) {
						// stored score has higher milisecond value, this is a new highscore
						storeHighscoreToFile(level, 0, minutes, seconds, miliseconds);
						return 0;
					}
					else {
						higherScore = true;
						return 0;
					}
				}
				else {
					higherScore = true;
					return 0;
				}
			}
			else {
				higherScore = true;
				return 0;
			}
		}
	}

	if (!higherScore) {
		storeHighscoreToFile(level, 0, minutes, seconds, miliseconds);
	}

	return 0;
}

ScoreManager::~ScoreManager()
{
}