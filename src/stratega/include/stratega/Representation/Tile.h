#pragma once
#include <stratega/Representation/Vector2.h>
namespace SGA {
struct TileType;

struct Tile {
  private:
   /// <summary>
   /// Type of tile
   /// </summary>
   const TileType* m_tileType;

   /// <summary>
   /// Indicates if this tile is walkable for units to occupy it.
   /// </summary>
   bool m_walkable;

   /// <summary>
   /// Indicates if this tile blocks the line of sight for entities in the game.
   /// </summary>
   bool m_blockSight;

   /// <summary>
   /// Position <x,y> of this tile in the board.
   /// </summary>
   Vector2i m_position;

  public:
   Tile(const TileType* tileType, int x, int y)
       : m_tileType(tileType), m_walkable(true), m_blockSight(false), m_position(x, y)
   {
   }

   /// <summary>
   /// Returns the tile type ID of this tile
   /// </summary>
   /// <returns>type ID of this tile</returns>
   int getTileTypeID() const;

   /// <summary>
   /// Returns the name of this tile type
   /// </summary>
   /// <returns>Name ot the tile type of this type.</returns>
   const std::string name() const;

   /*Getters and setters for Tile*/

   bool isWalkable() const { return m_walkable; }
   void setWalkable(bool w) { m_walkable = w; }

   bool blocksSight() const { return m_blockSight; }
   void setBlockSight(bool b) { m_blockSight = b; }

   void setPosition(int x, int y) { m_position = Vector2i(x, y); }
   int x() const { return m_position.x; }
   int y() const { return m_position.y; }
};

}  // namespace SGA