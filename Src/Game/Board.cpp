#include "Src/Game/Board.h"
#include "base/CCController.h"
#include "base/CCConsole.h"

#include "cocos2d/cocos/base/CCEventDispatcher.h"
#include "cocos2d/cocos/base/CCEventListenerController.h"

Sha::Board::Board()
: Scene()
, m_InputX(0.0f)
, m_InputY(0.0f)
, m_spawnTiles()
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
}

void  Sha::Board::onKeyDown(cocos2d::Controller* controller, int keycode, cocos2d::Event* event)
{
	cocos2d::log("Key pressed !");

	if (keycode == 0)
	{
		m_spawnTiles.push_back(pool.Alloc());
	}
}

void Sha::Board::onKeyRepeat(cocos2d::Controller* controller, int keycode, cocos2d::Event* event)
{
	cocos2d::log("Key repeat !");
}

void Sha::Board::onAxisEvent(cocos2d::Controller* controller, int value, cocos2d::Event* event)
{
	const cocos2d::Controller::KeyStatus& keyStatus = controller->getKeyStatus(value);

	if (value == 0)
	{
		m_InputX = keyStatus.value;
	}
	else if (value == 1)
	{
		m_InputY = keyStatus.value;
	}

	cocos2d::log("On axis moved! value : %d, keystatus %d, %f", value, keyStatus.isPressed, keyStatus.value);
}

void Sha::Board::update(float fDelta)
{
	Scene::update(fDelta);

	if (!m_spawnTiles.empty())
	{
		cells[Consts::kiRows - 1][Consts::kiCols - 1].SetTile(m_spawnTiles.front());
		m_gravityTiles.push_back(m_spawnTiles.front());
		addChild(m_spawnTiles.front());
		m_spawnTiles.pop_front();
	}

	if (!m_gravityTiles.empty())
	{

	}
}
