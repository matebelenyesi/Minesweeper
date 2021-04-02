#include "GameModel.h"
#include <tuple>
#include <iostream>

int getRandomNumber(int min, int max)
{
	static constexpr double fraction{ 1.0 / (RAND_MAX + 1.0) };  // static used for efficiency, so we only calculate this value once
																 // evenly distribute the random number across our range
	return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}

GameModel::GameModel(sql::Connection *con)
{
	connection = con;
	if (connection)
	{
		highScores.push_back(getHighscore(1));
		highScores.push_back(getHighscore(2));
		highScores.push_back(getHighscore(3));
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	std::vector<int> startRow;

	levelXSize =9;
	levelYSize =9;
	bombCount = 10;

	for (int i = 0; i < levelYSize; ++i)
	{
		startRow.push_back(12);
	}

	for (int i = 0; i < levelXSize; ++i)
	{
		this->level.push_back(startRow);
	}

	for (int i = 0; i < levelXSize; ++i)
	{
		for (int j = 0; j < levelYSize; ++j)
		{
			std::tuple<int, int> tuple = std::make_tuple(i, j);
			bombPositions.push_back(tuple);
		}
	}

	clickSoundBuffer.loadFromFile("click.ogg");
	clickSound.setBuffer(clickSoundBuffer);
	bombSoundBuffer.loadFromFile("bomb.wav");
	bombSound.setBuffer(bombSoundBuffer);
	flagSoundBuffer.loadFromFile("flag.wav");
	flagSound.setBuffer(flagSoundBuffer);
	wonSoundBuffer.loadFromFile("won.wav");
	wonSound.setBuffer(wonSoundBuffer);
	removeFlagSoundBuffer.loadFromFile("remove_flag.wav");
	removeFlagSound.setBuffer(removeFlagSoundBuffer);
	planSoundBuffer.loadFromFile("plan.wav");
	planSound.setBuffer(planSoundBuffer);
}

Level GameModel::getLevel()
{
	return level;
}

int GameModel::getLevelXSize()
{
	return levelXSize;
}

int GameModel::getLevelYSize()
{
	return levelYSize;
}

void GameModel::click(int x, int y, void(GameModel::*mode)(int, int))
{
	clickCount = 0;
	(this->*mode)(x, y);


	if (clickCount >= 20 && playerState == PlayerState::IN_GAME)
	{
		planSound.play();
	}
}

void GameModel::clickField(int x, int y)
{
	
	if (y >= levelYSize+1)
	{
		newGame(x / 2);
		return;
	}

	if (!isValidField(x, y) || !(level[x][y] == 12 || level[x][y] ==11) || playerState == PlayerState::LOST || playerState == PlayerState::WON)
		return;

	clickSound.play();
	clickCount++;

	if (level[x][y] == 11)
	{
		for (int i = 0; i < bombPositions.size(); ++i)
		{
			int x = std::get<0>(bombPositions[i]);
			int y = std::get<1>(bombPositions[i]);

			if (level[x][y] == 11)
			{
				level[x][y] = 10;
			}
		}

		for (int i = 0; i< levelXSize; ++i)
		{
			for (int j = 0; j < levelYSize; ++j)
			{
				if (level[i][j] == 9 && !isBombPosition(i, j))
				{
					level[i][j] = 13;
				}
			}
		}
		level[x][y] = 14;
		playerState = PlayerState::LOST;
		timer.pause();
		bombSound.play();
		return;
	}

	if (playerState == PlayerState::NOT_STARTED)
	{
		int index = x * levelYSize + y;
		freePositions.push_back(bombPositions[index]);
		bombPositions.erase(bombPositions.begin() + index);


		for (int i = 0; i < levelXSize*levelYSize - 1 - bombCount; ++i)
		{
			int index = getRandomNumber(0, bombPositions.size() - 1);
			freePositions.push_back(bombPositions[index]);
			bombPositions.erase(bombPositions.begin() + index);
		}


		for (int i = 0; i < bombPositions.size(); ++i)
		{
			int x = std::get<0>(bombPositions[i]);
			int y = std::get<1>(bombPositions[i]);

			if(level[x][y] == 12)
				level[x][y] = 11;
		}
		playerState = PlayerState::IN_GAME;
		timer.restart();
	}

	int bombCountNearField = 0;

	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			if (isValidField(i, j) && !(i == x && j == y) && isBombPosition(i,j))
			{
				++bombCountNearField;
			}
		}
	}

	level[x][y] = bombCountNearField;

	if (level[x][y] == 0)
	{
		
		for (int i = x - 1; i <= x + 1; ++i)
		{
			for (int j = y - 1; j <= y + 1; ++j)
			{
				clickField(i, j);
			}
		}
		
	}

	bool won = true;
	for (int i = 0; i < freePositions.size(); ++i)
	{
		int x = std::get<0>(freePositions[i]);
		int y = std::get<1>(freePositions[i]);
		won = won && level[x][y] <= 8;
	}

	if (won)
	{
		timer.pause();
		for (int i = 0; i < bombPositions.size(); ++i)
		{
			int x = std::get<0>(bombPositions[i]);
			int y = std::get<1>(bombPositions[i]);
			level[x][y] = 9;
		}
		flagCount = bombCount;
		playerState = PlayerState::WON;
		if (connection)
		{
			updateHighscore(hardness, timer.getElapsedTime());
			highScores[hardness - 1] = getHighscore(hardness);
		}
		wonSound.play();
	}
}

void GameModel::flagField(int x, int y)
{
	if (!isValidField(x, y) || !(level[x][y] == 12 || level[x][y] == 11 || level[x][y] == 9) || playerState == PlayerState::LOST || playerState == PlayerState::WON)
		return;

	if (level[x][y] != 9)
	{
		level[x][y] = 9;
		++flagCount;
		flagSound.play();
	}
	else
	{
		bool isBombInPosition = false;

		for (int i = 0; i < bombPositions.size(); ++i)
		{
			if (std::get<0>(bombPositions[i]) == x && std::get<1>(bombPositions[i]) == y)
			{
				isBombInPosition = true;
			}
		}

		if (isBombInPosition && playerState == PlayerState::IN_GAME)
		{
			level[x][y] = 11;
		}
		else
		{
			level[x][y] = 12;
		}
		--flagCount;
		removeFlagSound.play();
	}
}

void GameModel::planField(int x, int y)
{
	if (!isValidField(x, y) || level[x][y] > 8 || level[x][y] == 0 || playerState == PlayerState::LOST || playerState == PlayerState::WON)
		return;

	int flagCountNearField = 0;

	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			if (isValidField(i, j) && !(i == x && j == y) && level[i][j] == 9)
			{
				++flagCountNearField;
			}
		}
	}

	if (level[x][y] == flagCountNearField)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			for (int j = y - 1; j <= y + 1; ++j)
			{
				clickField(i, j);
			}
		}
	}
}

bool GameModel::isValidField(int x, int y)
{
	if(x<0 || y<0 || x>=levelXSize  || y>= levelYSize)
		return false;

	return true;
}

bool GameModel::isBombPosition(int x, int y)
{

	bool isBombPos = false;
	for (int i = 0; i < bombPositions.size(); ++i)
	{
		int xp = std::get<0>(bombPositions[i]);
		int yp = std::get<1>(bombPositions[i]);

		if (xp == x && yp == y)
		{
			isBombPos = true;
		}
	}

	return isBombPos;
}

int GameModel::getNumberOfBombsToSign()
{
	return bombCount - flagCount;
}

PlayerState GameModel::getPlayerState()
{
	return playerState;
}

Timer GameModel::getTimer()
{
	return timer;
}

void GameModel::newGame(int x)
{
	if (x == 0)
	{
		levelXSize = 9;
		levelYSize = 9;
		bombCount = 10;
		hardness = 1;
	}
	else if (x == 1)
	{
		levelXSize = 16;
		levelYSize = 16;
		bombCount = 40;
		hardness = 2;
	}
	else if (x == 2)
	{
		levelXSize = 16;
		levelYSize = 30;
		bombCount = 99;
		hardness = 3;
	}

	level.clear();
	bombPositions.clear();
	freePositions.clear();
	flagCount = 0;
	playerState = PlayerState::NOT_STARTED;

	std::vector<int> startRow;

	for (int i = 0; i < levelYSize; ++i)
	{
		startRow.push_back(12);
	}

	for (int i = 0; i < levelXSize; ++i)
	{
		this->level.push_back(startRow);
	}

	for (int i = 0; i < levelXSize; ++i)
	{
		for (int j = 0; j < levelYSize; ++j)
		{
			std::tuple<int, int> tuple = std::make_tuple(i, j);
			bombPositions.push_back(tuple);
		}
	}
}

std::vector<double> GameModel::getHighscores()
{
	return highScores;
}

int GameModel::getHardness()
{
	return hardness;
}

sql::Connection * GameModel::getConnection()
{
	return connection;
}

double GameModel::getHighscore(int level)
{
	sql::ResultSet *res;
	sql::PreparedStatement *pstmt;

	try
	{
		pstmt = connection->prepareStatement("SELECT score FROM Highscores WHERE id = ?");
		pstmt->setInt(1, level);
		res = pstmt->executeQuery();
		res->next();
		double returnValue = res->getDouble("score");
		delete res;
		delete pstmt;
		return returnValue;
	} 
	catch (sql::SQLException &e)
	{
		std::cout << e.what()<<std::endl;
	}
}

void GameModel::updateHighscore(int level, double score)
{
	sql::PreparedStatement *pstmt;

	try
	{
		double current = getHighscore(level);
		if (score < current)
		{
			pstmt = connection->prepareStatement("UPDATE Highscores SET score = ? WHERE id = ?");
			pstmt->setDouble(1, score);
			pstmt->setInt(2, level);
			pstmt->executeQuery();
			delete pstmt;
		}

	}
	catch (sql::SQLException &e)
	{
		std::cout << e.what() << std::endl;
	}
}


GameModel::~GameModel()
{
}
