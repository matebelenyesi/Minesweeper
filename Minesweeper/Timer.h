#pragma once
#include <SFML/System.hpp>
class Timer
{
public:

	Timer();
	void restart();
	void start();
	void pause();
	float getElapsedTime();
	~Timer();

private:
	sf::Clock mC;
	float runTime;
	bool bPaused;
};

