#pragma once
#include <random>
#include <stratega/Agent/ActionScripts/BaseActionScript.h>

namespace SGA {
class RandomActionScript: public BaseActionScript {
  public:
   using BaseActionScript::BaseActionScript;

   Action getAction(
      const GameState& gameState, std::vector< Action >& actionSpace, int playerID) override;
   Action getActionForUnit(
      const GameState& gameState,
      std::vector< Action >& actionSpace,
      int playerID,
      int unitID) override;
   [[nodiscard]] std::string toString() const override { return "RandomActionScript"; };

  private:
   std::mt19937_64 m_rng;
};
}  // namespace SGA
