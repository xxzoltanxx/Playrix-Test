#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

class ResourceCache
{
public:
	static ResourceCache* get()
	{
		if (instance == nullptr)
		{
			instance = new ResourceCache();
		}
		return instance;
	}
	std::shared_ptr<sf::Texture> getTexture(const std::string& filename)
	{
		if (mTextureCache.find(filename) == mTextureCache.end())
		{
			sf::Texture* texture = new sf::Texture();
			if (!texture->loadFromFile(filename))
			{
				throw std::runtime_error("failed to load " + filename);
			}
			mTextureCache.emplace(filename, std::shared_ptr<sf::Texture>(texture));
		}
		return mTextureCache[filename];
	}
	std::shared_ptr<sf::Font> getFont(const std::string& filename)
	{
		if (mFontCache.find(filename) == mFontCache.end())
		{
			sf::Font* font = new sf::Font();
			if (!font->loadFromFile(filename))
			{
				throw std::runtime_error("failed to load " + filename);
			}
			mFontCache.emplace(filename, std::shared_ptr<sf::Font>(font));
		}
		return mFontCache[filename];
	}
private:
	static ResourceCache* instance;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mTextureCache;
	std::unordered_map<std::string, std::shared_ptr<sf::Font>> mFontCache;
};