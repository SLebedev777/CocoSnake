#include <memory>

namespace NS_Snake
{
	class DirectedSprite;
	typedef std::unique_ptr<DirectedSprite> DirectedSpritePtr;

	class Snake;
	typedef std::unique_ptr<Snake> SnakePtr;

	class GameGrid;
	typedef std::unique_ptr<GameGrid> GameGridPtr;
}