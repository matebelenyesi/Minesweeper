#include "stdafx.h"
#include "GameView.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 


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

	beginner.setFillColor(sf::Color::Blue);
	intermediate.setFillColor(sf::Color::Green);
	hard.setFillColor(sf::Color::Red);

	endText.setString("");
	endText.setFillColor(sf::Color::White);
	endText.setFont(font);
	timeText.setFont(font);
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
			sf::Texture texture;

			switch (level[i][j])
			{
			case 0:
				rectangle.setFillColor(sf::Color(169, 169, 169));
				break;
			case 1:
				//rectangle.setFillColor(sf::Color::Blue);

				if (!texture.loadFromFile("1.png"))
				{
					std::cout << "Wrong";
				}

				rectangle.setTexture(&texture);
				break;
			case 2:
				if (!texture.loadFromFile("2.png"))
				{
					std::cout << "Wrong";
				}

				rectangle.setTexture(&texture);
				break;
			case 3:
				if (!texture.loadFromFile("3.png"))
				{
					std::cout << "Wrong";
				}

				rectangle.setTexture(&texture);
				break;
			case 4:
				if (!texture.loadFromFile("4.png"))
				{
					std::cout << "Wrong";
				}

				rectangle.setTexture(&texture);
				break;
			case 5:
				if(!texture.loadFromFile("5.png"))
				{
					std::cout << "Wrong";
				}

				rectangle.setTexture(&texture);
				break;
			case 6:
				if (!texture.loadFromFile("6.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			case 7:
				if (!texture.loadFromFile("7.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			case 8:
				if (!texture.loadFromFile("8.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			case 9:
				if (!texture.loadFromFile("flag.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			case 10:
				if (!texture.loadFromFile("bomb.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			case 11:
				rectangle.setFillColor(sf::Color(107, 107, 107));
				break;
			case 12:
				rectangle.setFillColor(sf::Color(107, 107, 107));
				break;
			case 13:
				if (!texture.loadFromFile("wrongbomb.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			case 14:
				if (!texture.loadFromFile("actualbomb.png"))
				{
					std::cout << "Wrong";
				}
				rectangle.setTexture(&texture);
				break;
			default:
				break;
			}

			rectangle.setOutlineColor(sf::Color::White);
			rectangle.setOutlineThickness(1);
			window->draw(rectangle);
		}
	}

	beginner.setPosition(sf::Vector2f(gameModel->getLevelYSize() * 25 +25, 0));
	beginner.setSize(sf::Vector2f(175, 50));
	sf::Texture beginnerTexture;
	beginnerTexture.loadFromFile("beginner.png");
	beginner.setTexture(&beginnerTexture);
	intermediate.setPosition(sf::Vector2f(gameModel->getLevelYSize() * 25 +25,50));
	intermediate.setSize(sf::Vector2f(175, 50));
	sf::Texture intermediateTexture;
	intermediateTexture.loadFromFile("intermediate.png");
	intermediate.setTexture(&intermediateTexture);
	hard.setPosition(sf::Vector2f(gameModel->getLevelYSize() * 25 +25, 100));
	hard.setSize(sf::Vector2f(175, 50));
	sf::Texture hardTexture;
	hardTexture.loadFromFile("hard.png");
	hard.setTexture(&hardTexture);

	leftBombsText.setFont(font);
	leftBombsText.setPosition(gameModel->getLevelYSize() * 25 + 25, 150);

	std::string leftBombsString = "Mines: ";

	leftBombsString = leftBombsString + std::to_string(gameModel->getNumberOfBombsToSign());
	leftBombsText.setString(leftBombsString);
	leftBombsText.setCharacterSize(18);

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << gameModel->getTimer().getElapsedTime();
	std::string s = stream.str();

	std::string timeString = "Time: ";
	timeString = timeString + s;
	timeText.setString(timeString);
	timeText.setPosition(gameModel->getLevelYSize() * 25 + 105, 150);
	timeText.setCharacterSize(18);

	if (gameModel->getPlayerState() == PlayerState::NOT_STARTED)
	{
		timeText.setString("Time: 0.00");
	}
	endText.setPosition(gameModel->getLevelYSize() * 25 + 50, 180);
	endText.setCharacterSize(18);




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
	window->draw(endText);
	window->draw(timeText);

	window->display();
}

GameView::~GameView()
{
}
