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
    
//    auto rootNode = CSLoader::createNode("MainScene.csb");
//
//    addChild(rootNode);
    
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
    
    auto bg = Sprite::create("HelloWorld.png");
    this->addChild(bg);
    bg->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
    
//    orCamera = Camera::createOrthographic(visibleSize.width, visibleSize.height, 1, 1000);
//    this->addChild(orCamera);
//    orCamera->setCameraFlag(CameraFlag::USER1);
//    float zeye = Director::getInstance()->getZEye();
//    Vec3 eye(visibleSize.width/2, visibleSize.height/2.0f, zeye), center(visibleSize.width/2, visibleSize.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
//    orCamera->setPosition3D(eye);
//    orCamera->lookAt(center, up);
    Mat4 mat = Mat4( 18.632, 0.046, 1.186, 3.814,
                    -0.074, 14.048, 0.610, -33.859,
                    -0.652,  -0.386,   9.971, -271.332,
                    0.001,   0.000,  -0.011,   0.498 );
    
    auto spModel = Sprite3D::create("res/model_1.c3t");
    this->addChild(spModel);
    spModel->setGlobalZOrder(1);
    spModel->setScaleX(6);
    spModel->setScaleY(6);
    spModel->setScaleZ(6);
    spModel->setPosition3D(Vec3(visibleSize.width/2, visibleSize.height/2, visibleSize.height/2));
//    spMonster->setRotation3D(Vec3(180, 0, 0));
//    spMonster->setCameraMask(2);
    
    auto dNode = DrawNode::create();
    this->addChild(dNode);
    dNode->drawLine(Vec2(0, 0), Vec2(568, 320), Color4F(1, 0, 0, 1));
        
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
        cocos2d::Mat4 cameraMat = cocos2d::Camera::getDefaultCamera()->getProjectionMatrix();
        mat = cameraMat.getInversed() * mat;
//        mat.translate(visibleSize.width/2, visibleSize.height/2, visibleSize.height/2);
//        mat.scale(10, 10, 10);
//        OcUtility::getInstance()->printMatrix(mat.m);
        
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
//        log("trans = %f, %f, %f", trans.x, trans.y, trans.z);
        log("scale = %f, %f, %f", scale.x, scale.y, scale.z);
        
//        spMonster->setPosition3D(Vec3(trans.x, trans.y, trans.z));
//        spMonster->setScaleX(scale.x);
//        spMonster->setScaleY(scale.y);
//        spMonster->setScaleZ(scale.z);
//        spMonster->setRotationQuat(quat);
//        OcUtility::getInstance()->printMatrix(spMonster->getNodeToParentTransform().m);
        
        spMonster->setScaleX(scale.x*10);
        spMonster->setScaleY(scale.y*10*853/640);
        spMonster->setScaleZ(scale.z*10);
//        spMonster->setScaleX(6);
//        spMonster->setScaleY(6);
//        spMonster->setScaleZ(6);
        spMonster->setPosition3D(Vec3(visibleSize.width/2+trans.x*2, visibleSize.height/2+trans.y*2, visibleSize.height/2+trans.z));
        spMonster->setRotation3D(Vec3(angle.x, angle.y, angle.z));
        
//        spMonster->setNodeToParentTransform(mat);
        
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
