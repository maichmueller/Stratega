#include <stratega/Representation/Entity.h>
#include <stratega/Representation/GameInfo.h>
#include <stratega/Representation/Player.h>

namespace SGA {
bool Entity::isNeutral() const
{
   return m_ownerID == Player::getNeutralPlayerID();
}

int Entity::getEntityTypeID() const
{
   return m_type->getID();
}

void Entity::init(const EntityType* type, int entityID)
{
   this->m_type = type;
   this->m_id = entityID;

   // Add actions
   m_attachedActions.reserve(type->getActionIDs().size());
   for(auto actionTypeID : type->getActionIDs()) {
      m_attachedActions.emplace_back(ActionInfo{actionTypeID, 0});
   }

   // Set parameter values
   m_lineOfSightRange = type->getLoSRange();
   m_parameters.reserve(type->getParameters().size());
   for(const auto& idParamPair : type->getParameters()) {
      m_parameters.emplace_back(idParamPair.second.getDefaultValue());
   }
}

double Entity::getParameter(const std::string& paramName) const
{
   for(const auto& param : m_type->getParameters()) {
      if(param.second.getName() == paramName) {
         return m_parameters[param.second.getIndex()];
      }
   }
}

std::vector< std::string > Entity::getEntityParameterNames() const
{
   std::vector< std::string > paramNames;
   for(const auto& param : m_type->getParameters())
      paramNames.emplace_back(param.second.getName());

   return paramNames;
}

std::unordered_map< std::string, double > Entity::getEntityParameters() const
{
   std::unordered_map< std::string, double > params;
   for(const auto& param : m_type->getParameters())
      params.emplace(param.second.getName(), m_parameters[param.second.getIndex()]);
   return params;
}

void Entity::setActionTicks(int actionTypeID, int tick)
{
   // ToDo We should probably find a way to avoid this loop (why attachedActions is not a map?)
   for(auto& actionInfo : m_attachedActions) {
      if(actionInfo.actionTypeID == actionTypeID) {
         actionInfo.lastExecutedTick = tick;
         return;
      }
   }
}

const ActionInfo& Entity::getActionInfo(int actionTypeID) const
{
   for(auto& actionInfo : m_attachedActions) {
      if(actionInfo.actionTypeID == actionTypeID)
         return actionInfo;
   }

   throw std::runtime_error("No action type associated to ID " + std::to_string(actionTypeID));
}

std::vector< ActionType > Entity::getActionTypes(const GameInfo& gameInfo) const
{
   std::vector< ActionType > aTypes;
   for(const ActionInfo aInfo : m_attachedActions) {
      ActionType at = gameInfo.getActionType(aInfo.actionTypeID);
      aTypes.emplace_back(at);
   }

   return aTypes;
}

void Entity::printInfo() const
{
   std::cout << "[" << m_type->getName() << "],";
   std::cout << " [ID: " << m_id << "],";
   std::cout << " [OwnerID: " << m_ownerID << "],";
   std::cout << " [Position: " << m_position.x << "," << m_position.y << "]";

   int parameterID = 0;
   if(m_parameters.empty()) {
      std::cout << std::endl;
      return;
   }

   std::cout << ", [Parameters: ";
   for(auto& parameter : m_parameters) {
      std::cout << "(" << m_type->getParameters().find(parameterID++)->second.getName() << ": "
                << parameter << ")";
   }

   std::cout << "]" << std::endl;
}

}  // namespace SGA
