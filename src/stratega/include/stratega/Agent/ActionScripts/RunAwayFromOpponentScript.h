#pragma once
#include <map>
#include <set>
#include <stratega/Agent/ActionScripts/BaseActionScript.h>

namespace SGA {
class RunAwayFromOpponentScript: public BaseActionScript {
  public:
   using BaseActionScript::BaseActionScript;

   Action getAction(
      const GameState& gameState, std::vector< Action >& actionSpace, int playerID) override;
   Action getActionForUnit(
      const GameState& gameState,
      std::vector< Action >& actionSpace,
      int playerID,
      int unitID) override;
   [[nodiscard]] std::string toString() const override { return "RunAwayFromOpponentScript"; };

  private:
   static double minimalDistanceToOpponents(
      const Vector2f position,
      std::map< int, Vector2f >& unitPositions,
      const std::set< int >& opponentUnits);
};

}  // namespace SGA
