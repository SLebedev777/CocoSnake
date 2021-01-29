#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "GameLevel.h"
#include "Snake_fwd.h"
#include <list>

USING_NS_CC;

class GameScene : public Scene
{
public:
    GameScene(GameLevel& level);
    static Scene* createScene(GameLevel& level);

    virtual bool init();

    static GameScene* create(GameLevel& level);


    void onMouseDown(Event* event);
    void onMouseUp(Event* event);
    void onMouseMove(Event* event);
    void onMouseScroll(Event* event);

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    void onGameMenuClose(Event* event);
    void onGameMenuOpen(Ref* sender);

    void onGameWin(Ref* sender);
    void onGameLoose(Ref* sender);

    void updateTimer(float dt);

    // calc up and right based on pressed and released keyboard keys
    void updateInputDirectionState();
    void drawInputDirectionStateString();

    void update(float dt) override;

protected:
    GameLevel currLevel;
    NS_Snake::SnakePtr snake;
    NS_Snake::GameGridPtr grid;
    std::list<Sprite*> food;
    float time_elapsed = 0.0f;

    bool up_pressed;
    bool down_pressed;
    bool right_pressed;
    bool left_pressed;
    int up;    // 1 = up, -1 = down
    int right; // 1 = right, -1 = left
};

#endif // __GAMESCENE_H__
