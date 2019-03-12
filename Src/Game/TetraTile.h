#pragma once

#include "Src/Game/Cell.h"
#include "Src/Game/Consts.h"
#include "Src/Game/TilePool.h"

#include "cocos2d\cocos\2d\CCScene.h"
#include <random>
#include <list>

namespace Sha
{

	struct TetraTile
	{
	public:
		std::list<Cell> tetraCells;

		void Init()
		{
			int iRand = rand() % 2;


			if (iRand == 1)
			{
				//	[*][*]
				//	[*][*]
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows, 0 });
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows, 1 });
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows - 1, 0 });
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows - 1, 1 });
			}
			else if (iRand == 1)
			{
				//	[*]
				//	[*]
				//  [*]
				//  [*]
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows, 0 });
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows - 1, 0});
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows - 2, 0 });
				tetraCells.push_back(Cell());
				tetraCells.back().SetPos({ (int)Consts::kiRows - 3, 0 });
			}
		}

		void PostInitGraphics(TilePool* pool, cocos2d::Scene* scene)
		{
			for (Cell& cell: tetraCells)
			{
				cell.SetTile(pool->Alloc());
				scene->addChild(cell.GetTile());
			}

			tetraCells.back().GetTile()->SetType(Sha::CTile::ETile::RED);
		}

		void Move(float offset)
		{
			for (Cell& cell : tetraCells)
			{
				cocos2d::Vec2 pos = cell.GetTile()->getPosition();
				pos.x += offset;
				pos.x = cocos2d::clampf(pos.x, 16.0f, 32.0f * Consts::kiCols - 16.0f);

				CellPos cellpos = cell.GetPos();

				offset > 0 ? cellpos.m_col++ : cellpos.m_col--;

				if (offset > 0)
				{
					if (cellpos.m_col >= Consts::kiCols)
					{
						cellpos.m_col = Consts::kiCols - 1;
					}
				}
				else
				{
					if (cellpos.m_col < 0)
					{
						cellpos.m_col = 0;
					}
				}

				cell.GetTile()->setPosition(pos);
				cell.SetPos(cellpos);
			}
		}

		void GravityFall(Cell (*cells)[Consts::kiRows][Consts::kiCols], float fDelta)
		{
			m_fgravityDelay += fDelta;

			if (m_fgravityDelay >= m_fgravityBound)
			{
				for (auto cell = tetraCells.rbegin(); cell != tetraCells.rend(); cell++)
				{
					int row = cell->GetPos().m_row - 1;
					int col = cell->GetPos().m_col;

					while (row >= 0 && !(*cells)[row][col].HasTile())
					{
						row--;
					}

					Cell& target = (*cells)[row + 1][col];

					if (isFallen)
					{
						target.SetTile(cell->GetTile());
						break;
					}

				    isFallen = cell->GravityTileFall(target, 32.0f, CC_CALLBACK_0(Sha::TetraTile::OnGravityFinish, this));
					m_fgravityDelay = 0.0f;
				}
			}
		}

		bool HasFallen() const { return isFallen; }

		TetraTile()
		: m_fgravityDelay(0.0f)
		, m_fgravityBound(1.0f)
		, isFallen(false)
		{
			Init();
		}

	private:

		void OnGravityFinish()
		{
		}

	private:
		float m_fgravityDelay;
		float m_fgravityBound;
		bool isFallen;
	};
}