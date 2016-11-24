//
//  ARNearbyScene.cpp
//  CocosAR
//
//  Created by liuhao on 23/11/2016.
//
//

#include "ARNearbyScene.hpp"
#include "OcUtility.hpp"
#include "RadarLayer.hpp"
#include "OneMonsterLayer.hpp"

USING_NS_CC;

using namespace std;


Scene* ARNearby::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ARNearby::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ARNearby::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    spBackgroud = NULL;
    perCamera = NULL;
    pRadar = NULL;
    
    return true;
}

void ARNearby::onEnter()
{
    Layer::onEnter();
    
    OcUtility::setDeviceMotionEnabled(true);
    
    auto listener = EventListenerCustom::create("DeviceMotion", [=](EventCustom* event){
        DeviceMotion *motion = static_cast<DeviceMotion*>(event->getUserData());
        if (perCamera) {
            perCamera->setRotation3D(Vec3(-CC_RADIANS_TO_DEGREES(motion->roll),
                                          CC_RADIANS_TO_DEGREES(motion->pintch),
                                          -CC_RADIANS_TO_DEGREES(motion->yaw)));
            updateCameraMonster();
        }
        if (pRadar) {
            pRadar->setRadarRotate(CC_RADIANS_TO_DEGREES(motion->yaw));
        }
        
    });
    
    auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithFixedPriority(listener, 1);

    
    this->scheduleUpdate();
    OcUtility::getInstance()->showARControl();
    showCameraMonster();
    showOneMonsterLayer();
    
}

void ARNearby::onExit()
{
    Layer::onExit();
}

void ARNearby::showRadarLayer()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    pRadar = Radar::create();
    this->addChild(pRadar, 1);
    pRadar->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
    pRadar->setAnchorPoint(Vec2(0.5, 0.5));
}

void ARNearby::showOneMonsterLayer()
{
    auto pOneMonster = OneMonster::create();
    this->addChild(pOneMonster, 1);
    pOneMonster->showMonster();
}

void ARNearby::update(float delta)
{
    Texture2D *tex = OcUtility::getInstance()->getARTexture2D();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (tex != NULL) {
        if (spBackgroud == NULL) {
            spBackgroud = Sprite::createWithTexture(tex);
            spBackgroud->setPosition(visibleSize.width/2, visibleSize.height/2);
            this->addChild(spBackgroud);
            spBackgroud->setScale(2.0);
        } else {
            spBackgroud->setTexture(tex);
        }
    }
}

void ARNearby::showCameraMonster()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float zeye = Director::getInstance()->getZEye();
    perCamera = Camera::createPerspective(60, (GLfloat)visibleSize.width / visibleSize.height, 10,
                                          zeye + visibleSize.height / 2.0f);
    this->addChild(perCamera);
    perCamera->setCameraFlag(CameraFlag::USER1);
    Vec3 eye(visibleSize.width/2, visibleSize.height/2.0f, zeye),
    center(visibleSize.width/2, visibleSize.height/2, 0.0f),
    up(0.0f, 1.0f, 0.0f);
    perCamera->setPosition3D(eye);
    perCamera->lookAt(center, up);
    
    auto radius = zeye;
    auto newX = visibleSize.width/2;
    auto newY = 0;
    auto newZ = 0;
    auto newAngle = M_PI/4;
    
    for (int i = 0; i < 8; i++) {
        newAngle = i*M_PI/4;
        newZ = zeye-100;
        newX = visibleSize.width/2-radius*sinf(newAngle);
        newY = visibleSize.height/2+radius*cosf(newAngle);
        showOneMonster(newX, newY, newZ, newAngle, i);
        
        newX = (newX-visibleSize.width/2)*0.15;
        newY = (newY-visibleSize.height/2)*0.15;
        if (pRadar) {
            pRadar->showRadarDot(newX, newY);
        }
    }
        
}

Sprite3D* ARNearby::showOneMonster(float posX, float posY, float posZ, float rotate, int iTag)
{
    auto spMon = Sprite3D::create("res/model_1.c3t");
    this->addChild(spMon, 1, iTag);
    spMon->setGlobalZOrder(1);
    spMon->setScaleX(10);
    spMon->setScaleY(10);
    spMon->setScaleZ(10);
    spMon->setPosition3D(Vec3(posX, posY, posZ));
    spMon->setRotation3D(Vec3(90, 0, -CC_RADIANS_TO_DEGREES(rotate)));
    arrMonster.push_back(spMon);
    auto transform = spMon->getNodeToParentTransform();
    arrTransform.push_back(transform);

    return spMon;
}

void ARNearby::updateCameraMonster()
{
    //    Size visibleSize = Director::getInstance()->getVisibleSize();
    //    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto monCount = arrMonster.size();
    for (int i = 0; i < monCount; i++) {
        auto spMon = arrMonster[i];
        
        auto viewProjection = perCamera->getViewProjectionMatrix();
        auto cameraMat = Camera::getDefaultCamera()->getViewProjectionMatrix();
        
        auto modelMat = arrTransform[i];
        auto newMat = cameraMat.getInversed() * viewProjection * modelMat;
        spMon->setNodeToParentTransform(newMat);
        
    }
}


