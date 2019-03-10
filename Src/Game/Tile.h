#pragma once

#include "cocos2d\cocos\2d\CCNode.h"


namespace cocos2d
{
	class Sprite;
	class Renderer;
	class Mat4;
}

namespace Sha
{


	class CTile : public cocos2d::Node
	{
	public:

		enum ETile
		{
			GREEN = 0,
			RED,
			YELLOW,
			PURPLE,

			GREEN_GLOSSY,
			RED_GLOSSY,
			YELLOW_GLOSSY,
			PURPLE_GLOSSY,

			ETILECOUNT
		};

		CTile();
		~CTile();

		void SetType(ETile m_eType);

		// Node
		virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
		virtual bool init() override;

		void reset();

	private:
		std::string GetTileType(ETile tile) const;

	private:
		cocos2d::Sprite*	m_sprite;
		ETile				m_eType;
	};
}