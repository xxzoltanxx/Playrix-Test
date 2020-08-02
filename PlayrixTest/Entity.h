#pragma once
#include <vector>
#include <functional>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

#define BACK_CARD_FILENAME "back.png"

class Entity
{
public:
	void onClick();
	void addHandler(std::function<void()> handler);
	void addUpdateFunction(std::function<void(float dt)> handler);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target) = 0;

	virtual sf::FloatRect getBoundingBox() = 0;
private:
	std::vector<std::function<void()>> mClickHandlers;
	std::vector <std::function<void(float)>> mUpdateFunctions;
};

class Sprite : public Entity
{
public:
	Sprite(const std::string& filename, float scale, float rotation, const sf::Vector2f& position);
	void draw(sf::RenderTarget& target) override;
	sf::FloatRect getBoundingBox() override;
private:
	sf::Sprite mSprite;
	std::shared_ptr<sf::Texture> mBoundTexture;
};

class Text : public Entity
{
public:
	Text(const std::string& fontFilename, float scale, float rotation, const sf::Vector2f& position, const std::string& text);
	void draw(sf::RenderTarget& target) override;
	void moveUp(float howMuch)
	{
		mText.move(sf::Vector2f(howMuch, 0));
	}
	sf::FloatRect getBoundingBox() override;

	void setText(const std::string& text);
private:
	sf::Text mText;
	std::shared_ptr<sf::Font> mBoundFont;
};

class GameCard : public Entity
{
public:
	GameCard(const sf::Vector2f& position, int id);
	void draw(sf::RenderTarget& target) override;
	sf::FloatRect getBoundingBox() override;
	void setRevealed(bool reveal) { mRevealed = reveal; }
	bool getRevealed() const { return mRevealed; }
	void setScale(float scalex, float scaley);
	sf::Vector2f getScale() const;
	void setPosition(const sf::Vector2f& newPos);
	int getId() const { return id; }
	void setShouldReveal(bool shouldReveal) { mShouldReveal = shouldReveal; }
	bool getShouldReveal() const{ return mShouldReveal;  }
	void setOpacity(float opacity);
	float getOpacity() const { return floatOpacity; }
	void setShouldHide(bool value);
	bool getShouldHide() const { return mShouldHide; }
	void lockIt() { mLocked = true; }
	bool getIsLocked() const { return mLocked; }

private:
	float floatOpacity = 255.0f;
	sf::Sprite mFaceSprite;
	sf::Sprite mBackSprite;
	std::shared_ptr<sf::Texture> mFaceTexture;
	std::shared_ptr<sf::Texture> mBackTexture;

	unsigned int id = 0;
	bool mRevealed = false;
	bool mShouldReveal = false;
	bool mShouldHide = false;

	bool mLocked = false;
};

