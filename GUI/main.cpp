#include <Stratega/Configuration/GameConfigParser.h>
#include <Stratega/Game/GameRunner.h>

#include "Stratega/GUI2/StateRenderer.h"

int main()
{
	std::mt19937 rngEngine(0);
	std::string configPath("../../../gameConfigs/TBS/KillTheKing.yaml");
	auto gameConfig = SGA::loadConfigFromYAML(configPath);
	
	SGA::Vector2f resolution;
	if (gameConfig->gameType == SGA::GameType::TBS)
	{
		resolution = SGA::Vector2f(800, 600);
	}
	else
	{
		resolution = SGA::Vector2f(1200, 800);
	}

	auto state = gameConfig->generateGameState();
	SGA::StateRenderer renderer;
	renderer.init(*state, *gameConfig);
	while(true)
	{
		renderer.render();
	}
}
