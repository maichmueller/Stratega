#pragma once
#include <stratega/Agent/ActionScripts/BaseActionScript.h>

namespace SGA {
class SkipTurnScript: public BaseActionScript {
  public:
   using BaseActionScript::BaseActionScript;

   Action getAction(
      const GameState& gameState, std::vector< Action >& actionSpace, int playerID) override;
   Action getActionForUnit(
      const GameState& gameState,
      std::vector< Action >& actionSpace,
      int playerID,
      int unitID) override;
   [[nodiscard]] std::string toString() const override { return "SkipTurnScript"; };
};

}  // namespace SGA
