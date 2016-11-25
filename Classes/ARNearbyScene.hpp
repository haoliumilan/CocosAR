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

class ARNearby : public cocos2d::Layer
{
    cocos2d::Sprite *spBackgroud;
    cocos2d::Camera *perCamera;
    cocos2d::Camera *particleCamera;

    std::vector<cocos2d::Sprite3D*> arrMonster;
    std::vector<cocos2d::Mat4> arrTransform;
    
    Radar* pRadar;
    
    cocos2d::DrawNode *pDrawNode;
    
    DrawNode3D *pDrawNode3D;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
  
    void update(float delta) override;

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ARNearby);
    
    void showCameraMonster();
    cocos2d::Sprite3D* showOneMonster(float posX, float posY, float posZ, float rotateY, int iTag);
    void updateCameraMonster();
    
    void showRadarLayer();
    void showOneMonsterLayer();
    
    void drawDot(float posX, float posY);
    
    void showParticle3D(float posX, float posY, float posZ);
    
    void generateMonster(cocos2d::Sprite3D *monster);
};

#endif /* ARNearbyScene_hpp */
