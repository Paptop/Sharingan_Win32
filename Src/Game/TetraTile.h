#pragma once

#include "Src/Game/Cell.h"
#include "Src/Game/Consts.h"
#include "Src/Game/TilePool.h"

#include "cocos2d\cocos\2d\CCScene.h"
#include <list>

namespace Sha
{

	struct TetraTile
	{
	public:
		std::list<Cell> tetraCells;

		void Init()
		{
			
			//	[*][*]
			//	[*][*]

			tetraCells.push_back(Cell());
			tetraCells.back().SetPos({ (int)Consts::kiRows, 0});
			tetraCells.push_back(Cell());
			tetraCells.back().SetPos({ (int)Consts::kiRows, 1});
			tetraCells.push_back(Cell());
			tetraCells.back().SetPos({ (int)Consts::kiRows - 1, 0});
			tetraCells.push_back(Cell());
			tetraCells.back().SetPos({(int)Consts::kiRows - 1, 1});
		}

		void PostInitGraphics(TilePool* pool, cocos2d::Scene* scene)
		{
			for (Cell& cell: tetraCells)
			{
				cell.SetTile(pool->Alloc());
				scene->addChild(cell.GetTile());
			}
		}

		TetraTile()
		{
			Init();
		}
	};
}