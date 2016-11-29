//
//  OneMonsterLayer.cpp
//  CocosAR
//
//  Created by liuhao on 24/11/2016.
//
//

#include "OneMonsterLayer.hpp"

USING_NS_CC;
using namespace std;

static string arrTalk[] = {"大腿求带", "一起玩的加好友", "出售数码蛋", "哥哥带带我吧", "公会收人",
    "求个活跃公会", "长期玩的加好友", "找个师傅教我玩", "组队任务来熟练工", "限时降临本找队友"};


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
    
    auto spBackground = Sprite::create("glass_bg.png");
    addChild(spBackground);
    spBackground->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    spBackground->setOpacity(150);
    
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
    runAnimation();
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

void OneMonster::runAnimation()
{
    pMonster->stopAllActions();
    auto fileName = StringUtils::format("monster/%s/%s_anim_%d.c3t",
                                   monId.c_str(), monId.c_str(), random(1, 3));
    auto anim = Animation3D::create(fileName, "Take 001");
    if (anim) {
        auto animate = Animate3D::create(anim);
        animate->setQuality(Animate3DQuality::QUALITY_HIGH);
        auto repeate = RepeatForever::create(animate);
        pMonster->runAction(repeate);
    }
}

void OneMonster::initMonster(std::string mId, int index)
{
    monId = mId;
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto backItem = MenuItemImage::create("btn_back.png", "btn_back.png",
                                          CC_CALLBACK_1(OneMonster::menuCallback, this));
    backItem->setTag(5);
    backItem->setPosition(Vec2(visibleSize.width - 60, visibleSize.height - 60));
    backItem->setScale(2);
    
    auto btn1 = MenuItemImage::create("btn_1.png", "btn_1.png",
                                          CC_CALLBACK_1(OneMonster::menuCallback, this));
    btn1->setTag(1);
    btn1->setPosition(Vec2(visibleSize.width/2 - 210, 100));
    btn1->setScale(2);

    auto btn2 = MenuItemImage::create("btn_2.png", "btn_2.png",
                                      CC_CALLBACK_1(OneMonster::menuCallback, this));
    btn2->setTag(2);
    btn2->setPosition(Vec2(visibleSize.width/2 - 70, 100));
    btn2->setScale(2);
    
    auto btn3 = MenuItemImage::create("btn_3.png", "btn_3.png",
                                      CC_CALLBACK_1(OneMonster::menuCallback, this));
    btn3->setTag(3);
    btn3->setPosition(Vec2(visibleSize.width/2 + 70, 100));
    btn3->setScale(2);
    
    auto btn4 = MenuItemImage::create("btn_4.png", "btn_4.png",
                                      CC_CALLBACK_1(OneMonster::menuCallback, this));
    btn4->setTag(4);
    btn4->setPosition(Vec2(visibleSize.width/2 + 210, 100));
    btn4->setScale(2);

    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, btn1, btn2, btn3, btn4, NULL);
    menu->setPosition(Vec2(0, 0));
    addChild(menu, 1);

    auto fileName = StringUtils::format("monster/%s/%s_model.c3t", mId.c_str(), mId.c_str());
    pMonster = Sprite3D::create(fileName);
    addChild(pMonster);
    pMonster->setScaleX(10);
    pMonster->setScaleY(10);
    pMonster->setScaleZ(10);
    pMonster->setPosition3D(Vec3(visibleSize.width*0.5, visibleSize.height*0.5-100, 0));
    pMonster->setGlobalZOrder(1);
    
    runAnimation();
    
    fileName = StringUtils::format("icon_2/head_%d.png", index+1);
    auto pHead = Sprite::create(fileName);
    addChild(pHead);
    pHead->setGlobalZOrder(1);
    pHead->setPosition(Vec2(80, visibleSize.height-100));
    pHead->setScale(0.4);
    
    fileName = StringUtils::format("icon_2/badge_%d.png", index%4+1);
    auto pBadge = Sprite::create(fileName);
    addChild(pBadge);
    pBadge->setGlobalZOrder(1);
    pBadge->setPosition(Vec2(80, visibleSize.height-100));
    pBadge->setScale(0.4);
    
    auto lbBg = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width, 100);
    addChild(lbBg);
    lbBg->setPosition(0, 200);
    
    int talkIndex = random(0, 9);
    auto pTalk = Label::createWithTTF(arrTalk[talkIndex].c_str(), "fonts/Marker Felt.ttf", 30);
    pTalk->setSystemFontName("Hiragino Sans GB");
    pTalk->setColor(Color3B::WHITE);
    pTalk->setPosition(visibleSize.width/2, 50);
    pTalk->setScale(2);
    lbBg->addChild(pTalk);
    
    
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
        case 5:
        {
            EventCustom event("Restart");
            auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
            dispatcher->dispatchEvent(&event);
            removeFromParent();
            break;
        }
        default:
            break;
    }
    
}

