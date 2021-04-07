#include "GameMenuLayer.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include "UISettings.h"
#include "UIButtonMenu.h"

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
    this->initWithColor(Color4B(127, 0, 127, 100));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s = Director::getInstance()->getWinSize();

    auto button_resume = ui::Button::create("button_green.png", "button_green.png");
    button_resume->setTitleText("Resume");
    button_resume->setTitleFontName(FONT_FILENAME_MENU);
    button_resume->setTitleFontSize(32);
    button_resume->setPosition(Vec2(s.width / 2, s.height / 2));
    this->addChild(button_resume);

    auto button_quit = ui::Button::create("button_red.png", "button_red.png");
    button_quit->setTitleText("Main Menu");
    button_quit->setTitleFontName(FONT_FILENAME_MENU);
    button_quit->setTitleFontSize(24);
    button_quit->setPosition(Vec2(s.width / 2, s.height / 2 - button_resume->getContentSize().height - 20));
    this->addChild(button_quit);

    std::vector<std::pair<ui::Button*, ui::Widget::ccWidgetClickCallback>> buttons_callbacks;
    buttons_callbacks.push_back({ button_resume, [=](Ref* sender) { menuResumeCallback(sender); } });
    buttons_callbacks.push_back({ button_quit, [=](Ref* sender) { menuBackToMainMenuCallback(sender); } });

    UIButtonMenu* menu = UIButtonMenu::create(buttons_callbacks, this, this->getEventDispatcher());
    this->addChild(menu);


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