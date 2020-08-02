#pragma once
#include "State.h"
class GameState : public State
{
public:
	virtual ~GameState();
	GameState(StateManager* manager);
	void checkForMatchAndDoPostLogic();
	void update(float dt) override;
	void handleEvent(sf::Event& event) override;
	virtual void draw(sf::RenderTarget& target) override;
private:
	void addToScoreSheet();
	void gameEndActions();
	void updateScoreSheetAndCheckForEndGame();
	void createPair(int face);
	void fillPositions();
	int currentPlayer = 0;
	int playerCount = 0;
	int numberOfCards = 0;
	std::map<int, int> mScoreSheet;
	std::vector<GameCard*> selectedCards;
	std::vector<sf::Vector2f> mPositions;
	bool mWaitingForReveal = false;
	bool mWaitingForHide = false;

	std::vector<Entity*> mScoreBoardAssets;
};