#ifndef __DIRECTEDSPRITE_H__
#define __DIRECTEDSPRITE_H__

#include "cocos2d.h"
#include <map>


namespace NS_Snake
{
	enum class SPRITE_DIRECTION
	{
		NONE = 0,
		UP = 1,
		DOWN = -1,
		RIGHT = 2,
		LEFT = -2
	};

	/* Struct that holds sprite frame and transformation parameters that can be applied to it later */
	struct SpriteFrameTransform
	{   
		SpriteFrameTransform() :
			frame(nullptr),
			rotAngle(0),
			flipX(false),
			flipY(false)
		{}
		explicit SpriteFrameTransform(cocos2d::SpriteFrame* fr, int rot=0, bool flX=false, bool flY=false) :
			frame(fr),
			rotAngle(rot),
			flipX(flX),
			flipY(flY)
		{}

		cocos2d::SpriteFrame* frame;
		int rotAngle;
		bool flipX;
		bool flipY;
	};

	// pair (direction from, direction to)
	typedef std::pair<SPRITE_DIRECTION, SPRITE_DIRECTION> DirectionPair;
	// mapping direction pair to transformed sprite frame
	typedef std::map<DirectionPair, SpriteFrameTransform>  DirToFrameTable;

	struct Point2d
	{
		Point2d(int xx, int yy) : 
			x(xx), y(yy)
		{}
		int x;
		int y;
	};

	static const DirectionPair DIRECTION_PAIR_NONE({ SPRITE_DIRECTION::NONE, SPRITE_DIRECTION::NONE });
	static const DirectionPair DIRECTION_PAIR_UP({ SPRITE_DIRECTION::UP, SPRITE_DIRECTION::UP });
	static const DirectionPair DIRECTION_PAIR_DOWN({ SPRITE_DIRECTION::DOWN, SPRITE_DIRECTION::DOWN });
	static const DirectionPair DIRECTION_PAIR_RIGHT({ SPRITE_DIRECTION::RIGHT, SPRITE_DIRECTION::RIGHT });
	static const DirectionPair DIRECTION_PAIR_LEFT({ SPRITE_DIRECTION::LEFT, SPRITE_DIRECTION::LEFT });

	// create table with transformed frames for basic directions: up, down, right, left, without kinks 
	DirToFrameTable dirToFrameTemplate(const std::string& image_name);

	// Sprite that holds 2 directions: from and to, and is able to change its frames according to (dir_from, dir_to) pair
	class DirectedSprite
	{
	public:
		DirectedSprite(DirToFrameTable& table);
		DirectedSprite(DirectedSprite& other);
		DirectedSprite& operator=(DirectedSprite& other);
		~DirectedSprite();

		DirToFrameTable getTable() { return m_dirToFrameTable; }  // refactor to ref
		cocos2d::Sprite* getSprite() const { return m_ccSprite; }
		void setDirFrom(SPRITE_DIRECTION from) { m_dirPair.first = from; }
		void setDirTo(SPRITE_DIRECTION to) { m_dirPair.second = to; }
		void setDirPair(DirectionPair pair) { m_dirPair = pair; }
		SPRITE_DIRECTION getDirFrom() const { return m_dirPair.first; }
		SPRITE_DIRECTION getDirTo() const { return m_dirPair.second; }
		DirectionPair getDirPair() const { return m_dirPair; }
		void setIdle() { setDirPair(DIRECTION_PAIR_NONE); }
		void update();  // updates frame by table, according to set direction
		Point2d& getPosition() const { const auto pos = m_ccSprite->getPosition(); return Point2d(pos.x, pos.y); }
		void setPosition(Point2d& pos) { m_ccSprite->setPosition(cocos2d::Vec2(pos.x, pos.y)); }

	private:
		cocos2d::Sprite* m_ccSprite;
		DirToFrameTable m_dirToFrameTable;
		DirectionPair m_dirPair;
	};

}

#endif  // __DIRECTEDSPRITE_H__
