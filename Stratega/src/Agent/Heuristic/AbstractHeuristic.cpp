#include <map>
#include <set>
#include <Stratega/Agent/Heuristic/AbstractHeuristic.h>

namespace SGA
{

	double AbstractHeuristic::evaluateGameState(const TBSForwardModel& forwardModel, GameState& gameState, int playerID)
	{
		double score = 0.0;

		if (gameState.isGameOver)
		{
			if (gameState.winnerPlayerID == playerID)
				score -= 1000;	// since this score should be minimized we need to deduct points for winning
			else
				score += 1000;
		}

		for (auto entry : attributeWeights)
		{
			const std::string parameterName = entry.first;
			const int parameterWeight = entry.second;
			
			if (parameterWeight != 0) {

				std::vector<double> parameterValues;
				double sum = 0;

				for (auto entity : gameState.entities) {
					const auto& entityType = gameState.getEntityType(entity.typeID);
					for (const auto& parameter : entityType.parameters)
					{
						if (parameter.second.name == parameterName)
						{
							if (entity.ownerID == playerID) {
								const double val = (entity.parameters[parameter.second.index] / maxValue[parameterName]) * parameterWeight;
								parameterValues.push_back(val);
								sum += val;
							}
							else {
								const double val = (entity.parameters[parameter.second.index] / maxValue[parameterName]) * -parameterWeight;
								parameterValues.push_back(val);
								sum += val;
							}
							break;
						}
					}
				}
				score += sum;
			}
		}


		return score;
	}
}