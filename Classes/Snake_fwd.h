#include <memory>

namespace NS_Snake
{
	class DirectedSprite;
	typedef std::unique_ptr<DirectedSprite> DirectedSpritePtr;

	class Snake;
	typedef std::unique_ptr<Snake> SnakePtr;

	class GameGrid;
	typedef std::shared_ptr<GameGrid> GameGridPtr;

	class IFood;
	typedef std::unique_ptr<IFood> IFoodPtr;

	class IFoodFactory;
	typedef std::unique_ptr<IFoodFactory> IFoodFactoryPtr;

	class FoodGenerator;
	typedef std::unique_ptr<FoodGenerator> FoodGeneratorPtr;

}