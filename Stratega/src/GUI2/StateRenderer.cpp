#include <Stratega/GUI2/StateRenderer.h>
#include <Stratega/Configuration/GameConfig.h>
#include <Stratega/Representation/GameState.h>
#include <Stratega/Representation/LevelDefinition.h>

#include "TBSRendererImpl.h"

namespace SGA
{
	StateRenderer::StateRenderer():
		tileWidth(256),
		tileHeight(143),
		renderImpl(nullptr),
		atlas(),
		state(nullptr),
		config(nullptr),
		fpsCounter(0)
	{
	}

	StateRenderer::~StateRenderer() = default;

	void StateRenderer::init(const GameState& initialState, const GameConfig& config)
	{
		this->state = &initialState;
		this->config = &config;
		window.create(sf::VideoMode(1920 * 0.5, 1080 * 0.5), "Stratega GUI");
		window.setFramerateLimit(60);

		if(config.gameType == GameType::TBS)
		{
			renderImpl = std::make_unique<Detail::TBSRendererImpl>();
		}
		
		// Start clock
		deltaClock.restart();
		fpsClock.restart();

		// Initialize texture atlas
		sf::Image image;
		image.loadFromFile(config.renderConfig->tileSpritePaths.begin()->second);
		atlas.create(image.getSize(), config.renderConfig->tileSpritePaths.size() + config.renderConfig->entitySpritePaths.size(), 2);
		
		// Load sprites and place them into the texture-atlas
		for(const auto& namePathPair : config.renderConfig->tileSpritePaths)
		{
			image.loadFromFile(namePathPair.second);
			atlas.emplace(namePathPair.first, image);
		}
		for (const auto& namePathPair : config.renderConfig->entitySpritePaths)
		{
			image.loadFromFile(namePathPair.second);
			atlas.emplace(namePathPair.first, image);
		}

		auto view = window.getView();
		view.setCenter(toWorldSpace(0, 0));
		window.setView(view);
	}

	Vector2f toGridSpaceTest(sf::Vector2f worldPosition, int tileHeight, int tileWidth)
	{
		auto yRatio = worldPosition.y / static_cast<float>(tileHeight);
		auto xRatio = worldPosition.x / static_cast<float>(tileWidth);
		return Vector2f{ yRatio + xRatio, yRatio - xRatio };
	}

	std::unordered_set<sf::Keyboard::Key> pressedKeys;
	void StateRenderer::render()
	{
		auto deltaTime = deltaClock.restart().asSeconds();
		
		sf::Event event{};
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed: exit(0); break;
				case sf::Event::KeyPressed:
				{
					pressedKeys.emplace(event.key.code);
				} break;
				case sf::Event::KeyReleased:
				{
					pressedKeys.erase(event.key.code);
				}
				default: break;
			}
		}

		float moveSpeed = 5;
		auto view = window.getView();
		if (pressedKeys.find(sf::Keyboard::W) != pressedKeys.end())
		{
			view.move(0, -tileHeight * deltaTime * moveSpeed);
		}
		else if (pressedKeys.find(sf::Keyboard::S) != pressedKeys.end())
		{
			view.move(0, tileHeight * deltaTime * moveSpeed);
		}

		if (pressedKeys.find(sf::Keyboard::A) != pressedKeys.end())
		{
			view.move(-tileWidth * deltaTime * moveSpeed, 0);
		}
		else if (pressedKeys.find(sf::Keyboard::D) != pressedKeys.end())
		{
			view.move(tileWidth * deltaTime * moveSpeed, 0);
		}
		window.setView(view);

		window.clear();

		renderImpl->render(*state, *this);
		
		for(auto& entity : state->entities)
		{
			sf::CircleShape s;
			s.setPosition(toWorldSpace(entity.position));
			s.setRadius(10);
			s.setOrigin(5, 5);
			window.draw(s);
		}

		renderDebugGrid();

		fpsCounter++;
		if(fpsClock.getElapsedTime().asSeconds() >= 1)
		{
			sf::Font f;
			f.loadFromFile("C:/QMUL/StrategaPublic/GUI/Assets/arial.ttf");
			auto text = std::to_string(fpsCounter);
			sf::Text t(text, f);
			t.setPosition(view.getViewport().left, view.getViewport().top);
			window.draw(t);

			fpsCounter = 0;
			fpsClock.restart();
		}
		
		window.display();

	}
	
	void StateRenderer::update(const GameState& state)
	{
		this->state = &state;
	}

	void StateRenderer::renderDebugGrid()
	{
		// We need 2 entries for each line and we need 2 extra lines (aka 4 entries) because a rectangle always needs 2 lines more than it's width + height
		sf::VertexArray vertices(sf::Lines, 4 + 2 * (state->board.getHeight() + state->board.getWidth()));
		for (size_t y = 0; y < state->board.getHeight() + 1; y++)
		{
			vertices[y * 2] = sf::Vertex(toWorldSpace(0, y), sf::Color::White);
			vertices[y * 2 + 1] = sf::Vertex(toWorldSpace(state->board.getWidth(), y), sf::Color::White);
		}

		for (size_t x = 0; x < state->board.getWidth() + 1; x++)
		{
			auto index = (state->board.getHeight() + 1) * 2 + x * 2;
			vertices[index] = sf::Vertex(toWorldSpace(x, 0), sf::Color::White);
			vertices[index + 1] = sf::Vertex(toWorldSpace(x, state->board.getHeight()), sf::Color::White);
		}
		window.draw(vertices);
	}

	void StateRenderer::drawTile(const Tile& tile)
	{
		auto tileCenter = toWorldSpace(static_cast<float>(tile.position.x), static_cast<float>(tile.position.y)) + sf::Vector2f(0.f, static_cast<float>(tileHeight) / 2.f);
		drawSprite(tile.getType().name, tileCenter);
	}

	void StateRenderer::drawEntity(const Entity& entity, const Vector2f& gridPosition)
	{
		auto worldPos = toWorldSpace(static_cast<float>(gridPosition.x), static_cast<float>(gridPosition.y));
		drawSprite(entity.getEntityType().name, worldPos);
	}

	void StateRenderer::drawSprite(const std::string& spriteName, const sf::Vector2f& worldPosition)
	{
		// ToDo Batch draw calls
		auto textureRect = atlas.getSpriteRect(spriteName);
		auto spriteSize = atlas.getSpriteSize();
		auto start = worldPosition - sf::Vector2f(atlas.getSpriteSize().x / 2., atlas.getSpriteSize().y - tileHeight / 2);

		sf::Vertex vertices[4];
		// Upper Left Corner
		vertices[0].position = sf::Vector2f(start.x, start.y);
		vertices[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
		vertices[0].color = sf::Color::White;
		// Upper Right Corner
		vertices[1].position = sf::Vector2f(start.x + spriteSize.x, start.y);
		vertices[1].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top);
		vertices[1].color = sf::Color::White;
		// Lower Right Corner
		vertices[2].position = sf::Vector2f(start.x + spriteSize.x, start.y + spriteSize.y);
		vertices[2].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height);
		vertices[2].color = sf::Color::White;
		// Lower Left Corner
		vertices[3].position = sf::Vector2f(start.x, start.y + spriteSize.y);
		vertices[3].texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height);
		vertices[3].color = sf::Color::White;

		// Draw to window
		sf::RenderStates states;
		states.texture = &atlas.getAtlasTexture();
		window.draw(vertices, 4, sf::Quads, states);
	}

	sf::Vector2f StateRenderer::toWorldSpace(Vector2f gridPosition) const
	{
		return toWorldSpace(gridPosition.x, gridPosition.y);
	}

	sf::Vector2f StateRenderer::toWorldSpace(float gridX, float gridY) const
	{
		return sf::Vector2f{static_cast<float>(tileWidth) / 2.f * (gridX - gridY), static_cast<float>(tileHeight) / 2.f * (gridY + gridX)};
	}

	Vector2i StateRenderer::toGridSpace(sf::Vector2f worldPosition) const
	{
		auto yRatio = worldPosition.y / static_cast<float>(tileHeight);
		auto xRatio = worldPosition.x / static_cast<float>(tileWidth);
		return Vector2i{ static_cast<int>(std::floor(yRatio + xRatio)), static_cast<int>(std::floor(yRatio - xRatio)) };
	}
}