#include <stratega/Representation/Player.h>
namespace SGA {
[[nodiscard]] bool Player::canExecuteAction(int actionTypeID) const
{
   for(const auto& actionType : m_attachedActions) {
      if(actionType.actionTypeID == actionTypeID)
         return true;
   }
   return false;
}

[[nodiscard]] const ActionInfo& Player::getActionInfo(int actionTypeID) const
{
   for(const auto& actionInfo : m_attachedActions) {
      if(actionInfo.actionTypeID == actionTypeID)
         return actionInfo;
   }

   throw std::runtime_error("Tried accessing action with unknown actionType");
}
}  // namespace SGA
