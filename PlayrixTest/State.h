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

class State;

class Entity
{
public:
	Entity(const std::string& filename, float scale, float rotation, const sf::Vector2f& position);
	void onClick();
	void addHandler(std::function<void()> handler);
	void addUpdateFunction(std::function<void(float dt)> handler);
	void update(float dt);
	void draw(sf::RenderTarget& target);
private:
	std::vector<std::function<void()>> mClickHandlers;
	std::vector <std::function<void(float)>> mUpdateFunctions;
	sf::Sprite mSprite;
	std::shared_ptr<sf::Texture> mBoundTexture;
};

void Entity::draw(sf::RenderTarget& target)
{
	target.draw(mSprite);
}

void Entity::addUpdateFunction(std::function<void(float dt)> handler)
{
	mUpdateFunctions.push_back(handler);
}

void Entity::update(float dt)
{
	for (auto& updateFunction : mUpdateFunctions)
	{
		updateFunction(dt);
	}
}

void Entity::addHandler(std::function<void()> handler)
{
	mClickHandlers.push_back(handler);
}

Entity::Entity(const std::string& filename, float scale, float rotation, const sf::Vector2f& position)
{
	mBoundTexture = TextureCache::get()->getTexture(filename);
	mSprite = sf::Sprite(*mBoundTexture);
	mSprite.setOrigin(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds.height / 2);
	mSprite.setScale(sf::Vector2f(scale, scale));
	mSprite.setRotation(rotation);
	mSprite.setPosition(position);
}

void Entity::onClick()
{
	for (auto& handler : mClickHandlers)
	{
		handler();
	}
}

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
protected:
	bool mIsTranscendent = false;
	bool mIsTransparent = false;
	std::vector<Entity> mEntities;
};

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
	mEntities.push_back(Entity("arrow.png", 1, 0, sf::Vector2f(0, 0)));
	mEntities.back().addHandler(
		[this]() -> void {
			++playerCount;
		}
	);
	mEntities.push_back(Entity("arrow.png", 1, 180, sf::Vector2f(200, 0)));
	mEntities.back().addHandler(
		[this]() -> void {
			playerCount = std::max(0, --playerCount);
		}
	);

}

struct Context
{
public:
	sf::RenderWindow* mWindow;
};

class StateManager
{
public:
	explicit StateManager(const Context& context);
	void pushState(State::StateType type);
	void popState();

	void update(float dt);
	void draw();
	void handleEvent(sf::Event& event);
private:
	template <typename T> void registerState(State::StateType type);
	std::deque<std::unique_ptr<State>> mStateStack;

	std::unordered_map<State::StateType, std::function<std::unique_ptr<State>()>> mStateFactory;
	Context mContext;
};

StateManager::StateManager(const Context& context)
{
	mContext = context;
}

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