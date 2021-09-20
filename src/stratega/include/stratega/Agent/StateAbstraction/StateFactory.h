#pragma once
#include <iostream>
#include <map>
#include <stratega/Agent/StateAbstraction/AbstractState.h>
#include <stratega/Representation/GameState.h>
#include <string>

namespace SGA {
struct StateFactoryConfiguration {
   bool insertMap = false;
   bool insertEntityPositions = true;
   // std::map<std::string, bool> insertPlayerParameters;
   std::map< std::string, bool > insertEntityParameters;
   // std::map<std::string, std::map<int, int>> mapParameterNameToUnitTypeParameterID;

   StateFactoryConfiguration() {}
};

class StateFactory {
  public:
   StateFactoryConfiguration config;

  public:
   StateFactory(StateFactoryConfiguration _config) : config(_config) {}

   StateFactory(GameState& tbs)
   {
      // bool flip = true;
      config = StateFactoryConfiguration();
      const auto& parameters = tbs.getGameInfo()->getParameterIDLookup();
      for(auto entry : parameters) {
         if(! config.insertEntityParameters.contains(entry.first)) {
            config.insertEntityParameters[entry.first] = true;
            // config.insertEntityParameters[entry.first] = flip;
            // flip = !flip;
         }
      }
   }

   AbstractState createAbstractState(GameState& tbsState);

   friend std::ostream& operator<<(std::ostream& os, const StateFactory& dt)
   {
      os << "Map: " << dt.config.insertMap << std::endl;
      os << "Positions: " << dt.config.insertEntityPositions << std::endl;
      for(auto entry : dt.config.insertEntityParameters) {
         os << entry.first << ": " << entry.second << std::endl;
      }

      return os;
   }
};
}  // namespace SGA