#include "GameState.h"
#include "StateManager.h"

constexpr float movementFactor = 200.0f;
constexpr float crossFadeFactor = 200.0f;


void GameState::createPair(int face)
{
	int columns = 2;
	int rows = numberOfCards / columns;

	GameCard* first = new GameCard(sf::Vector2f(0, 0), face);
	GameCard* second = new GameCard(sf::Vector2f(0, 0), face);
	first->setScale(0.1f, 0.1f);
	second->setScale(0.1f, 0.1f);
	first->setPosition(mPositions.back());
	mPositions.pop_back();
	second->setPosition(mPositions.back());
	mPositions.pop_back();
	mEntities.push_back(first);
	mEntities.push_back(second);

	first->addUpdateFunction(
		[first](float dt) -> void
		{
			if (first->getShouldReveal())
			{
				first->setOpacity(std::max(first->getOpacity() - dt * crossFadeFactor, 0.0f));
				if (first->getOpacity() == 0.0f)
				{
					first->setRevealed(true);
					first->setShouldReveal(false);
				}
			}
			if (first->getShouldHide())
			{
				first->setOpacity(std::min(first->getOpacity() + dt * crossFadeFactor, 255.0f));
				if (first->getOpacity() == 255.0f)
				{
					first->setRevealed(false);
					first->setShouldHide(false);
				}
			}
		});

	second->addUpdateFunction(
		[second](float dt) -> void
		{
			if (second->getShouldReveal())
			{
				second->setOpacity(std::max(second->getOpacity() - dt * crossFadeFactor, 0.0f));
				if (second->getOpacity() == 0.0f)
				{
					second->setRevealed(true);
					second->setShouldReveal(false);
				}
			}
			if (second->getShouldHide())
			{
				second->setOpacity(std::min(second->getOpacity() + dt * crossFadeFactor, 255.0f));
				if (second->getOpacity() == 255.0f)
				{
					second->setRevealed(false);
					second->setShouldHide(false);
				}
			}
		});

	first->addHandler([this, first]()-> void {
		if (first->getIsLocked()) return;
		selectedCards.push_back(first);
		first->setShouldReveal(true);
		checkForMatchAndDoPostLogic();
		});
	second->addHandler([this, second]()-> void {
		if (second->getIsLocked()) return;
		second->setShouldReveal(true);
		selectedCards.push_back(second);
		checkForMatchAndDoPostLogic();
		});
}

void GameState::checkForMatchAndDoPostLogic()
{
	if (selectedCards.size() != 2)
	{
		return;
	}

	mLockedHandling = true;
	mWaitingForReveal = true;
}

void GameState::fillPositions()
{
	float upperPosition = 0;
	int lastUpperSwitch = 0;
	for (int i = 0; i < numberOfCards; ++i)
	{
		if ((i - lastUpperSwitch) * 60 > 640)
		{
			upperPosition += 80.0f;
			lastUpperSwitch = i;
		}
		mPositions.push_back(sf::Vector2f(((i - lastUpperSwitch) * 60), upperPosition));
	}

	std::random_shuffle(mPositions.begin(), mPositions.end());

	for (int i = 0; i < playerCount; ++i)
	{
		mScoreSheet.emplace(i, 0);
	}
}

GameState::GameState(StateManager* manager)
	:State(manager)
{
	std::srand(time(nullptr));
	currentPlayer = 0;
	playerCount = mManager->mContext->numberOfPlayers;
	numberOfCards = mManager->mContext->numberOfCards;

	fillPositions();

	for (int i = 0; i < numberOfCards / 2; ++i)
	{
		int randomFace = rand() % 54 + 1;
		createPair(randomFace);
	}
}

void GameState::addToScoreSheet()
{
	for (Entity* text : mScoreBoardAssets)
	{
		delete text;
	}
	mScoreBoardAssets.clear();
	typedef std::function<bool(std::pair<int, int>, std::pair<int, int>)> Comparator;
	Comparator compFunctor =
		[](std::pair<int, int> elem1, std::pair<int, int> elem2)
	{
		return elem1.second > elem2.second;
	};


	std::vector<std::pair<int, int>> setOfPlayers(
		mScoreSheet.begin(), mScoreSheet.end());
	std::sort(setOfPlayers.begin(), setOfPlayers.end(), compFunctor);

	int playersPrinted = 0;
	for (std::pair<int, int> element : setOfPlayers)
	{
		mScoreBoardAssets.push_back(new Text("arial.ttf", 0.5, 0, sf::Vector2f(150, 40 * (playersPrinted + 1)), "Player " + std::to_string(playersPrinted + 1) + ":" + std::to_string(element.first) + " SCORE: " + std::to_string(element.second)));
		++playersPrinted;
		if (playersPrinted == 4 || playersPrinted >= playerCount)
		{
			break;
		}
	}

}

void GameState::updateScoreSheetAndCheckForEndGame()
{
	addToScoreSheet();

	bool unlockedRemaining = false;
	for (Entity* card : mEntities)
	{
		if (!static_cast<GameCard*>(card)->getIsLocked())
		{
			unlockedRemaining = true;
			break;
		}
	}

	//Game ended
	if (!unlockedRemaining)
	{
		gameEndActions();
	}

}

void GameState::gameEndActions()
{
	for (Entity* entity : mEntities)
	{
		delete entity;
	}
	mEntities.clear();

	mScoreBoardAssets.push_back(new Sprite("btn.png", 0.1, 0, sf::Vector2f(300, 400)));
	mScoreBoardAssets.back()->addHandler(
		[this]()->void {
			mManager->prepareState(State::StateType::Game);
		}
	);
	mScoreBoardAssets.push_back(new Sprite("btn.png", 0.1, 0, sf::Vector2f(600, 400)));
	mScoreBoardAssets.back()->addHandler(
		[this]()->void {
			mManager->prepareState(State::StateType::MainMenu);
		}
	);
	mScoreBoardAssets.push_back(new Text("arial.ttf", 0.3, 0, sf::Vector2f(600, 400), "Main Menu"));
	mScoreBoardAssets.push_back(new Text("arial.ttf", 0.3, 0, sf::Vector2f(300, 400), "Just restart"));
}

void GameState::update(float dt)
{
	State::update(dt);
	if (mWaitingForReveal)
	{
		if (selectedCards[0]->getRevealed() && selectedCards[1]->getRevealed())
		{
			if (selectedCards[0]->getId() == selectedCards[1]->getId())
			{
				selectedCards[1]->lockIt();
				selectedCards[0]->lockIt();
				++mScoreSheet[currentPlayer];
				updateScoreSheetAndCheckForEndGame();
				selectedCards.clear();
				mLockedHandling = false;
			}
			else
			{
				mScoreBoardAssets.push_back(new Text("arial.ttf", 0.8, 0, sf::Vector2f(320, 240), "PLAYER CHANGED"));
				mWaitingForHide = true;
				selectedCards[1]->setShouldHide(true);
				selectedCards[0]->setShouldHide(true);
				++currentPlayer;
				if (currentPlayer == playerCount)
				{
					currentPlayer = 0;
				}
			}
			mWaitingForReveal = false;
		}
	}
	else if (mWaitingForHide)
	{
		static_cast<Text*>(mScoreBoardAssets.back())->moveUp(dt * movementFactor);
		if (!selectedCards[1]->getRevealed() && !selectedCards[0]->getRevealed())
		{
			delete mScoreBoardAssets.back();
			mScoreBoardAssets.pop_back();
			mWaitingForHide = false;
			selectedCards.clear();
			mLockedHandling = false;
		}
	}
}

void GameState::draw(sf::RenderTarget& target)
{
	State::draw(target);
	sf::View oldView = target.getView();
	sf::View defaultView = target.getDefaultView();
	target.setView(defaultView);
	for (Entity* text : mScoreBoardAssets)
	{
		text->draw(target);
	}
	target.setView(oldView);
}

void GameState::handleEvent(sf::Event& event)
{
	State::handleEvent(event);
	if (event.type == sf::Event::MouseButtonPressed)
	{
		for (auto& a : mScoreBoardAssets)
		{
			sf::Vector2i position = sf::Mouse::getPosition(*mManager->mContext->mWindow);
			if (a->getBoundingBox().contains(sf::Vector2f(position.x, position.y)))
			{
				a->onClick();
			}
		}
	}
}

GameState::~GameState()
{
	for (Entity* entity : mScoreBoardAssets)
	{
		delete entity;
	}
	mScoreBoardAssets.clear();
}