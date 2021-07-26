#pragma once
#include "RendererImpl.h"

namespace SGA::Detail
{
	class TBSRendererImpl : public RendererImpl
	{
	public:
		void render(const GameState& state, StateRenderer& renderer) override;
	};
}
