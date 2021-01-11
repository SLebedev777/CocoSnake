#include "SplashScene.h"
#include "MainMenuScene.h"

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

    auto continue_label = Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 24);
    auto quit_label = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24);

    auto continue_menu_item = MenuItemLabel::create(continue_label, [&](Ref* sender) { onContinueCallback(sender); } );
    auto quit_menu_item = MenuItemLabel::create(quit_label, [&](Ref* sender) { onQuitCallback(sender); });

    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(continue_menu_item);
    menu_items.pushBack(quit_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    auto s = Director::getInstance()->getWinSize();

    menu->setPosition(Vec2(s.width / 2, s.height / 2));

    auto label = Label::createWithTTF("You Win!!!", "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    return true;
}


void WinSplashScene::onContinueCallback(Ref* pSender)
{
    Director::getInstance()->popScene();
    //Director::getInstance()->replaceScene(GameScene::create(GameLevel(555)));

}


void WinSplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
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

    auto continue_label = Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 24);
    auto quit_label = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24);

    auto continue_menu_item = MenuItemLabel::create(continue_label, [&](Ref* sender) { onContinueCallback(sender); });
    auto quit_menu_item = MenuItemLabel::create(quit_label, [&](Ref* sender) { onQuitCallback(sender); });

    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(continue_menu_item);
    menu_items.pushBack(quit_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    auto s = Director::getInstance()->getWinSize();

    menu->setPosition(Vec2(s.width / 2, s.height / 2));

    auto label = Label::createWithTTF("You Loose...", "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    return true;
}


void LooseSplashScene::onContinueCallback(Ref* pSender)
{
    Director::getInstance()->popScene();
}


void LooseSplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
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

    auto quit_label = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24);

    auto quit_menu_item = MenuItemLabel::create(quit_label, [&](Ref* sender) { onQuitCallback(sender); });

    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(quit_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    auto s = Director::getInstance()->getWinSize();

    menu->setPosition(Vec2(s.width / 2, s.height / 2));

    auto label = Label::createWithTTF("All game finished!!!", "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    return true;
}



void FinalSplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(MainMenuScene::create());
}
