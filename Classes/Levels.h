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

cocos2d::Action* actionCallbackButterflyIdle()
{
	auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.8f, 1.0f);
	auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f, 1.0f);
	auto delay = cocos2d::DelayTime::create(1);
	auto seq = cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr);
	return cocos2d::RepeatForever::create(seq);

}

cocos2d::Action* actionCallbackButterflyMove(float move_time, cocos2d::Vec2 pos_to)
{
	auto mover = cocos2d::MoveTo::create(move_time, pos_to);
	auto bouncer = cocos2d::ScaleTo::create(0.25f * move_time, 0.7f, 1.0f);
	auto unbouncer = cocos2d::ScaleTo::create(0.25f * move_time, 1.0f, 1.0f);
	auto delay = cocos2d::DelayTime::create(0.5f * move_time);
	auto seq = cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, nullptr);
	return cocos2d::Spawn::createWithTwoActions(mover, seq);
}

cocos2d::Action* actionCallbackSpyderMove(float move_time, cocos2d::Vec2 pos_to)
{
	auto mover = cocos2d::MoveTo::create(move_time, pos_to);
	auto flipper = cocos2d::FlipX::create(true);
	auto unflipper = cocos2d::FlipX::create(false);
	auto delay = cocos2d::DelayTime::create(0.25f * move_time);
	auto seq = cocos2d::Sequence::create(flipper, delay, unflipper, delay, flipper, delay, unflipper, delay, nullptr);
	return cocos2d::Spawn::createWithTwoActions(mover, seq);
}

GameLevelsArray getGameLevels()
{
	using namespace NS_Snake;
	
	// level 1

	StaticFoodTable static_food_table1 = {
		{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, -1, actionCallbackBounce) },
		{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, -1, actionCallbackRockSideToSide) },
	};

	DirToFrameTable DFT_butterfly = dirToFrameTemplate("butterfly.png");
	DirToFrameTable DFT_spyder = dirToFrameTemplate("spyder.png");
	
	MovingFoodTable moving_food_table1 = 
	{
		{MovingFoodType::BUTTERFLY, 
		MovingFoodDescription(DFT_butterfly, 1, 1, false, -1, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)}
	};
	
	TypeToProbasMap static_food_probas1 = {
		{StaticFoodType::APPLE, 0.5},
		{StaticFoodType::BANANA, 0.3}
	};

	TypeToProbasMap moving_food_probas1 = {
		{MovingFoodType::BUTTERFLY, 0.0}
	};
	
	TypeToProbasMap food_type_probas1 = {
		{FoodType::STATIC, 1.0},
		{FoodType::MOVING, 0.0}
	};

	FoodTable food_table1(static_food_table1, moving_food_table1, static_food_probas1, moving_food_probas1, food_type_probas1);

	GameLevelsArray levels;

	levels.push_back(std::make_shared<GameLevel>(1, 10, 12, food_table1, "Level1", 3, 10, 5, 10, -1, 5));

	// level 2

	StaticFoodTable static_food_table2 = {
	{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::MUSHROOM, StaticFoodDescription("mushroom.png", -10, 1, false, -1, actionCallbackBounce) },
	};

	MovingFoodTable moving_food_table2 =
	{
		{MovingFoodType::BUTTERFLY,
		MovingFoodDescription(DFT_butterfly, 1, 1, false, -1, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)}
	};

	TypeToProbasMap static_food_probas2 = {
		{StaticFoodType::APPLE, 0.5},
		{StaticFoodType::BANANA, 0.3},
		{StaticFoodType::MUSHROOM, 0.1},
	};

	TypeToProbasMap moving_food_probas2 = {
		{MovingFoodType::BUTTERFLY, 0.0}
	};

	TypeToProbasMap food_type_probas2 = {
		{FoodType::STATIC, 1.0},
		{FoodType::MOVING, 0.0}
	};

	FoodTable food_table2(static_food_table2, moving_food_table2, static_food_probas2, moving_food_probas2, food_type_probas2);

	levels.push_back(std::make_shared<GameLevel>(2, 12, 13, food_table2, "Level2", 4, 8, 6, 20, 90, 5));

	// level 3

	StaticFoodTable static_food_table3 = {
	{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::MUSHROOM, StaticFoodDescription("mushroom.png", -10, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::ANANAS, StaticFoodDescription("ananas.png", 1, 3, false, -1, actionCallbackRockSideToSide) },
	};

	MovingFoodTable moving_food_table3 =
	{
		{MovingFoodType::BUTTERFLY,
		MovingFoodDescription(DFT_butterfly, 1, 1, false, -1, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)}
	};

	TypeToProbasMap static_food_probas3 = {
		{StaticFoodType::APPLE, 0.4},
		{StaticFoodType::BANANA, 0.4},
		{StaticFoodType::MUSHROOM, 0.1},
		{StaticFoodType::ANANAS, 0.1},
	};

	TypeToProbasMap moving_food_probas3 = {
		{MovingFoodType::BUTTERFLY, 0.0}
	};

	TypeToProbasMap food_type_probas3 = {
		{FoodType::STATIC, 1.0},
		{FoodType::MOVING, 0.0}
	};

	FoodTable food_table3(static_food_table3, moving_food_table3, static_food_probas3, moving_food_probas3, food_type_probas3);

	levels.push_back(std::make_shared<GameLevel>(3, 15, 15, food_table3, "Level3", 5, 7, 8, 30, 75, 8));

	// level 4

	StaticFoodTable static_food_table4 = {
	{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::MUSHROOM, StaticFoodDescription("mushroom.png", -10, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::ANANAS, StaticFoodDescription("ananas.png", 1, 2, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::PORTAL, StaticFoodDescription("portal.png", 1, 1, true, -1, actionCallbackRotateForever) },
	};

	MovingFoodTable moving_food_table4 =
	{
		{MovingFoodType::BUTTERFLY,
		MovingFoodDescription(DFT_butterfly, 1, 3, false, -1, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)}
	};

	TypeToProbasMap static_food_probas4 = {
		{StaticFoodType::APPLE, 0.4},
		{StaticFoodType::BANANA, 0.4},
		{StaticFoodType::MUSHROOM, 0.1},
		{StaticFoodType::ANANAS, 0.1},
		{StaticFoodType::PORTAL, 0.2},
	};

	TypeToProbasMap moving_food_probas4 = {
		{MovingFoodType::BUTTERFLY, 1.0}
	};

	TypeToProbasMap food_type_probas4 = {
		{FoodType::STATIC, 0.8},
		{FoodType::MOVING, 0.2}
	};

	FoodTable food_table4(static_food_table4, moving_food_table4, static_food_probas4, moving_food_probas4, food_type_probas4);

	levels.push_back(std::make_shared<GameLevel>(4, 16, 15, food_table4, "Level4", 5, 6, 8, 40, 60, 10));

	// level 5

	StaticFoodTable static_food_table5 = {
	{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::MUSHROOM, StaticFoodDescription("mushroom.png", -10, 1, false, 10, actionCallbackBounce) },
	{ StaticFoodType::ANANAS, StaticFoodDescription("ananas.png", 1, 2, false, 10, actionCallbackRockSideToSide) },
	{ StaticFoodType::PORTAL, StaticFoodDescription("portal.png", 1, 1, true, -1, actionCallbackRotateForever) },
	};

	MovingFoodTable moving_food_table5 =
	{
		{MovingFoodType::BUTTERFLY,
		MovingFoodDescription(DFT_butterfly, 1, 3, false, 10, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)},
		{MovingFoodType::SPYDER,
		MovingFoodDescription(DFT_spyder, -30, 0, false, 10, 0.9, 0.3f, nullptr, actionCallbackSpyderMove)}
	};

	TypeToProbasMap static_food_probas5 = {
		{StaticFoodType::APPLE, 0.4},
		{StaticFoodType::BANANA, 0.4},
		{StaticFoodType::MUSHROOM, 0.1},
		{StaticFoodType::ANANAS, 0.2},
		{StaticFoodType::PORTAL, 0.2},
	};

	TypeToProbasMap moving_food_probas5 = {
		{MovingFoodType::BUTTERFLY, 0.8},
		{MovingFoodType::SPYDER, 0.2}
	};

	TypeToProbasMap food_type_probas5 = {
		{FoodType::STATIC, 0.7},
		{FoodType::MOVING, 0.3}
	};

	FoodTable food_table5(static_food_table5, moving_food_table5, static_food_probas5, moving_food_probas5, food_type_probas5);

	levels.push_back(std::make_shared<GameLevel>(5, 20, 18, food_table5, "Level5", 5, 5, 8, 50, 60, 10));

	// level 6

	StaticFoodTable static_food_table6 = {
	{ StaticFoodType::MUSHROOM, StaticFoodDescription("mushroom.png", -10, 1, false, 5, actionCallbackBounce) },
	{ StaticFoodType::PORTAL, StaticFoodDescription("portal.png", 1, 1, true, -1, actionCallbackRotateForever) },
	};

	MovingFoodTable moving_food_table6 =
	{
		{MovingFoodType::BUTTERFLY,
		MovingFoodDescription(DFT_butterfly, 1, 3, false, 8, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)},
		{MovingFoodType::SPYDER,
		MovingFoodDescription(DFT_spyder, -30, 0, false, 5, 0.9, 0.3f, nullptr, actionCallbackSpyderMove)}
	};

	TypeToProbasMap static_food_probas6 = {
		{StaticFoodType::MUSHROOM, 0.5},
		{StaticFoodType::PORTAL, 0.3},
	};

	TypeToProbasMap moving_food_probas6 = {
		{MovingFoodType::BUTTERFLY, 0.6},
		{MovingFoodType::SPYDER, 0.4}
	};

	TypeToProbasMap food_type_probas6 = {
		{FoodType::STATIC, 0.2},
		{FoodType::MOVING, 0.8}
	};

	FoodTable food_table6(static_food_table6, moving_food_table6, static_food_probas6, moving_food_probas6, food_type_probas6);

	levels.push_back(std::make_shared<GameLevel>(6, 15, 15, food_table6, "Level6", 5, 5, 8, 30, 30, 5));

	// level 7

	StaticFoodTable static_food_table7 = {
	{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::MUSHROOM, StaticFoodDescription("mushroom.png", -10, 1, false, -1, actionCallbackBounce) },
	{ StaticFoodType::ANANAS, StaticFoodDescription("ananas.png", 1, 2, false, -1, actionCallbackRockSideToSide) },
	{ StaticFoodType::PORTAL, StaticFoodDescription("portal.png", 1, 1, true, -1, actionCallbackRotateForever) },
	};

	MovingFoodTable moving_food_table7 =
	{
		{MovingFoodType::BUTTERFLY,
		MovingFoodDescription(DFT_butterfly, 1, 3, false, -1, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)},
		{MovingFoodType::SPYDER,
		MovingFoodDescription(DFT_spyder, -30, 0, false, -1, 0.9, 0.3f, nullptr, actionCallbackSpyderMove)}
	};

	TypeToProbasMap static_food_probas7 = {
		{StaticFoodType::APPLE, 0.2},
		{StaticFoodType::BANANA, 0.3},
		{StaticFoodType::MUSHROOM, 0.1},
		{StaticFoodType::ANANAS, 0.3},
		{StaticFoodType::PORTAL, 0.1},
	};

	TypeToProbasMap moving_food_probas7 = {
		{MovingFoodType::BUTTERFLY, 0.7},
		{MovingFoodType::SPYDER, 0.3}
	};

	TypeToProbasMap food_type_probas7 = {
		{FoodType::STATIC, 0.7},
		{FoodType::MOVING, 0.3}
	};

	FoodTable food_table7(static_food_table7, moving_food_table7, static_food_probas7, moving_food_probas7, food_type_probas7);

	levels.push_back(std::make_shared<GameLevel>(7, 20, 17, food_table7, "Level7", 5, 10, 10, 100, 120, 0));

	return levels;

}