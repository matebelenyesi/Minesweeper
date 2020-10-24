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
	sf::Text highScoreText;

	void loadTextures();
	std::vector<sf::Texture> numberTextures;
	sf::Texture flagTexture;
	sf::Texture bombTexture;
	sf::Texture wrongbombTexture;
	sf::Texture actualbombTexture;
	sf::Texture beginnerTexture;
	sf::Texture intermediateTexture;
	sf::Texture hardTexture;
public:
	GameView();
	GameView(sf::RenderWindow *gameWindow);

	void draw(GameModel *gameModel);
	~GameView();
};

