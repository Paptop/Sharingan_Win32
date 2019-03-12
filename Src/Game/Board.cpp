#include "Src/Game/Board.h"
#include "base/CCController.h"
#include "base/CCConsole.h"

#include "cocos2d/cocos/base/CCEventDispatcher.h"
#include "cocos2d/cocos/base/CCEventListenerController.h"

#include <random>

Sha::Board::Board()
: Scene()
, m_InputX(0.0f)
, m_InputY(0.0f)
, m_spawnTiles()
, m_spawnCell()
{
	m_inputListener = cocos2d::EventListenerController::create();
	m_inputListener->onKeyDown = CC_CALLBACK_3(Sha::Board::onKeyDown, this);
	m_inputListener->onKeyRepeat = CC_CALLBACK_3(Sha::Board::onKeyRepeat, this);
	m_inputListener->onAxisEvent = CC_CALLBACK_3(Sha::Board::onAxisEvent, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_inputListener, this);
	init();
}

Sha::Board::~Board()
{
	_eventDispatcher->removeEventListener(m_inputListener);
}

bool Sha::Board::init()
{
	InitCells();
	return true;
}

void Sha::Board::InitCells()
{
	for (int i = 0; i < Consts::kiRows; ++i)
	{
		for (int j = 0; j < Consts::kiCols; ++j)
		{
			cells[i][j].SetPos({i, j});
			/*
			CTile* tile = pool.Alloc();
			cells[i][j].SetTile(tile);
			addChild(tile);
			*/
		}
	}


	m_spawnCell.SetPos({ Consts::kiRows - 1, rand() % Consts::kiCols});
	m_spawnCell.SetTile(pool.Alloc());
	m_spawnCell.GetTile()->SetType(Sha::CTile::RED_GLOSSY);
	addChild(m_spawnCell.GetTile());
	m_spawnTetra.PostInitGraphics(&pool, this);
	//m_spawnTiles.push_back(m_spawnCell.GetTile());
}

void  Sha::Board::onKeyDown(cocos2d::Controller* controller, int keycode, cocos2d::Event* event)
{
	//cocos2d::log("Key pressed !");

	if (keycode == 0)
	{
		
	}
	else if (keycode == 1)
	{

	}
	else if (keycode == 2)
	{

	}
}

void Sha::Board::onKeyRepeat(cocos2d::Controller* controller, int keycode, cocos2d::Event* event)
{
	//cocos2d::log("Key repeat !");
}

void Sha::Board::onAxisEvent(cocos2d::Controller* controller, int value, cocos2d::Event* event)
{
	const cocos2d::Controller::KeyStatus& keyStatus = controller->getKeyStatus(value);
	static bool isStep = false;

	if (value == 0)
	{
		m_InputX = keyStatus.value;

		if (m_InputX == 0.0f)
		{
			isStep = false;
		}
		
		if (m_InputX >= 0.25f && !isStep)
		{
			m_spawnTetra.Move(32.0f);

			isStep = true;
		}
		else if (m_InputX <= -0.25f && !isStep)
		{
			m_spawnTetra.Move(-32.0f);

			isStep = true;
		}
	}
	else if (value == 1)
	{
		m_InputY = keyStatus.value;
	}
	else if (value == 5)
	{
		m_InputTriggerLeft = keyStatus.value;
		m_InputTriggerLeft = cocos2d::clampf(m_InputTriggerLeft, 0.0f, 1.0f);
	}

	//cocos2d::log("On axis moved! value : %d, keystatus %d, %f", value, keyStatus.isPressed, keyStatus.value);
}

void Sha::Board::update(float fDelta)
{
	Scene::update(fDelta);

	/*
	static CellPos pos = CellPos();

	if (pos != m_spawnCell.GetPos())
	{
		cocos2d::log("Spawn cell row : %d,  col %d", m_spawnCell.GetPos().m_row, m_spawnCell.GetPos().m_col);
	}

	pos = m_spawnCell.GetPos();

	static float gravityProgress = 0.0f;
	static float gravityDelay = 0.0f;
	static float gravityBound = 1.0f;

	Cell& cell = m_spawnCell;

	if (!m_spawnTiles.empty())
	{

		gravityProgress = 0.0f;
	}

	gravityDelay += (fDelta * 3.0f) + m_InputTriggerLeft;

	if (!cell.HasTile())
	{
		return;
	}

	int row = cell.GetPos().m_row - 1;
	int col = cell.GetPos().m_col;

	while (row >= 0 && !cells[row][col].HasTile())
	{
		row--;
	}

	Cell& target = cells[row + 1][col];

	if (gravityDelay >= gravityBound)
	{
		cell.GravityTileFall(target, 32.0f, CC_CALLBACK_0(Sha::Board::OnGravityFinish,this));
		gravityDelay = 0.0f;
	}
	*/

	if (!m_spawnTetra.HasFallen())
	{
		m_spawnTetra.GravityFall(&cells, fDelta * 3.0f + m_InputTriggerLeft);
	}
	else
	{
		m_spawnTetra = TetraTile();
		m_spawnTetra.PostInitGraphics(&pool,this);
	}
	

}

void Sha::Board::OnGravityFinish()
{
	//m_spawnTiles.pop_front();
	//m_spawnTiles.push_back(pool.Alloc());
	CellPos pos = m_spawnCell.GetPos();
	m_spawnCell.SetPos({ Consts::kiRows - 1, rand() % Consts::kiCols });
	m_spawnCell.SetTile(pool.Alloc());
	m_spawnCell.GetTile()->SetType(static_cast<Sha::CTile::ETile>(rand() % CTile::ETILECOUNT));
	addChild(m_spawnCell.GetTile());
	
	bool bBreak = true;

	for (int j = 0; j < Consts::kiCols; ++j)
	{
		bBreak &= cells[pos.m_row][j].HasTile();
	}

	if(bBreak)
	{
		for (int j = 0; j < Consts::kiCols; ++j)
		{
			Sha::CTile* tile = cells[pos.m_row][j].GetTile();
			cells[pos.m_row][j].Reset();
			this->removeChild(tile);
			pool.Free(tile);
		}

		/*
		for (int i = Consts::kiRows - 1; i >= pos.m_row; ++i)
		{
			for (int j = 0; j < Consts::kiCols; ++j)
			{
				if (cells[i][j].HasTile())
				{
					cells[i][j].GravityTileFall
				}

			}
		}*/
	}


}