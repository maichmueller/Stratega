#pragma once
#include <SFML/Graphics.hpp>
#include <Stratega/Representation/GameState.h>
#include <Stratega/GUI2/TextureAtlas.h>

namespace SGA
{
	struct GameConfig;
	struct GameState;

	class StateRenderer
	{
	public:
		StateRenderer();
		
		void init(const GameState& initialState, const GameConfig& config);
		void render();
		void update(const GameState& state);

	private:
		void renderDebugGrid();
		
		sf::Vector2f toWorldSpace(Vector2f gridPosition) const;
		sf::Vector2f toWorldSpace(float gridX, float gridY) const;
		Vector2i toGridSpace(sf::Vector2f worldPosition) const;
		
		int tileWidth;
		int tileHeight;

		TextureAtlas atlas;
		sf::RenderWindow window;
		sf::Clock deltaClock;
		const GameState* state;
		const GameConfig* config;
	};
}