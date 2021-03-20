#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameLevel.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"

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

    auto s = Director::getInstance()->getWinSize();

    auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.9f);
    auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f);
    auto delay = cocos2d::DelayTime::create(3);
    auto seq = cocos2d::RepeatForever::create(cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr));

    auto button_play = ui::Button::create("button_green.png", "button_green.png");
    button_play->setTitleText("Play");
    button_play->setTitleFontName("fonts/arial.ttf");
    button_play->setTitleFontSize(32);
    button_play->setPosition(Vec2(s.width / 2, s.height / 2));
    button_play->addClickEventListener([=](Ref* sender) {
        menuNewGameCallback(sender);
        });
    button_play->runAction(seq);
    this->addChild(button_play);

    auto button_quit = ui::Button::create("button_red.png", "button_red.png");
    button_quit->setTitleText("Quit");
    button_quit->setTitleFontName("fonts/arial.ttf");
    button_quit->setTitleFontSize(24);
    button_quit->setScale(0.8);
    button_quit->setPosition(Vec2(s.width / 2, s.height / 2 - button_play->getContentSize().height - 20));
    button_quit->addClickEventListener([=](Ref* sender) {
        menuCloseCallback(sender);
        });
    this->addChild(button_quit);

    AudioEngine::play2d("background.mp3", true, 0.25f);

    return true;
}


void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    AudioEngine::end();
}


void MainMenuScene::menuNewGameCallback(cocos2d::Ref* pSender)
{
    AudioEngine::stopAll();
    GameLevelManager::getInstance().rewind();
    auto& curr_level = GameLevelManager::getInstance().getCurrLevel();
    game_scene = GameScene::create(curr_level);
    Director::getInstance()->pushScene(game_scene);
}
