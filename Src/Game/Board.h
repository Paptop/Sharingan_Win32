#pragma once

#include "cocos2d\cocos\2d\CCNode.h"

namespace Sha
{
	class CTile;

	class Board : public cocos2d::Node
	{
	public:
		Board();
		~Board();

		virtual bool init() override;
	private:
		std::vector<CTile*>	 m_boardTiles;


	};
}