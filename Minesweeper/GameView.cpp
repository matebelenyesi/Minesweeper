#include "GameView.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <memory>

GameView::GameView()
{

}

GameView::GameView(sf::RenderWindow *gameWindow)
{
	window = gameWindow;

	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Error to load arial.ttf font-style";
	}

	loadTextures();

	beginner.setFillColor(sf::Color::Blue);
	beginner.setSize(sf::Vector2f(175, 50));
	beginnerTexture.loadFromFile("beginner.png");
	beginner.setTexture(&beginnerTexture);

	intermediate.setFillColor(sf::Color::Green);
	intermediate.setSize(sf::Vector2f(175, 50));
	intermediateTexture.loadFromFile("intermediate.png");
	intermediate.setTexture(&intermediateTexture);

	hard.setFillColor(sf::Color::Red);
	hard.setSize(sf::Vector2f(175, 50));
	hardTexture.loadFromFile("hard.png");
	hard.setTexture(&hardTexture);

	leftBombsText.setFont(font);
	leftBombsText.setCharacterSize(18);

	timeText.setFont(font);
	timeText.setCharacterSize(18);

	endText.setString("");
	endText.setFillColor(sf::Color::White);
	endText.setFont(font);
	endText.setCharacterSize(18);

	highScoreText.setFont(font);
	highScoreText.setFillColor(sf::Color::Yellow);
	highScoreText.setCharacterSize(18);
}

void GameView::loadTextures()
{
	sf::Texture texture;

	for (int i = 0; i < 8; ++i)
	{
		std::ostringstream os;
		os << i + 1 << ".png";

		if (!texture.loadFromFile(os.str()))
		{
			std::cout << "Can't load texture: "<< os.str() << std::endl;
		}

		numberTextures.push_back(texture);
	}

	if (!flagTexture.loadFromFile("flag.png"))
	{
		std::cout << "Can't load texture: " << "flag.png" << std::endl;
	}

	if (!bombTexture.loadFromFile("bomb.png"))
	{
		std::cout << "Can't load texture: " << "bomb.png" << std::endl;
	}

	if (!wrongbombTexture.loadFromFile("wrongbomb.png"))
	{
		std::cout << "Can't load texture: " << "wrongbomb.png" << std::endl;
	}

	if (!actualbombTexture.loadFromFile("actualbomb.png"))
	{
		std::cout << "Can't load texture: " << "actualbomb.png" << std::endl;
	}
}

void GameView::draw(GameModel *gameModel)
{
	window->clear();
	Level level = gameModel->getLevel();

	for (int i = 0; i < gameModel->getLevelXSize(); ++i)
	{
		for (int j = 0; j < gameModel->getLevelYSize(); ++j)
		{
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(25.f, 25.f));
			rectangle.setPosition(sf::Vector2f(25.f * j, 25.f * i));

			switch (level[i][j])
			{
			case 0:
				rectangle.setFillColor(sf::Color(169, 169, 169));
				break;
			case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
				rectangle.setTexture(&numberTextures[level[i][j]-1]);
				break;
			case 9: //Flag
				rectangle.setTexture(&flagTexture);
				break;
			case 10: //Bomb
				rectangle.setTexture(&bombTexture);
				break;
			case 11: //Unclicked filed with bomb
				rectangle.setFillColor(sf::Color(107, 107, 107));
				break;
			case 12: //Unclicked field without bomb
				rectangle.setFillColor(sf::Color(107, 107, 107));
				break;
			case 13: //Wrongly flagged bomb
				rectangle.setTexture(&wrongbombTexture);
				break;
			case 14: //Clicked bomb
				rectangle.setTexture(&actualbombTexture);
				break;
			default:
				break;
			}

			rectangle.setOutlineColor(sf::Color::White);
			rectangle.setOutlineThickness(1);
			window->draw(rectangle);
		}
	}

	beginner.setPosition(sf::Vector2f(gameModel->getLevelYSize() * 25.f +25, 0.f));
	intermediate.setPosition(sf::Vector2f(gameModel->getLevelYSize() * 25.f +25,50.f));
	hard.setPosition(sf::Vector2f(gameModel->getLevelYSize() * 25.f +25, 100.f));

	
	leftBombsText.setPosition(gameModel->getLevelYSize() * 25.f + 25, 150.f);
	std::string leftBombsString = "Mines: ";
	leftBombsString = leftBombsString + std::to_string(gameModel->getNumberOfBombsToSign());
	leftBombsText.setString(leftBombsString);

	std::stringstream timeStream;
	timeStream << std::fixed << std::setprecision(2) << gameModel->getTimer().getElapsedTime();
	std::string time = timeStream.str();

	std::string timeString = "Time: ";
	timeString = timeString + time;
	timeText.setString(timeString);
	timeText.setPosition(gameModel->getLevelYSize() * 25.f + 105, 150.f);
	

	if (gameModel->getPlayerState() == PlayerState::NOT_STARTED)
	{
		timeText.setString("Time: 0.00");
	}

	endText.setPosition(gameModel->getLevelYSize() * 25.f + 60, 180.f);

	if (gameModel->getConnection())
	{
		double score = gameModel->getHighscores()[gameModel->getHardness() - 1];
		std::stringstream scoreStream;
		scoreStream << "Best: " << std::fixed << std::setprecision(2) << score;
		highScoreText.setString(scoreStream.str());
		highScoreText.setPosition(gameModel->getLevelYSize() * 25.f + 60, 200.f);
	}

	switch (gameModel->getPlayerState())
	{
	case PlayerState::LOST:
		endText.setString("YOU LOST");
		endText.setFillColor(sf::Color::Red);
		break;
	case PlayerState::WON:
		endText.setString("YOU WON");
		endText.setFillColor(sf::Color::Green);
		break;
	default:
		endText.setString("");
		break;
	}

	window->draw(beginner);
	window->draw(intermediate);
	window->draw(hard);
	window->draw(leftBombsText);
	window->draw(timeText);
	window->draw(endText);

	if (gameModel->getConnection())
	{
		window->draw(highScoreText);
	}

	window->display();
}

GameView::~GameView()
{
}
