#pragma once
#include <map>
#include <stratega/Agent/ActionScripts/BaseActionScript.h>

namespace SGA {
class AttackWeakestOpponentScript: public BaseActionScript {
  private:
   double getHealth(const Entity& entity, const GameState& gamestate) const;

  public:
   AttackWeakestOpponentScript() : BaseActionScript(){};

   Action getAction(
      const GameState& gameState, std::vector< Action >& actionSpace, int playerID) override;
   Action getActionForUnit(
      const GameState& gameState,
      std::vector< Action >& actionSpace,
      int playerID,
      int unitID) override;
   [[nodiscard]] std::string toString() const override { return "AttackWeakestOpponentScript"; };
};

}  // namespace SGA
