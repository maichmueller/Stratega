#pragma once
#include <list>
#include <memory>
#include <stratega/Agent/Agent.h>
#include <stratega/Agent/AgentParameters.h>
#include <stratega/Agent/TreeSearchAgents/TreeNode.h>

namespace SGA {
struct BFSParameters: public AgentParameters {
   bool continuePreviousSearch = true;
};

class BFSAgent: public Agent {
   using Agent::Agent;

   std::unique_ptr< TreeNode > rootNode = nullptr;
   std::list< TreeNode* > openNodes = std::list< TreeNode* >();
   std::list< TreeNode* > knownLeaves = std::list< TreeNode* >();
   int previousActionIndex = -1;

   BFSParameters parameters_;

  public:
   ActionAssignment computeAction(
      GameState state, const ForwardModel& forwardModel, Timer timer) override;

  private:
   void search(ForwardModel& forwardModel, std::list< TreeNode* >& nodes);
   int getBestActionIdx(ForwardModel& forwardModel);
   void fillOpenNodeListWithLeaves();
   void init(ForwardModel& forwardModel, GameState& gameState);
   void init(GameState initialState, const ForwardModel& forwardModel, Timer timer) override;
};
}  // namespace SGA
