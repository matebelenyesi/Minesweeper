#pragma once
#include <vector>
#include <cstdlib> // for std::rand() and std::srand()
#include <ctime> // for std::time()
#include <SFML/Audio.hpp>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "PlayerState.h"
#include "Timer.h"

typedef std::vector<std::vector<int>> Level;

class GameModel
{
private:

	Level level;
	int levelXSize;
	int levelYSize;
	int bombCount = 10;
	int flagCount =0;
	int hardness = 1;
	PlayerState playerState = PlayerState::NOT_STARTED;
	std::vector<std::tuple<int, int>> bombPositions;
	std::vector<std::tuple<int, int>> freePositions;

	Timer timer;

	sf::SoundBuffer clickSoundBuffer;
	sf::Sound clickSound;
	sf::SoundBuffer bombSoundBuffer;
	sf::Sound bombSound;
	sf::SoundBuffer flagSoundBuffer;
	sf::Sound flagSound;
	sf::SoundBuffer wonSoundBuffer;
	sf::Sound wonSound;
	sf::SoundBuffer removeFlagSoundBuffer;
	sf::Sound removeFlagSound;

	sql::Connection *connection;
	std::vector<double> highScores;

	double getHighscore(int level);
	void updateHighscore(int level, double score);


public:
	GameModel(sql::Connection *con);

	Level getLevel();
	int getLevelXSize();
	int getLevelYSize();

	void clickField(int x, int y);
	void flagField(int x, int y);
	void planField(int x, int y);
	bool isValidField(int x, int y);
	bool isBombPosition(int x, int y);
	int getNumberOfBombsToSign();
	PlayerState getPlayerState();
	Timer getTimer();

	void newGame(int x);

	std::vector<double> getHighscores();
	int getHardness();

	sql::Connection* getConnection();

	~GameModel();
};

