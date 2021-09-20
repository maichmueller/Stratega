#pragma once
#include <iostream>
#include <random>
#include <stratega/Agent/RHEAAgent/RHEAParameters.h>
#include <stratega/ForwardModel/ForwardModel.h>
#include <stratega/Representation/GameState.h>
#include <vector>

namespace SGA {

class RHEAGenome {
  private:
   std::vector< Action > actions;  // Sequence of actions that defines this RHEA individual.
   double value = 0;  // Fitness of this individual.

  public:
   // Creates a random individual
   RHEAGenome(const ForwardModel& forwardModel, GameState gameState, RHEAParameters& params);

   // Creates a copy of an existing individual
   RHEAGenome(const RHEAGenome& other) = default;

   // Returns a sequence of actions
   std::vector< Action >& getActions() { return actions; };

   // Mutates the current individual.
   void mutate(
      const ForwardModel& forwardModel,
      GameState gameState,
      RHEAParameters& params,
      std::mt19937& randomGenerator);

   // Getter and setter for the fitness of this individual.
   [[nodiscard]] double getValue() const { return value; };
   void setValue(double newValue) { this->value = newValue; };

   // Shifts this individual to the left, eliminating the first action and padding from the right
   // with a new valid random action.
   void shift(const ForwardModel& forwardModel, GameState gameState, RHEAParameters& params);

   // Prints contents of this individual.
   void toString() const;

   // Crosses two individuals (parent1 and parent2) and returns a new one.
   static RHEAGenome crossover(
      const ForwardModel& forwardModel,
      GameState gameState,
      RHEAParameters& params,
      std::mt19937& randomGenerator,
      RHEAGenome& parent1,
      RHEAGenome& parent2);

  private:
   // Creates a new individual with a sequence of actions and a fitness already calculated.
   RHEAGenome(std::vector< Action >& actions, double value);

   // Applies an action "action" to a game state "gameState", using "forwardModel". It updates
   // "params" for budget concerns.
   static void applyActionToGameState(
      const ForwardModel& forwardModel,
      GameState& gameState,
      const Action& action,
      RHEAParameters& params);
};
}  // namespace SGA
