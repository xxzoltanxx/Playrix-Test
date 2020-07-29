#pragma once
#include <deque>
#include <memory>
#include <unordered_map>
#include <functional>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "TextureCache.h"
#include <functional>
#include "Entity.h"

class StateManager;

class State
{
public:
	friend class StateManager;
	State(StateManager* manager);
	enum StateType
	{
		MainMenu = 0,
		Game = 1
	};
	void update(float dt);
	void draw(sf::RenderTarget& target);
	void handleEvent(sf::Event& event);
protected:
	bool mIsTranscendent = false;
	bool mIsTransparent = false;
	std::vector<Entity> mEntities;

	StateManager* mManager = nullptr;
};

class MainMenuState : public State
{
public:
	MainMenuState(StateManager* manager);
private:
	int playerCount = 0;
	int numberOfCards = 2;
};
