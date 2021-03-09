#include "IFood.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <memory>
#include "cocos2d.h"
#include "GameGrid.h"


namespace NS_Snake
{
	IFood::IFood(FoodType ft, FoodSubType fst, GameGridPtr grid, int health, int score, int lifetime) :
		m_foodType(ft),
		m_foodSubType(static_cast<FoodSubType>(fst)),
		m_grid(grid),
		m_health(health),
		m_score(score),
		m_lifetime(lifetime),
		m_timeElapsed(0)
	{}

	StaticFood::StaticFood(StaticFoodType ft, const StaticFoodDescription& fd, GameGridPtr grid) :
		IFood(FoodType::STATIC, static_cast<FoodSubType>(ft), grid, fd.health, fd.score, fd.lifetime)
	{
		m_ccSprite = cocos2d::Sprite::createWithSpriteFrameName(fd.image_name);
		if (!m_ccSprite)
		{
			throw std::runtime_error("failed to create Food by sprite frame name: " + fd.image_name);
		}

		auto action = fd.actionCallback();
		if (action)
			m_ccSprite->runAction(action);

		if (m_lifetime)
		{
			m_ccSprite->schedule([this](float dt) { m_timeElapsed++; }, 1.0f, "FoodTimer");
		}
	}

	StaticFood::~StaticFood()
	{
		auto food_pos = Point2d::fromVec2(m_ccSprite->getPosition());
		auto food_cell = m_grid->xyToCell(food_pos);
		m_grid->releaseCell(food_cell);

		if (m_ccSprite)
		{
			m_ccSprite->removeFromParent();
		}

	}


	MovingFood::MovingFood(MovingFoodType ft, const MovingFoodDescription& fd, GameGridPtr grid) :
		IFood(FoodType::MOVING, static_cast<FoodSubType>(ft), grid, fd.health, fd.score, fd.lifetime),
		m_dirSprite(std::make_unique<DirectedSprite>(fd.dtfTable)),
		m_moveDistr(TypeToProbasMap({ {0, 1.0 - fd.moveProba}, {1, fd.moveProba} })),
		m_chooseDirectionDistr(TypeToProbasMap({ {SPRITE_DIRECTION::UP, 1.0}, 
												 {SPRITE_DIRECTION::DOWN, 1.0}, 
												 {SPRITE_DIRECTION::RIGHT, 1.0}, 
												 {SPRITE_DIRECTION::LEFT, 1.0} })),
		m_moveActionCallback(fd.moveActionCallback),
		m_moveDuration(fd.moveDuration)
	{
		if (fd.idleActionCallback)
			getSprite()->runAction(fd.idleActionCallback());

		if (m_lifetime)
			getSprite()->schedule([this](float dt) { m_timeElapsed++; }, 1.0f, "FoodTimer");

		getSprite()->schedule([this](float dt) { moveCallback(dt); }, m_moveDuration, "MoveCallback");
	}

	MovingFood::MovingFood(const MovingFood& other) :
		IFood(FoodType::MOVING, static_cast<FoodSubType>(other.getFoodSubType()), other.getGameGrid(), 
			other.getHealth(), other.getScore(), other.getLifetime()),
		m_dirSprite(std::make_unique<DirectedSprite>(other.getDirectedSprite())),
		m_moveDistr(other.getMoveDistr()),
		m_chooseDirectionDistr(other.getChooseDirectionDistr()),
		m_moveActionCallback(other.getMoveActionCallback()),
		m_moveDuration(other.getMoveDuration())
	{
		if (m_lifetime)
			getSprite()->schedule([this](float dt) { m_timeElapsed++; }, 1.0f, "FoodTimer");

		getSprite()->schedule([this](float dt) { moveCallback(dt); }, 0.5f, "MoveCallback");
	}

	MovingFood::~MovingFood()
	{
		auto food_pos = Point2d::fromVec2(getSprite()->getPosition());
		auto food_cell = m_grid->xyToCell(food_pos);
		m_grid->releaseCell(food_cell);

	}

	void MovingFood::update()
	{
	}

	void MovingFood::moveCallback(float dt)
	{
		// proba not to make movement
		if (m_moveDistr.drawOnce() == 0)
			return;

		auto food_pos = Point2d::fromVec2(getSprite()->getPosition());
		auto food_cell = m_grid->xyToCell(food_pos);

		int shift_cix = 0;
		int shift_ciy = 0;
		auto new_dir = static_cast<SPRITE_DIRECTION>(m_chooseDirectionDistr.drawOnce());
		switch (new_dir)
		{
		case SPRITE_DIRECTION::UP:
			shift_ciy = 1;
			break;
		case SPRITE_DIRECTION::DOWN:
			shift_ciy = -1;
			break;
		case SPRITE_DIRECTION::RIGHT:
			shift_cix = 1;
			break;
		case SPRITE_DIRECTION::LEFT:
			shift_cix = -1;
			break;
		}
		auto new_cell = GameGrid::Cell(food_cell.cix + shift_cix, food_cell.ciy + shift_ciy);
		new_cell = m_grid->boundToRect(new_cell);
		if (m_grid->isCellOccupied(new_cell))
			return;
		m_grid->releaseCell(food_cell);
		m_grid->occupyCell(new_cell);

		if (m_moveActionCallback)
			getSprite()->runAction(m_moveActionCallback(m_moveDuration, m_grid->cellToXyCenter(new_cell).toVec2()));

		m_dirSprite->setDirPair({ new_dir, new_dir });
		m_dirSprite->update();
	}

	MovingFoodDescription::MovingFoodDescription(const MovingFoodDescription& other):
		dtfTable(other.dtfTable),
		health(other.health),
		score(other.score),
		once(other.once),
		lifetime(other.lifetime),
		idleActionCallback(other.idleActionCallback),
		moveActionCallback(other.moveActionCallback),
		moveProba(other.moveProba),
		moveDuration(other.moveDuration)
	{}


	IFoodFactory::IFoodFactory(const TypeToProbasMap& probas, GameGridPtr grid) :
		m_foodTypeProbas(probas),
		m_distr(probas),
		m_grid(grid)
	{}


	StaticFoodFactory::StaticFoodFactory(const TypeToProbasMap& probas, const StaticFoodTable& food_table, GameGridPtr grid) :
		IFoodFactory(probas, grid),
		m_foodTable(food_table)
	{
	}


	IFoodPtr StaticFoodFactory::makeRandom()
	{
		int index = m_distr.drawOnce();  // get random index according to probas of food types
		auto ft = static_cast<StaticFoodType>(index);
		auto fd = m_foodTable[ft];
		if (fd.once)
		{
			StaticFoodTable::iterator ft_iter = m_foodTable.find(ft);
			m_foodTable.erase(ft_iter);
			TypeToProbasMap::iterator fp_iter = m_foodTypeProbas.find(static_cast<int>(ft));
			m_foodTypeProbas.erase(fp_iter);
			m_distr = CategoricalDistribution(m_foodTypeProbas);
		}
		return std::make_unique<StaticFood>(ft, fd, m_grid);
	}


	MovingFoodFactory::MovingFoodFactory(const TypeToProbasMap& probas, const MovingFoodTable& food_table, GameGridPtr grid) :
		IFoodFactory(probas, grid),
		m_foodTable(food_table)
	{
	}

	IFoodPtr MovingFoodFactory::makeRandom()
	{
		int index = m_distr.drawOnce();  // get random index according to probas of food types
		auto ft = static_cast<MovingFoodType>(index);
		auto fd = m_foodTable[ft];
		if (fd.once)
		{
			MovingFoodTable::iterator ft_iter = m_foodTable.find(ft);
			m_foodTable.erase(ft_iter);
			TypeToProbasMap::iterator fp_iter = m_foodTypeProbas.find(static_cast<int>(ft));
			m_foodTypeProbas.erase(fp_iter);
			m_distr = CategoricalDistribution(m_foodTypeProbas);
		}
		return std::make_unique<MovingFood>(ft, fd, m_grid);
	}

	
	FoodGenerator::FoodGenerator(const FoodTable& t, GameGridPtr grid) :
		m_foodTable(t),
		m_staticFoodFactory(std::make_unique<StaticFoodFactory>(t.staticFoodProbas, t.staticFoodTable, grid)),
		m_movingFoodFactory(std::make_unique<MovingFoodFactory>(t.movingFoodProbas, t.movingFoodTable, grid)),
		m_foodTypeDistr(t.foodTypeProbas),
		m_grid(grid)
	{}

	IFoodPtr FoodGenerator::makeRandom()
	{
		int index = static_cast<FoodType>(m_foodTypeDistr.drawOnce());
		switch (index)
		{
		case FoodType::STATIC:
			return m_staticFoodFactory->makeRandom();
		case FoodType::MOVING:
			return m_movingFoodFactory->makeRandom();
		default:
			return nullptr;
		}
	}
	
}