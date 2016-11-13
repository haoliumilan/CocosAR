#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
    cocos2d::Sprite *spBackgroud;
    cocos2d::Sprite3D *spMonster;
    cocos2d::Label *lbLog;
    cocos2d::Camera *perCamera;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void menuCloseCallback(Ref* sender);
    
    void show3DMonsters();
    
    void showARMonster();
    
    void update(float delta) override;
    
    void showSomeMonster();
    void showOneMonster(float posX, float posY, float posZ, float rotateY);
    void showCameraMonster();
    
};

#endif // __HELLOWORLD_SCENE_H__
