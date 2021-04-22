#include "GameScene_VFX.h"


USING_NS_CC;
using namespace NS_Snake;

ParticleSystemQuad* getParticleVFXSpawnFood(FoodType ft, FoodSubType fst)
{
    auto emitter = ParticleExplosion::createWithTotalParticles(200);
    emitter->setDuration(0.25f);
    emitter->setEmitterMode(cocos2d::ParticleSystem::Mode::RADIUS);
    emitter->setStartRadius(10.0f);
    emitter->setEndRadius(80.0f);
    emitter->setLife(0.5f);
    emitter->setStartSize(15.0f);
    emitter->setEndSize(2.0f);
    std::map<StaticFoodType, cocos2d::Color4F> sft2color = {
        {StaticFoodType::ANANAS, cocos2d::Color4F::MAGENTA},
        {StaticFoodType::APPLE, cocos2d::Color4F::MAGENTA},
        {StaticFoodType::BANANA, cocos2d::Color4F::MAGENTA},
        {StaticFoodType::MUSHROOM, cocos2d::Color4F::WHITE},
        {StaticFoodType::POTION, cocos2d::Color4F::BLUE},
        {StaticFoodType::PORTAL, cocos2d::Color4F::GREEN}
    };
    std::map<MovingFoodType, cocos2d::Color4F> mft2color = {
        {MovingFoodType::BUTTERFLY, cocos2d::Color4F::ORANGE},
        {MovingFoodType::SPYDER, cocos2d::Color4F::GRAY}
    };
    cocos2d::Color4F start_color;
    if (ft == FoodType::STATIC)
    {
        auto food_subtype = static_cast<StaticFoodType>(fst);
        if (sft2color.find(food_subtype) != sft2color.end())
            start_color = sft2color[food_subtype];
        else
            start_color = cocos2d::Color4F::MAGENTA;
    }
    else if (ft == FoodType::MOVING)
    {
        auto food_subtype = static_cast<MovingFoodType>(fst);
        if (mft2color.find(food_subtype) != mft2color.end())
            start_color = mft2color[food_subtype];
        else
            start_color = cocos2d::Color4F::ORANGE;
    }

    emitter->setStartColor(start_color);
    emitter->setStartColorVar(cocos2d::Color4F(0.1, 0.1, 0.1, 0));
    emitter->setEndColor(cocos2d::Color4F::WHITE);
    emitter->setEndColorVar(cocos2d::Color4F(0, 0, 0, 0));
    emitter->setEmissionRate(200);

    return emitter;
}

ParticleSystemQuad* getParticleVFXDisappearFood(FoodType ft, FoodSubType fst)
{
    auto emitter = ParticleSmoke::createWithTotalParticles(50);
    
    emitter->setDuration(0.3f);
    emitter->setLife(1.0f);
    emitter->setStartSize(30.0f);
    emitter->setEndSize(50.0f);

    auto start_color = cocos2d::Color4F::GRAY;
    emitter->setStartColor(start_color);
    emitter->setStartColorVar(cocos2d::Color4F(0.1, 0.1, 0.1, 0));
    emitter->setEndColor(cocos2d::Color4F(1.0f, 1.0f, 1.0f, 0.2f));
    emitter->setEndColorVar(cocos2d::Color4F(0, 0, 0, 0));
    
    return emitter;
}

ParticleSystemQuad* getParticleVFXSnakeDies()
{
    auto emitter = ParticleExplosion::createWithTotalParticles(500);
    emitter->setDuration(0.25f);
    emitter->setEmitterMode(cocos2d::ParticleSystem::Mode::RADIUS);
    emitter->setStartRadius(10.0f);
    emitter->setEndRadius(80.0f);
    emitter->setLife(0.5f);
    emitter->setStartSize(15.0f);
    emitter->setEndSize(2.0f);
    cocos2d::Color4F start_color(cocos2d::Color4F::GREEN);
    emitter->setStartColor(start_color);
    emitter->setStartColorVar(cocos2d::Color4F(0.1, 0.1, 0.1, 0));
    emitter->setEndColor(cocos2d::Color4F::BLACK);
    emitter->setEndColorVar(cocos2d::Color4F(0, 0, 0, 0));
    emitter->setEmissionRate(200);

    return emitter;
}
