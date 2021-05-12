#include "DirectedSprite.h"
#include "cocos/2d/CCSpriteFrameCache.h"
#include "cocos/math/CCGeometry.h"
#include <stdexcept>

namespace NS_Snake
{
	SPRITE_DIRECTION oppositeDir(SPRITE_DIRECTION dir)
	{
		switch (dir)
		{
		case SPRITE_DIRECTION::NONE:
			return SPRITE_DIRECTION::NONE;
			break;
		case SPRITE_DIRECTION::UP:
			return SPRITE_DIRECTION::DOWN;
			break;
		case SPRITE_DIRECTION::DOWN:
			return SPRITE_DIRECTION::UP;
			break;
		case SPRITE_DIRECTION::RIGHT:
			return SPRITE_DIRECTION::LEFT;
			break;
		case SPRITE_DIRECTION::LEFT:
			return SPRITE_DIRECTION::RIGHT;
			break;
		default:
			return SPRITE_DIRECTION::NONE;
			break;
		}
	}


	DirToFrameTable dirToFrameTemplate(const std::string& image_name)
	{
		DirToFrameTable table;
		auto idle_frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(image_name);
		table.insert(std::pair<DirectionPair, SpriteFrameTransform>(DIRECTION_PAIR_NONE, SpriteFrameTransform(idle_frame)));
		table.insert(std::pair<DirectionPair, SpriteFrameTransform>(DIRECTION_PAIR_UP, SpriteFrameTransform(idle_frame)));
		table.insert(std::pair<DirectionPair, SpriteFrameTransform>(DIRECTION_PAIR_RIGHT, SpriteFrameTransform(idle_frame, 90, false, false)));
		table.insert(std::pair<DirectionPair, SpriteFrameTransform>(DIRECTION_PAIR_DOWN, SpriteFrameTransform(idle_frame, 180, false, false)));
		table.insert(std::pair<DirectionPair, SpriteFrameTransform>(DIRECTION_PAIR_LEFT, SpriteFrameTransform(idle_frame, 270, false, false)));

		return table;
	}


	DirectedSprite::DirectedSprite(const DirToFrameTable& table) :
		m_dirToFrameTable(table),
		m_ccSprite(nullptr),
		m_dirPair(DIRECTION_PAIR_NONE)
	{
		if (m_dirToFrameTable.empty())
			throw std::invalid_argument("Empty DirToImageTable");
		if (m_dirToFrameTable.find(DIRECTION_PAIR_NONE) == m_dirToFrameTable.end())
			throw std::invalid_argument("DirToImageTable must contain idle image for direction = none");

		SpriteFrameTransform idle_sft = m_dirToFrameTable[DIRECTION_PAIR_NONE];
		m_ccSprite = cocos2d::Sprite::createWithSpriteFrame(idle_sft.frame);
	}


	DirectedSprite::DirectedSprite(const DirectedSprite& other) :
		m_dirToFrameTable(other.getTable()),
		m_ccSprite(cocos2d::Sprite::createWithSpriteFrame(other.getSprite()->getSpriteFrame())),
		m_dirPair(DIRECTION_PAIR_NONE)
	{
	}

	DirectedSprite& DirectedSprite::operator=(const DirectedSprite& other)
	{
		if (this == &other)
			return *this;
		m_dirToFrameTable = other.getTable();
		this->setDirPair(other.getDirPair());
		this->update();
		return *this;
	}

	DirectedSprite::~DirectedSprite()
	{
		m_ccSprite->removeFromParent();
	}

	void DirectedSprite::update()
	{
		if (m_dirToFrameTable.find(m_dirPair) == m_dirToFrameTable.end())
			return;
		m_ccSprite->setSpriteFrame(m_dirToFrameTable[m_dirPair].frame);
		m_ccSprite->setRotation(m_dirToFrameTable[m_dirPair].rotAngle);
		m_ccSprite->setFlippedX(m_dirToFrameTable[m_dirPair].flipX);
		m_ccSprite->setFlippedY(m_dirToFrameTable[m_dirPair].flipY);
	}

	Point2d DirectedSprite::getFromPosition() const
	{
		int x = getPosition().x;
		int y = getPosition().y;
		cocos2d::Rect rect = getSprite()->getBoundingBox();

		switch (getDirFrom())
		{
		case SPRITE_DIRECTION::UP:
			y = rect.getMinY();
			break;
		case SPRITE_DIRECTION::DOWN:
			y = rect.getMaxY();
			break;
		case SPRITE_DIRECTION::RIGHT:
			x = rect.getMinX();
			break;
		case SPRITE_DIRECTION::LEFT:
			x = rect.getMaxX();
			break;
		case SPRITE_DIRECTION::NONE:
		default:
			break;
		}

		return Point2d(x, y);
	}

	Point2d DirectedSprite::getToPosition() const
	{
		int x = getPosition().x;
		int y = getPosition().y;
		cocos2d::Rect rect = getSprite()->getBoundingBox();

		switch (getDirTo())
		{
		case SPRITE_DIRECTION::UP:
			y = rect.getMaxY();
			break;
		case SPRITE_DIRECTION::DOWN:
			y = rect.getMinY();
			break;
		case SPRITE_DIRECTION::RIGHT:
			x = rect.getMaxX();
			break;
		case SPRITE_DIRECTION::LEFT:
			x = rect.getMinX();
			break;
		case SPRITE_DIRECTION::NONE:
		default:
			break;
		}

		return Point2d(x, y);
	}

}