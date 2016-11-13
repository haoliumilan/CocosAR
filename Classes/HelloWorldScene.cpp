#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "OcUtility.hpp"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    auto rootNode = CSLoader::createNode("MainScene.csb");
//
//    addChild(rootNode);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
//    auto bg = Sprite::create("Icon-100.png");
//    this->addChild(bg);
//    bg->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
    
    lbLog = Label::createWithTTF("Hello World", "fonts/arial.ttf", 32);
    lbLog->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - lbLog->getContentSize().height));
    lbLog->setColor(Color3B::WHITE);
    this->addChild(lbLog, 1);

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - 180,
                                origin.y + 50));
    closeItem->setScale(2.0);
    closeItem->setTag(1);

    auto closeItem2 = MenuItemImage::create(
                                            "CloseNormal.png",
                                            "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem2->setPosition(Vec2(origin.x + visibleSize.width - 80,
                                 origin.y + 50));
    closeItem2->setScale(2.0);
    closeItem2->setTag(2);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, closeItem2, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);
    
    spBackgroud = NULL;
    spMonster = NULL;
    
//    auto dNode = DrawNode::create();
//    this->addChild(dNode);
//    dNode->drawLine(Vec2(0, 0), Vec2(568, 320), Color4F(1, 0, 0, 1));
    
    showCameraMonster();
    
    return true;
}

void HelloWorld::showCameraMonster()
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
    perCamera->setDepth(1);
    
    auto perCamera2 = Camera::createPerspective(60, (GLfloat)visibleSize.width / visibleSize.height, 10, zeye + visibleSize.height / 2.0f);
    this->addChild(perCamera2);
    perCamera2->setCameraFlag(CameraFlag::USER2);
    perCamera2->setPosition3D(eye);
    perCamera2->lookAt(center, up);
    
    auto radius = zeye;
    auto newX = visibleSize.width/2;
    auto newY = 0;
    auto newZ = 0;
    auto newAngle = M_PI/4;
    
    for (int i = 0; i < 8; i++) {
        newAngle = i*M_PI/4;
//        newZ = radius - radius * cosf(newAngle);
//        newX = visibleSize.width/2 - radius * sinf(newAngle);
        newZ = radius;
        newX = visibleSize.width/2 - radius*sinf(newAngle);
        newY = visibleSize.height/2 + radius*cosf(newAngle);
        showOneMonster(newX, newY, newZ, newAngle);
    }

}

void HelloWorld::showOneMonster(float posX, float posY, float posZ, float rotate)
{
    auto spMon = Sprite3D::create("res/model_1.c3t");
    this->addChild(spMon);
    spMon->setGlobalZOrder(1);
    spMon->setScaleX(10);
    spMon->setScaleY(10);
    spMon->setScaleZ(10);
    spMon->setPosition3D(Vec3(posX, posY, posZ));
    spMon->setRotation3D(Vec3(90, 0, -CC_RADIANS_TO_DEGREES(rotate)));
    spMon->setCameraMask(2);

}

void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    OcUtility::setDeviceMotionEnabled(true);
    
    auto listener = EventListenerCustom::create("DeviceMotion", [=](EventCustom* event){
        DeviceMotion *motion = static_cast<DeviceMotion*>(event->getUserData());
        lbLog->setString(StringUtils::format("motion:pintch = %0.f, yaw = %0.f, roll = %0.f",
                                             CC_RADIANS_TO_DEGREES(motion->pintch),
                                             CC_RADIANS_TO_DEGREES(motion->yaw),
                                             CC_RADIANS_TO_DEGREES(motion->roll)));
//        perCamera->setRotation3D(Vec3(90-CC_RADIANS_TO_DEGREES(motion->attitudeZ),
//                                      -CC_RADIANS_TO_DEGREES(motion->attitudeY),
//                                      180-CC_RADIANS_TO_DEGREES(motion->attitudeX)));
        perCamera->setRotation3D(Vec3(-CC_RADIANS_TO_DEGREES(motion->roll),
                                      CC_RADIANS_TO_DEGREES(motion->pintch),
                                      -CC_RADIANS_TO_DEGREES(motion->yaw)));
        

    });
    
    auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithFixedPriority(listener, 1);

}

void HelloWorld::onExit()
{
    OcUtility::setDeviceMotionEnabled(false);
    Layer::onExit();
}

void HelloWorld::update(float delta)
{
    Texture2D *tex = OcUtility::getInstance()->getARTexture2D();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (tex != NULL) {
        if (spBackgroud == NULL) {
            spBackgroud = Sprite::createWithTexture(tex);
            spBackgroud->setPosition(visibleSize.width/2, visibleSize.height/2);
            this->addChild(spBackgroud);
            spBackgroud->setScale(2.0);
            spBackgroud->setCameraMask(4);
        } else {
            spBackgroud->setTexture(tex);
        }
    }
    
    if (OcUtility::getInstance()->getIsTarget() == true) {
        Mat4 mat = OcUtility::getInstance()->getTargetMat();
        cocos2d::Mat4 cameraMat = cocos2d::Camera::getDefaultCamera()->getProjectionMatrix();
        mat = cameraMat.getInversed() * mat;
        
        Vec3 scale;
        Quaternion quat;
        Vec3 trans;
        mat.decompose(&scale, &quat, &trans);
        Vec3 angle;
        OcUtility::getInstance()->getRotat3DFromQuat(&angle, &quat);
        
        if (spMonster == NULL) {
            spMonster = Sprite3D::create("res/model_1.c3t");
            this->addChild(spMonster);
            spMonster->setGlobalZOrder(1);
//            spMonster->setCameraMask(2);
        }
        
        spMonster->setScaleX(scale.x*10);
        spMonster->setScaleY(scale.y*10*853/640);
        spMonster->setScaleZ(scale.z*10);
        spMonster->setPosition3D(Vec3(visibleSize.width/2+trans.x*2, visibleSize.height/2+trans.y*2, visibleSize.height/2+trans.z));
        spMonster->setRotation3D(Vec3(angle.x, angle.y, angle.z));
        
    } else {
        if (spMonster != NULL) {
            spMonster->removeFromParent();
            spMonster = NULL;
        }
    }

}

void HelloWorld::showSomeMonster() {
    for (int i = 0; i < 20; i++) {
        auto sp = Sprite3D::create("res/model_1.c3t");
        this->addChild(sp);
        sp->setGlobalZOrder(1);
        sp->setScale(10);
        float x = random(100, 500);
        float y = random(100, 500);
        sp->setPosition(Vec2(x, y));
        sp->setTag(111);
        
        auto anim = Animation3D::create("res/motion_1.c3t", "Take 001");
        if (anim) {
            auto animate = Animate3D::create(anim);
            animate->setQuality(Animate3DQuality::QUALITY_HIGH);
            auto repeate = RepeatForever::create(animate);
            sp->runAction(repeate);
        }
    }
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    MenuItemImage *item = (MenuItemImage *)sender;
    if (item->getTag() == 1) {
        showARMonster();
    } else {
        show3DMonsters();
    }
    
}

void HelloWorld::show3DMonsters()
{
    if (spMonster != NULL) {
        spMonster->removeFromParent();
        spMonster = NULL;
    }
    if (spBackgroud != NULL) {
        spBackgroud->removeFromParent();
        spBackgroud = NULL;
    }
    
    this->unscheduleUpdate();
    this->showSomeMonster();

}

void HelloWorld::showARMonster()
{
    auto sp = this->getChildByTag(111);
    while (sp) {
        sp->removeFromParent();
        sp = NULL;
        sp = this->getChildByTag(111);
    }
    
    this->scheduleUpdate();
    OcUtility::getInstance()->showARControl();

}

