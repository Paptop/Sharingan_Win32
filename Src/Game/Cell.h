#pragma once

#include "Src\Game\Consts.h"
#include "cocos2d\cocos\math\Vec2.h"

namespace cocos2d
{
	class Node;
}

namespace Sha
{
	class CTile;

	struct CellPos
	{
		CellPos(int row, int col);
		CellPos();
		bool Evaluate() const;
		void Transpose();
		// Matrix mult
		void Mult(int a0, int a1, int b1, int b0);
		bool operator ==(const CellPos& pos);
		bool operator !=(const CellPos& pos);
		CellPos& operator +=(const CellPos& pos);
		CellPos& operator -=(const CellPos& pos);
		CellPos& operator *=(const CellPos& pos);
		CellPos& operator /=(const CellPos& pos);

		CellPos& operator +=(int value);
		CellPos& operator -=(int value);
		CellPos& operator *=(int value);
		CellPos& operator /=(int value);
	
		CellPos& operator ++();
		CellPos operator ++(int);
		CellPos& operator --();
		CellPos operator --(int);

		int m_row, m_col;
	};

	class Cell
	{
	public:
		Cell();
		void SetPos(const CellPos& pos);
		void SetTile(CTile* tile);
		void SetIdleBack(CTile* tile, cocos2d::Node* node);

		void PlaceTile(CTile* tile) { m_tile = tile; }

		void Mult(float a0, float a1, float b0, float b1);
		void Translate(float fDeltaX, float fDeltaY);
		cocos2d::Vec2 GetPoint(int index);

		CTile* GetTile() { return m_tile;  }

		inline CellPos GetPos() const { return m_pos; }

		cocos2d::Vec2 GetScreenPos() const;

		bool HasTile() { return m_tile != nullptr;  }
		bool GravityTileFall(Cell (*cells)[Consts::kiRows][Consts::kiCols], float fDelta, const std::function<void()>& OnFinish);
		void ApplyGravity(float fDelta);

		inline void Reset() { m_tile = nullptr; }
	private:
		CTile*			m_idle;
		CTile*			m_tile;
		CellPos			m_pos;
		/*
		 [3]		[2]
		(1,0)     (1,1)
			* --- *
			|(0,0)|
			* --- *
		 (0,0)    (1,0)
		  [0]       [1]
		*/
		cocos2d::Vec2 m_points[4];
	};
}