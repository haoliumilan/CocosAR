#include "ARScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "OcUtility.hpp"

USING_NS_CC;

using namespace std;
using namespace cocostudio::timeline;

static string arrTalk[] = {"大腿求带", "一起玩的加", "出售数码蛋", "哥哥带带我吧", "公会收人", "求个活跃公会", "长期玩的加好友", "找个师傅教我玩",
    "组队任务来熟练工", "限时降临本找队友"};

Scene* ARScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ARScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ARScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    spBackgroud = NULL;
    spMonster = NULL;
    
    
//    auto rootNode = CSLoader::createNode("MainScene.csb");
//
//    addChild(rootNode);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
//    auto bg = Sprite::create("HelloWorld.png");
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
                                           CC_CALLBACK_1(ARScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - 180,
                                origin.y + 50));
    closeItem->setScale(2.0);
    closeItem->setTag(1);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);
    
    return true;
}

void ARScene::onEnter()
{
    Layer::onEnter();
    
}

void ARScene::onExit()
{
    Layer::onExit();
}

void ARScene::update(float delta)
{
    Texture2D *tex = OcUtility::getInstance()->getARTexture2D();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (tex != NULL) {
        if (spBackgroud == NULL) {
            spBackgroud = Sprite::createWithTexture(tex);
            spBackgroud->setPosition(visibleSize.width/2, visibleSize.height/2);
            this->addChild(spBackgroud);
            spBackgroud->setScale(2.0);
            spBackgroud->setRotation(90);
        } else {
            spBackgroud->setTexture(tex);
        }
    }
    
    if (OcUtility::getInstance()->getIsTarget() == true) {
        Mat4 mat = OcUtility::getInstance()->getTargetMat();
        cocos2d::Mat4 cameraMat = cocos2d::Camera::getDefaultCamera()->getProjectionMatrix();
//        cameraMat = cocos2d::Camera::getDefaultCamera()->getViewProjectionMatrix();
        mat = cameraMat.getInversed() * mat;
        
        Vec3 scale;
        Quaternion quat;
        Vec3 trans;
        mat.decompose(&scale, &quat, &trans);
        Vec3 angle;
        OcUtility::getInstance()->getRotat3DFromQuat(&angle, &quat);
        
        if (spMonster == NULL) {
            spMonster = Sprite3D::create("monster/050/050_model.c3t");
            this->addChild(spMonster);
            spMonster->setGlobalZOrder(1);
        }
        
        spMonster->setScaleX(scale.x*10);
        spMonster->setScaleY(scale.y*10*853/640);
//        spMonster->setScaleY(scale.y*10);
        spMonster->setScaleZ(scale.z*10);
        spMonster->setPosition3D(Vec3(visibleSize.width/2+trans.x*2,
                                      visibleSize.height/2+trans.y*2,
                                      visibleSize.height/2+trans.z));
//        spMonster->setPosition3D(trans);
        spMonster->setRotation3D(Vec3(angle.x, angle.y, angle.z));
        
    } else {
        if (spMonster != NULL) {
            spMonster->removeFromParent();
            spMonster = NULL;
        }
    }
    
}

void ARScene::menuCloseCallback(Ref* sender)
{
    MenuItemImage *item = (MenuItemImage *)sender;
    int iTag = item->getTag();
    if (iTag == 1) {
        showARMonster();

    }
}

void ARScene::showARMonster()
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

