#pragma once
#include <stratega/Agent/ActionScripts/BaseActionScript.h>
#include <stratega/Agent/ActionScripts/RandomActionScript.h>
#include <stratega/Agent/Agent.h>
#include <stratega/Agent/Heuristic/MinimizeDistanceHeuristic.h>
#include <stratega/ForwardModel/ForwardModel.h>

namespace SGA {
class DFSAgent: public Agent {
  public:
   MinimizeDistanceHeuristic _stateHeuristic;
   int maxDepth = 3;
   int forwardModelCalls = 2000;
   int remainingForwardModelCalls = forwardModelCalls;
   std::unique_ptr< BaseActionScript > opponentModel = std::make_unique<
      RandomActionScript >();  // the portfolio the opponent is simulated with, if set to nullptr
                               // the opponent's turn will be skipped

   AgentParameters agentParams;

   DFSAgent(const std::string& name) : Agent{name}, _stateHeuristic() {}

   ActionAssignment computeAction(
      GameState state, const ForwardModel& forwardModel, Timer timer) override;
   void init(GameState initialState, const ForwardModel& forwardModel, Timer timer) override;

   double evaluateRollout(
      const ForwardModel& forwardModel, GameState& gameState, int depth, int playerID);
   void applyActionToGameState(
      const ForwardModel& forwardModel, GameState& gameState, const Action& action, int playerID);
};
}  // namespace SGA
