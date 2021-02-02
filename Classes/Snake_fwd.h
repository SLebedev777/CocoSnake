#include <memory>

namespace NS_Snake
{
	class DirectedSprite;
	typedef std::unique_ptr<DirectedSprite> DirectedSpritePtr;

	class Snake;
	typedef std::unique_ptr<Snake> SnakePtr;

	class GameGrid;
	typedef std::shared_ptr<GameGrid> GameGridPtr;

	class Food;
	typedef std::unique_ptr<Food> FoodPtr;

	class FoodFactory;
	typedef std::unique_ptr<FoodFactory> FoodFactoryPtr;
}