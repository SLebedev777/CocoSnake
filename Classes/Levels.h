#pragma once

#include "GameLevel.h"

static GameLevelsArray getGameLevels()
{
	using namespace NS_Snake;

	FoodDescription fd_apple("apple.png", 1, 1, 0.5);
	FoodDescription fd_banana("banana.png", 1, 1, 0.5);
	FoodDescription fd_mushroom("mushroom.png", -10, 1, 0.5);
	FoodDescription fd_ananas("ananas.png", 1, 3, 0.5);

	FoodTable food_table_default = {
		{ FoodType::APPLE, fd_apple },
		{ FoodType::BANANA, fd_banana },
		{ FoodType::MUSHROOM, fd_mushroom },
		{ FoodType::ANANAS, fd_ananas }
	};

	FoodTable food_table1 = {
	{ FoodType::APPLE, fd_apple },
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