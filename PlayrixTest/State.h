#pragma once
#include <deque>
#include <memory>
#include <unordered_map>
#include <functional>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "TextureCache.h"

class State;

class GameObject
{
public:
	explicit GameObject(State* boundState)
	{
		mBoundState = boundState;
	}
	virtual void draw() = 0;
	virtual void update(float dt) = 0;
	void setPosition(const sf::Vector2i& position)
	{
		mPosition = position;
	}
private:
	sf::Vector2i mPosition;
	State* mBoundState;
};

class State
{
public:
	friend class StateManager;
	enum StateType
	{
		MainMenu = 0,
		Game = 1
	};
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void handleEvent(sf::Event& event) = 0;
private:
	bool mIsTranscendent = false;
	bool mIsTransparent = false;
	std::vector<std::unique_ptr<GameObject>> mBoundObjects;
};

class Clickable : public GameObject
{
public:
	Clickable(State* boundState);
	virtual void handleClick(const sf::Vector2i& position) = 0;
protected:
	sf::FloatRect mBoundingRectangle;
};

class Arrow : public Clickable
{
public:
	enum Facing
	{
		left = 0,
		right = 1
	};
	Arrow(State* boundState, float x, float y, Facing facing);
	void handleClick(const sf::Vector2i& position) override;
private:
	sf::Sprite mArrowSprite;
	std::shared_ptr<sf::Texture> mBoundTexture;
};

Arrow::Arrow(State* boundState, float x, float y, Facing facing)
	:Clickable(boundState)
{
	mBoundTexture = TextureCache::get()->getTexture("arrow.png");
	mArrowSprite = sf::Sprite(*mBoundTexture.get());
	mBoundingRectangle = mArrowSprite.getGlobalBounds();
	mBoundingRectangle.left = x;
	mBoundingRectangle.top = y;
	mArrowSprite.setPosition(x, y);
}

Clickable::Clickable(State* boundState)
	:GameObject(boundState)
{

}

class MainMenuState : public State
{
public:
	MainMenuState();
	void update(float dt) override;
	void draw() override;
	void handleEvent(sf::Event& event) override;
private:
	int playerCount = 0;
	int numberOfCards = 2;
};

MainMenuState::MainMenuState()
{

}

class StateManager
{
public:
	StateManager();
	static StateManager* get();
	void pushState(State::StateType type);
	void popState();

	void update(float dt);
	void draw();
	void handleEvent(sf::Event& event);
private:
	template <typename T> void registerState(State::StateType type);
	std::deque<std::unique_ptr<State>> mStateStack;

	std::unordered_map<State::StateType, std::function<std::unique_ptr<State>()>> mStateFactory;
	static StateManager* mInstance;
};

void StateManager::handleEvent(sf::Event& event)
{
	for (auto end = mStateStack.rbegin(); end != mStateStack.rend(); ++end)
	{
		(*end)->handleEvent(event);
		if (!(*end)->mIsTranscendent)
		{
			break;
		}
	}
}

StateManager* StateManager::get()
{
	if (mInstance == nullptr)
	{
		mInstance = new StateManager();
	}
	return mInstance;
}

StateManager* StateManager::mInstance = nullptr;

template <typename T> void StateManager::registerState(State::StateType type)
{
	mStateFactory.emplace(type, []()->std::unique_ptr<State> { return std::make_unique(T()); });
}

void StateManager::pushState(State::StateType type)
{
	mStateStack.push_front(std::move(mStateFactory[type]()));
}

void StateManager::popState()
{
	mStateStack.pop_front();
}

void StateManager::update(float dt)
{
	for (auto end = mStateStack.rbegin(); end != mStateStack.rend(); ++end)
	{
		(*end)->update(dt);
		if (!(*end)->mIsTranscendent)
		{
			break;
		}
	}
}

void StateManager::draw()
{
	for (auto end = mStateStack.rbegin(); end != mStateStack.rend(); ++end)
	{
		(*end)->draw();
		if (!(*end)->mIsTransparent)
		{
			break;
		}
	}
}