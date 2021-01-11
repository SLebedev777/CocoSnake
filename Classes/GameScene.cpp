#include <string>
#include <vector>
#include "GameScene.h"
#include "GameMenuLayer.h"
#include "SplashScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include "CCHelpers.h"


USING_NS_CC;

enum LayersTags
{
    TAG_GAME_LAYER = 1,
    TAG_HUD_LAYER,
    TAG_HUD_CONTROL_LAYER,
    TAG_GAME_MENU_LAYER
};

enum GameLayerTags
{

};

enum HUDLayerTags
{
    TAG_HUD_LAYER_TIMER_STRING = 1
};

GameScene::GameScene(GameLevel* level) :
    currLevel(level)
{}

GameScene* GameScene::create(GameLevel* level)
{
    GameScene* pRet = new(std::nothrow) GameScene(level);
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

Scene* GameScene::createScene(GameLevel* level)
{
    return GameScene::create(level);
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////////////////////////
    // GAME LAYER
    //
    auto game_layer = Layer::create();

    player = Sprite::create("apple.png");
    player->setPosition(Vec2(50, 50));
    player->runAction(RepeatForever::create(RotateBy::create(3.0f, 360.0f)));

    game_layer->addChild(player);

    //////////////////////////////////////////////////
    // HUD LAYER
    // non-interactive elements over main game process
    
    auto hud_layer = LayerColor::create(Color4B(0, 0, 0, 0));

    auto label = Label::createWithTTF("GameScene, level=" + std::to_string(this->currLevel->getNumber()), "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));
    }

    hud_layer->addChild(label);

    auto label_time = Label::createWithTTF("time elapsed: ", "fonts/Marker Felt.ttf", 24);
    if (label_time)
    {
        // position the label on the center of the screen
        label_time->setPosition(Vec2(origin.x + visibleSize.width  - 100,
            origin.y + label_time->getContentSize().height));
    }

    hud_layer->addChild(label_time, 1, TAG_HUD_LAYER_TIMER_STRING);


    //////////////////////////////////////////////////
    // HUD CONTROL LAYER
    // interactive elements over main game process

    auto hud_control_layer = LayerColor::create(Color4B(0, 0, 0, 0));
    
    auto game_menu_label = Label::createWithTTF("Game Menu", "fonts/Marker Felt.ttf", 24);

    auto game_menu_item = MenuItemLabel::create(game_menu_label,
        [&](Ref* sender) {
            onGameMenuOpen(sender);
        }
    );
    // create menu, it's an autorelease object
    Vector<MenuItem*> menu_items;

    menu_items.pushBack(game_menu_item);

    auto menu = Menu::createWithArray(menu_items);
    menu->alignItemsVertically();
    auto s = Director::getInstance()->getWinSize();
    menu->setPosition(Vec2(s.width / 2, s.height - 100));
    
    hud_control_layer->addChild(menu, 1, "HUD_CONTROL_MENU");

    auto button_win = ui::Button::create("CloseNormal.png", "CloseSelected.png");
    button_win->setTitleText("Win");
    button_win->setPosition(Vec2(50, 200));
    button_win->addClickEventListener([=](Ref* sender) { 
        onGameWin(sender); 
        } );
    hud_control_layer->addChild(button_win);

    auto button_loose = ui::Button::create("CloseNormal.png", "CloseSelected.png");
    button_loose->setTitleText("Loose");
    button_loose->setPosition(Vec2(50, 250));
    button_loose->addClickEventListener([=](Ref* sender) {
        onGameLoose(sender);
        });
    hud_control_layer->addChild(button_loose);
    //////////////////////////////////////////////////

    auto _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
    _mouseListener->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
    _mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
    _mouseListener->onMouseScroll = CC_CALLBACK_1(GameScene::onMouseScroll, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, game_layer);

    auto _keyboard_listener = EventListenerKeyboard::create();
    _keyboard_listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    _keyboard_listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboard_listener, this);

    auto _game_menu_close_listener = EventListenerCustom::create("event_game_menu_close", [=](EventCustom* event) {
        onGameMenuClose(event);
        });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_game_menu_close_listener, this);

    // construct game scene from layers
    this->addChild(game_layer, 1, TAG_GAME_LAYER);
    this->addChild(hud_layer, 2, TAG_HUD_LAYER);
    this->addChild(hud_control_layer, 3, TAG_HUD_CONTROL_LAYER);

    // setup Timer
    time_elapsed = 0.0f;
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateTimer), 1.0f);

    return true;
}

void GameScene::onMouseDown(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    player->setPosition(Vec2(e->getCursorX(), e->getCursorY()));
}

void GameScene::onMouseUp(Event* event)
{
}

void GameScene::onMouseMove(Event* event)
{
}

void GameScene::onMouseScroll(Event* event)
{
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        if (this->getChildByTag(TAG_GAME_MENU_LAYER))
        {
            onGameMenuClose(event);
        }
        else
        {
            onGameMenuOpen(nullptr);
        }
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void GameScene::onGameMenuOpen(Ref* sender)
{
    Director::getInstance()->getScheduler()->pauseTarget(this);

    for (auto& tag : std::vector<LayersTags>({ TAG_GAME_LAYER, TAG_HUD_LAYER, TAG_HUD_CONTROL_LAYER }))
    {
        auto layer = this->getChildByTag(tag);
        MyCocos2dHelpers::CallCCNodeMethodRecursively(layer, [](Node* node) { node->pause(); });
    }
    auto hud_control_menu = static_cast<Menu*> (this->getChildByTag(TAG_HUD_CONTROL_LAYER)->getChildByName("HUD_CONTROL_MENU"));
    hud_control_menu->setEnabled(false);

    auto game_menu_layer = GameMenuLayer::create(this);
    this->addChild(game_menu_layer, 255, TAG_GAME_MENU_LAYER);
}

void GameScene::onGameMenuClose(Event* event)
{
    Director::getInstance()->getScheduler()->resumeTarget(this);

    auto game_menu_layer = this->getChildByTag(TAG_GAME_MENU_LAYER);
    if (game_menu_layer)
    {
        this->removeChildByTag(TAG_GAME_MENU_LAYER);

        for (auto& tag : std::vector<LayersTags>({ TAG_GAME_LAYER, TAG_HUD_LAYER, TAG_HUD_CONTROL_LAYER }))
        {
            auto layer = this->getChildByTag(tag);
            MyCocos2dHelpers::CallCCNodeMethodRecursively(layer, [](Node* node) { node->resume(); });
        }
        auto hud_control_menu = static_cast<Menu*> (this->getChildByTag(TAG_HUD_CONTROL_LAYER)->getChildByName("HUD_CONTROL_MENU"));
        hud_control_menu->setEnabled(true);

    }
}


void GameScene::onGameWin(Ref* sender)
{
    auto glm = GameLevelManager::getInstance();
    auto director = Director::getInstance();
    if (glm->moveToNextLevel())
    {
        director->replaceScene(GameScene::create(glm->getCurrLevel()));
        director->pushScene(WinSplashScene::create());
    }
    else 
    {
        director->replaceScene(FinalSplashScene::create());
    }
}

void GameScene::onGameLoose(Ref* sender)
{
    auto glm = GameLevelManager::getInstance();
    auto director = Director::getInstance();
    director->replaceScene(GameScene::create(glm->getCurrLevel()));
    director->pushScene(LooseSplashScene::create());
}

void GameScene::updateTimer(float dt)
{
    auto hud_layer = this->getChildByTag(TAG_HUD_LAYER);
    auto label_time_node = hud_layer->getChildByTag(TAG_HUD_LAYER_TIMER_STRING);
    auto label_time = static_cast<cocos2d::Label*> (label_time_node);
    time_elapsed += dt;
    label_time->setString("time elapsed: " + std::to_string(int(time_elapsed)));
}