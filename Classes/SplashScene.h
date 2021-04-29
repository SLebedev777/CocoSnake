#ifndef __SPLASHSCENE_H__
#define __SPLASHSCENE_H__

#include "cocos2d.h"

class SplashScene : public cocos2d::Scene
{
public:
    struct SplashSceneDescription
    {
        std::string caption;
        std::string continue_button_text;
        std::string quit_button_text;
        cocos2d::Color4F central_rect_color;
        std::string sound_file;
    };

    SplashScene(SplashSceneDescription& descr);
    static cocos2d::Scene* createScene(SplashSceneDescription& descr);
    static SplashScene* create(SplashSceneDescription& descr);

    virtual bool init();

    virtual void onContinueCallback(cocos2d::Ref* pSender);
    virtual void onQuitCallback(cocos2d::Ref* pSender);

protected:
    SplashSceneDescription m_descr;
};

class WinSplashScene : public SplashScene
{
public:
    static WinSplashScene* create()
    {
        SplashScene::SplashSceneDescription descr = { "You Win!!!", "Next", "Main Menu", cocos2d::Color4F(1.0f, 0.5f, 0.0f, 0.5f), "sound/you_win.mp3" };
        return static_cast<WinSplashScene*>(SplashScene::create(descr));
    }
};

class LooseSplashScene : public SplashScene
{
public:
    static LooseSplashScene* create()
    {
        SplashScene::SplashSceneDescription descr = { "You Loose...", "Retry", "Main Menu", cocos2d::Color4F(0.0f, 0.0f, 1.0f, 0.5f), "sound/you_loose.mp3" };
        return static_cast<LooseSplashScene*>(SplashScene::create(descr));
    }
};


class FinalSplashScene : public SplashScene
{
public:
    static FinalSplashScene* create()
    {
        SplashScene::SplashSceneDescription descr = { "All game finished!!!", "Retry", "Main Menu", cocos2d::Color4F(0.0f, 0.0f, 1.0f, 0.5f), "sound/you_win.mp3" };
        return static_cast<FinalSplashScene*>(SplashScene::create(descr));
    }
};

/*
class FinalSplashScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void onQuitCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(FinalSplashScene);

};
*/

#endif // __SPLASHSCENE_H__
