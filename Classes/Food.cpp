#include "Food.h"
#include <stdexcept>
#include <ctime>
#include <vector>
#include "cocos2d.h"

namespace NS_Snake
{
	Food::Food(FoodType ft, const FoodDescription& food_descr) :
		m_foodType(ft),
		m_health(food_descr.health),
		m_score(food_descr.score),
		m_lifetime(food_descr.lifetime),
		m_timeElapsed(0)
	{
		m_ccSprite = cocos2d::Sprite::createWithSpriteFrameName(food_descr.image_name);
		if (!m_ccSprite)
		{
			throw std::runtime_error("failed to create Food by sprite frame name: " + food_descr.image_name);
		}

		auto action = food_descr.actionCallback();
		if (action)
			m_ccSprite->runAction(action);

		if (m_lifetime)
		{
			m_ccSprite->schedule([this](float dt) { m_timeElapsed++; }, 1.0f, "FoodTimer");
		}
	}

	Food::Food(const Food& other) :
		m_foodType(other.getFoodType()),
		m_health(other.getHealth()),
		m_score(other.getScore()),
		m_ccSprite(cocos2d::Sprite::createWithSpriteFrame(other.getSprite()->getSpriteFrame())),
		m_lifetime(other.m_lifetime),
		m_timeElapsed(0)
	{
		auto parent = other.getSprite()->getParent();
		if (parent)
		{
			parent->addChild(m_ccSprite);
		}

		if (m_lifetime)
		{
			m_ccSprite->schedule([this](float dt) { m_timeElapsed++; }, 1.0f, "FoodTimer");
		}

	}

	Food& Food::operator=(const Food& other)
	{
		if (this == &other)
			return *this;

		this->m_foodType = other.getFoodType();
		this->m_health = other.getHealth();
		this->m_score = other.getScore();
		this->m_lifetime = other.getLifetime();
		// TODO: m_timeElapsed ??? reset to 0 or remain as was in this?
		m_ccSprite->release();
		m_ccSprite = cocos2d::Sprite::createWithSpriteFrame(other.getSprite()->getSpriteFrame());

		auto parent = other.getSprite()->getParent();
		if (parent)
		{
			parent->addChild(m_ccSprite);
		}
		// TODO: Warning: what if food had not time limit, but assigned to it a copy of another food with time limit?
		if (m_lifetime)
		{
			m_ccSprite->schedule([this](float dt) { m_timeElapsed++; }, 1.0f, "FoodTimer");
		}


		return *this;
	}

	Food::~Food()
	{
		if (m_ccSprite)
		{
			m_ccSprite->removeFromParent();
		}
	}


	FoodFactory::FoodFactory(const FoodTable& food_table) :
		m_foodTable(food_table)
	{
		int i = 0;
		for (const auto& it : m_foodTable)
		{
			m_foodTypeProbas.insert({i, it.second.proba });
			i++;
		}
		m_distr = CategoricalDistribution(m_foodTypeProbas);
	}


	FoodPtr FoodFactory::makeRandom()
	{
		std::vector<FoodType> keys;
		for (const auto& it : m_foodTable)
			keys.push_back(it.first);
		int index = m_distr.drawOnce();  // get random index according to probas of food types
		FoodType ft = keys[index];
		FoodDescription fd = m_foodTable[ft];
		if (fd.once)
		{
			FoodTable::iterator ft_iter = m_foodTable.find(ft);
			m_foodTable.erase(ft_iter);
			m_foodTypeProbas.clear();
			int i = 0;
			for (const auto& it : m_foodTable)
			{
				m_foodTypeProbas.insert({ i, it.second.proba });
				i++;
			}
			m_distr = CategoricalDistribution(m_foodTypeProbas);
		}
		return std::make_unique<Food>(ft, fd);
	}
}