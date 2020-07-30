#pragma once
#include <vector>
#include <functional>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

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
	sf::FloatRect getBoundingBox() override;

	void setText(const std::string& text);
private:
	sf::Text mText;
	std::shared_ptr<sf::Font> mBoundFont;
};