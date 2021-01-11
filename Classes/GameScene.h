#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "GameLevel.h"

USING_NS_CC;

class GameScene : public Scene
{
public:
    GameScene(GameLevel* level);
    static Scene* createScene(GameLevel* level);

    virtual bool init();

    static GameScene* create(GameLevel* level);


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

    GameLevel* getCurrLevel() { return currLevel; }

    void updateTimer(float dt);

protected:
    Sprite* player;
    GameLevel* currLevel;
    float time_elapsed = float(0.0);
};

#endif // __GAMESCENE_H__
