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

class ARNearby : public cocos2d::Layer
{
    cocos2d::Sprite *spBackgroud;
    cocos2d::Camera *perCamera;

    std::vector<cocos2d::Sprite3D*> arrMonster;
    std::vector<cocos2d::Mat4> arrTransform;
    
    Radar* pRadar;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
  
    void update(float delta) override;

    // implement the "static create()" method manually
    CREATE_FUNC(ARNearby);
    
    void showCameraMonster();
    cocos2d::Sprite3D* showOneMonster(float posX, float posY, float posZ, float rotateY, int iTag);
    void updateCameraMonster();
    
    void showRadarLayer();
    void showOneMonsterLayer();
};

#endif /* ARNearbyScene_hpp */
