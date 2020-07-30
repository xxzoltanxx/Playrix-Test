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
	std::vector<Entity*> mEntities;

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

class GameState : public State
{
public:
	GameState(StateManager* manager);
private:
	void createPair(int face);

	int currentPlayer = 0;
	int playerCount = 0;
	int numberOfCards = 0;
	std::unordered_map<int, int> mScoreSheet;
	std::vector<sf::Vector2f> mPositions;
};

void GameState::createPair(int face)
{
	GameCard* first = new GameCard(sf::Vector2f(0, 0), face);
	GameCard* second = new GameCard(sf::Vector2f(0, 0), face);
	first->setScale(3000 / (640.0f / numberOfCards), 4200 / (480.0f / numberOfCards));
	second->setScale(3000 / (640.0f / numberOfCards), 4200 / (480.0f / numberOfCards));
}

GameState::GameState(StateManager* manager)
	:State(manager)
{
	std::srand(time(nullptr));
	currentPlayer = 0;
	playerCount = mManager->mContext->numberOfPlayers;
	numberOfCards = mManager->mContext->numberOfCards;



	for (int i = 0; i < playerCount; ++i)
	{
		mScoreSheet.emplace(i, 0);
	}

	for (int i = 0; i < numberOfCards / 2; ++i)
	{
		int randomFace = rand() % 54 + 1;

	}
}