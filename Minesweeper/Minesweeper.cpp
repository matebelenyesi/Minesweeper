// Minesweeper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameModel.h"
#include "GameView.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	GameModel gameModel;
	sf::RenderWindow gameWindow(sf::VideoMode(25*gameModel.getLevelYSize() + 200, 25 * gameModel.getLevelXSize()), "Minesweeper");
	GameView gameView(&gameWindow);

	while (gameWindow.isOpen())
	{

		sf::Event event;
		while (gameWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				gameWindow.setView(sf::View(visibleArea));
			}

			if (event.type == sf::Event::Closed)
				gameWindow.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				gameModel.clickField((sf::Mouse::getPosition(gameWindow).y) / 25, (sf::Mouse::getPosition(gameWindow).x) / 25);
				//std::cout << (sf::Mouse::getPosition(gameWindow).y)/25 << " " << (sf::Mouse::getPosition(gameWindow).x)/25 << std::endl;

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
				gameModel.flagField((sf::Mouse::getPosition(gameWindow).y) / 25, (sf::Mouse::getPosition(gameWindow).x) / 25);
			//std::cout << (sf::Mouse::getPosition(gameWindow).y)/25 << " " << (sf::Mouse::getPosition(gameWindow).x)/25 << std::endl;

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
				gameModel.planField((sf::Mouse::getPosition(gameWindow).y) / 25, (sf::Mouse::getPosition(gameWindow).x) / 25);
			//std::cout << (sf::Mouse::getPosition(gameWindow).y)/25 << " " << (sf::Mouse::getPosition(gameWindow).x)/25 << std::endl;

		}
		gameWindow.setSize(sf::Vector2u(25 * gameModel.getLevelYSize() + 200, 25 * gameModel.getLevelXSize()));

		gameView.draw(&gameModel);

	}

	return 0;
}

