#include "Src/Game/Cell.h"
#include "Src/Game/Consts.h"
#include "Src/Game/Tile.h"

Sha::CellPos::CellPos(int row, int col)
	: m_row(row)
	, m_col(col)
{
}

Sha::CellPos::CellPos()
	: m_row(0)
	, m_col(0)
{
}

bool Sha::CellPos::Evaluate() const
{
	bool isRowValid = m_row >= 0 && m_row < Consts::kiRows;
	bool isColValid = m_col >= 0 && m_col < Consts::kiCols;

	if(isRowValid && isColValid)
	{
		return true;
	}

	return false;
}

bool Sha::CellPos::operator ==(const CellPos& pos)
{
	return m_row == pos.m_row && m_col == pos.m_col;
}

bool Sha::CellPos::operator !=(const CellPos& pos)
{
	return m_row != pos.m_row || m_col != pos.m_col;
}

Sha::CellPos& Sha::CellPos::operator+= (const CellPos& pos)
{
	m_row += pos.m_row;
	m_col += pos.m_col;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator-= (const CellPos& pos)
{
	m_row -= pos.m_row;
	m_col -= pos.m_col;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator*= (const CellPos& pos)
{
	m_row *= pos.m_row;
	m_col *= pos.m_col;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator/= (const CellPos& pos)
{
	m_row /= pos.m_row;
	m_col /= pos.m_col;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator+= (int value)
{
	m_row += value;
	m_col += value;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator-= (int value)
{
	m_row -= value;
	m_col -= value;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator*= (int value)
{
	m_row *= value;
	m_col *= value;
	return *this;
}

Sha::CellPos& Sha::CellPos::operator/= (int value)
{
	m_row /= value;
	m_col /= value;
	return *this;
}


Sha::CellPos& Sha::CellPos::operator ++()
{
	m_row++; m_col++;
	return *this;
}

Sha::CellPos Sha::CellPos::operator ++(int)
{
	Sha::CellPos pos = *this;
	pos.m_col ++;
	pos.m_row ++;
	return pos;
}

Sha::CellPos& Sha::CellPos::operator --()
{
	m_row--; m_col--;
	return *this;
}

Sha::CellPos Sha::CellPos::operator --(int)
{
	Sha::CellPos pos = *this;
	pos.m_col--;
	pos.m_row--;
	return pos;
}

Sha::Cell::Cell()
: m_pos(-1,-1)
, m_tile(nullptr)
{
}

void Sha::Cell::SetTile(CTile* tile)
{
	m_tile = tile;
	cocos2d::Vec2 pos = m_tile->getPosition();
	pos.x = 0;
	pos.y = 0;

	pos.x += 32 * m_pos.m_col + 16;
	pos.y += 32 * m_pos.m_row + 16;
	m_tile->setPosition(pos);
}

cocos2d::Vec2 Sha::Cell::GetScreenPos() const
{
	cocos2d::Vec2 pos;
	pos.x = 32 * m_pos.m_col + 16;
	pos.y = 32 * m_pos.m_row + 16;
	return pos;
}

float fLerp(float value, float target , float alpha)
{
	return  value * (1.f - alpha) + target * alpha;
}

bool Sha::Cell::GravityTileFall(Cell& target, float fDelta, const std::function<void()>& OnFinish)
{
	cocos2d::Vec2 pos = m_tile->getPosition();
	cocos2d::Vec2 vtarget = target.GetScreenPos();
	float distance = pos.getDistanceSq(vtarget);

	if (distance == 0.0f)
	{
		target.SetTile(m_tile);
		//m_tile = nullptr;
		//OnFinish();
		return true;
	}

	cocos2d::Vec2 oldPos = m_tile->getPosition();
	oldPos.y -= fDelta;

	m_pos.m_row = (int)(oldPos.y / 32.0f);
	m_tile->setPosition(oldPos);


	return false;
}



