#include <stratega/Representation/Tile.h>
#include <stratega/Representation/TileType.h>
namespace SGA {
int Tile::getTileTypeID() const
{
   int tileTypeID = -1;
   if(m_tileType)
      tileTypeID = m_tileType->getID();

   return tileTypeID;
}

const std::string Tile::name() const
{
   if(m_tileType != nullptr)
      return m_tileType->getName();
   return "Fog";
}

}  // namespace SGA