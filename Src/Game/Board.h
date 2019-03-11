#pragma once


#include "Src\Game\Cell.h"
#include "Src\Game\Consts.h"
#include "Src\Game\TilePool.h"
#include "Src\Game\TetraTile.h"

#include "cocos2d\cocos\2d\CCScene.h"


namespace cocos2d
{
	class Controller;
	class EventListenerController;
}

namespace Sha
{
	class CTile;

	class Board : public cocos2d::Scene
	{
	public:
		Board();
		~Board();

		virtual void update(float fDelta) override;
		virtual bool init() override;

		void OnGravityFinish();
		// Controller listener
		void onKeyDown(cocos2d::Controller* controller, int keycode, cocos2d::Event* event);
		void onKeyRepeat(cocos2d::Controller* controller, int keycode, cocos2d::Event* event);
		void onAxisEvent(cocos2d::Controller*, int value, cocos2d::Event* event);

	private:
		void InitCells();

	private:
		Cell					cells[Consts::kiRows][Consts::kiCols];
		TilePool				pool;
		// Input
		cocos2d::EventListenerController*	m_inputListener;
		std::list<CTile*>		m_spawnTiles;

		// Debug
		Cell					m_spawnCell;
		TetraTile				m_spawnTetra;

		float m_InputX;
		float m_InputY;
		float m_InputTriggerLeft;
		
	};
}