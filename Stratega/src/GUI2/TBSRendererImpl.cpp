#include "TBSRendererImpl.h"

namespace SGA::Detail
{
	void TBSRendererImpl::render(const GameState& state, StateRenderer& renderer)
	{
		// Sort entities by x-axis and then y-axis
		std::vector<const Entity*> entities;
		entities.resize(state.entities.size());
		std::ranges::transform(state.entities, entities.begin(), [](const Entity& e) {return &e; });
		std::ranges::sort(entities, [](const Entity* e1, const Entity* e2)
		{
			return std::tie(e1->position.x, e1->position.y) < std::tie(e2->position.x, e2->position.y);
		});

		// Render the board
		size_t entityIndex = 0;
		for (size_t x = 0; x < state.board.getWidth(); ++x)
		{
			for (size_t y = 0; y < state.board.getHeight(); ++y)
			{
				renderer.drawTile(state.board.get(x, y));

				// Draw all entities standing on this tile, since we sorted the array we don't have to search for all of them
				while(entityIndex < entities.size() && entities[entityIndex]->getBoardPosition() == Vector2i{static_cast<int>(x), static_cast<int>(y)})
				{
					const auto* e = entities[entityIndex];
					entityIndex++;

					// Always draw the entity in the center of the tile
					auto pos = Vector2f{ e->getBoardPosition().x + 0.5, e->getBoardPosition().y + 0.5 };
					renderer.drawEntity(*e, pos);
				}
			}
		}
	}
}