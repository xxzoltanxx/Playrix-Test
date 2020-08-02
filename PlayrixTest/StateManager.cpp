#include "StateManager.h"
#include "State.h"
#include "GameState.h"

StateManager::StateManager(Context* context)
{
	mContext = context;
	registerState<MainMenuState>(State::StateType::MainMenu);
	registerState<GameState>(State::StateType::Game);
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
	mStateFactory.emplace(type, [this]()->std::unique_ptr<State> { return std::make_unique<T>(this); });
}

void StateManager::pushState(State::StateType type)
{
	mStateStack.push_front(std::move(mStateFactory[type]()));
	mContext->mWindow->setView(mStateStack.front()->mView);
}

void StateManager::prepareState(State::StateType type)
{
	toPush = type;
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

void StateManager::draw(sf::RenderTarget& target)
{
	for (auto end = mStateStack.rbegin(); end != mStateStack.rend(); ++end)
	{
		(*end)->draw(target);
		if (!(*end)->mIsTransparent)
		{
			break;
		}
	}
}