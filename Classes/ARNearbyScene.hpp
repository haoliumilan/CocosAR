//
//  ARNearbyScene.hpp
//  CocosAR
//
//  Created by liuhao on 23/11/2016.
//
//

#ifndef ARNearbyScene_hpp
#define ARNearbyScene_hpp

#include <stdio.h>
#include "cocos2d.h"

class Radar;
class DrawNode3D;
class Monster;

class ARNearby : public cocos2d::Layer
{
    cocos2d::Sprite *spBackgroud;
    cocos2d::Camera *perCamera;
    cocos2d::Camera *particleCamera;

    std::vector<Monster*> arrMonster;
    std::vector<cocos2d::Mat4> arrTransform;
    std::vector<cocos2d::Vec2> arrRadarPos;
    
    Radar* pRadar;
    
    cocos2d::DrawNode *pDrawNode;
    
    DrawNode3D *pDrawNode3D;
    
    cocos2d::EventListenerCustom *pMotionListener;
    cocos2d::EventListenerTouchOneByOne *pTouchListener;
    cocos2d::EventListenerCustom *pRestartListener;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
  
    void update(float delta) override;

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ARNearby);
    
    void showCameraMonster();
    void initMonster(float posX, float posY, float posZ, float rotateY, int iTag);
    void updateCameraMonster();
    
    void showRadarLayer();
    void showOneMonsterLayer(std::string mId, int index);
                
    void updateRadar();
    
};

#endif /* ARNearbyScene_hpp */
