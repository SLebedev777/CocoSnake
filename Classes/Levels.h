#pragma once

#include "GameLevel.h"
#include "cocos2d.h"

cocos2d::Action* actionCallbackRotateForever()
{
	return cocos2d::RepeatForever::create(cocos2d::RotateBy::create(1, 180));
}

cocos2d::Action* actionCallbackBounce()
{
	auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.8f);
	auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f);
	auto delay = cocos2d::DelayTime::create(3);
	auto seq = cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr);
	return cocos2d::RepeatForever::create(seq);
}

cocos2d::Action* actionCallbackRockSideToSide()
{
	auto rock1 = cocos2d::RotateBy::create(0.2, -10);
	auto rock2 = cocos2d::RotateBy::create(0.2, 20);
	auto delay = cocos2d::DelayTime::create(3);
	auto seq = cocos2d::Sequence::create(rock1, rock2, rock1, delay, nullptr);
	return cocos2d::RepeatForever::create(seq);
}


GameLevelsArray getGameLevels()
{
	using namespace NS_Snake;
	/*
	FoodDescription fd_apple_def("apple.png", 1, 1, 0.5, false, -1, actionCallbackBounce);
	FoodDescription fd_banana_def("banana.png", 1, 1, 0.5, false, -1, actionCallbackRockSideToSide);
	FoodDescription fd_mushroom_def("mushroom.png", -10, 1, 0.5, false, -1);
	FoodDescription fd_ananas_def("ananas.png", 1, 3, 0.5, false, -1);
	FoodDescription fd_potion_def("potion.png", 1, 1, 0.5, false, -1);
	FoodDescription fd_portal_def("portal.png", 1, 1, 0.5, true, -1, actionCallbackRotateForever);

	FoodTable food_table_default = {
		{ FoodType::APPLE, fd_apple_def },
		{ FoodType::BANANA, fd_banana_def },
		{ FoodType::MUSHROOM, fd_mushroom_def },
		{ FoodType::ANANAS, fd_ananas_def },
		{ FoodType::POTION, fd_potion_def },
		{ FoodType::PORTAL, fd_portal_def }
	};

	FoodTable food_table1 = {
	{ FoodType::APPLE,  FoodDescription("apple.png", 1, 1, 0.5, false, 3, actionCallbackBounce) },
	{ FoodType::BANANA, FoodDescription("banana.png", 1, 1, 0.5, false, 5, actionCallbackRockSideToSide) },
	{ FoodType::PORTAL, FoodDescription("portal.png", 1, 1, 0.5, true, -1, actionCallbackRotateForever) }
	};

	FoodTable food_table2 = {
	{ FoodType::BANANA, fd_banana_def },
	};
	*/
	StaticFoodTable static_food_table1 = {
		{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, 3, actionCallbackBounce) },
		{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, 5, actionCallbackRockSideToSide) },
		{ StaticFoodType::PORTAL, StaticFoodDescription("portal.png", 1, 1, true, -1, actionCallbackRotateForever) }
	};
	MovingFoodTable moving_food_table1 = {
		{MovingFoodType::BIRD, MovingFoodDescription()}
	};
	TypeToProbasMap static_food_probas1 = {
		{StaticFoodType::APPLE, 0.5},
		{StaticFoodType::BANANA, 0.3},
		{StaticFoodType::PORTAL, 0.2}
	};
	TypeToProbasMap moving_food_probas1 = {
		{MovingFoodType::BIRD, 0.0}
	};
	TypeToProbasMap food_type_probas1 = {
		{FoodType::STATIC, 1.0},
		{FoodType::MOVING, 0.0}
	};

	FoodTable food_table1(static_food_table1, moving_food_table1, static_food_probas1, moving_food_probas1, food_type_probas1);

	GameLevelsArray levels;

	levels.push_back(std::make_shared<GameLevel>(1, food_table1, "Level1", 3, 10, 5, 10, 120));
	levels.push_back(std::make_shared<GameLevel>(2, food_table1, "Level2", 4, 8, 6, 15, 90));
	levels.push_back(std::make_shared<GameLevel>(3, food_table1, "Level3", 5, 7, 8, 20, 60));

	return levels;

}