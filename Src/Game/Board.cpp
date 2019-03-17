#include "Src/Game/Board.h"
#include "base/CCController.h"
#include "base/CCConsole.h"

#include "cocos2d.h"
#include "cocos2d/cocos/base/CCEventDispatcher.h"
#include "cocos2d/cocos/base/CCEventListenerController.h"

#include <random>


cocos2d::Scene* Sha::Board::createScene()
{
	return Sha::Board::create();
}

Sha::Board::Board()
: Scene()
, m_InputX(0.0f)
, m_InputY(0.0f)
, m_spawnTiles()
, m_spawnCell()
, m_spawnTetra()
, m_board()
{
	Debug_OutputBoard();
	m_inputListener = cocos2d::EventListenerController::create();
	m_inputListener->onKeyDown = CC_CALLBACK_3(Sha::Board::onKeyDown, this);
	m_inputListener->onKeyRepeat = CC_CALLBACK_3(Sha::Board::onKeyRepeat, this);
	m_inputListener->onAxisEvent = CC_CALLBACK_3(Sha::Board::onAxisEvent, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_inputListener, this);
}

Sha::Board::~Board()
{
	_eventDispatcher->removeEventListener(m_inputListener);
}

bool Sha::Board::init()
{
	if (!cocos2d::Scene::init())
	{
		return false;
	}

	InitCells();
	scheduleUpdate();

	return true;
}

void Sha::Board::InitCells()
{
	for (int i = 0; i < Consts::kiRows; ++i)
	{
		for (int j = 0; j < Consts::kiCols; ++j)
		{
			cells[i][j].SetPos({ i, j });
			cells[i][j].SetIdleBack(pool.Alloc(), &m_board);
			/*
			if (i == Consts::kiRows - 5 || i == Consts::kiRows - 4)
			{
				cells[i][j].SetTile(pool.Alloc());
				m_board.addChild(cells[i][j].GetTile());
			}
			*/
		}
	}

	/*
	m_spawnCell.SetPos({ Consts::kiRows - 1, rand() % Consts::kiCols });
	m_spawnCell.SetTile(pool.Alloc());
	m_spawnCell.GetTile()->SetType(Sha::CTile::RED_GLOSSY);
	addChild(m_spawnCell.GetTile());
	*/

	m_spawnTetra.PostInitGraphics(&pool, &m_board);
	this->addChild(&m_board);

	cocos2d::Vec2 size = cocos2d::Director::getInstance()->getVisibleSize();
	m_board.setPositionX(m_board.getPositionX() + size.x * 0.5f - Consts::kiCols * 0.5f * 32.0f);
}

void  Sha::Board::onKeyDown(cocos2d::Controller* controller, int keycode, cocos2d::Event* event)
{
	if (keycode == 0)
	{
		m_spawnTetra.Flip90();
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
		
		if (m_InputX >= 0.15f && !isStep)
		{
			m_spawnTetra.Move(&cells, 32.0f);

			isStep = true;
		}
		else if (m_InputX <= -0.15f && !isStep)
		{
			m_spawnTetra.Move(&cells, -32.0f);

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
	//return;

	static bool bUpdateGravity = false;

	if (!m_spawnTetra.HasFallen())
	{
		m_spawnTetra.GravityFall(&cells, fDelta * 3.0f + m_InputTriggerLeft);
	}
	else
	{
		m_spawnTetra.Reset();
		m_spawnTetra.Init();
		m_spawnTetra.PostInitGraphics(&pool, &m_board);

		for (int i = Consts::kiRows - 1; i >= 0; --i)
		{
			bool bBreakRow = true;

			for (int j = 0; j < Consts::kiCols; ++j)
			{
				bBreakRow &= cells[i][j].HasTile();
			}

			if (!bBreakRow)
			{
				continue;
			}

			for (int j = 0; j < Consts::kiCols; ++j)
			{
				bUpdateGravity = true;
				m_board.removeChild(cells[i][j].GetTile());
				pool.Free(cells[i][j].GetTile());
				cells[i][j].PlaceTile(nullptr);
			}
		}
	}

	if (!bUpdateGravity)
	{
		return;
	}
	
	for (int i = Consts::kiRows - 1; i >= 0; --i)
	{
		bool bRowHasUpdated = false;

		for (int j = 0; j < Consts::kiCols; ++j)
		{
			Cell currentCell = cells[i][j];

			if (currentCell.HasTile() && !currentCell.GravityTileFall(&cells, fDelta, CC_CALLBACK_0(Sha::Board::OnGravityFinish, this)))
			{
				const CellPos pos = cells[i][j].GetPos();

				currentCell.ApplyGravity(32.0f);
				const CellPos newPos = currentCell.GetPos();

				cells[newPos.m_row][newPos.m_col].SetTile(currentCell.GetTile());
				cells[pos.m_row][pos.m_col].PlaceTile(nullptr);

				currentCell.SetPos(pos);
				bRowHasUpdated = true;
			}
		}

		if (bRowHasUpdated)
		{
			return;
		}
		
	}


	bUpdateGravity = false;
}

void Sha::Board::OnGravityFinish()
{
}

void Sha::Board::Debug_OutputBoard()
{
	std::string output;

	for (int i = Consts::kiRows - 1; i >= 0; --i)
	{
		for (int j = 0; j < Consts::kiCols; ++j)
		{
			if (cells[i][j].HasTile())
			{
				output += " * ";
			}
			else
			{
				output += " - ";
			}
		}

		output += "\n";
	}

	cocos2d::log(output.c_str());
}


void Sha::Board::onEnter()
{
	cocos2d::Scene::onEnter();
	this->scheduleUpdate();
	return;
}

