#ifndef __MAINMENUSCENE_H__
#define __MAINMENUSCENE_H__

#include "cocos2d.h"
#include "GameScene.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // start new game scene callback
    void menuNewGameCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);

private:
    GameScene* game_scene = nullptr;
};

#endif // __MAINMENUSCENE_H__
