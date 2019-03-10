#pragma once


namespace Sha
{
	class CTile;

	struct CellPos
	{
		CellPos(int row, int col);
		CellPos();
		bool Evaluate() const;
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
		inline void SetPos(const CellPos& pos) { m_pos = pos; }
		void SetTile(CTile* tile);

		CTile* GetTile() { return m_tile;  }

		inline CellPos GetPos() const { return m_pos; }
	private:
		CTile* m_tile;
		CellPos m_pos;
	};
}