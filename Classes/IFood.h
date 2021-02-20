#ifndef __IFOOD_H__
#define __IFOOD_H__

#include "Snake_types.h"
#include "Snake_fwd.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/2d/CCAction.h"
#include <string>
#include <functional>
#include "CategoricalDistribution.h"
#include "DirectedSprite.h"

namespace NS_Snake
{
	enum FoodType
	{
		STATIC = 0,
		MOVING
	};

	typedef int FoodSubType;

	enum StaticFoodType
	{
		APPLE = 0,
		BANANA,
		MUSHROOM,
		ANANAS,
		POTION,
		PORTAL
	};

	enum MovingFoodType
	{
		BUTTERFLY = 0,
		BIRD,
		MOUSE,
		LIZARD
	};

	/// <summary>
	/// food description structs
	/// </summary>
	struct StaticFoodDescription
	{
		// TODO: refactor: really bad default ctor
		StaticFoodDescription() :
			image_name(""),
			health(1),
			score(1),
			once(false),
			lifetime(-1)
		{}

		StaticFoodDescription(const std::string& _image_name, int _health, int _score, bool _once, float _lifetime=-1) :
			image_name(_image_name),
			health(_health),
			score(_score),
			once(_once),
			lifetime(_lifetime),
			actionCallback([]() {return nullptr; })
		{}


		StaticFoodDescription(const std::string& _image_name, int _health, int _score, bool _once, float _lifetime,
			std::function<cocos2d::Action*()> action_callback) :
			image_name(_image_name),
			health(_health),
			score(_score),
			once(_once),
			lifetime(_lifetime),
			actionCallback(action_callback)
		{
		}

		std::string image_name;
		int health;
		int score;
		bool once;  // flag to generate this food only once per level
		float lifetime;  // in seconds. If <=0 then endless.
		std::function<cocos2d::Action* ()> actionCallback;
	};

	struct MovingFoodDescription
	{
		MovingFoodDescription() :
			health(1),
			score(1),
			once(false),
			lifetime(-1)
		{}

		MovingFoodDescription(DirToFrameTable& _dir_to_frame_table, int _health, int _score, bool _once, float _lifetime = -1) :
			m_dtfTable(_dir_to_frame_table),
			health(_health),
			score(_score),
			once(_once),
			lifetime(_lifetime),
			actionCallback([]() {return nullptr; })
		{}

		MovingFoodDescription(const MovingFoodDescription& other);

		DirToFrameTable m_dtfTable;
		int health;
		int score;
		bool once;  // flag to generate this food only once per level
		float lifetime;  // in seconds. If <=0 then endless.
		std::function<cocos2d::Action* ()> actionCallback;
	};


	/// <summary>
	/// IFood base and derived
	/// </summary>
	class IFood
	{
	public:
		IFood(FoodType ft, FoodSubType fst, GameGridPtr grid, int health, int score, int lifetime);
		virtual ~IFood() {}

		virtual cocos2d::Sprite* getSprite() const = 0;
		virtual void update() = 0;

		FoodType getFoodType() const { return m_foodType; }
		FoodSubType getFoodSubType() const { return m_foodSubType; }
		int getHealth() const { return m_health; }
		int getScore() const { return m_score; }
		Point2d getPosition() const { const auto pos = getSprite()->getPosition(); return Point2d(pos.x, pos.y); }
		void setPosition(Point2d& pos) { getSprite()->setPosition(cocos2d::Vec2(pos.x, pos.y)); }
		void setPosition(float x, float y) { getSprite()->setPosition(cocos2d::Vec2(x, y)); }
		float getLifetime() const { return m_lifetime; }
		float getTimeElapsed() const { return m_timeElapsed; }
		GameGridPtr getGameGrid() const { return m_grid; }

	protected:
		GameGridPtr m_grid;
		FoodType m_foodType;  // static or moving
		FoodSubType m_foodSubType;
		int m_health;
		int m_score;
		float m_lifetime;  // limit of lifetime in seconds
		float m_timeElapsed; // seconds since object was created
	};


	class StaticFood : public IFood
	{
	public:
		StaticFood(StaticFoodType ft, const StaticFoodDescription& fd, GameGridPtr grid);
		// Rule of Three: make copy ctor and operator=
		cocos2d::Sprite* getSprite() const override { return m_ccSprite; }
		void update() override {}
		~StaticFood();

	private:
		cocos2d::Sprite* m_ccSprite;
	};


	class MovingFood : public IFood
	{
	public:
		MovingFood(MovingFoodType ft, const MovingFoodDescription& fd, GameGridPtr grid);
		// Rule of Three: make copy ctor and operator=
		MovingFood(const MovingFood& other);

		~MovingFood();

		cocos2d::Sprite* getSprite() const override { return m_dirSprite->getSprite(); }
		void update() override;

		void moveCallback(float dt);
		const DirectedSprite& getDirectedSprite() const { return *m_dirSprite; }
	private:
		DirectedSpritePtr m_dirSprite;
	};

	/// <summary>
	/// Food Factories
	/// </summary>
	typedef std::map<StaticFoodType, StaticFoodDescription> StaticFoodTable;
	typedef std::map<MovingFoodType, MovingFoodDescription> MovingFoodTable;
	typedef std::map<int, float> TypeToProbasMap;

	struct FoodTable
	{
		StaticFoodTable staticFoodTable;
		MovingFoodTable movingFoodTable;
		TypeToProbasMap staticFoodProbas;
		TypeToProbasMap movingFoodProbas;
		TypeToProbasMap foodTypeProbas;

		FoodTable(const StaticFoodTable& staticFoodTable, 
			const MovingFoodTable& movingFoodTable,
			const TypeToProbasMap& staticFoodProbas,
			const TypeToProbasMap& movingFoodProbas,
			const TypeToProbasMap& foodTypeProbas
		) :
			staticFoodTable(staticFoodTable),
			movingFoodTable(movingFoodTable),
			staticFoodProbas(staticFoodProbas),
			movingFoodProbas(movingFoodProbas),
			foodTypeProbas(foodTypeProbas)
		{}

	};

	class IFoodFactory
	{
	public:
		IFoodFactory(const TypeToProbasMap& probas, GameGridPtr grid);
		virtual IFoodPtr makeRandom() = 0;

	protected:
		TypeToProbasMap m_foodTypeProbas;
		CategoricalDistribution m_distr;
		GameGridPtr m_grid;
	};

	class StaticFoodFactory : public IFoodFactory
	{
	public:
		StaticFoodFactory(const TypeToProbasMap& probas, const StaticFoodTable& food_table, GameGridPtr grid);
		IFoodPtr makeRandom() override;	
	private:
		StaticFoodTable m_foodTable;
	};

	class MovingFoodFactory : public IFoodFactory
	{
	public:
		MovingFoodFactory(const TypeToProbasMap& probas, const MovingFoodTable& food_table, GameGridPtr grid);
		IFoodPtr makeRandom() override;
	private:
		MovingFoodTable m_foodTable;
	};
	
	/////////////////////////
	// food generator
	/////////////////////////
	class FoodGenerator
	{
	public:
		FoodGenerator(const FoodTable& t, GameGridPtr grid);
		IFoodPtr makeRandom();
	private:
		FoodTable m_foodTable;
		IFoodFactoryPtr m_staticFoodFactory;
		IFoodFactoryPtr m_movingFoodFactory;
		CategoricalDistribution m_foodTypeDistr;
		GameGridPtr m_grid;
	};
	
}
#endif // __IFOOD_H__