//
//  OneMonsterLayer.cpp
//  CocosAR
//
//  Created by liuhao on 24/11/2016.
//
//

#include "OneMonsterLayer.hpp"

USING_NS_CC;

// on "init" you need to initialize your instance
bool OneMonster::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    pMonster = NULL;
    pParticle = NULL;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float zeye = Director::getInstance()->getZEye();

    auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width, visibleSize.height);
    this->addChild(bgLayer);
    
    Vec3 eye(visibleSize.width/2, visibleSize.height/2.0f, zeye),
    center(visibleSize.width/2, visibleSize.height/2, 0.0f),
    up(0.0f, 1.0f, 0.0f);
    auto particleCamera = Camera::createPerspective(60, (GLfloat)visibleSize.width / visibleSize.height, 10,
                                               zeye + visibleSize.height / 2.0f);
    addChild(particleCamera);
    particleCamera->setCameraFlag(CameraFlag::USER4);
    particleCamera->setPosition3D(eye);
    particleCamera->lookAt(center, up);
    particleCamera->setDepth(1);

    return true;
}

void OneMonster::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{

}

void OneMonster::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if (touches.size() == 1) {
        auto touch = touches[0];
        auto direction = touch->getDelta();
        auto rotation = pMonster->getRotation3D();
        if (std::abs(direction.x) > std::abs(direction.y))
        {
            pMonster->setRotation3D(Vec3(rotation.x, rotation.y+direction.x/10, rotation.z));
            
        } else {
            pMonster->setRotation3D(Vec3(rotation.x-direction.y/10, rotation.y, rotation.z));
        }
        
    } else if (touches.size() == 2) {
        auto curLength = touches[0]->getLocation().getDistance(touches[1]->getLocation());
        auto preLength = touches[0]->getPreviousLocation().getDistance(touches[1]->getPreviousLocation());
        auto scale = pMonster->getScale();
        if (curLength > preLength && scale < 17)
        {
            pMonster->setScale(scale+0.2);
        } else if (curLength < preLength && scale > 5) {
            pMonster->setScale(scale-0.2);
        }
    }
};

void OneMonster::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{

};


void OneMonster::initMonster(std::string mId, int index)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto backItem = MenuItemFont::create("BACK", CC_CALLBACK_1(OneMonster::menuCallback, this));
    backItem->setTag(1);
    backItem->setPosition(Vec2(visibleSize.width - 80, visibleSize.height - 50));

    auto scanItem = MenuItemFont::create("SCAN", CC_CALLBACK_1(OneMonster::menuCallback, this));
    scanItem->setTag(2);
    scanItem->setPosition(Vec2(visibleSize.width*0.5, 100));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, scanItem, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);

    auto fileName = StringUtils::format("monster/%s/%s_model.c3t", mId.c_str(), mId.c_str());
    pMonster = Sprite3D::create(fileName);
    this->addChild(pMonster);
    pMonster->setScaleX(10);
    pMonster->setScaleY(10);
    pMonster->setScaleZ(10);
    pMonster->setPosition3D(Vec3(visibleSize.width*0.5, visibleSize.height*0.5-100, 0));
    pMonster->setGlobalZOrder(1);
    
    fileName = StringUtils::format("monster/%s/%s_anim_%d.c3t",
                                   mId.c_str(), mId.c_str(), random(1, 3));
    auto anim = Animation3D::create(fileName, "Take 001");
    if (anim) {
        auto animate = Animate3D::create(anim);
        animate->setQuality(Animate3DQuality::QUALITY_HIGH);
        auto repeate = RepeatForever::create(animate);
        pMonster->runAction(repeate);
    }
    
    fileName = StringUtils::format("icon_2/head_%d.png", index+1);
    auto pHead = Sprite::create(fileName);
    this->addChild(pHead);
    pHead->setGlobalZOrder(1);
    pHead->setPosition(Vec2(80, visibleSize.height-100));
    pHead->setScale(0.4);
    
    fileName = StringUtils::format("icon_2/badge_%d.png", index%4+1);
    auto pBadge = Sprite::create(fileName);
    this->addChild(pBadge);
    pBadge->setGlobalZOrder(1);
    pBadge->setPosition(Vec2(80, visibleSize.height-100));
    pBadge->setScale(0.4);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(OneMonster::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(OneMonster::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(OneMonster::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}

void OneMonster::menuCallback(Ref* sender)
{
    MenuItemImage *item = (MenuItemImage *)sender;
    int iTag = item->getTag();
    switch (iTag) {
        case 1:
        {
            EventCustom event("Restart");
            auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
            dispatcher->dispatchEvent(&event);
            removeFromParent();
            break;
        }
        case 2:
        {
            pParticle = PUParticleSystem3D::create("timeShift.pu", "pu_mediapack_01.material");
            Size visibleSize = Director::getInstance()->getVisibleSize();
            pParticle->setPosition3D(Vec3(visibleSize.width*0.5, visibleSize.height*0.5-100, 0));
            pParticle->startParticleSystem();
            addChild(pParticle, 1);
            pParticle->setGlobalZOrder(1);
            pParticle->setCameraMask(8);
            
            break;
        }
        default:
            break;
    }
    
}

