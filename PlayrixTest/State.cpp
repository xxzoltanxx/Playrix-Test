#include "State.h"
#include "StateManager.h"

ResourceCache* ResourceCache::instance = nullptr;

State::State(StateManager* manager)
{
	mManager = manager;
}

void State::update(float dt)
{
	for (Entity* entity : mEntities)
	{
		entity->update(dt);
	}
}

void State::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*mManager->mContext->mWindow);
		for (Entity* entity : mEntities)
		{
			if (entity->getBoundingBox().contains(sf::Vector2f(mousePos.x, mousePos.y)))
			{
				entity->onClick();
			}
		}
	}
}

void State::draw(sf::RenderTarget& target)
{
	for (Entity* entity : mEntities)
	{
		entity->draw(target);
	}
}

MainMenuState::MainMenuState(StateManager* manager)
: State(manager)
{
	Text* numberOfPlayers = new Text("arial.ttf", 1, 0, sf::Vector2f(150, 40), "Number of players:");
	mEntities.push_back(numberOfPlayers);
	mEntities.push_back(new Sprite("arrow.png", 0.5f, 180, sf::Vector2f(100, 100)));
	mEntities.back()->addHandler(
		[this]() -> void {
			playerCount = std::max(0, --playerCount);
		}
	);
	mEntities.push_back(new Sprite("arrow.png", 0.5f, 0, sf::Vector2f(250, 100)));
	mEntities.back()->addHandler(
		[this]() -> void {
			++playerCount;
		}
	);
	Text* playerCountText = new Text("arial.ttf", 1, 0, sf::Vector2f(170, 100), std::to_string(playerCount));
	mEntities.push_back(playerCountText);
	playerCountText->addUpdateFunction((
		[this, playerCountText](float dt) -> void {
			playerCountText->setText(std::to_string(playerCount));
		}
		)
	);


	Text* numberOfCardsDescription = new Text("arial.ttf", 1, 0, sf::Vector2f(450, 40), "Number of cards:");
	mEntities.push_back(numberOfCardsDescription);
	mEntities.push_back(new Sprite("arrow.png", 0.5f, 180, sf::Vector2f(350, 100)));
	mEntities.back()->addHandler(
		[this]() -> void {
			numberOfCards = std::max(2, numberOfCards - 2);
		}
	);
	mEntities.push_back(new Sprite("arrow.png", 0.5f, 0, sf::Vector2f(500, 100)));
	mEntities.back()->addHandler(
		[this]() -> void {
			numberOfCards += 2;
		}
	);
	Text* numberOfCardsText = new Text("arial.ttf", 1, 0, sf::Vector2f(420, 100), std::to_string(numberOfCards));
	mEntities.push_back(numberOfCardsText);
	numberOfCardsText->addUpdateFunction((
		[this, numberOfCardsText](float dt) -> void {
			numberOfCardsText->setText(std::to_string(numberOfCards));
		}
		)
	);

}