#pragma once
#include "StateManager.h"
#include <SFML/Window.hpp>

class Game
{
public:
	Game();
	void run();
private:
	Context mContext;
	std::unique_ptr<StateManager> mManager;
	std::unique_ptr<sf::RenderWindow> mWindow;
};