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
		std::vector<Cell> tetraCells;

		void Reset()
		{
			isFallen = false;
			tetraCells.clear();
		}

		void Init()
		{
			std::random_device rd;  
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 1);
			std::uniform_int_distribution<> disX(0, Consts::kiCols - 1);

			int tetraType = dis(gen);
			int xPos = disX(gen);

			switch(tetraType)
			{
				case 0:
					//	[*][*]
					//	[*][*]

					if (xPos == Consts::kiCols - 1)
					{
						xPos = Consts::kiCols - 2;
					}

					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 1, xPos });
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 1, xPos + 1});
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 2, xPos});
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 2, xPos + 1});
					break;
				case 1:
					//	[*]
					//	[*]
					//  [*]
					//  [*]
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 1, xPos });
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 2, xPos});
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 3, xPos});
					tetraCells.push_back(Cell());
					tetraCells.back().SetPos({ (int)Consts::kiRows - 4, xPos});
					break;
				default:
					assert(false);
			}
		}

		void PostInitGraphics(TilePool* pool, cocos2d::Node* scene)
		{
			for (Cell& cell: tetraCells)
			{
				cell.SetTile(pool->Alloc());
				scene->addChild(cell.GetTile());
			}

			tetraCells.back().GetTile()->SetType(Sha::CTile::ETile::RED);
		}

		void Move(Cell (*cells)[Consts::kiRows][Consts::kiCols], float offset)
		{
			std::vector<std::pair<CellPos, cocos2d::Vec2> > newData;

			for (Cell& cell : tetraCells)
			{
				cocos2d::Vec2 pos = cell.GetTile()->getPosition();
				pos.x += offset;
				pos.x = cocos2d::clampf(pos.x, 16.0f, 32.0f * Consts::kiCols - 16.0f);

				CellPos cellpos = cell.GetPos();

				offset > 0 ? cellpos.m_col++ : cellpos.m_col--;

				if (cellpos.m_col >= Consts::kiCols || cellpos.m_col < 0)
				{
					return;
				}

				if ((*cells)[cellpos.m_row][cellpos.m_col].HasTile())
				{
					return;
				}

				newData.push_back({ cellpos, pos });
			}

			assert(newData.size() == tetraCells.size());

			for (int i = 0; i < tetraCells.size(); ++i)
			{
				const CellPos pos = newData[i].first;
				const cocos2d::Vec2 vpos = newData[i].second;

				tetraCells[i].GetTile()->setPosition(vpos);
				tetraCells[i].SetPos(pos);
			}
		}

		void GravityFall(Cell (*cells)[Consts::kiRows][Consts::kiCols], float fDelta)
		{
			m_fgravityDelay += fDelta;

			if (m_fgravityDelay < m_fgravityBound)
			{
				return;
			}

			bool bStopGravity = false;

			for (auto cell = tetraCells.rbegin(); cell != tetraCells.rend(); ++cell)
			{
				bStopGravity |= cell->GravityTileFall(cells, 32.0f, CC_CALLBACK_0(Sha::TetraTile::OnGravityFinish, this));
				m_fgravityDelay = 0.0f;
			}

			if (!bStopGravity)
			{
				for (auto cell = tetraCells.rbegin(); cell != tetraCells.rend(); ++cell)
				{
					cell->ApplyGravity(32.0f);
				}
			}
			else
			{
				assert(isFallen == false);
				isFallen = true;
				for (auto cell = tetraCells.rbegin(); cell != tetraCells.rend(); cell++)
				{
					Cell& currentCell = (*cells)[cell->GetPos().m_row][cell->GetPos().m_col];
					currentCell.SetTile(cell->GetTile());
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