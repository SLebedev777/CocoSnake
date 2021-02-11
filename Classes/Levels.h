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

	FoodDescription fd_apple_def("apple.png", 1, 1, 0.5, actionCallbackBounce);
	FoodDescription fd_banana_def("banana.png", 1, 1, 0.5, actionCallbackRockSideToSide);
	FoodDescription fd_mushroom_def("mushroom.png", -10, 1, 0.5);
	FoodDescription fd_ananas_def("ananas.png", 1, 3, 0.5);
	FoodDescription fd_potion_def("potion.png", 1, 1, 0.5);
	FoodDescription fd_portal_def("portal.png", 1, 1, 0.5, actionCallbackRotateForever);

	FoodTable food_table_default = {
		{ FoodType::APPLE, fd_apple_def },
		{ FoodType::BANANA, fd_banana_def },
		{ FoodType::MUSHROOM, fd_mushroom_def },
		{ FoodType::ANANAS, fd_ananas_def },
		{ FoodType::POTION, fd_potion_def },
		{ FoodType::PORTAL, fd_portal_def }
	};

	FoodTable food_table1 = {
	{ FoodType::APPLE,  FoodDescription("apple.png", 1, 1, 0.9, actionCallbackBounce) },
	{ FoodType::PORTAL, FoodDescription("portal.png", 1, 1, 0.1, actionCallbackRotateForever) }
	};

	FoodTable food_table2 = {
	{ FoodType::BANANA, fd_banana_def },
	};

	GameLevelsArray levels;

	levels.push_back(std::make_shared<GameLevel>(1, food_table1, "Level1", 3, 10, 5, 10, 120));
	levels.push_back(std::make_shared<GameLevel>(2, food_table2, "Level2", 4, 8, 6, 15, 90));
	levels.push_back(std::make_shared<GameLevel>(3, food_table_default, "Level3", 5, 7, 8, 20, 60));

	return levels;

}