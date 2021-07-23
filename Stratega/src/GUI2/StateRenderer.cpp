#include <Stratega/GUI2/StateRenderer.h>
#include <Stratega/Configuration/GameConfig.h>
#include <Stratega/Representation/GameState.h>
#include <Stratega/Representation/LevelDefinition.h>

namespace SGA
{
	StateRenderer::StateRenderer():
		tileWidth(256),
		tileHeight(143),
		atlas(),
		state(nullptr),
		config(nullptr)
	{
	}
	
	void StateRenderer::init(const GameState& initialState, const GameConfig& config)
	{
		this->state = &initialState;
		this->config = &config;
		window.create(sf::VideoMode(1920 * 0.5, 1080 * 0.5), "Stratega GUI");
		window.setFramerateLimit(60);

		// Start clock
		deltaClock.restart();

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

		// Render grid
		std::vector<const Entity*> entities(state->entities.size());
		std::ranges::transform(state->entities, entities.begin(), [](const Entity& e) {return &e; });
		std::ranges::sort(entities, [](const Entity* e1, const Entity* e2)
		{
			return std::tie(e1->position.x, e1->position.y) < std::tie(e2->position.x, e2->position.y);
		});

		int entityIndex = 0;
		sf::VertexArray vertices(sf::Quads);
		for(size_t x = 0; x < state->board.getWidth(); ++x)
		{
			for (size_t y = 0; y < state->board.getHeight(); ++y)
			{
				const auto& tile = state->board.get(x, y);
				while(entityIndex < entities.size() && std::tie(entities[entityIndex]->position.x, entities[entityIndex]->position.y) < std::tie(tile.position.x, tile.position.y))
				{
					const auto* entity = entities[entityIndex];
					auto entityCenter = toWorldSpace(entity->position.x, entity->position.y) + sf::Vector2f(0, tileHeight / 2.);
					auto start = entityCenter - sf::Vector2f(atlas.getSpriteSize().x / 2., atlas.getSpriteSize().y - tileHeight / 2);
					auto rect = atlas.getSpriteRect(entity->getEntityType().name);
					auto tileSize = atlas.getSpriteSize();

					sf::Vertex v;
					v.position = sf::Vector2f(start.x, start.y);
					v.texCoords = sf::Vector2f(rect.left, rect.top);
					v.color = sf::Color::White;
					vertices.append(v);

					v.position = sf::Vector2f(start.x + tileSize.x, start.y);
					v.texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
					vertices.append(v);

					v.position = sf::Vector2f(start.x + tileSize.x, start.y + tileSize.y);
					v.texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
					vertices.append(v);

					v.position = sf::Vector2f(start.x, start.y + tileSize.y);
					v.texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
					vertices.append(v);
					
					entityIndex++;
				}
				
				
				auto tileCenter = toWorldSpace(x, y) + sf::Vector2f(0, tileHeight / 2.);
				auto start = tileCenter - sf::Vector2f(atlas.getSpriteSize().x / 2., atlas.getSpriteSize().y - tileHeight / 2);
				auto rect = atlas.getSpriteRect(tile.name());
				auto tileSize = atlas.getSpriteSize();
				
				sf::Vertex v;
				v.position = sf::Vector2f(start.x, start.y);
				v.texCoords = sf::Vector2f(rect.left, rect.top);
				v.color = sf::Color::White;
				vertices.append(v);

				v.position = sf::Vector2f(start.x + tileSize.x, start.y);
				v.texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
				vertices.append(v);

				v.position = sf::Vector2f(start.x + tileSize.x, start.y + tileSize.y);
				v.texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
				vertices.append(v);

				v.position = sf::Vector2f(start.x, start.y + tileSize.y);
				v.texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
				vertices.append(v);
			}
		}
		window.draw(vertices, &atlas.getAtlasTexture());

		renderDebugGrid();
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