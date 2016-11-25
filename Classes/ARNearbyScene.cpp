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
    
    pDrawNode3D = DrawNode3D::create();
    addChild(pDrawNode3D);
    pDrawNode3D->setCameraMask(4);
    
    pTouchListener = EventListenerTouchAllAtOnce::create();
    pTouchListener->onTouchesBegan = CC_CALLBACK_2(ARNearby::onTouchesBegan, this);
    pTouchListener->onTouchesMoved = CC_CALLBACK_2(ARNearby::onTouchesMoved, this);
    pTouchListener->onTouchesEnded = CC_CALLBACK_2(ARNearby::onTouchesEnded, this);
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

void ARNearby::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
    auto touch = touches[0];
    auto pos = touch->getLocation();
    
    pDrawNode3D->clear();
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
        auto isInRect = isScreenPointInRect(pos, Camera::getDefaultCamera(), getWorldToNodeTransform(),
                                    rect, nullptr);
        if (isInRect) {
            Vec3 corners[8];
            AABB aabb = monster->getAABB();
            OBB obb = OBB(aabb);
            obb.getCorners(corners);
            pDrawNode3D->drawCube(corners, Color4F(0, 1, 0, 1));
//            showOneMonsterLayer("1", monster->getTag());
            return;
        }
    }
}

void ARNearby::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{

};

void ARNearby::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    
};

void ARNearby::drawDot(float posX, float posY)
{
    pDrawNode->drawDot(Vec2(posX, posY), 10, Color4F(1, 0, 0, 1));
}

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
    auto newY = 0;
    auto newZ = 0;
    auto newAngle = M_PI/4;
    
    for (int i = 0; i < 8; i++) {
        newAngle = i*M_PI/4;
        newZ = zeye-100;
        newX = visibleSize.width/2-radius*sinf(newAngle);
        newY = visibleSize.height/2+radius*cosf(newAngle);
        
        initMonster(newX, newY, newZ, newAngle, i);
        
        newX = (newX-visibleSize.width/2)*0.15;
        newY = (newY-visibleSize.height/2)*0.15;
        if (pRadar) {
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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float zeye = Director::getInstance()->getZEye();
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
        
        newX = (newX-visibleSize.width/2)*0.15;
        newY = (newY-visibleSize.height/2)*0.15;
        if (arrMonster[i]->getIsGenerate() == false) {
            pRadar->showRadarDot(newX, newY);
        } else {
            pRadar->showRadarTriangle(newX, newY);
        }
    }

}

void ARNearby::initMonster(float posX, float posY, float posZ, float rotate, int iTag)
{
    auto pMonster = Monster::create();
    pMonster->initMonster("1", iTag);
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


