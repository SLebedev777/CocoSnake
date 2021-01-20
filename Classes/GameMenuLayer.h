#ifndef __GAMEMENULAYER_H__
#define __GAMEMENULAYER_H__

#include "cocos2d.h"
// use fwd decl to skip include GameScene.h in this header (include only in .cpp)
// we can do that because we use only GameScene*
class GameScene;

class GameMenuLayer : public cocos2d::LayerColor
{
public:
    static GameMenuLayer* create(GameScene* from);

    GameMenuLayer(GameScene* from);
    ~GameMenuLayer() {}

    bool init() override;

    // resume game scene callback
    void menuResumeCallback(cocos2d::Ref* pSender);
    // back to main menu scene callback
    void menuBackToMainMenuCallback(cocos2d::Ref* pSender);

private:
    GameScene* game_scene = nullptr;
};

#endif // __GAMEMENULAYER_H__
