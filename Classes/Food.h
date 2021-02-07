#ifndef __FOOD_H__
#define __FOOD_H__

#include "Snake_types.h"
#include "Snake_fwd.h"
#include "cocos/2d/CCSprite.h"
#include <string>

namespace NS_Snake
{
	enum class FoodType
	{
		APPLE = 0,
		BANANA,
		MUSHROOM,
		ANANAS,
		POTION,
		PORTAL
	};

	struct FoodDescription
	{
		// really bad default ctor
		FoodDescription() :
			image_name(""),
			health(1),
			score(1),
			proba(0.0f)
		{}
		FoodDescription(const std::string& _image_name, int _health, int _score, float _proba) :
			image_name(_image_name),
			health(_health),
			score(_score),
			proba(_proba)
		{
			if (proba < 0.0f || proba > 1.0f)
			{
				throw std::invalid_argument("proba value should be in range [0;1]");
			}
		}

		std::string image_name;
		int health;
		int score;
		float proba;
	};


	class Food
	{
	public:
		Food(FoodType ft, const FoodDescription& food_descr);
		Food(const Food& other);
		Food& operator=(const Food& other);
		~Food();

		FoodType getFoodType() const { return m_foodType; }
		int getHealth() const { return m_health; }
		int getScore() const { return m_score; }
		cocos2d::Sprite* getSprite() const { return m_ccSprite; }
		void update() {}
		Point2d getPosition() const { const auto pos = m_ccSprite->getPosition(); return Point2d(pos.x, pos.y); }
		void setPosition(Point2d& pos) { m_ccSprite->setPosition(cocos2d::Vec2(pos.x, pos.y)); }
		void setPosition(int x, int y) { m_ccSprite->setPosition(cocos2d::Vec2(x, y)); }

	private:
		cocos2d::Sprite* m_ccSprite;
		FoodType m_foodType;
		int m_health;
		int m_score;
	};

	typedef std::map<FoodType, FoodDescription> FoodTable;


	class FoodFactory
	{
	public:
		FoodFactory(const FoodTable& food_table):
			m_foodTable(food_table)
		{}

		FoodPtr makeRandom();
	private:
		FoodTable m_foodTable;
	};

}
#endif // __FOOD_H__