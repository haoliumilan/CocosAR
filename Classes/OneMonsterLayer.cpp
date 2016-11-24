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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width, visibleSize.height);
    this->addChild(bgLayer);
    
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
        log("direction = %f, %f", direction.x, direction.y);
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


void OneMonster::showMonster()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto backItem = MenuItemFont::create("BACK", CC_CALLBACK_1(OneMonster::menuCallback, this));
    backItem->setTag(1);
    backItem->setPosition(Vec2(visibleSize.width - 80, visibleSize.height - 50));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);

    pMonster = Sprite3D::create("model_1.c3t");
    this->addChild(pMonster);
    pMonster->setScaleX(10);
    pMonster->setScaleY(10);
    pMonster->setScaleZ(10);
    pMonster->setPosition3D(Vec3(visibleSize.width*0.5, visibleSize.height*0.5-100, 0));
    pMonster->setGlobalZOrder(1);
    
    auto pHead = Sprite::create("icon_2/head_1.png");
    this->addChild(pHead);
    pHead->setGlobalZOrder(1);
    pHead->setPosition(Vec2(80, visibleSize.height-100));
    pHead->setScale(0.4);
    
    auto pBadge = Sprite::create("icon_2/badge_1.png");
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
            removeFromParent();
            break;
            
        default:
            break;
    }
    
}

