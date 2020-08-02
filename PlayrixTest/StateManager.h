#pragma once
#include "State.h"

struct Context
{
public:
	sf::RenderWindow* mWindow;
	int numberOfPlayers = 0;
	int numberOfCards = 0;
};

class StateManager
{
public:
	explicit StateManager(Context* context);
	void pushState(State::StateType type);
	void prepareState(State::StateType type);
	void popState();

	void update(float dt);
	void draw(sf::RenderTarget& target);
	void handleEvent(sf::Event& event);

	Context* mContext;
	State::StateType toPush = State::StateType::None;
private:
	template <typename T> void registerState(State::StateType type);
	std::deque<std::unique_ptr<State>> mStateStack;

	std::unordered_map<State::StateType, std::function<std::unique_ptr<State>()>> mStateFactory;
};