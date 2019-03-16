#include "Tile.h"

#include "cocos2d\cocos\2d\CCSprite.h"
#include "cocos2d\cocos\2d\CCSpriteFrameCache.h"


Sha::CTile::CTile()
: Node()
, m_eType(ETILECOUNT)
, m_sprite(nullptr)
{
	reset();
}

Sha::CTile::~CTile()
{

}

void Sha::CTile::SetType(ETile eType)
{
	reset();
	m_eType = eType;

	if (m_sprite == nullptr)
	{
		m_sprite = cocos2d::Sprite::createWithSpriteFrameName(GetTileType(m_eType));
		this->addChild(m_sprite, 0);
	}
}

void Sha::CTile::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);
}

bool Sha::CTile::init()
{
	Node::init();
	SetType(YELLOW_GLOSSY);
	return true;
}

void Sha::CTile::reset()
{
	m_eType = ETILECOUNT;
	if (m_sprite)
	{
		this->removeChild(m_sprite);
		m_sprite = nullptr;
	}	
}

std::string Sha::CTile::GetTileType(Sha::CTile::ETile tile) const
{
	switch (tile)
	{
		case GREEN:
			return "element_green_square.png";
		case RED:
			return "element_red_square.png";
		case YELLOW:
			return "element_yellow_square.png";
		case PURPLE:
			return "element_purple_square.png";
		case RED_GLOSSY:
			return "element_red_square_glossy.png";
		case YELLOW_GLOSSY:
			return "element_yellow_square_glossy.png";
		case GREEN_GLOSSY:
			return "element_green_square_glossy.png";
		case PURPLE_GLOSSY:
			return "element_purple_cube_glossy.png";
		case IDLE:
			return "element_neutral_square.png";
		defaut:
			assert(false);
			break;
	}
	return "";
}