#include "Entity.h"
#include "TextureCache.h"

void Entity::draw(sf::RenderTarget& target)
{
	target.draw(mSprite);
}

void Entity::addUpdateFunction(std::function<void(float dt)> handler)
{
	mUpdateFunctions.push_back(handler);
}

void Entity::update(float dt)
{
	for (auto& updateFunction : mUpdateFunctions)
	{
		updateFunction(dt);
	}
}

void Entity::addHandler(std::function<void()> handler)
{
	mClickHandlers.push_back(handler);
}

Entity::Entity(const std::string& filename, float scale, float rotation, const sf::Vector2f& position)
{
	mBoundTexture = TextureCache::get()->getTexture(filename);
	mSprite = sf::Sprite(*mBoundTexture);
	mSprite.setOrigin(mSprite.getLocalBounds().width / 2.0f, mSprite.getLocalBounds().height / 2.0f);
	mSprite.setScale(sf::Vector2f(scale, scale));
	mSprite.setRotation(rotation);
	mSprite.setPosition(position);
}

void Entity::onClick()
{
	for (auto& handler : mClickHandlers)
	{
		handler();
	}
}