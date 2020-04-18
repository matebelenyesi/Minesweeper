#pragma once
#include <SFML/Graphics.hpp>
#include "GameModel.h"
class GameView
{
private:
	sf::RenderWindow *window;

	sf::Font font;
	sf::RectangleShape beginner;
	sf::RectangleShape intermediate;
	sf::RectangleShape hard;
	sf::Text timeText;
	sf::Text leftBombsText;
	sf::Text endText;
public:
	GameView();
	GameView(sf::RenderWindow *gameWindow);

	void draw(GameModel *gameModel);
	~GameView();
};

