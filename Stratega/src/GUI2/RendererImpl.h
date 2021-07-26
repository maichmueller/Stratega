#pragma once
#include <Stratega/GUI2/StateRenderer.h>

namespace SGA::Detail
{
	class RendererImpl
	{
	public:
		virtual ~RendererImpl() = default;
		RendererImpl() = default;
		RendererImpl(const RendererImpl& other) = delete;
		RendererImpl(RendererImpl&& other) noexcept = delete;
		RendererImpl& operator=(const RendererImpl& other) = delete;
		RendererImpl& operator=(RendererImpl&& other) noexcept = delete;
		
		virtual void render(const GameState& state, StateRenderer& renderer) = 0;
	};
}
