#ifndef __SNAKETYPES_H__
#define __SNAKETYPES_H__

namespace NS_Snake
{
	struct Point2d
	{
		Point2d(int xx, int yy) :
			x(xx), y(yy)
		{}
		int x;
		int y;
	};

}

#endif // __SNAKETYPES_H__