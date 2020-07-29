#include "State.h"
#include "StateManager.h"

TextureCache* TextureCache::instance = nullptr;

State::State(StateManager* manager)
{
	mManager = manager;
}

void State::update(float dt)
{
	for (Entity& entity : mEntities)
	{
		entity.update(dt);
	}
}

void State::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*mManager->mContext->mWindow);
		for (Entity& entity : mEntities)
		{
			if (entity.getBoundingBox().contains(sf::Vector2f(mousePos.x, mousePos.y)))
			{
				entity.onClick();
			}
		}
	}
}

void State::draw(sf::RenderTarget& target)
{
	for (Entity& entity : mEntities)
	{
		entity.draw(target);
	}
}

MainMenuState::MainMenuState(StateManager* manager)
: State(manager)
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