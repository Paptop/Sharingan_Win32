#pragma once
#include <cassert>
#include <list>

#include "Src/Game/Tile.h"

#define TILES_COUT 500

namespace Sha
{
	class TilePool
	{
	public:
		void Free(CTile* tile)
		{
			tile->reset();
			tiles.push_back(tile);
		}

		CTile* Alloc()
		{
			if (tiles.empty())
			{
				assert(false);
			}
			else
			{
				CTile* tile = tiles.front();
				tile->init();
				tiles.pop_front();
				return tile;
			}
		}

		TilePool()
		{
			for (int i = 0; i < TILES_COUT; ++i)
			{
				tiles.push_back(new CTile());
			}
		}

		~TilePool()
		{
			for (CTile* tile : tiles)
			{
				delete tile;
			}
		}

	private:
		std::list<CTile*> tiles;

	}; // TilePool
};