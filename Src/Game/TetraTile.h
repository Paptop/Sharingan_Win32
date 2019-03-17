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
		std::vector<Cell> _tetraCells;
		int				  _originIndex;

		void Reset()
		{
			isFallen = false;
			_tetraCells.clear();
		}

		void Init()
		{
			std::random_device rd;  
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(2, 2);
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

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos });
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos + 1});

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 2 - 5, xPos});
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 2 - 5, xPos + 1});

					_originIndex = 1;

					break;
				case 1:
					//	[*]
					//	[*]
					//  [*]
					//  [*]
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos });
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 2 - 5, xPos});

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 3 - 5, xPos});
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 4 - 5, xPos});

					_originIndex = 1;
					break;

				case 2:

					//	[*]
					//	[*]
					//  [*][*]

					if (xPos == Consts::kiCols - 1)
					{
						xPos = Consts::kiCols - 2;
					}

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos });
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 2 - 5, xPos });

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 3 - 5, xPos });
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 3 - 5, xPos + 1});

					_originIndex = 1;
					break;
				default:
					assert(false);
			}
		}

		void PostInitGraphics(TilePool* pool, cocos2d::Node* scene)
		{
			for (Cell& cell: _tetraCells)
			{
				cell.SetTile(pool->Alloc());
				scene->addChild(cell.GetTile());
			}

			_tetraCells.back().GetTile()->SetType(Sha::CTile::ETile::RED);
		}

		void Move(Cell (*cells)[Consts::kiRows][Consts::kiCols], float offset)
		{
			std::vector<std::pair<CellPos, cocos2d::Vec2> > newData;

			for (Cell& cell : _tetraCells)
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

			assert(newData.size() == _tetraCells.size());

			for (int i = 0; i < _tetraCells.size(); ++i)
			{
				const CellPos pos = newData[i].first;
				const cocos2d::Vec2 vpos = newData[i].second;

				_tetraCells[i].GetTile()->setPosition(vpos);
				_tetraCells[i].SetPos(pos);
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

			for (auto cell = _tetraCells.rbegin(); cell != _tetraCells.rend(); ++cell)
			{
				bStopGravity |= cell->GravityTileFall(cells, 32.0f, CC_CALLBACK_0(Sha::TetraTile::OnGravityFinish, this));
				m_fgravityDelay = 0.0f;
			}

			if (!bStopGravity)
			{
				for (auto cell = _tetraCells.rbegin(); cell != _tetraCells.rend(); ++cell)
				{
					cell->ApplyGravity(32.0f);
				}
			}
			else
			{
				assert(isFallen == false);
				isFallen = true;
				for (auto cell = _tetraCells.rbegin(); cell != _tetraCells.rend(); cell++)
				{
					Cell& currentCell = (*cells)[cell->GetPos().m_row][cell->GetPos().m_col];
					currentCell.SetTile(cell->GetTile());
				}
			}
		}

		bool HasFallen() const { return isFallen; }

		void Flip90()
		{
			// Translate to 0,0 rotate translate back to origin
			/*
				Matrix
				|  0  1 |
				| -1  0 |
			*/
			static int flipCount = 0;

			cocos2d::Vec2 originPoint = _tetraCells[_originIndex].GetPoint(0);

			for (Cell& cell : _tetraCells)
			{
				/*
				CellPos newPos = cell.GetPos();
				newPos -= _originPoint;
				newPos.Mult(0, 1, -1, 0);
				newPos += _originPoint;
				cell.SetPos(newPos);
				cell.SetTile(cell.GetTile());
				*/
				float originX = originPoint.x;
				float originY = originPoint.y;
				cell.Translate(-originX, -originY);
				cell.Mult(0, 1, -1, 0);
				cell.Translate(originX, originY);
				cocos2d::Vec2 vec = cell.GetPoint(0);
				cell.SetPos({ (int)vec.y, (int)vec.x });
				cell.SetTile(cell.GetTile());
			}

			flipCount++;
			flipCount %= 4;
		}

		TetraTile()
		: m_fgravityDelay(0.0f)
		, m_fgravityBound(1.0f)
		, isFallen(false)
		, _originIndex(-1)
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