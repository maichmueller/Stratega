#include <Stratega/Representation/Tile.h>
#include <Stratega/Representation/TileType.h>
namespace SGA
{
	int Tile::getTileTypeID() const
	{
		int tileTypeID = -1;
		if (tileType)
			tileTypeID = tileType->id;

		return tileTypeID;
	}

	const std::string Tile::name() const
	{
		if(tileType != nullptr)
			return tileType->name;
		return "Fog";
	}

	const TileType& Tile::getType() const
	{
		return *tileType;
	}


}