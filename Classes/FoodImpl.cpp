#include "IFood.h"
#include <stdexcept>
#include <ctime>
#include <vector>
#include <memory>
#include "cocos2d.h"

namespace NS_Snake
{
	IFood::IFood(FoodType ft, FoodSubType fst, int health, int score, int lifetime) :
		m_foodType(ft),
		m_foodSubType(static_cast<FoodSubType>(fst)),
		m_health(health),
		m_score(score),
		m_lifetime(lifetime),
		m_timeElapsed(0)
	{}

	StaticFood::StaticFood(StaticFoodType ft, const StaticFoodDescription& fd) :
		IFood(FoodType::STATIC, static_cast<FoodSubType>(ft), fd.health, fd.score, fd.lifetime)
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
		if (m_ccSprite)
		{
			m_ccSprite->removeFromParent();
		}
	}


	MovingFood::MovingFood(MovingFoodType ft, const MovingFoodDescription& fd) :
		IFood(FoodType::MOVING, static_cast<FoodSubType>(ft), fd.health, fd.score, fd.lifetime)
	{
	}

	MovingFood::~MovingFood()
	{
	}


	IFoodFactory::IFoodFactory(const TypeToProbasMap& probas) :
		m_foodTypeProbas(probas),
		m_distr(probas)
	{}


	StaticFoodFactory::StaticFoodFactory(const TypeToProbasMap& probas, const StaticFoodTable& food_table) :
		IFoodFactory(probas),
		m_foodTable(food_table)
	{
	}


	IFoodPtr StaticFoodFactory::makeRandom()
	{
		std::vector<StaticFoodType> keys;
		for (const auto& it : m_foodTable)
			keys.push_back(it.first);
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
		return std::make_unique<StaticFood>(ft, fd);
	}


	MovingFoodFactory::MovingFoodFactory(const TypeToProbasMap& probas, const MovingFoodTable& food_table) :
		IFoodFactory(probas),
		m_foodTable(food_table)
	{
	}


	IFoodPtr MovingFoodFactory::makeRandom()
	{
		std::vector<MovingFoodType> keys;
		for (const auto& it : m_foodTable)
			keys.push_back(it.first);
		int index = m_distr.drawOnce();  // get random index according to probas of food types
		auto ft = keys[index];
		auto fd = m_foodTable[ft];
		if (fd.once)
		{
			MovingFoodTable::iterator ft_iter = m_foodTable.find(ft);
			m_foodTable.erase(ft_iter);
			TypeToProbasMap::iterator fp_iter = m_foodTypeProbas.find(static_cast<int>(ft));
			m_foodTypeProbas.erase(fp_iter);
			m_distr = CategoricalDistribution(m_foodTypeProbas);
		}
		return std::make_unique<MovingFood>(ft, fd);
	}

	
	FoodGenerator::FoodGenerator(const FoodTable& t) :
		m_foodTable(t),
		m_staticFoodFactory(std::make_unique<StaticFoodFactory>(t.staticFoodProbas, t.staticFoodTable)),
		m_movingFoodFactory(std::make_unique<MovingFoodFactory>(t.movingFoodProbas, t.movingFoodTable)),
		m_foodTypeDistr(t.foodTypeProbas)
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