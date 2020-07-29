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
	Entity(const std::string& filename, float scale, float rotation, const sf::Vector2f& position);
	void onClick();
	void addHandler(std::function<void()> handler);
	void addUpdateFunction(std::function<void(float dt)> handler);
	void update(float dt);
	void draw(sf::RenderTarget& target);

	sf::FloatRect getBoundingBox() { return mSprite.getGlobalBounds(); }
private:
	std::vector<std::function<void()>> mClickHandlers;
	std::vector <std::function<void(float)>> mUpdateFunctions;
	sf::Sprite mSprite;
	std::shared_ptr<sf::Texture> mBoundTexture;
};