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
	//float move_time = 0.4f;
	auto mover = cocos2d::MoveTo::create(move_time, pos_to);
	auto bouncer = cocos2d::ScaleTo::create(0.25f * move_time, 0.7f, 1.0f);
	auto unbouncer = cocos2d::ScaleTo::create(0.25f * move_time, 1.0f, 1.0f);
	auto delay = cocos2d::DelayTime::create(0.5f * move_time);
	auto seq = cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, nullptr);
	return cocos2d::Spawn::createWithTwoActions(mover, seq);
}

GameLevelsArray getGameLevels()
{
	using namespace NS_Snake;
	StaticFoodTable static_food_table1 = {
		{ StaticFoodType::APPLE,  StaticFoodDescription("apple.png", 1, 1, false, 3, actionCallbackBounce) },
		{ StaticFoodType::BANANA, StaticFoodDescription("banana.png", 1, 1, false, 5, actionCallbackRockSideToSide) },
		{ StaticFoodType::PORTAL, StaticFoodDescription("portal.png", 1, 1, true, -1, actionCallbackRotateForever) }
	};

	DirToFrameTable DFT_butterfly = dirToFrameTemplate("butterfly.png");
	MovingFoodTable moving_food_table1 = {
		{MovingFoodType::BUTTERFLY, 
		MovingFoodDescription(DFT_butterfly, 1, 1, false, -1, 0.7, 0.4f, actionCallbackButterflyIdle, actionCallbackButterflyMove)}
	};
	TypeToProbasMap static_food_probas1 = {
		{StaticFoodType::APPLE, 0.5},
		{StaticFoodType::BANANA, 0.3},
		{StaticFoodType::PORTAL, 0.2}
	};
	TypeToProbasMap moving_food_probas1 = {
		{MovingFoodType::BUTTERFLY, 0.5}
	};
	TypeToProbasMap food_type_probas1 = {
		{FoodType::STATIC, 0.9},
		{FoodType::MOVING, 0.1}
	};

	FoodTable food_table1(static_food_table1, moving_food_table1, static_food_probas1, moving_food_probas1, food_type_probas1);

	GameLevelsArray levels;

	levels.push_back(std::make_shared<GameLevel>(1, food_table1, "Level1", 3, 10, 5, 10, 120));
	levels.push_back(std::make_shared<GameLevel>(2, food_table1, "Level2", 4, 8, 6, 15, 90));
	levels.push_back(std::make_shared<GameLevel>(3, food_table1, "Level3", 5, 7, 8, 20, 60));

	return levels;

}