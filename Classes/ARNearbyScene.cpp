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
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "DrawNode3D.h"

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
    this->addChild(pDrawNode);
    
    pDrawNode3D = DrawNode3D::create();
    this->addChild(pDrawNode3D);
    pDrawNode3D->setCameraMask(4);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(ARNearby::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(ARNearby::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(ARNearby::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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
        auto aabb = monster->getAABB();
        auto center = aabb.getCenter();
        if (center.z < zeye && rect.containsPoint(Vec2(center.x, center.y))) {
            generateMonster(monster);
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

void ARNearby::generateMonster(cocos2d::Sprite3D *monster)
{
    auto aabb = monster->getAABB();
    auto center = aabb.getCenter();
    showParticle3D(center.x, center.y, center.z);
    monster->runAction(FadeIn::create(5.0));

}

void ARNearby::showParticle3D(float posX, float posY, float posZ)
{
    auto rootps = PUParticleSystem3D::create("timeShift.pu", "pu_mediapack_01.material");
    rootps->setScale(10.0f);
    rootps->startParticleSystem();
    this->addChild(rootps, 1);
    rootps->setGlobalZOrder(2);
    rootps->setPosition3D(Vec3(posX, posY, posZ));
    rootps->setCameraMask(4);
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
    
    particleCamera = Camera::createPerspective(60, (GLfloat)visibleSize.width / visibleSize.height, 10,
                                          zeye + visibleSize.height / 2.0f);
    this->addChild(particleCamera);
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
    spMon->setCameraMask(4);
    spMon->setOpacity(0);
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
    for (auto&& monster : arrMonster) {
        auto viewProjection = perCamera->getViewProjectionMatrix();
        auto cameraMat = Camera::getDefaultCamera()->getViewProjectionMatrix();
        
        auto modelMat = arrTransform[monster->getTag()];
        auto newMat = cameraMat.getInversed() * viewProjection * modelMat;
        monster->setNodeToParentTransform(newMat);
    }
}


