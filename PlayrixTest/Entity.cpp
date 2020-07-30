#include "Entity.h"
#include "TextureCache.h"

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

void Entity::onClick()
{
	for (auto& handler : mClickHandlers)
	{
		handler();
	}
}

Sprite::Sprite(const std::string& filename, float scale, float rotation, const sf::Vector2f& position)
{
	mBoundTexture = ResourceCache::get()->getTexture(filename);
	mSprite = sf::Sprite(*mBoundTexture);
	mSprite.setOrigin(mSprite.getLocalBounds().width / 2.0f, mSprite.getLocalBounds().height / 2.0f);
	mSprite.setScale(sf::Vector2f(scale, scale));
	mSprite.setRotation(rotation);
	mSprite.setPosition(position);
}

void Sprite::draw(sf::RenderTarget& target)
{
	target.draw(mSprite);
}

sf::FloatRect Sprite::getBoundingBox()
{
	return mSprite.getGlobalBounds();
}

Text::Text(const std::string& fontFilename, float scale, float rotation, const sf::Vector2f& position, const std::string& text)
{
	mBoundFont = ResourceCache::get()->getFont(fontFilename);
	mText = sf::Text(text, *mBoundFont);
	mText.setOrigin(mText.getLocalBounds().width / 2.0f, mText.getLocalBounds().height / 2.0f);
	mText.setScale(sf::Vector2f(scale, scale));
	mText.setRotation(rotation);
	mText.setPosition(position);
}

void Text::draw(sf::RenderTarget& target)
{
	target.draw(mText);
}

sf::FloatRect Text::getBoundingBox()
{
	return mText.getGlobalBounds();
}

void Text::setText(const std::string& text)
{
	mText.setString(text);
	mText.setOrigin(mText.getLocalBounds().width / 2, mText.getLocalBounds().height / 2);
}