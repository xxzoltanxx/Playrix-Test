#include "Game.h"

Game::Game()
{
	mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 480), "Playrix Test");
	mContext = Context();
	mContext.mWindow = mWindow.get();
	mManager = std::make_unique<StateManager>(&mContext);
}
void Game::run()
{
	sf::Clock frameTimer;
	sf::Time timeElapsed = sf::Time::Zero;
	sf::Time frameTime = sf::seconds(1.0f / 60.0f);

	mManager->pushState(State::MainMenu);
	while (mWindow->isOpen())
	{
		timeElapsed += frameTimer.restart();
		mManager->draw(*mWindow);
		mWindow->display();
		sf::Event polledEvent;
		while (mWindow->pollEvent(polledEvent))
		{
			mManager->handleEvent(polledEvent);
		}
		while (timeElapsed > frameTime)
		{
			mManager->update(frameTime.asSeconds());
			timeElapsed -= frameTime;
		}
		mWindow->clear(sf::Color(0, 0, 0, 255));
	}
}