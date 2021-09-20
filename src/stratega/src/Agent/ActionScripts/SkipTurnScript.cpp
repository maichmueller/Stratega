#include <stratega/Agent/ActionScripts/SkipTurnScript.h>

namespace SGA {
Action SkipTurnScript::getAction(
   const GameState& /*gameState*/, std::vector< Action >& /*actionSpace*/, int playerID)
{
   return Action::createEndAction(playerID);
}

Action SkipTurnScript::getActionForUnit(
   const GameState& /*gameState*/, std::vector< Action >& /*actionSpace*/, int playerID, int /*unitID*/)
{
   return Action::createEndAction(playerID);
}
}  // namespace SGA
