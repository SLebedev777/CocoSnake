#pragma once

#include "GameLevel.h"
#include "cocos2d.h"

cocos2d::Action* actionCallbackRotateForever()
{
	return cocos2d::RepeatForever::create(cocos2d::RotateBy::create(1, 180));
}

cocos2d::Action* actionCallbackBounce()
{
	auto bouncer = cocos2d::ScaleTo::create(0.2, 0.8);
	auto unbouncer = cocos2d::ScaleTo::create(0.2, 1.0);
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

	FoodDescription fd_apple("apple.png", 1, 1, 0.5, actionCallbackBounce);
	FoodDescription fd_banana("banana.png", 1, 1, 0.5, actionCallbackRockSideToSide);
	FoodDescription fd_mushroom("mushroom.png", -10, 1, 0.5);
	FoodDescription fd_ananas("ananas.png", 1, 3, 0.5);
	FoodDescription fd_potion("potion.png", 1, 1, 0.5);
	FoodDescription fd_portal("portal.png", 1, 1, 0.5, actionCallbackRotateForever);

	FoodTable food_table_default = {
		{ FoodType::APPLE, fd_apple },
		{ FoodType::BANANA, fd_banana },
		{ FoodType::MUSHROOM, fd_mushroom },
		{ FoodType::ANANAS, fd_ananas },
		{ FoodType::POTION, fd_potion },
		{ FoodType::PORTAL, fd_portal }
	};

	FoodTable food_table1 = {
	{ FoodType::APPLE, fd_apple },
	{ FoodType::PORTAL, fd_portal }
	};

	FoodTable food_table2 = {
	{ FoodType::BANANA, fd_banana },
	};

	GameLevelsArray levels;

	levels.push_back(std::make_shared<GameLevel>(1, food_table1, "Level1", 3, 10, 5, 10, 120));
	levels.push_back(std::make_shared<GameLevel>(2, food_table2, "Level2", 4, 8, 6, 15, 90));
	levels.push_back(std::make_shared<GameLevel>(3, food_table_default, "Level3", 5, 7, 8, 20, 60));

	return levels;

}