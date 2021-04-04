#include "SplashScene.h"
#include "MainMenuScene.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "UISettings.h"
#include "GameGrid.h"
#include <ctime>

USING_NS_CC;

SplashScene::SplashScene(SplashSceneDescription& descr) :
    m_descr(descr)
{}

SplashScene* SplashScene::create(SplashSceneDescription& descr)
{
    SplashScene* pRet = new(std::nothrow) SplashScene(descr);
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

Scene* SplashScene::createScene(SplashSceneDescription& descr)
{
    return SplashScene::create(descr);
}


bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    using namespace NS_Snake;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto back_layer = LayerColor::create(Color4B(70, 100, 0, 255)); // green
    std::vector<std::string> sprite_names = { "apple.png", "portal.png", "ananas.png", "potion.png" };

    float scale_factor = 0.5f;
    size_t cell_size = Sprite::createWithSpriteFrameName("apple.png")->getTextureRect().size.width;
    auto grid = std::make_shared<GameGrid>(origin.x, origin.y, visibleSize.width, visibleSize.height, scale_factor * cell_size);
    for (int i = 0; i < grid->getNumCellsX(); i++)
    {
        for (int j = i % 2; j < grid->getNumCellsY(); j += 2)
        {
            auto spr = Sprite::createWithSpriteFrameName(sprite_names[rand() % sprite_names.size()]);
            spr->setColor(Color3B(30, 100, 30));
            spr->setOpacity(50);
            spr->setScale(scale_factor);
            spr->setRotation(rand() % 360);
            spr->setPosition(grid->cellToXyCenter(GameGrid::Cell(i, j)).toVec2());
            back_layer->addChild(spr);
        }
    }
    this->addChild(back_layer, -2);

    auto central_rect = DrawNode::create();
    int central_rect_width = 450;
    central_rect->drawSolidRect(Vec2(origin.x + visibleSize.width / 2 - central_rect_width / 2, origin.y),
        Vec2(origin.x + visibleSize.width / 2 + central_rect_width / 2, origin.y + visibleSize.height), m_descr.central_rect_color);
    auto central_rect_border = DrawNode::create(50);
    int central_rect_border_width = central_rect_width;
    central_rect_border->drawRect(Vec2(origin.x + visibleSize.width / 2 - central_rect_border_width / 2, origin.y - 100),
        Vec2(origin.x + visibleSize.width / 2 + central_rect_border_width / 2, origin.y + visibleSize.height + 100), Color4F::RED);

    this->addChild(central_rect_border, -1);
    this->addChild(central_rect, -1);

    auto s = Director::getInstance()->getWinSize();

    auto bouncer = cocos2d::ScaleTo::create(0.2f, 0.9f);
    auto unbouncer = cocos2d::ScaleTo::create(0.2f, 1.0f);
    auto delay = cocos2d::DelayTime::create(3);
    auto seq = cocos2d::RepeatForever::create(cocos2d::Sequence::create(bouncer, unbouncer, bouncer, unbouncer, delay, nullptr));

    auto button_continue = ui::Button::create("button_green.png", "button_green.png");
    button_continue->setTitleText(m_descr.continue_button_text);
    button_continue->setTitleFontName(FONT_FILENAME_MENU);
    button_continue->setTitleFontSize(32);
    button_continue->setPosition(Vec2(s.width / 2, s.height / 2));
    button_continue->addClickEventListener([=](Ref* sender) {
        onContinueCallback(sender);
        });
    button_continue->runAction(seq);
    this->addChild(button_continue);

    auto button_quit = ui::Button::create("button_red.png", "button_red.png");
    button_quit->setTitleText(m_descr.quit_button_text);
    button_quit->setTitleFontName(FONT_FILENAME_MENU);
    button_quit->setTitleFontSize(24);
    button_quit->setScale(0.8);
    button_quit->setPosition(Vec2(s.width / 2, s.height / 2 - button_continue->getContentSize().height - 20));
    button_quit->addClickEventListener([=](Ref* sender) {
        onQuitCallback(sender);
        });
    this->addChild(button_quit);


    auto label = Label::createWithTTF(m_descr.caption, FONT_FILENAME_MENU, 64);
    if (label)
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(s.width / 2, s.height - 3 * label->getContentSize().height));
        label->enableShadow(Color4B(0, 0, 0, 50), cocos2d::Size(8, -8));

        auto label_scaler_disappear = cocos2d::ScaleTo::create(0, 0);
        auto label_scaler_popup = cocos2d::ScaleTo::create(0.3f, 1.3f);
        auto label_scaler_tonormal = cocos2d::ScaleTo::create(0.15f, 1.0f);
        auto label_rotator = cocos2d::RotateBy::create(1.0f, 3);
        auto label_appear_seq = cocos2d::Sequence::create(label_scaler_disappear, label_scaler_popup, label_scaler_tonormal, nullptr);
        auto label_rotate_seq = cocos2d::RepeatForever::create(cocos2d::Sequence::create(
            label_rotator, label_rotator->reverse(), label_rotator->reverse(), label_rotator, nullptr));
        label->runAction(label_appear_seq);
        label->runAction(label_rotate_seq);
        this->addChild(label, 2);
    }

    AudioEngine::play2d(m_descr.sound_file, false, 0.5f);

    return true;
}

void SplashScene::onContinueCallback(Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->popScene();
}


void SplashScene::onQuitCallback(cocos2d::Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->replaceScene(MainMenuScene::create());
}



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
