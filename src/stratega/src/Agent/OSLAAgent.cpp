#include <stratega/Agent/Heuristic/MinimizeDistanceHeuristic.h>
#include <stratega/Agent/OSLAAgent.h>

namespace SGA {
ActionAssignment OSLAAgent::computeAction(
   GameState state, const ForwardModel& forwardModel, Timer /*timer*/)
{
   std::vector< Action > actionSpace = forwardModel.generateActions(state, getPlayerID());
   MinimizeDistanceHeuristic heuristic;
   double bestHeuristicValue = -std::numeric_limits< double >::max();

   int bestActionIndex = 0;
   for(int i = 0; i < actionSpace.size(); i++) {
      GameState gsCopy(state);
      forwardModel.advanceGameState(gsCopy, actionSpace.at(i));
      const double value = heuristic.evaluateGameState(forwardModel, gsCopy, getPlayerID());
      if(value > bestHeuristicValue) {
         bestHeuristicValue = value;
         bestActionIndex = i;
      }
   }

   return ActionAssignment::fromSingleAction(actionSpace.at(bestActionIndex));
}
}  // namespace SGA
