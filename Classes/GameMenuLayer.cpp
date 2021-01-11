#include "GameMenuLayer.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;


GameMenuLayer::GameMenuLayer(GameScene* from) : game_scene(from)
{
}

GameMenuLayer* GameMenuLayer::create(GameScene* from)
{
    GameMenuLayer* pRet = new (std::nothrow) GameMenuLayer(from);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

bool GameMenuLayer::init()
{
    this->initWithColor(Color4B(0, 0, 0, 255));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //this->setColor(Color3B(0, 0, 0));

    auto resume_label = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 24);
    auto back_to_main_label = Label::createWithTTF("Back to Main Menu", "fonts/Marker Felt.ttf", 24);

    auto resume_menu_item = MenuItemLabel::create(resume_label, [&](Ref* sender) { menuResumeCallback(sender); } );
    auto back_to_main_menu_item = MenuItemLabel::create(back_to_main_label, [&](Ref* sender) { menuBackToMainMenuCallback(sender); });

    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(resume_menu_item);
    menu_items.pushBack(back_to_main_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    auto s = Director::getInstance()->getWinSize();

    menu->setPosition(Vec2(s.width / 2, s.height / 2));

    /////////////////////////////
    // 3. add your codes below...

    auto label = Label::createWithTTF("Game Menu", "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    return true;
}

void GameMenuLayer::menuResumeCallback(cocos2d::Ref* pSender)
{
    if (!game_scene)
    {
        return;
    }
    EventCustom custom_event("event_game_menu_close");
    _eventDispatcher->dispatchEvent(&custom_event);
}

void GameMenuLayer::menuBackToMainMenuCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(MainMenuScene::create());
}