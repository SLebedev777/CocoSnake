#ifndef __SPLASHSCENE_H__
#define __SPLASHSCENE_H__

#include "cocos2d.h"

class WinSplashScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
 
    void onContinueCallback(cocos2d::Ref* pSender);
    void onQuitCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(WinSplashScene);

};

class LooseSplashScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void onContinueCallback(cocos2d::Ref* pSender);
    void onQuitCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(LooseSplashScene);

};


class FinalSplashScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void onQuitCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(FinalSplashScene);

};

#endif // __SPLASHSCENE_H__
