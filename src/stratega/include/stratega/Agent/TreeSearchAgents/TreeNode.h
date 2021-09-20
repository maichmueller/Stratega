#pragma once
#include <stratega/Agent/AgentParameters.h>
#include <stratega/Agent/TreeSearchAgents/ITreeNode.h>

namespace SGA {

class TreeNode: public ITreeNode< TreeNode > {
  protected:
   TreeNode(
      ForwardModel& forwardModel,
      GameState gameState,
      TreeNode* parent,
      int childIndex,
      int ownerID);

  public:
   // Root Node Constructor
   TreeNode(ForwardModel& forwardModel, GameState gameState, int ownerID);
   TreeNode* expand(ForwardModel& forwardModel, AgentParameters& agentParameters);

   void print() const override;
};

}  // namespace SGA
