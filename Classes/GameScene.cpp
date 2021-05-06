#include "GameScene.h"
#include "GameMenuLayer.h"
#include "SplashScene.h"
#include "MainMenuScene.h"
#include "DirectedSprite.h"
#include "Snake.h"
#include "GameGrid.h"
#include "IFood.h"
#include "ui/CocosGUI.h"
#include "UISettings.h"
#include "CCHelpers.h"
#include <string>
#include <vector>
#include "ccRandom.h"
#include "audio/include/AudioEngine.h"
#include "cocos/2d/CCParticleSystemQuad.h"
#include "GameScene_VFX.h"

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
    TAG_GAME_LAYER_GRID_BORDER_RECT = 1
};

enum HUDLayerTags
{
    TAG_HUD_LAYER_TIMER_STRING = 1,
    TAG_HUD_LAYER_SNAKE_HEALTH_STRING,
    TAG_HUD_LAYER_SCORE_STRING,
    TAG_HUD_LAYER_SNAKE_HEAD_DEBUG_STRING
};

GameScene::GameScene(GameLevel& level) :
    currLevel(level)
{}

GameScene* GameScene::create(GameLevel& level)
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

Scene* GameScene::createScene(GameLevel& level)
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
    
    using namespace NS_Snake;

    // detect grid cell size (sprite size), depending on design resolution, scale factor and loaded art
    int cell_size = Sprite::createWithSpriteFrameName("apple.png")->getTextureRect().size.width;
    int border = 2 * cell_size;
    int num_cells_x = currLevel.getNumCellsX();
    int num_cells_y = currLevel.getNumCellsY();
    int grid_width = num_cells_x * cell_size;  // WAS: visibleSize.width - 2*border
    int grid_height = num_cells_y * cell_size; // WAS: visibleSize.height - 2*border
    int grid_origin_x = origin.x + (visibleSize.width - grid_width) / 2;  // WAS: origin.x + border
    int grid_origin_y = origin.y + (visibleSize.height - grid_height) / 2; // WAS: origin.y + border

    grid = std::make_shared<GameGrid>(grid_origin_x, grid_origin_y, grid_width, grid_height, cell_size);

    auto GAMEGRIDRECT = Rect(grid->getOrigin().x, grid->getOrigin().y, grid->getWidth(), grid->getHeight());

    // create gray border around game grid - surrounding wall, that will vanish in wrap-around mode
    int grid_border_width = 20;
    auto grid_draw_border = DrawNode::create(2);
    grid_draw_border->drawSolidRect(Vec2(grid->getOrigin().x - grid_border_width / 2, grid->getOrigin().y - grid_border_width / 2),
        Vec2(grid->getOrigin().x + grid->getWidth() + grid_border_width / 2, grid->getOrigin().y + grid->getHeight() + grid_border_width / 2),
        Color4F::GRAY);
    game_layer->addChild(grid_draw_border, -1, TAG_GAME_LAYER_GRID_BORDER_RECT);

    // tile background with sand bitmap
    Texture2D::TexParams texParams;
    texParams.magFilter = backend::SamplerFilter::LINEAR;
    texParams.minFilter = backend::SamplerFilter::LINEAR;
    texParams.sAddressMode = backend::SamplerAddressMode::REPEAT;
    texParams.tAddressMode = backend::SamplerAddressMode::REPEAT;
    Sprite* background = Sprite::create("ground32x32.png", GAMEGRIDRECT);
    background->getTexture()->setTexParameters(texParams);
    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(grid->getOrigin().x, grid->getOrigin().y);
    game_layer->addChild(background);

#ifdef COCOS2D_DEBUG

    auto grid_draw = DrawNode::create(2);
    grid_draw->drawRect(Vec2(grid->getOrigin().x, grid->getOrigin().y),
        Vec2(grid->getOrigin().x + grid->getWidth(), grid->getOrigin().y + grid->getHeight()),
        Color4F::RED);
    for (size_t row = 0; row < grid->getNumCellsY(); ++row)
    {
        for (size_t col = 0; col < grid->getNumCellsX(); ++col)
        {
            auto cell_corner = grid->cellToXy(GameGrid::Cell(col, row));
            grid_draw->drawRect(Vec2(cell_corner.x, cell_corner.y),
                Vec2(cell_corner.x + grid->getCellSize(), cell_corner.y + grid->getCellSize()),
                Color4F::RED);
        }
    }
    game_layer->addChild(grid_draw);

#endif

    std::vector<DirectedSpritePtr> parts;
    int start_x = grid->getOrigin().x + grid->getWidth() / 2;
    int start_y = grid->getOrigin().y + grid->getHeight() / 2;
    NS_Snake::Point2d snake_start_pos = grid->cellToXyCenter(grid->xyToCell(start_x, start_y));
    start_x = snake_start_pos.x;
    start_y = snake_start_pos.y;

    int num_parts_body = currLevel.getSnakeStartingLength();
    uint8_t accel = 5;
    // make head
    DirToFrameTable snakeDFT_head = dirToFrameTemplate("head.png");
    parts.push_back(std::make_unique<DirectedSprite>(snakeDFT_head));

    // make body
    DirToFrameTable snakeDFT_body = dirToFrameTemplate("body_straight.png");
    auto body_curve_frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("body_curve.png");
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::UP, SPRITE_DIRECTION::RIGHT }, 
        SpriteFrameTransform(body_curve_frame)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::UP, SPRITE_DIRECTION::LEFT },
        SpriteFrameTransform(body_curve_frame, 0, true, false)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::RIGHT, SPRITE_DIRECTION::DOWN },
        SpriteFrameTransform(body_curve_frame, 90)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::DOWN, SPRITE_DIRECTION::LEFT },
        SpriteFrameTransform(body_curve_frame, 180)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::LEFT, SPRITE_DIRECTION::UP },
        SpriteFrameTransform(body_curve_frame, 270)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::RIGHT, SPRITE_DIRECTION::UP },
        SpriteFrameTransform(body_curve_frame, 270, false, true)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::DOWN, SPRITE_DIRECTION::RIGHT },
        SpriteFrameTransform(body_curve_frame, 180, true, false)));
    snakeDFT_body.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::LEFT, SPRITE_DIRECTION::DOWN },
        SpriteFrameTransform(body_curve_frame, 270, true, false)));

    for (int i = 0; i < num_parts_body; i++)
        parts.push_back(std::make_unique<DirectedSprite>(snakeDFT_body));

    // make tail
    DirToFrameTable snakeDFT_tail = dirToFrameTemplate("tail.png");
    auto tail_curve_frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("tail_curve.png");
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::UP, SPRITE_DIRECTION::RIGHT },
        SpriteFrameTransform(tail_curve_frame)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::UP, SPRITE_DIRECTION::LEFT },
        SpriteFrameTransform(tail_curve_frame, 0, true, false)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::RIGHT, SPRITE_DIRECTION::DOWN },
        SpriteFrameTransform(tail_curve_frame, 90)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::DOWN, SPRITE_DIRECTION::LEFT },
        SpriteFrameTransform(tail_curve_frame, 180)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::LEFT, SPRITE_DIRECTION::UP },
        SpriteFrameTransform(tail_curve_frame, 270)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::RIGHT, SPRITE_DIRECTION::UP },
        SpriteFrameTransform(tail_curve_frame, 270, false, true)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::DOWN, SPRITE_DIRECTION::RIGHT },
        SpriteFrameTransform(tail_curve_frame, 180, true, false)));
    snakeDFT_tail.insert(std::pair<DirectionPair, SpriteFrameTransform>({ SPRITE_DIRECTION::LEFT, SPRITE_DIRECTION::DOWN },
        SpriteFrameTransform(tail_curve_frame, 270, true, false)));

    parts.push_back(std::make_unique<DirectedSprite>(snakeDFT_tail));

    // setup parts and construct snake
    // IMPORTANT: because we use interactively transformed sprites (rotated sprites) while snake moves and turns,
    // we must hold anchor point = (0.5, 0.5) for snake parts to correctly rotate sprite within the SAME grid cell!
    for (size_t i = 0; i < parts.size(); i++)
    {
        parts[i]->setPosition(Point2d(start_x, start_y - i*cell_size));
        parts[i]->setDirPair(DIRECTION_PAIR_UP);
        game_layer->addChild(parts[i]->getSprite());
    }
    // TODO: refactor to Builder pattern
    snake = std::make_unique<Snake>(parts, grid, /*speed*/cell_size, /*accel*/accel, /*max_health*/100, /*can_move_alone*/true);
    snake->setWrapAround(false);
 
    // setup food factory according to level's food table settings
    food_gen = std::make_unique<FoodGenerator>(currLevel.getFoodTable(), grid);

    // create some food
    for (int i = 0; i < currLevel.getNumStartingFood(); i++)
    {
        spawnFood(0, game_layer);
    }

    //set food spawner callback at random free grid cell
    if (currLevel.getSpawnFoodInterval() > 0)
    {
        game_layer->schedule([this](float dt) { spawnFood(dt, this->getChildByTag(TAG_GAME_LAYER)); }, currLevel.getSpawnFoodInterval(), "food_spawn");
    }

    // create some walls
    for (int i = 0; i < currLevel.getNumStartingWalls(); i++)
    {
        auto cell = NS_Snake::GameGrid::Cell(-1, -1);
        if (grid->getRandomFreeCell(cell))
        {
            grid->occupyCell(cell, NS_Snake::GameGrid::CellType::WALL);
            auto wall = Sprite::createWithSpriteFrameName("wall.png");
            NS_Snake::Point2d pos = grid->cellToXyCenter(cell);
            wall->setPosition(pos.toVec2());
            game_layer->addChild(wall);
        }
    }


    //////////////////////////////////////////////////
    // HUD LAYER
    // non-interactive elements over main game process
    
    auto hud_layer = LayerColor::create(Color4B(0, 0, 0, 0));

    auto label_level = Label::createWithTTF("level: " + std::to_string(this->currLevel.getNumber()), FONT_FILENAME_GAME_HUD, 24);
    if (label_level)
    {
        label_level->setPosition(Vec2(origin.x + visibleSize.width / 2 - 200,
            origin.y + visibleSize.height - 50));
    }
    hud_layer->addChild(label_level);

    ///////
    auto sprite_hud_time = Sprite::createWithSpriteFrameName("hud_clock.png");
    sprite_hud_time->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50));
    hud_layer->addChild(sprite_hud_time, 1);

    auto label_time = Label::createWithTTF("", FONT_FILENAME_GAME_HUD, 24);
    if (label_time)
    {
        label_time->setAnchorPoint(Vec2(0, 0.5f));
        label_time->setPosition(Vec2(sprite_hud_time->getBoundingBox().getMaxX() + 20,
            origin.y + visibleSize.height - 50));
    }
    hud_layer->addChild(label_time, 1, TAG_HUD_LAYER_TIMER_STRING);

    ///////
    auto sprite_hud_score = Sprite::createWithSpriteFrameName("hud_score.png");
    sprite_hud_score->setPosition(Vec2(origin.x + visibleSize.width * 0.66f, origin.y + visibleSize.height - 50));
    hud_layer->addChild(sprite_hud_score, 1);

    auto label_score = Label::createWithTTF("0", FONT_FILENAME_GAME_HUD, 24);
    label_score->setAnchorPoint(Vec2(0, 0.5f));
    if (label_score)
    {
        label_score->setPosition(Vec2(sprite_hud_score->getBoundingBox().getMaxX() + 20,
            origin.y + visibleSize.height - 50));
    }
    hud_layer->addChild(label_score, 1, TAG_HUD_LAYER_SCORE_STRING);

    ///////
    auto sprite_hud_health = Sprite::createWithSpriteFrameName("hud_health.png");
    sprite_hud_health->setPosition(Vec2(origin.x + visibleSize.width - 180, origin.y + visibleSize.height - 50));
    hud_layer->addChild(sprite_hud_health, 1);

    auto label_snake_health = Label::createWithTTF("", FONT_FILENAME_GAME_HUD, 24);
    if (label_snake_health)
    {
        label_snake_health->setAnchorPoint(Vec2(0, 0.5f));
        label_snake_health->setPosition(Vec2(sprite_hud_health->getBoundingBox().getMaxX() + 20,
            origin.y + visibleSize.height - 50));
    }
    hud_layer->addChild(label_snake_health, 1, TAG_HUD_LAYER_SNAKE_HEALTH_STRING);

    ///////
#ifdef COCOS2D_DEBUG
    auto label_snake_debug = Label::createWithTTF("", FONT_FILENAME_GAME_HUD, 24);
    if (label_snake_debug)
    {
        label_snake_debug->setPosition(Vec2(origin.x + 500,
            origin.y + 50));
    }
    hud_layer->addChild(label_snake_debug, 1, TAG_HUD_LAYER_SNAKE_HEAD_DEBUG_STRING);
#endif

    //////////////////////////////////////////////////
    // HUD CONTROL LAYER
    // interactive elements over main game process

    auto hud_control_layer = LayerColor::create(Color4B(0, 0, 0, 0));
    
    auto button_menu = ui::Button::create("game_menu_icon.png", "game_menu_icon.png");
    button_menu->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 50));
    button_menu->addClickEventListener([=](Ref* sender) {
        onGameMenuOpen(sender);
        });
    hud_control_layer->addChild(button_menu);

#ifdef COCOS2D_DEBUG
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
#endif
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
    this->schedule([this](float dt) {GameScene::updateTimer(dt); }, 1.0f, "UpdateTimerCallback");

    // callback when time is running out
    this->schedule([this](float dt) {
        if (time_elapsed > currLevel.getMaxTime() - 10)
        {
            AudioEngine::play2d("sound/tick.mp3");
            auto hud_layer = this->getChildByTag(TAG_HUD_LAYER);
            auto label_node = hud_layer->getChildByTag(TAG_HUD_LAYER_TIMER_STRING);
            auto label_time = static_cast<cocos2d::Label*> (label_node);
            label_time->setColor(cocos2d::Color3B::RED);
        }
     }, 1.0f, "clock_tick");

    up_pressed = false;
    down_pressed = false;
    right_pressed = false;
    left_pressed = false;
    up = 0;
    right = 0;

    score = 0;

    this->scheduleUpdate();

    return true;
}

void GameScene::onMouseDown(Event* event)
{
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
        return;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
    {
        left_pressed = true;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        right_pressed = true;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
        up_pressed = true;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        down_pressed = true;
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
    {
        left_pressed = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        right_pressed = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
        up_pressed = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        down_pressed = false;
    }
}

void GameScene::onGameMenuOpen(Ref* sender)
{
    Director::getInstance()->getScheduler()->pauseTarget(this);

    for (auto& tag : std::vector<LayersTags>({ TAG_GAME_LAYER, TAG_HUD_LAYER, TAG_HUD_CONTROL_LAYER }))
    {
        auto layer = this->getChildByTag(tag);
        MyCocos2dHelpers::CallCCNodeMethodRecursively(layer, [](Node* node) { node->pause(); });
    }

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

    }
}


void GameScene::onGameWin(Ref* sender)
{
    auto& glm = GameLevelManager::getInstance();  // auto& because returned type is ref: GameLevelManager& (instead, auto makes local copy of class instance, not ref)
    auto director = Director::getInstance();  // auto because returned type is pointer: Director* (makes local copy of pointer, but it is address on heap, so ok)
    if (glm.moveToNextLevel())
    {
        director->replaceScene(GameScene::create(glm.getCurrLevel()));
        director->pushScene(WinSplashScene::create());
    }
    else 
    {
        director->replaceScene(FinalSplashScene::create());
    }
}

void GameScene::onGameLoose(Ref* sender)
{
    auto& glm = GameLevelManager::getInstance();
    auto director = Director::getInstance();
    director->replaceScene(GameScene::create(glm.getCurrLevel()));
    director->pushScene(LooseSplashScene::create());
}

void GameScene::updateTimer(float dt)
{
    time_elapsed += dt;
}

void GameScene::updateInputDirectionState()
{
    up = int(up_pressed) - int(down_pressed);
    right = int(right_pressed) - int(left_pressed);
}

void GameScene::drawHUDString(int str_tag, const std::string& str)
{
    auto hud_layer = this->getChildByTag(TAG_HUD_LAYER);
    auto label_node = hud_layer->getChildByTag(str_tag);
    auto label = static_cast<cocos2d::Label*> (label_node);
    label->setString(str);

}

void GameScene::createHUDMovingScoreLabel(NS_Snake::IFoodPtr& f)
{
    float score_label_duration = 1.0f;
    auto score_label = cocos2d::Label::createWithTTF("+" + std::to_string(f->getScore()), FONT_FILENAME_GAME_HUD, 18);
    auto score_label_action = cocos2d::MoveBy::create(score_label_duration, cocos2d::Vec2(-40, 40));
    score_label->setPosition(f->getPosition().toVec2());
    score_label->runAction(score_label_action);
    auto score_label_name = "score_label_" + std::to_string(time_elapsed);
    this->getChildByTag(TAG_HUD_LAYER)->addChild(score_label, 999, score_label_name);
    score_label->scheduleOnce(
        [=](float dt) {score_label->removeFromParentAndCleanup(true); }, score_label_duration, "auto_remove_score_label");
}

void GameScene::spawnFood(float dt, cocos2d::Node* parent)
{
    using namespace NS_Snake;
    auto cell = NS_Snake::GameGrid::Cell(-1, -1);
    if (grid->getRandomFreeCell(cell))
    {
        grid->occupyCell(cell);
        auto new_food = food_gen->makeRandom();
        NS_Snake::Point2d pos = grid->cellToXyCenter(cell);
        
        // particle effect
        auto emitter = getParticleVFXSpawnFood(new_food->getFoodType(), new_food->getFoodSubType());
        parent->addChild(emitter, 10);
        emitter->setPosition(Vec2(pos.x, pos.y));

        new_food->getSprite()->setPosition(Vec2(pos.x, pos.y));
        parent->addChild(new_food->getSprite());
        food.push_back(std::move(new_food));
    }
}


void GameScene::update(float dt)
{
    // GAME LOGIC HERE

    if (snake->isAlive())
    {
        updateInputDirectionState();
        snake->move(up, right);

        snake->update();
    }
  
    if (!end_animation) 
    {
        if (!snake->isAlive() || time_elapsed >= currLevel.getMaxTime())
        {
            snake->kill();
            up = 0;
            right = 0;
            this->unschedule("UpdateTimerCallback");
            
            snake->runDeathAction(RepeatForever::create(Sequence::create(ToggleVisibility::create(), DelayTime::create(0.25f), nullptr)));
            auto wait_action = DelayTime::create(2.0f);
            wait_action->setTag(12345);
            auto wait_node = Node::create();
            wait_node->runAction(wait_action);
            this->addChild(wait_node, 2, "WaitNode");

            if (time_elapsed < currLevel.getMaxTime())
            {
                auto emitter = getParticleVFXSnakeDies();
                emitter->setPosition(snake->head().getToPosition().toVec2());
                this->addChild(emitter, 20);
                AudioEngine::play2d("sound/snake_collide.mp3", false, 0.5f);
            }

            end_animation = true;      
        }
    }
    if (end_animation && !this->getChildByName("WaitNode")->getActionByTag(12345))
    {
        onGameLoose(nullptr);
    }

    // disappear food if its lifetime is over
    for (auto food_iter = food.begin(); food_iter != food.end(); )
    {
        auto& f = *food_iter;
        if (f->getLifetime() > 0 && f->getTimeElapsed() >= f->getLifetime())
        {
            auto emitter = getParticleVFXDisappearFood(f->getFoodType(), f->getFoodSubType());
            this->getChildByTag(TAG_GAME_LAYER)->addChild(emitter, 10);
            emitter->setPosition(f->getPosition().toVec2());

            food_iter = food.erase(food_iter);
        }
        else
            ++food_iter;
    }
    
    for (auto& f : food)
    {
        if (snake->head().getSprite()->getBoundingBox().containsPoint(f->getSprite()->getPosition()))
        {
            snake->addHealth(f->getHealth());
            if (f->getHealth() > 0)
            {
                score += f->getScore();
                AudioEngine::play2d("sound/eat_good_food.mp3", false, 0.5f);
            }
            else
            {
                int grid_border_width = 10;
                auto grid_draw_border = DrawNode::create(grid_border_width);
                grid_draw_border->drawRect(Vec2(grid->getOrigin().x - grid_border_width / 2, grid->getOrigin().y - grid_border_width / 2),
                    Vec2(grid->getOrigin().x + grid->getWidth() + grid_border_width / 2, grid->getOrigin().y + grid->getHeight() + grid_border_width / 2),
                    Color4F::RED);
                grid_draw_border->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
                this->addChild(grid_draw_border, 1);

                AudioEngine::play2d("sound/eat_bad_food.ogg", false, 0.5f);
            }
            if (f->getFoodSubType() == NS_Snake::StaticFoodType::PORTAL)
            {
                if (!snake->isWrapAround())
                {
                    snake->setWrapAround(true);
                    auto grid_draw_border = this->getChildByTag(TAG_GAME_LAYER)->getChildByTag(TAG_GAME_LAYER_GRID_BORDER_RECT);
                    auto seq = Sequence::create(FadeOut::create(1.0f), nullptr);  // don't delete, reserve to option to restore after time 
                    grid_draw_border->runAction(seq);

                    AudioEngine::play2d("sound/portal_activate.mp3", false, 0.5f);
                }
            }

            createHUDMovingScoreLabel(f);
 
            food.remove(f);
            snake->addPart();
            spawnFood(0, this->getChildByTag(TAG_GAME_LAYER));
            break;
        }

        f->update();
    }

    if (score >= currLevel.getScoreNeeded())
    {
        onGameWin(nullptr);
    }
    
    //
    drawHUDString(TAG_HUD_LAYER_SCORE_STRING, std::to_string(score) + "/" + std::to_string(currLevel.getScoreNeeded()));
    drawHUDString(TAG_HUD_LAYER_TIMER_STRING, std::to_string(int(time_elapsed)) + "/" + std::to_string(currLevel.getMaxTime()));
    drawHUDString(TAG_HUD_LAYER_SNAKE_HEALTH_STRING, std::to_string(snake->getHealth()));

#ifdef COCOS2D_DEBUG
    auto head_cell = grid->xyToCell(snake->head().getPosition());
    drawHUDString(TAG_HUD_LAYER_SNAKE_HEAD_DEBUG_STRING, 
        "head_x:  " + std::to_string(snake->head().getPosition().x) + "   head_y:   " + std::to_string(snake->head().getPosition().y) + 
        "   head_cell_cix(col):  " + std::to_string(head_cell.cix) + "   head_cell_ciy(row):   " + std::to_string(head_cell.ciy)
    );
#endif

}