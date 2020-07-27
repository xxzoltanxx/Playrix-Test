#pragma once
#include <deque>
#include <memory>
#include <unordered_map>
#include <functional>
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

private:
	bool mIsTranscendent = false;
	bool mIsTransparent = false;
};

class StateManager
{
public:
	StateManager();
	void pushState(State::StateType type);
	void popState();

	void update(float dt);
	void draw();
private:
	template <typename T> void registerState(State::StateType type);
	std::deque<std::unique_ptr<State>> mStateStack;

	std::unordered_map<State::StateType, std::function<std::unique_ptr<State>()>> mStateFactory;
};

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