#pragma once

#include "IFood.h"
#include "cocos/2d/CCParticleSystemQuad.h"

cocos2d::ParticleSystemQuad* getParticleVFXSpawnFood(NS_Snake::FoodType ft, NS_Snake::FoodSubType fst);