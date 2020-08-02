#include "State.h"
#include "StateManager.h"
#include <set>

constexpr float movementFactor = 200.0f;

ResourceCache* ResourceCache::instance = nullptr;

State::State(StateManager* manager)
{
	mManager = manager;
	movingCamera = 0;
}

void State::update(float dt)
{
	float deltax = 0;
	float deltay = 0;
	if (movingCamera & MovingCamera::Left)
	{
		deltax -= dt * movementFactor;
	}
	else if (movingCamera & MovingCamera::Right)
	{
		deltax += dt * movementFactor;
	}
	if (movingCamera & MovingCamera::Up)
	{
		deltay += dt * movementFactor;
	}
	else if (movingCamera & MovingCamera::Down)
	{
		deltay -= dt * movementFactor;
	}
	mView.move(sf::Vector2f(deltax, deltay));
	mManager->mContext->mWindow->setView(mView);
	for (Entity* entity : mEntities)
	{
		entity->update(dt);
	}
}

void State::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2i position = sf::Mouse::getPosition(*mManager->mContext->mWindow);
		if (position.x > 630)
		{
			movingCamera |= MovingCamera::Right;
		}
		else if (position.x < 10)
		{
			movingCamera |= MovingCamera::Left;
		}
		else
		{
			movingCamera = MovingCamera::Nowhere;
		}
		if (position.y > 470)
		{
			movingCamera |= MovingCamera::Up;
		}
		else if (position.y < 10)
		{
			movingCamera |= MovingCamera::Down;
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed && !mLockedHandling)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*mManager->mContext->mWindow);
		sf::Vector2f center = mView.getCenter();
		sf::Vector2f offset = sf::Vector2f(center.x - 320, center.y - 240);

		for (Entity* entity : mEntities)
		{
			if (entity->getBoundingBox().contains(sf::Vector2f(mousePos.x + offset.x, mousePos.y + offset.y)))
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
			playerCount = std::max(1, --playerCount);
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

mEntities.push_back(new Sprite("btn.png", 0.2f, 0, sf::Vector2f(320, 400)));
mEntities.back()->addHandler(
	[this]() -> void {
		mManager->mContext->numberOfCards = numberOfCards;
		mManager->mContext->numberOfPlayers = playerCount;
		mManager->prepareState(StateType::Game);
	}
);

}