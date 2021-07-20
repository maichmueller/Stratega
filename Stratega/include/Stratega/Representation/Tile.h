#pragma once
#include <Stratega/Representation/Vector2.h>
namespace SGA
{
	struct TileType;
	
	struct Tile
	{
		Tile(int typeID,const TileType* tileType,int x, int y):
			isWalkable(true),
			blocksSight(false),
			position(x, y),
			tileType(tileType)
		{

		}

		/// <summary>
		/// Returns the tile type ID of this tile
		/// </summary>
		/// <returns>type ID of this tile</returns>
		int getTileTypeID() const;
		
		/// <summary>
		/// Indicates if this tile is walkable for units to occupy it.
		/// </summary>
		bool isWalkable;

		/// <summary>
		/// Indicates if this tile blocks the line of sight for entities in the game. 
		/// </summary>
		bool blocksSight;

		/// <summary>
		/// Position <x,y> of this tile in the board.
		/// </summary>
		Vector2i position;

		/// <summary>
		/// Returns the name of this tile type
		/// </summary>
		/// <returns>Name ot the tile type of this type.</returns>
		const std::string name() const;

		/// <summary>
		/// Returns the type of this tile
		/// </summary>
		const TileType& getType() const;
		
	private:

		/// <summary>
		/// Type of tile
		/// </summary>
		const TileType* tileType;
	};

}