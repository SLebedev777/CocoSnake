#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameLevel.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "UISettings.h"

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

    Size visible_size = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float margin = 80;
    Rect nodegrid_rect = Rect(origin - Vec2(margin, margin), visible_size + Size(2*margin, 2*margin));
    auto node_grid = NodeGrid::create(nodegrid_rect);
    this->addChild(node_grid, -1);

    float scroll_interval = 10.0f;
    Sprite* back1 = Sprite::create("menu_background1.png");
    Size back_size = back1->getContentSize();
    Sprite* back2 = Sprite::create("menu_background1.png");
    float back_y_pos = origin.y + visible_size.height / 2;
    back1->setPosition(Vec2(origin.x + visible_size.width / 2, back_y_pos));
    back2->setPosition(Vec2(origin.x + visible_size.width / 2 - back_size.width, back_y_pos));


    auto back1_move_seq = Sequence::create(
        MoveBy::create(scroll_interval, Vec2(back_size.width, 0)),
        MoveBy::create(0.0f, Vec2(-2*back_size.width, 0)),
        MoveBy::create(scroll_interval, Vec2(back_size.width, 0)),
        nullptr
    );
    back1->runAction(RepeatForever::create(back1_move_seq));
    
    auto back2_move_seq = Sequence::create(
        MoveBy::create(2*scroll_interval, Vec2(2*back_size.width, 0)),
        MoveBy::create(0.0f, Vec2(-2 * back_size.width, 0)),
        nullptr
    );
    back2->runAction(RepeatForever::create(back2_move_seq));

    node_grid->addChild(back1, 0);
    node_grid->addChild(back2, 0);

    auto waves = Waves::create(7, Size(16, 12), 3, 15, true, true);
    node_grid->runAction(RepeatForever::create(waves));

    auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.9f);
    auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f);
    auto delay = cocos2d::DelayTime::create(3);
    auto seq = cocos2d::RepeatForever::create(cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr));

    auto button_play = ui::Button::create("button_green.png", "button_green.png");
    button_play->setTitleText("Play");
    button_play->setTitleFontName(FONT_FILENAME_MENU);
    button_play->setTitleFontSize(32);
    button_play->setPosition(Vec2(visible_size.width / 2, visible_size.height / 2));
    button_play->addClickEventListener([=](Ref* sender) {
        menuNewGameCallback(sender);
        });
    button_play->runAction(seq);
    this->addChild(button_play);

    auto button_quit = ui::Button::create("button_red.png", "button_red.png");
    button_quit->setTitleText("Quit");
    button_quit->setTitleFontName(FONT_FILENAME_MENU);
    button_quit->setTitleFontSize(24);
    button_quit->setScale(0.8);
    button_quit->setPosition(Vec2(visible_size.width / 2, visible_size.height / 2 - button_play->getContentSize().height - 20));
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
