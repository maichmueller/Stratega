#include <cassert>
#include <stratega/ForwardModel/Action.h>
#include <stratega/Representation/GameState.h>

namespace SGA {
void Action::execute(GameState& state, const ForwardModel& fm) const
{
   const auto& type = state.getGameInfo()->getActionTypes().at(getActionTypeID());
   for(const auto& effect : type.getEffects()) {
      effect->execute(state, fm, m_targets);
   }
}

bool Action::validate(GameState& state) const
{
   // Continuous or special action
   if(getActionTypeID() == -1)
      return true;

   auto& actionType = getActionType();

   // Check if source and targets are valid
   if(isEntityAction()) {
      auto entityID = m_targets.at(0).getEntityID();
      auto* entity = state.getEntity(entityID);

      if(entity != nullptr) {
         // Check if this action can be executed
         if(state.getCurrentTick() - entity->getActionInfo(getActionTypeID()).lastExecutedTick
            < actionType.getCooldown())
            return false;

         // Check preconditions
         for(auto& precondition : actionType.getPreconditions()) {
            if(! precondition->isFullfiled(state, m_targets))
               return false;
         }

         // Check if action targets are still valid in gamestate
         for(auto& target : m_targets) {
            if(! target.isValid(state))
               return false;
         }

         // Check target conditions
         for(auto& actionTarget : actionType.getTargets()) {
            for(auto& condition : actionTarget.second) {
               if(! condition->isFullfiled(state, m_targets))
                  return false;
            }
         }

         return true;
      }

      return false;

   } else {
      // Check if this action can be executed
      auto* player = state.getPlayer(m_targets.at(0).getPlayerID());
      if(state.getCurrentTick() - player->getActionInfo(getActionTypeID()).lastExecutedTick
         < actionType.getCooldown())
         return true;

      // Not found
      return false;
   }
}

bool Action::isEntityAction() const
{
   return m_targets.at(0).getType() == ActionTarget::EntityReference;
}

bool Action::isPlayerAction() const
{
   return m_targets.at(0).getType() == ActionTarget::PlayerReference;
}

[[nodiscard]] int Action::getSourceID() const
{
   return isEntityAction() ? m_targets.at(0).getEntityID() : m_targets.at(0).getPlayerID();
}

int Action::getActionTypeID() const
{
   int actionTypeID = -1;
   if(m_actionType)
      actionTypeID = m_actionType->getID();

   return actionTypeID;
}

std::string Action::getActionName() const
{
   std::string name = "undefined";

   if(m_actionTypeFlags == ActionFlag::EndTickAction)
      name = "End Turn / Pass Action";

   if(m_actionType)
      name = m_actionType->getName();

   return name;
}

ActionSourceType Action::getActionSourceType() const
{
   if(m_actionTypeFlags == ActionFlag::EndTickAction)
      return SGA::ActionSourceType::Player;

   if(m_actionType)
      return m_actionType->getSourceType();

   return SGA::ActionSourceType::Player;
}
}  // namespace SGA
