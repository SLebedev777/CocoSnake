#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameLevel.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}


// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto new_game_label = Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", 24);
    auto quit_label = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24);

    auto new_game_menu_item = MenuItemLabel::create(new_game_label, [&](Ref* sender) { menuNewGameCallback(sender); } );
    auto quit_menu_item = MenuItemLabel::create(quit_label, [&](Ref* sender) { menuCloseCallback(sender); });

    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(new_game_menu_item);
    menu_items.pushBack(quit_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    auto s = Director::getInstance()->getWinSize();

    menu->setPosition(Vec2(s.width / 2, s.height / 2));

    auto label = Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    return true;
}


void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

}


void MainMenuScene::menuNewGameCallback(cocos2d::Ref* pSender)
{
    GameLevelManager::getInstance().rewind();
    auto& curr_level = GameLevelManager::getInstance().getCurrLevel();
    game_scene = GameScene::create(curr_level);
    Director::getInstance()->pushScene(game_scene);
}
