#pragma once
#include <optional>
#include <stratega/ForwardModel/Action.h>
#include <stratega/Representation/Path.h>
#include <stratega/Representation/Vector2.h>
#include <unordered_map>
#include <vector>

namespace SGA {
struct GameInfo;

/// <summary>
/// Is an object that interacts with the game and responds to player input or other entities.
/// This entity can be assigned to a player performing a set of actions per tick/turn.
/// It has a list of parameters that store real values.
/// If the player wants to check the <see cref="SGA::EntityType"/> it can search it using the
/// assigned typeID.
/// </summary>
struct Entity {
  private:
   /// <summary>
   // ID of this entity in the game.
   /// <summary>
   int m_id;

   /// <summary>
   // ID of the player who owns this entity.
   /// <summary>
   int m_ownerID;

   /// <summary>
   // Position of the entity in the board.
   /// <summary>
   Vector2f m_position;

   /// <summary>
   // Range of the line of sight of this entity.
   /// <summary>
   double m_lineOfSightRange;

   /// <summary>
   /// Flag that indicates if this entity will be removed from the game.
   /// </summary>
   bool m_remove = false;

   /// <summary>
   /// Actions that this entity can execute in this game.
   /// </summary>
   std::vector< ActionInfo > m_attachedActions;

   /// <summary>
   /// Array of actions currently being executed by this entity
   /// </summary>
   std::vector< Action > m_continuousAction;

   /// <summary>
   /// Values for the parameters of this entity. Indexed by ID. Use getParameter(...) functions to
   /// access these.
   /// </summary>
   std::vector< double > m_parameters;

   /// <summary>
   /// Entity type
   /// <summary>
   const EntityType* m_type;

   /* Variables for continuous movement of the unit in RTS games. */

   /// <summary>
   /// Path that this entity is following (RTS)
   /// <summary>
   Path m_path;

   /// <summary>
   /// Movement speed for this entity (RTS)
   /// <summary>
   double m_movementSpeed;

   /// <summary>
   /// Collision radius for this entity (RTS)
   /// <summary>
   double m_collisionRadius;

  public:
   /// <summary>
   /// Creates an instance of an entity.
   /// <summary>

   Entity()
       : m_id(0),
         m_ownerID(0),
         m_position(),
         m_lineOfSightRange(5),
         m_type(0),
         m_path(),
         m_movementSpeed(2),
         m_collisionRadius(0.5)
   {
   }

   /// <summary>
   // Initializes the entity with a given ID. Creates default attached actions and parameters list
   // from the entity type.
   /// <summary>
   /// <param name="entityID">(Unique) ID of this entity in the game.</param>
   void init(const EntityType* type, int entityID);

   /// <summary>
   /// Return entity type
   /// <summary>
   const EntityType& getEntityType() const { return *m_type; }

   /// <summary>
   /// Return entity type ID
   /// <summary>
   int getEntityTypeID() const;

   /// <summary>
   /// Indicates if this unit is neutral (i.e. does not belong to any playing faction).
   /// <summary>
   /// <returns>True if this entity is neutral.</returns>
   bool isNeutral() const;

   /// <summary>
   /// Returns a <see cref="SGA::ActionInfo"/> object linked to action type,
   /// used to check when was the last time the action type was executed by this entity
   /// </summary>
   /// <param name="actionTypeID">The ID of the action type we're looking for.</param>
   /// <returns>The action info object, if it exists. Throws a runtime error if it
   /// doesn't.</returns>
   const ActionInfo& getActionInfo(int actionTypeID) const;

   /// <summary>
   /// Returns the actions attached to this entity.
   /// <summary>
   /// <returns>The list of <see cref="SGA::ActionInfo"/> attached to this entity.</returns>
   const std::vector< ActionInfo >& getAttachedActions() const { return m_attachedActions; }

   /// <summary>
   /// Retrieves the list of action types this entity can execute.
   /// </summary>
   /// <param name="gameInfo">Reference to the game information object (from GameState)</param>
   /// <returns>Vector with all action types this entity can execute</returns>
   std::vector< ActionType > getActionTypes(const GameInfo& gameInfo) const;

   /// <summary>
   /// Sets the last executed game ticks to the tick passed by parameter for the action
   /// corresponding to the type received. <summary> <param name="actionTypeID">The ID of the action
   /// type we're looking for.</param> <param name="tick">The value of the tick to set in the action
   /// information</param>
   void setActionTicks(int actionTypeID, int tick);

   /// <summary>
   // Flags this entity to be removed by the game engine.
   /// <summary>
   void flagRemove() { m_remove = true; }

   /// <summary>
   /// Checks if this entity should be removed by the game engine.
   /// <summary>
   /// <returns>True if this entity is marked to be removed.</returns>
   bool flagged() { return m_remove; }

   /// <summary>
   // Prints information about the entity, parameters and actions
   /// <summary>
   void printInfo() const;

   /// <summary>
   /// Gets the list of parameters values. Modifiable.
   /// <summary>
   /// <returns>The list of parameters values.</returns>
   std::vector< double >& getParamValues() { return m_parameters; }

   /// <summary>
   /// Gets the list of parameters values.
   /// <summary>
   /// <returns>The list of parameters values.</returns>
   const std::vector< double >& getParamValues() const { return m_parameters; }

   /// <summary>
   /// Gets the list of parameters names.
   /// <summary>
   /// <returns>The list of parameters names of this entity.</returns>
   std::vector< std::string > getEntityParameterNames() const;

   /// <summary>
   /// Gets a map with all pairs <parameter,value>
   /// <summary>
   /// <returns>Returns a map with all the parameters.</returns>
   std::unordered_map< std::string, double > getEntityParameters() const;

   /// <summary>
   /// Gets the value of a specific parameter, by name
   /// <summary>
   /// <returns>The parameter value.</returns>
   double getParameter(const std::string& paramName) const;

   /// <summary>
   /// Gets a specific parameters, by index
   /// <summary>
   /// <returns>The parameter value.</returns>
   double& getParameterAt(int paramIdx)
   {
      return m_parameters[static_cast< unsigned long >(paramIdx)];
   }

   /// <summary>
   /// Gets the list of continuous actions attached to this entity. Modifiable.
   /// <summary>
   /// <returns>The list of continuous actions attached to this entity.</returns>
   std::vector< Action >& getContinuousActions() { return m_continuousAction; }

   /// <summary>
   /// Gets the list of continuous actions attached to this entity.
   /// <summary>
   /// <returns>The list of continuous actions attached to this entity.</returns>
   const std::vector< Action >& getContinuousActions() const { return m_continuousAction; }

   /// <summary>
   /// Returns the movement speed of this entity.
   /// </summary>
   double getMovementSpeed() const { return m_movementSpeed; }

   /// <summary>
   /// Returns the collision radius of this entity.
   /// </summary>
   double getCollisionRadius() const { return m_collisionRadius; }

   /// <summary>
   /// Returns theID of this entity.
   /// </summary>
   int getID() const { return m_id; }

   /// <summary>
   /// Returns the owner ID (i.e. player ID) who controls this entity.
   /// </summary>
   int getOwnerID() const { return m_ownerID; }

   /// <summary>
   /// Sets the owner ID (player ID) in control of this entity..
   /// </summary>
   void setOwnerID(int oID) { m_ownerID = oID; }

   /// <summary>
   /// Returns the line of sight of this entity.
   /// </summary>
   double getLineOfSightRange() const { return m_lineOfSightRange; }

   /// <summary>
   /// Returns x position of this entity.
   /// </summary>
   double x() const { return m_position.x; }

   /// <summary>
   /// Returns y position of this entity.
   /// </summary>
   double y() const { return m_position.y; }

   /// <summary>
   /// Returns position of this entity.
   /// </summary>
   const Vector2f& getPosition() const { return m_position; }

   /// <summary>
   /// Sets the position of this entity in the board. Does not modify the board.
   /// </summary>
   void setPosition(Vector2f v) { m_position = v; }

   /// <summary>
   /// Returns the path that this entity is following (RTS games only)
   /// </summary>
   const Path& getPath() const { return m_path; }

   /// <summary>
   /// Increments the current index of the path that this entity is following (RTS games only)
   /// </summary>
   void incPathIndex() { m_path.currentPathIndex++; }

   /// <summary>
   /// Sets the path this entity is following (RTS games only)
   /// </summary>
   void setPath(Path p) { m_path = p; }
};
}  // namespace SGA