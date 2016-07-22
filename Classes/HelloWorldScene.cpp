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
    isShow = false;
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
                                           "res/CloseNormal.png",
                                           "res/CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width,
                                origin.y + closeItem->getContentSize().height));
    closeItem->setScale(2.0);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    spBackgroud = NULL;
    
    spMonster = Sprite3D::create("res/model_1.c3t");
    this->addChild(spMonster);
    spMonster->setGlobalZOrder(1);
    spMonster->setPosition(visibleSize.width/2, visibleSize.height/2);
    spMonster->setScale(10);
//    spMonster->setRotation3D(Vec3(0, 45, 0));

    Mat4 mat;
//    mat = Mat4(10.000, 0.000, 0.000, 480.000, 0.000, 10.000, 0.000, 320.000, 0.000, 0.000, 10.000, 0.000, 0.000, 0.000, 0.000, 1.000);
//    spMonster->setNodeToParentTransform(mat);
//
    mat = spMonster->getNodeToParentTransform();
    OcUtility::getInstance()->printMatrix(mat.m);
    
    Vec3 scale;
    mat.getScale(&scale);
    log("scale = %f, %f, %f", scale.x, scale.y, scale.z);
    
    Quaternion rotat;
    mat.getRotation(&rotat);
    log("quat = %f, %f, %f, %f", rotat.x, rotat.y, rotat.z, rotat.w);
    
    Vec3 trans;
    mat.getTranslation(&trans);
    log("trans = %f, %f, %f", trans.x, trans.y, trans.z);
    
    mat = this->getNodeToParentTransform();
    OcUtility::getInstance()->printMatrix(mat.m);

    this->scheduleUpdate();

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
        } else {
            spBackgroud->setTexture(tex);
        }
    }
    
    if (OcUtility::getInstance()->getIsTarget() == true) {
        Mat4 mat = OcUtility::getInstance()->getTargetMat();
//        spMonster->setNodeToParentTransform(mat);
        Vec3 scale;
        Quaternion rotat;
        Vec3 trans;
        mat.decompose(&scale, &rotat, &trans);
        
        log("scale = %f, %f, %f", scale.x, scale.y, scale.z);
        log("quat = %f, %f, %f, %f", rotat.x, rotat.y, rotat.z, rotat.w);
        log("trans = %f, %f, %f", trans.x, trans.y, trans.z);
    }

}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    OcUtility::getInstance()->showARViewController();
    
//    if (isShow == false) {
//        OcUtility::getInstance()->showARViewController();
//        //    OcUtility::getInstance()->showARViewController();
//        isShow = true;
//    } else {
//        update(0);
//    }
    
}

