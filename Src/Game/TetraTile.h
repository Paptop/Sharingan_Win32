#pragma once

#include "Src/Game/Cell.h"
#include "Src/Game/Consts.h"
#include "Src/Game/TilePool.h"

#include "cocos2d\cocos\2d\CCScene.h"
#include <random>
#include <list>

namespace Sha
{
	enum ETetraType
	{
		ETetra_Pipe = 0,
		ETetra_Square,
		ETetra_L,
		ETetra_T,
		ETetra_Z,
		ETetra_RZ,
		ETetra_count
	};

	struct TetraTile
	{
	public:
		std::vector<Cell> _tetraCells;
		int				  _originIndex;
		cocos2d::Vec2     _originPoint;
		ETetraType		  _currentType;

		void Reset()
		{
			isFallen = false;
			_tetraCells.clear();
		}

		void Init()
		{
			std::random_device rd;  
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, ETetra_T);
			std::uniform_int_distribution<> disX(0, Consts::kiCols - 1);

			ETetraType tetraType = static_cast<ETetraType>(dis(gen));
			int xPos = disX(gen);

			switch(tetraType)
			{
				case ETetra_Square:
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
					break;

				case ETetra_Pipe:
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
					_originPoint = _tetraCells[_originIndex].GetPoint(0);
					break;

				case ETetra_L:

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
					break;
				case ETetra_T:

					//	[*][*][*]
					//	   [*]

					if (xPos >= Consts::kiCols - 2)
					{
						xPos = Consts::kiCols - 3;
					}

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos });
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos + 1});

					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 1 - 5, xPos + 2});
					_tetraCells.push_back(Cell());
					_tetraCells.back().SetPos({ (int)Consts::kiRows - 2 - 5, xPos + 1});
					break;
				default:
					assert(false);
			}


			_originIndex = 1;
			_originPoint = _tetraCells[_originIndex].GetPoint(0);
			_currentType = tetraType;
		}

		void PostInitGraphics(TilePool* pool, cocos2d::Node* scene)
		{
			CTile::ETile tileColour = static_cast<CTile::ETile>(rand() % CTile::ETile::IDLE);

			for (Cell& cell: _tetraCells)
			{
				CTile* tile = pool->Alloc();
				tile->SetType(tileColour);

				cell.SetTile(tile);
				scene->addChild(tile);
			}
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
					_originPoint.y -= 1.0f;
				}
			}
			else
			{
				assert(isFallen == false);
				isFallen = true;
				for (auto cell = _tetraCells.rbegin(); cell != _tetraCells.rend(); ++cell)
				{
					Cell& currentCell = (*cells)[cell->GetPos().m_row][cell->GetPos().m_col];
					currentCell.SetTile(cell->GetTile());
				}
			}
		}

		bool HasFallen() const { return isFallen; }

		void Flip90(Cell (*cells)[Consts::kiRows][Consts::kiCols])
		{
			// Translate to 0,0 rotate translate back to origin
			/*
				Matrix
				|  0  1 |
				| -1  0 |
			*/
			static int flipCount = 0;

			//cocos2d::Vec2 originPoint = _tetraCells[_originIndex].GetPoint(0);
			cocos2d::Vec2 originPoint = _tetraCells[_originIndex].GetPoint(0);

			std::vector<CellPos> newData;

			for (const Cell& kCell : _tetraCells)
			{
				Cell cell = kCell;
				float originX = originPoint.x;
				float originY = originPoint.y;
				cell.Translate(-originX, -originY);
				cell.Mult(0, 1, -1, 0);
				cell.Translate(originX, originY);
				cocos2d::Vec2 vec = cell.GetPoint(0);
				CellPos pos = { (int)vec.y, (int)vec.x };
				
				if (!pos.Evaluate())
				{
					return;
				}

				if ((*cells)[pos.m_row][pos.m_col].HasTile())
				{
					return;
				}

				newData.push_back(pos);
			}

			//Collsion and wall kick

			assert(newData.size() == _tetraCells.size());

			for (int i = 0; i < _tetraCells.size(); ++i)
			{
				const CellPos pos = newData[i];

				_tetraCells[i].SetPos(pos);
				_tetraCells[i].SetTile(_tetraCells[i].GetTile());
			}


		}

		TetraTile()
		: m_fgravityDelay(0.0f)
		, m_fgravityBound(1.0f)
		, isFallen(false)
		, _originIndex(-1)
		, _currentType(ETetra_count)
		, _originPoint(-1.0f,-1.0f)
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