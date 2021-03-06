// Minesweeper.cpp : Defines the entry point for the console application.
//

#include "GameModel.h"
#include "GameView.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


int main()
{
	sql::Connection *con = nullptr;
	try {
		sql::Driver *driver;
		driver = get_driver_instance();
		con = driver->connect("localhost:3306", "root", "root");
		con->setSchema("minesweeper");
		if (con)
		{
			std::cout << "Database connection successfull." << std::endl;
		}
	}
	catch (sql::SQLException &e) {
		std::cout << "Database connection failed: " << e.what() << std::endl;
	}

	GameModel gameModel(con);
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
			{
				gameModel.click((sf::Mouse::getPosition(gameWindow).y) / 25, (sf::Mouse::getPosition(gameWindow).x) / 25, &GameModel::clickField);
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
				gameModel.flagField((sf::Mouse::getPosition(gameWindow).y) / 25, (sf::Mouse::getPosition(gameWindow).x) / 25);

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
			{
				gameModel.click((sf::Mouse::getPosition(gameWindow).y) / 25, (sf::Mouse::getPosition(gameWindow).x) / 25, &GameModel::planField);
			}

		}
		gameWindow.setSize(sf::Vector2u(25 * gameModel.getLevelYSize() + 200, 25 * gameModel.getLevelXSize()));

		gameView.draw(&gameModel);

	}

	return 0;
}

