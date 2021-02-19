#ifndef __FOOD_H__
#define __FOOD_H__

#include "Snake_types.h"
#include "Snake_fwd.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/2d/CCAction.h"
#include <string>
#include <functional>
#include "CategoricalDistribution.h"

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
		// TODO: refactor: really bad default ctor
		FoodDescription() :
			image_name(""),
			health(1),
			score(1),
			proba(0.0f),
			once(false),
			lifetime(-1)
		{}

		FoodDescription(const std::string& _image_name, int _health, int _score, float _proba, bool _once, float _lifetime=-1) :
			image_name(_image_name),
			health(_health),
			score(_score),
			proba(_proba),
			once(_once),
			lifetime(_lifetime),
			actionCallback([]() {return nullptr; })
		{
			if (proba < 0.0f || proba > 1.0f)
			{
				throw std::invalid_argument("proba value should be in range [0;1]");
			}
		}


		FoodDescription(const std::string& _image_name, int _health, int _score, float _proba, bool _once, float _lifetime,
			std::function<cocos2d::Action*()> action_callback) :
			image_name(_image_name),
			health(_health),
			score(_score),
			proba(_proba),
			once(_once),
			lifetime(_lifetime),
			actionCallback(action_callback)
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
		bool once;  // flag to generate this food only once per level
		float lifetime;  // in seconds. If <=0 then endless.
		std::function<cocos2d::Action* ()> actionCallback;
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
		void setPosition(float x, float y) { m_ccSprite->setPosition(cocos2d::Vec2(x, y)); }
		float getLifetime() const { return m_lifetime; }
		float getTimeElapsed() const { return m_timeElapsed; }

	private:
		cocos2d::Sprite* m_ccSprite;
		FoodType m_foodType;
		int m_health;
		int m_score;
		float m_lifetime;  // limit of lifetime in seconds
		float m_timeElapsed; // seconds since object was created
	};

	typedef std::map<FoodType, FoodDescription> FoodTable;


	class FoodFactory
	{
	public:
		FoodFactory(const FoodTable& food_table);

		FoodPtr makeRandom();	
	private:
		FoodTable m_foodTable;
		std::map<int, float> m_foodTypeProbas;
		CategoricalDistribution m_distr;
	};

}
#endif // __FOOD_H__