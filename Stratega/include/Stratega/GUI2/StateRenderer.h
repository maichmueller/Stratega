#pragma once
#include <SFML/Graphics.hpp>
#include <Stratega/Representation/GameState.h>
#include <Stratega/GUI2/TextureAtlas.h>

namespace SGA::Detail
{
	class RendererImpl;
}

namespace SGA
{
	struct GameConfig;
	struct GameState;

	class StateRenderer
	{
	public:
		StateRenderer();
		~StateRenderer();
		
		void init(const GameState& initialState, const GameConfig& config);
		void render();
		void update(const GameState& state);

		void drawTile(const Tile& tile);
		void drawEntity(const Entity& entity, const SGA::Vector2f& gridPosition);
		void drawSprite(const std::string& spriteName, const sf::Vector2f& worldPosition);

	private:
		void renderDebugGrid();
		
		sf::Vector2f toWorldSpace(Vector2f gridPosition) const;
		sf::Vector2f toWorldSpace(float gridX, float gridY) const;
		Vector2i toGridSpace(sf::Vector2f worldPosition) const;
		
		int tileWidth;
		int tileHeight;

		std::unique_ptr<Detail::RendererImpl> renderImpl;
		TextureAtlas atlas;
		sf::RenderWindow window;
		sf::Clock deltaClock;
		const GameState* state;
		const GameConfig* config;

		sf::Clock fpsClock;
		int fpsCounter;
	};
}