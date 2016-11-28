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
#include "DrawNode3D.h"
#include "Monster.hpp"

USING_NS_CC;

using namespace std;

static string arrMonName[] = {"027", "050", "088", "151", "302", "326"};
static float arrMonHeightRate[] = {1.2, 1.2, 0.85, 1.0, 0.9, 0.9};

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
    
    FileUtils::getInstance()->addSearchPath("res/particle3D/materials");
    FileUtils::getInstance()->addSearchPath("res/particle3D/scripts");

    pDrawNode = DrawNode::create();
    addChild(pDrawNode);
    pDrawNode->setCameraMask(4);
    
    pDrawNode3D = DrawNode3D::create();
    addChild(pDrawNode3D);
    pDrawNode3D->setCameraMask(4);
    
    pTouchListener = EventListenerTouchOneByOne::create();
    pTouchListener->onTouchBegan = CC_CALLBACK_2(ARNearby::onTouchBegan, this);
    pTouchListener->onTouchMoved = CC_CALLBACK_2(ARNearby::onTouchMoved, this);
    pTouchListener->onTouchEnded = CC_CALLBACK_2(ARNearby::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pTouchListener, this);

    pRestartListener = EventListenerCustom::create("Restart", [=](EventCustom* event){
        scheduleUpdate();
        pTouchListener->setEnabled(true);
        pMotionListener->setEnabled(true);
        
        for (auto&& monster : arrMonster) {
            monster->setVisible(true);
        }
        pRadar->setVisible(true);
    });
    _eventDispatcher->addEventListenerWithFixedPriority(pRestartListener, 1);

    return true;
}

void ARNearby::onEnter()
{
    Layer::onEnter();
    
    OcUtility::setDeviceMotionEnabled(true);
    
    pMotionListener = EventListenerCustom::create("DeviceMotion", [=](EventCustom* event){
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
    _eventDispatcher->addEventListenerWithFixedPriority(pMotionListener, 1);

    scheduleUpdate();
    OcUtility::getInstance()->showARControl();
    showRadarLayer();
    showCameraMonster();
}

void ARNearby::onExit()
{
    Layer::onExit();
}

bool ARNearby::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
//    pDrawNode->clear();
//    pDrawNode3D->clear();
 
    auto touchPos = touch->getLocation();
//    pDrawNode->drawDot(touchPos, 10, Color4F::ORANGE);

    float zeye = Director::getInstance()->getZEye();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Rect rect = Rect(origin.x+100, origin.y+100, visibleSize.width-200, visibleSize.height-200);
    for (auto&& monster : arrMonster) {
        auto center = monster->getCenter();
        auto pos = Camera::getDefaultCamera()->project(center);
        if (center.z > zeye || monster->getIsGenerate() == false) {
            continue;
        }
        
        Rect rect = monster->getTouchRect();
        auto isInRect = rect.containsPoint(touchPos);
        if (isInRect) {
//            pDrawNode->drawRect(rect.origin,
//                                Vec2(rect.origin.x+rect.size.width, rect.origin.y+rect.size.height),
//                                Color4F::RED);
//
//            Vec3 corners[8];
//            AABB aabb = monster->getAABB();
//            OBB obb = OBB(aabb);
//            obb.getCorners(corners);
//            pDrawNode3D->drawCube(corners, Color4F(0, 1, 0, 1));
            auto monId = monster->getMonsterId();
            showOneMonsterLayer(monId, monster->getTag());
            return true;
        }
    }
    
    return false;
}

void ARNearby::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

};

void ARNearby::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
};

void ARNearby::showRadarLayer()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    pRadar = Radar::create();
    addChild(pRadar, 1);
    pRadar->setPosition(100, visibleSize.height-100);
    pRadar->setAnchorPoint(Vec2(0.5, 0.5));
    pRadar->setScale(0.5);
}

void ARNearby::showOneMonsterLayer(std::string mId, int index)
{
    auto pOneMonster = OneMonster::create();
    addChild(pOneMonster, 1);
    pOneMonster->initMonster(mId, index);
    pOneMonster->setCameraMask(4);
    pOneMonster->setGlobalZOrder(10);
    
    unscheduleUpdate();
    pTouchListener->setEnabled(false);
    pMotionListener->setEnabled(false);
    
    for (auto&& monster : arrMonster) {
        monster->setVisible(false);
    }
    pRadar->setVisible(false);
}

void ARNearby::update(float delta)
{
    Texture2D *tex = OcUtility::getInstance()->getARTexture2D();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (tex != NULL) {
        if (spBackgroud == NULL) {
            spBackgroud = Sprite::createWithTexture(tex);
            spBackgroud->setPosition(visibleSize.width/2, visibleSize.height/2);
            addChild(spBackgroud);
            spBackgroud->setScale(2.0);
        } else {
            spBackgroud->setTexture(tex);
        }
    }
    
    float zeye = Director::getInstance()->getZEye();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Rect rect = Rect(origin.x+100, origin.y+100, visibleSize.width-200, visibleSize.height-200);
    bool isGen = false;
    for (auto&& monster : arrMonster) {
        if (monster->getIsGenerate()) {
            continue;
        }
        
        auto center = monster->getCenter();
        auto pos = Camera::getDefaultCamera()->project(center);
        if (center.z < zeye && rect.containsPoint(pos)) {
            auto time = monster->getTimeInScreen();
            monster->setTimeInScreen(time+delta);
            if (time > 2) {
                isGen = true;
                monster->generateMonster();
            }
            
        } else {
            monster->setTimeInScreen(0);
        }
    }
    
    if (isGen) {
        updateRadar();
    }

}

void ARNearby::showCameraMonster()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float zeye = Director::getInstance()->getZEye();
    perCamera = Camera::createPerspective(60, (GLfloat)visibleSize.width / visibleSize.height, 10,
                                          zeye + visibleSize.height / 2.0f);
    addChild(perCamera);
    perCamera->setCameraFlag(CameraFlag::USER1);
    Vec3 eye(visibleSize.width/2, visibleSize.height/2.0f, zeye),
    center(visibleSize.width/2, visibleSize.height/2, 0.0f),
    up(0.0f, 1.0f, 0.0f);
    perCamera->setPosition3D(eye);
    perCamera->lookAt(center, up);
    
    particleCamera = Camera::createPerspective(60, (GLfloat)visibleSize.width / visibleSize.height, 10,
                                          zeye + visibleSize.height / 2.0f);
    addChild(particleCamera);
    particleCamera->setCameraFlag(CameraFlag::USER2);
    particleCamera->setPosition3D(eye);
    particleCamera->lookAt(center, up);
    particleCamera->setDepth(1);
    
    auto radius = zeye;
    auto newX = visibleSize.width/2;
    auto newY = 0.0;
    auto newZ = 0.0;
    auto newAngle = 0.0;
    
    for (int i = 0; i < 10; i++) {
        newAngle = i*M_PI/5;
        newZ = zeye-300+200*rand_0_1();
        newX = visibleSize.width/2-radius*sinf(newAngle);
        newY = visibleSize.height/2+radius*cosf(newAngle);
        
        initMonster(newX, newY, newZ, newAngle, i);
        
        newX = (newX-visibleSize.width/2)*0.2;
        newY = (newY-visibleSize.height/2)*0.2;
        if (pRadar) {
            arrRadarPos.push_back(Vec2(newX, newY));
            pRadar->showRadarDot(newX, newY);
        }
    }
        
}

void ARNearby::updateRadar()
{
    if (pRadar == NULL)
    {
        return;
    }
    
    pRadar->resetRadar();
    
    for (auto&& monster : arrMonster) {
        auto i = monster->getTag();
        auto pos = arrRadarPos[i];
        if (arrMonster[i]->getIsGenerate() == false) {
            pRadar->showRadarDot(pos.x, pos.y);
        } else {
            pRadar->showRadarTriangle(pos.x, pos.y);
        }
    }

}

void ARNearby::initMonster(float posX, float posY, float posZ, float rotate, int iTag)
{
    auto index = random(0, 5);
    auto pMonster = Monster::create();
    pMonster->initMonster(arrMonName[index], iTag, arrMonHeightRate[index]);
    addChild(pMonster, 1, iTag);
    pMonster->setGlobalZOrder(1);
    pMonster->setCameraMask(4);
    pMonster->setScale(10);
    pMonster->setPosition3D(Vec3(posX, posY, posZ));
    pMonster->setRotation3D(Vec3(90, 0, -CC_RADIANS_TO_DEGREES(rotate)));
    
    arrMonster.push_back(pMonster);
    auto transform = pMonster->getNodeToParentTransform();
    arrTransform.push_back(transform);

}

void ARNearby::updateCameraMonster()
{
    for (auto&& monster : arrMonster) {
        auto viewProjection = perCamera->getViewProjectionMatrix();
        auto cameraMat = Camera::getDefaultCamera()->getViewProjectionMatrix();
        
        auto modelMat = arrTransform[monster->getTag()];
        auto newMat = cameraMat.getInversed() * viewProjection * modelMat;
        monster->setNodeToParentTransform(newMat);
    }
}

