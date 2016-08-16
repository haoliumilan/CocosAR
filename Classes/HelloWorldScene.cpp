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
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width,
                                origin.y + closeItem->getContentSize().height));
    closeItem->setScale(2.0);
    closeItem->setTag(1);

    auto closeItem2 = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem2->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width*5,
                                origin.y + closeItem->getContentSize().height));
    closeItem2->setScale(2.0);
    closeItem2->setTag(2);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, closeItem2, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    spBackgroud = NULL;
    spMonster = NULL;

    return true;
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
        } else {
            spBackgroud->setTexture(tex);
        }
    }
    
    if (OcUtility::getInstance()->getIsTarget() == true) {
        Mat4 mat = OcUtility::getInstance()->getTargetMat();
        Vec3 scale;
        Quaternion quat;
        Vec3 trans;
        mat.decompose(&scale, &quat, &trans);
        log("scale = %f, %f, %f", scale.x, scale.y, scale.z);
        log("trans = %f, %f, %f", trans.x, trans.y, trans.z);
        Vec3 angle;
        OcUtility::getInstance()->getRotat3DFromQuat(&angle, &quat);
        log("angle = %f, %f, %f", angle.x, angle.y, angle.z);
        
        if (spMonster == NULL) {
            spMonster = Sprite3D::create("res/model_1.c3t");
            this->addChild(spMonster);
            spMonster->setGlobalZOrder(1);
        }

        spMonster->setPosition3D(Vec3(visibleSize.width/2+trans.x, visibleSize.height/2+trans.y, 600 - trans.z));
        spMonster->setScaleX(6);
        spMonster->setScaleY(6);
        spMonster->setScaleZ(6);
        spMonster->setRotation3D(Vec3(-angle.x + 90, -angle.y, angle.z));
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
        auto sp = this->getChildByTag(111);
        while (sp) {
            sp->removeFromParent();
            sp = NULL;
            sp = this->getChildByTag(111);
        }
        
        this->scheduleUpdate();
        OcUtility::getInstance()->showARControl();
    } else {
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
    
}

