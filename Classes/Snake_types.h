#ifndef __SNAKETYPES_H__
#define __SNAKETYPES_H__

#include "cocos\math\Vec2.h"

namespace NS_Snake
{
	struct Point2d
	{
		Point2d(int xx, int yy) :
			x(xx), y(yy)
		{}
		int x;
		int y;

		cocos2d::Vec2 toVec2() { return cocos2d::Vec2(x, y); }
		static Point2d fromVec2(const cocos2d::Vec2& vec) { return Point2d(vec.x, vec.y); }
	};

}

#endif // __SNAKETYPES_H__