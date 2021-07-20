#pragma once
#include <cassert>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <SFML/Graphics.hpp>

namespace SGA
{
	class TextureAtlas
	{
		sf::Vector2u spriteSize;
		sf::Vector2u currentPosition;
		sf::Vector2u spriteCount;
		sf::Texture atlas;
		int pixelGap;

		std::unordered_map<std::string, sf::Rect<float>> rectLookup;
		
	public:
		TextureAtlas()
			: spriteSize(), spriteCount(), atlas(), pixelGap()
		{
		}

		void create(sf::Vector2u spriteSize, int numSprites, int pixelGap = 0)
		{
			this->spriteSize = spriteSize;
			
			// Compute optimal number of sprites in the x-axis and y-axis to form a perfect square
			auto ratio = spriteSize.y / static_cast<double>(spriteSize.x);
			auto xCountF = std::sqrt(numSprites / ratio);
			auto yCountF = xCountF * ratio;

			// Since we don't want to split up a texture, turn the square into a slightly inaccurate rectangle
			spriteCount.x = static_cast<unsigned int>(std::ceil(xCountF));
			spriteCount.y = static_cast<unsigned int>(std::ceil(yCountF));
			assert(spriteCount.x * spriteCount.y >= numSprites);

			// Create atlas
			auto width = spriteSize.x * spriteCount.x + (spriteSize.x - 1) * pixelGap;
			auto height = spriteSize.y * spriteCount.y + (spriteSize.y - 1) * pixelGap;
			atlas.create(width, height);
		}

		void emplace(const std::string& key, const sf::Image& image)
		{
			if(currentPosition == spriteCount)
			{
				throw std::out_of_range("Exceeded the number of sprites that can be inserted into the texture atlas");
			}
			if(image.getSize() != spriteSize)
			{
				auto expectedDim = "(" + std::to_string(spriteSize.x) + "," + std::to_string(spriteSize.y) + ")";
				auto givenDim = "(" + std::to_string(image.getSize().x) + "," + std::to_string(image.getSize().y) + ")";
				throw std::invalid_argument("Tried adding a sprite to texture-atlas with dimension " + givenDim + " expected " + expectedDim);
			}
			
			// Insert the image into the atlas
			auto startX = currentPosition.x * (spriteSize.x + pixelGap);
			auto startY = currentPosition.y * (spriteSize.y + pixelGap);
			atlas.update(image, startX, startY);
			rectLookup.emplace(key, sf::Rect<float>(static_cast<float>(startX), static_cast<float>(startY), static_cast<float>(spriteSize.x), static_cast<float>(spriteSize.y)));

			// Update position
			currentPosition.x++;
			if (currentPosition.x == spriteCount.x)
			{
				currentPosition.x = 0;
				currentPosition.y++;
			}
		}

		const sf::Texture& getAtlasTexture() const { return atlas; }
		sf::Rect<float> getSpriteRect(const std::string& spritePath) const { return rectLookup.at(spritePath); }
		const sf::Vector2u& getSpriteSize() const { return spriteSize; }
	};
}