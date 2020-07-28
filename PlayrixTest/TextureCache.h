#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

class TextureCache
{
public:
	static TextureCache* get()
	{
		if (instance == nullptr)
		{
			instance = new TextureCache();
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
private:
	static TextureCache* instance;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mTextureCache;
};

TextureCache* TextureCache::instance = nullptr;