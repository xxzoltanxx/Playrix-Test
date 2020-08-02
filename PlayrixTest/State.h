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
	virtual ~State()
	{
		for (Entity* entity : mEntities)
		{
			delete entity;
		}
	}
	enum StateType
	{
		None = 3,
		MainMenu = 0,
		Game = 1
	};
	enum MovingCamera
	{
		Nowhere = 0,
		Left = 1,
		Right = 2,
		Up = 4,
		Down = 8
	};
	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& target);
	virtual void handleEvent(sf::Event& event);
protected:
	bool mIsTranscendent = false;
	bool mIsTransparent = false;
	bool mLockedHandling = false;
	std::vector<Entity*> mEntities;

	StateManager* mManager = nullptr;
	sf::View mView = sf::View(sf::FloatRect(0, 0, 640, 480));
	int movingCamera;
};

class MainMenuState : public State
{
public:
	MainMenuState(StateManager* manager);
private:
	int playerCount = 1;
	int numberOfCards = 2;
};