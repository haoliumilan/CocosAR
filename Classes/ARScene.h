#ifndef __ARSCENE_SCENE_H__
#define __ARSCENE_SCENE_H__

#include "cocos2d.h"

class ARScene : public cocos2d::Layer
{
    cocos2d::Sprite *spBackgroud;
    cocos2d::Sprite3D *spMonster;
    cocos2d::Label *lbLog;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ARScene);
    
    void menuCloseCallback(Ref* sender);
    
    void showARMonster();
    
    void update(float delta) override;
    
};

#endif // __ARSCENE_SCENE_H__
