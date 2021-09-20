#include <stratega/Agent/ActionScripts/RandomActionScript.h>
#include <stratega/ForwardModel/ForwardModel.h>

namespace SGA {
Action RandomActionScript::getAction(
   const GameState& /*gameState*/, std::vector< Action >& actionSpace, int playerID)
{
   if(actionSpace.size() > 0) {
      return actionSpace
         [std::uniform_int_distribution<size_t>(0, actionSpace.size())(m_rng)];
   } else
      return Action::createEndAction(playerID);
}

Action RandomActionScript::getActionForUnit(
   const GameState& /*gameState*/,
   std::vector< Action >& actionSpace,
   int playerID,
   int /*unitID*/)
{
   // std::vector<Action> suitableActions;
   /*for (const auto& action : actionSpace)
   {
           auto& actionType = gameState.getActionType(action.actionTypeID);
           if (actionType.sourceType == ActionSourceType::Unit)
           {
                   auto sourceEntity = targetToEntity(gameState, action.targets[0]);
                   if(sourceEntity.id == unitID)
                           suitableActions.push_back(action);
           }
   }

   if (!suitableActions.empty())
           return suitableActions.at(rand() % suitableActions.size());*/

   if(actionSpace.size() > 0)
      return actionSpace[std::uniform_int_distribution<size_t>(0, actionSpace.size())(m_rng)];
   else
      return Action::createEndAction(playerID);
}
}  // namespace SGA
