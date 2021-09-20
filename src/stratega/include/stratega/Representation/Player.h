#pragma once
#include <stratega/ForwardModel/Action.h>
#include <stratega/Representation/Parameter.h>
#include <unordered_map>
#include <vector>

namespace SGA {
struct Player {
  private:
   /// <summary>
   /// ID of this player, unique for players.
   /// </summary>
   int m_id;

   /// <summary>
   /// Indicates if this player can play at this time.
   /// </summary>
   bool m_canIPlay;

   /// <summary>
   /// Array of actions currently being executed by this player
   /// </summary>
   std::vector< Action > m_continuousActions;

   /// <summary>
   /// List of parameter values. Use GameState.getPlayerParameter(...) functions to retrieve this.
   /// </summary>
   std::vector< double > m_parameters;

   /// <summary>
   /// Actions that this player can execute in this game.
   /// </summary>
   std::vector< ActionInfo > m_attachedActions;

  public:
   Player(int id, bool canPlay) : m_id(id), m_canIPlay(canPlay) {}

   /// <summary>
   /// Indicates if a given action type can be executed by this player.
   /// </summary>
   [[nodiscard]] bool canExecuteAction(int actionTypeID) const;

   /// <summary>
   /// Returns the ActionInfo of an action type this player can execute.
   /// </summary>
   [[nodiscard]] const ActionInfo& getActionInfo(int actionTypeID) const;

   /// <summary>
   /// Returns the player ID defined for a neutral player.
   /// </summary>
   /// <returns>A constant ID for neutral player.</returns>
   static int getNeutralPlayerID() { return -1; }

   /// <summary>
   /// Returns the ID of this player
   /// </summary>
   int getID() const { return m_id; }

   /// <summary>
   /// Indicates if this player can play at this time.
   /// </summary>
   bool canPlay() const { return m_canIPlay; }

   /// <summary>
   /// Sets if the player can play now or not.
   /// </summary>
   void setCanPlay(bool can) { m_canIPlay = can; }

   /// <summary>
   /// Returns the list of parameters, can't be modified.
   /// </summary>
   const std::vector< double >& getParameters() const { return m_parameters; }

   /// <summary>
   /// Returns the list of parameters of this player (can be modified)
   /// </summary>
   std::vector< double >& getParameters() { return m_parameters; }

   /// <summary>
   /// Returns a reference to a parameter value of this player.
   /// </summary>
   double& getParameter(unsigned long paramIdx) { return m_parameters[paramIdx]; }

   /// <summary>
   /// Returns a const value of a parameter of this player.
   /// </summary>
   const double& getParameter(unsigned long paramIdx) const { return m_parameters[paramIdx]; }

   /// <summary>
   /// Sets the parameter of this player to a certain value
   /// </summary>
   /// <param name="paramIdx">Parameter index of this param.</param>
   /// <param name="val">Value to be set for the parameter.</param>
   void setParameter(unsigned long paramIdx, double val) { m_parameters[paramIdx] = val; }

   /// <summary>
   /// Sets a size for the vector of parameters of this player.
   /// </summary>
   void resizeParameters(unsigned long cap) { m_parameters.resize(cap); }

   /// <summary>
   /// Removes a continuous action from the vector of continuous actions of this player.
   /// </summary>
   /// <param name="idx">IDx of the action to remove.</param>
   void removeContinuousAction(int idx)
   {
      m_continuousActions.erase(m_continuousActions.begin() + idx);
   }

   /// <summary>
   /// Adds a continuous action to the list of this player.
   /// </summary>
   void addContinuousAction(Action newAction) { m_continuousActions.emplace_back(newAction); }

   /// <summary>
   /// Advances the tick counter of the continuous action with index 'idx'
   /// </summary>
   void advanceContinuousAction(unsigned long idx) { m_continuousActions[idx].incTicks(); }

   /// <summary>
   /// Returns the list of continuous actions this player is executing.
   /// </summary>
   const std::vector< Action >& getContinuousActions() const { return m_continuousActions; }

   /// <summary>
   /// Returns the list of attached actions to this player.
   /// </summary>
   const std::vector< ActionInfo >& getAttachedActions() const { return m_attachedActions; }

   /// <summary>
   /// Returns the action info of the attached action with index 'idx'.
   /// </summary>
   const ActionInfo& getAttachedAction(unsigned long idx) const { return m_attachedActions[idx]; }

   /// <summary>
   /// Adds a new attached action to this player.
   /// </summary>
   void addAttachedAction(int actionTypeID, int lastExecutedTick)
   {
      m_attachedActions.emplace_back(ActionInfo{actionTypeID, lastExecutedTick});
   }

   /// <summary>
   /// Sets the last tick on an attached action, indexed by 'idx'
   /// </summary>
   void setActionLastTick(unsigned long idx, int lastTick)
   {
      m_attachedActions[idx].lastExecutedTick = lastTick;
   }

   /// <summary>
   /// Reserves space for attached actions.
   /// </summary>
   void resAttachedActions(unsigned long cap) { m_attachedActions.reserve(cap); }
};
}  // namespace SGA
