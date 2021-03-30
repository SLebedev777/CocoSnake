#include "SplashScene.h"
#include "MainMenuScene.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "UISettings.h"

USING_NS_CC;

Scene* WinSplashScene::createScene()
{
    return WinSplashScene::create();
}


// on "init" you need to initialize your instance
bool WinSplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto s = Director::getInstance()->getWinSize();

    auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.9f);
    auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f);
    auto delay = cocos2d::DelayTime::create(3);
    auto seq = cocos2d::RepeatForever::create(cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr));

    auto button_play = ui::Button::create("button_green.png", "button_green.png");
    button_play->setTitleText("Next");
    button_play->setTitleFontName(FONT_FILENAME_MENU);
    button_play->setTitleFontSize(32);
    button_play->setPosition(Vec2(s.width / 2, s.height / 2));
    button_play->addClickEventListener([=](Ref* sender) {
        onContinueCallback(sender);
        });
    button_play->runAction(seq);
    this->addChild(button_play);

    auto button_quit = ui::Button::create("button_red.png", "button_red.png");
    button_quit->setTitleText("Main Menu");
    button_quit->setTitleFontName(FONT_FILENAME_MENU);
    button_quit->setTitleFontSize(24);
    button_quit->setScale(0.8);
    button_quit->setPosition(Vec2(s.width / 2, s.height / 2 - button_play->getContentSize().height - 20));
    button_quit->addClickEventListener([=](Ref* sender) {
        onQuitCallback(sender);
        });
    this->addChild(button_quit);


    auto label = Label::createWithTTF("You Win!!!", FONT_FILENAME_MENU, 48);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    AudioEngine::play2d("sound/you_win.mp3", false, 0.5f);

    return true;
}


void WinSplashScene::onContinueCallback(Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->popScene();
}


void WinSplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(MainMenuScene::create());
}


Scene* LooseSplashScene::createScene()
{
    return LooseSplashScene::create();
}


// on "init" you need to initialize your instance
bool LooseSplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto s = Director::getInstance()->getWinSize();

    auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.9f);
    auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f);
    auto delay = cocos2d::DelayTime::create(3);
    auto seq = cocos2d::RepeatForever::create(cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr));

    auto button_play = ui::Button::create("button_green.png", "button_green.png");
    button_play->setTitleText("Retry");
    button_play->setTitleFontName(FONT_FILENAME_MENU);
    button_play->setTitleFontSize(32);
    button_play->setPosition(Vec2(s.width / 2, s.height / 2));
    button_play->addClickEventListener([=](Ref* sender) {
        onContinueCallback(sender);
        });
    button_play->runAction(seq);
    this->addChild(button_play);

    auto button_quit = ui::Button::create("button_red.png", "button_red.png");
    button_quit->setTitleText("Main Menu");
    button_quit->setTitleFontName(FONT_FILENAME_MENU);
    button_quit->setTitleFontSize(24);
    button_quit->setScale(0.8);
    button_quit->setPosition(Vec2(s.width / 2, s.height / 2 - button_play->getContentSize().height - 20));
    button_quit->addClickEventListener([=](Ref* sender) {
        onQuitCallback(sender);
        });
    this->addChild(button_quit);

    auto label = Label::createWithTTF("You Loose...", FONT_FILENAME_MENU, 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    AudioEngine::play2d("sound/you_loose.mp3", false, 0.5f);

    return true;
}


void LooseSplashScene::onContinueCallback(Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->popScene();
}


void LooseSplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(MainMenuScene::create());
}


///////////////////////////////////////////////////////////////////////////////////////////////


Scene* FinalSplashScene::createScene()
{
    return FinalSplashScene::create();
}


// on "init" you need to initialize your instance
bool FinalSplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto quit_label = Label::createWithTTF("Quit", FONT_FILENAME_MENU, 24);

    auto quit_menu_item = MenuItemLabel::create(quit_label, [&](Ref* sender) { onQuitCallback(sender); });

    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(quit_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    auto s = Director::getInstance()->getWinSize();

    menu->setPosition(Vec2(s.width / 2, s.height / 2));

    auto label = Label::createWithTTF("All game finished!!!", FONT_FILENAME_MENU, 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    AudioEngine::play2d("sound/final_tune.ogg", false, 0.5f);

    return true;
}



void FinalSplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(MainMenuScene::create());
}
