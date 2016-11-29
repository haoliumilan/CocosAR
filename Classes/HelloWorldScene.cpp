#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "OcUtility.hpp"

USING_NS_CC;

using namespace std;
using namespace cocostudio::timeline;

static string arrTalk[] = {"大腿求带", "一起玩的加", "出售数码蛋", "哥哥带带我吧", "公会收人", "求个活跃公会", "长期玩的加好友", "找个师傅教我玩",
    "组队任务来熟练工", "限时降临本找队友"};

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
    
    spBackgroud = NULL;
    spMonster = NULL;
    perCamera = NULL;
    selectedMon = NULL;
    lbLog = NULL;
    monScale = 1.0;
    isCameraBack = true;
    
    
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
    
    auto cameraItem = MenuItemFont::create("CAMERA", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    cameraItem->setTag(3);
    cameraItem->setPosition(Vec2(origin.x + visibleSize.width - 80,
                                 origin.y + 50));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(cameraItem, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu, 1);
    
    return true;
}

void HelloWorld::drawLine(cocos2d::Vec2 pos)
{
    auto dNode = DrawNode::create();
    this->addChild(dNode);
    dNode->drawLine(Vec2(0, 0), pos, Color4F(1, 0, 0, 1));
}

void HelloWorld::drawRect(cocos2d::Rect rect)
{
    auto dNode = DrawNode::create();
    this->addChild(dNode, 0, 9999);
    dNode->drawRect(rect.origin, rect.size, Color4F(1, 0, 0, 1));
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
    
    auto radius = zeye;
    auto newX = visibleSize.width/2;
    auto newY = 0;
    auto newZ = 0;
    auto newAngle = M_PI/4;
    
    scaleNode = Node::create();
    this->addChild(scaleNode, 1);
    
    for (int i = 0; i < 8; i++) {
        newAngle = i*M_PI/4;
        newZ = zeye-400;
        newX = visibleSize.width/2-radius*sinf(newAngle);
        newY = visibleSize.height/2+radius*cosf(newAngle);
        showOneMonster(newX, newY, newZ, newAngle, i);
    }

//    for (int i = 0; i < 8; i++) {
//        newAngle = i*M_PI/4+M_PI/8;
//        newZ = radius-100;
//        newX = visibleSize.width/2 - radius*sinf(newAngle)/2;
//        newY = visibleSize.height/2 + radius*cosf(newAngle)/2;
//        showOneMonster(newX, newY, newZ, newAngle, i+8);
//    }

    auto listener1 = EventListenerTouchAllAtOnce::create();
    listener1->onTouchesBegan = [](const std::vector<Touch*>& touches, Event  *event){
        auto touch = touches[0];
        auto layer = static_cast<HelloWorld*>(event->getCurrentTarget());
        if (touches.size() == 1 and touches[0]->getID() == 0) {
            auto monCount = layer->arrMonster.size();
            for (int i = 0; i < monCount; i++) {
                auto target = layer->arrMonster[i];
                Rect rect = target->getBoundingBox();
                auto isInRect = isScreenPointInRect(touch->getLocation(),
                                                    Camera::getDefaultCamera(),
                                                    layer->getWorldToNodeTransform(),
                                                    rect,
                                                    nullptr);
                //            auto isInRect = rect.containsPoint(touch->getLocation());
                if (isInRect)
                {
//                    layer->drawRect(rect);
                    log("sprite3d %d, %f, %f", i, -target->getRotation3D().z, layer->perCamera->getRotation3D().y);
                    log("rect = %f, %f, %f, %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
                    layer->selectedMon = target;
                    auto talk = layer->arrHead[i]->getChildByTag(2);
                    if (talk) {
                        log("talk talk, talk!!!");
                        talk->setVisible(true);
                    }
                    return;
                }
            }
        } else {
            layer->touchEnd();
        }
    };
    
    listener1->onTouchesMoved = [](const std::vector<Touch*>& touches, Event  *event){
        auto layer = static_cast<HelloWorld*>(event->getCurrentTarget());
        auto target = layer->selectedMon;
        if (touches.size() == 1 and touches[0]->getID() == 0 and target) {
            auto touch = touches[0];
            auto direction = touch->getDelta();
            
            auto mat = target->getWorldToNodeTransform();
            Vec3 scale;
            Quaternion quat;
            Vec3 trans;
            mat.decompose(&scale, &quat, &trans);
            Vec3 angle;
            OcUtility::getInstance()->getRotat3DFromQuat(&angle, &quat);
            
            auto monRotation = CC_DEGREES_TO_RADIANS(angle.z);
            direction.rotate(Vec2::ZERO, -monRotation);
            auto direction3D = Vec3(direction.x, direction.y, 0);
            
            auto index = target->getTag();
            if (std::abs(direction.x) > std::abs(direction.y))
            {
                auto roatationY = layer->arrRotationY[index];
                roatationY += direction.x/250;
                layer->arrRotationY.erase(layer->arrRotationY.begin()+index);
                layer->arrRotationY.insert(layer->arrRotationY.begin()+index, roatationY);
                
            } else {
                auto roatationX = layer->arrRotationX[index];
                roatationX -= direction.y/250;
                layer->arrRotationX.erase(layer->arrRotationX.begin()+index);
                layer->arrRotationX.insert(layer->arrRotationX.begin()+index, roatationX);
                
            }
        
        } else if (touches.size() == 2) {
            auto curLength = touches[0]->getLocation().getDistance(touches[1]->getLocation());
            auto preLength = touches[0]->getPreviousLocation().getDistance(touches[1]->getPreviousLocation());
            if (curLength > preLength)
            {
                layer->monScale += 0.02;
                layer->monScale = MIN(layer->monScale, 1.75);
            } else if (curLength < preLength) {
                layer->monScale -= 0.02;
                layer->monScale = MAX(layer->monScale, 0.75);
            }
//            layer->setScale(layer->monScale);
        }
    };
    
    listener1->onTouchesEnded = [=](const std::vector<Touch*>& touches, Event  *event){
        for( auto &touch: touches)
        {
            if (touch->getID() == 0)
            {
                auto layer = static_cast<HelloWorld*>(event->getCurrentTarget());
//                auto dNode = layer->getChildByTag(9999);
//                while(dNode)
//                {
//                    dNode->removeFromParent();
//                    dNode = layer->getChildByTag(9999);
//                }
                layer->touchEnd();
            }
        }

        log("sprite3d onTouchesEnded.. ");
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

}

void HelloWorld::touchEnd()
{
    if (selectedMon)
    {
        auto iTag = selectedMon->getTag();
        auto talk = arrHead[iTag]->getChildByTag(2);
        if (talk) {
            talk->setVisible(false);
        }
        selectedMon = NULL;
    }
}

void HelloWorld::updateCameraMonster()
{
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto monCount = arrMonster.size();
    for (int i = 0; i < monCount; i++) {
        auto spMon = arrMonster[i];
        auto spHead = arrHead[i];

        auto viewProjection = perCamera->getViewProjectionMatrix();
        auto cameraMat = Camera::getDefaultCamera()->getViewProjectionMatrix();

        auto modelMat = arrTransform[i];
        modelMat.rotateY(arrRotationY[i]);
        modelMat.rotateX(arrRotationX[i]);
        modelMat.scale(monScale);
        auto newMat = cameraMat.getInversed() * viewProjection * modelMat;
        spMon->setNodeToParentTransform(newMat);

        modelMat = arrTransform[i];
        modelMat.scale(0.1*monScale);
        modelMat.translate(-100, 200, 0);
        newMat = cameraMat.getInversed() * viewProjection * modelMat;
        spHead->setNodeToParentTransform(newMat);
    }
}

Sprite3D* HelloWorld::showOneMonster(float posX, float posY, float posZ, float rotate, int iTag)
{
    auto spMon = Sprite3D::create("monster/050/050_model.c3t");
    scaleNode->addChild(spMon, 1, iTag);
    spMon->setGlobalZOrder(1);
    spMon->setScaleX(10);
    spMon->setScaleY(10);
    spMon->setScaleZ(10);
    spMon->setPosition3D(Vec3(posX, posY, posZ));
    spMon->setRotation3D(Vec3(90, 0, -CC_RADIANS_TO_DEGREES(rotate)));
    arrMonster.push_back(spMon);
    auto transform = spMon->getNodeToParentTransform();
    arrTransform.push_back(transform);
    arrRotationY.push_back(0);
    arrRotationX.push_back(0);
    
//    auto aniName = "res/motion_1.c3t";
//    if (rand_0_1() > 0.5) {
//        aniName = "res/motion_2.c3t";
//    }
//    auto anim = Animation3D::create(aniName, "Take 001");
//    if (anim) {
//        auto animate = Animate3D::create(anim);
//        animate->setQuality(Animate3DQuality::QUALITY_HIGH);
//        auto repeate = RepeatForever::create(animate);
//        spMon->runAction(repeate);
//    }
    
    
    auto fileName = StringUtils::format("icon/head_%d.png", iTag+1);
    auto pBillBoard = Sprite::create(fileName);
    scaleNode->addChild(pBillBoard, 1, 1);
    pBillBoard->setGlobalZOrder(2);
    arrHead.push_back(pBillBoard);
    
    fileName = StringUtils::format("icon/badge_%d.png", random(1, 4));
    log("fileName = %s", fileName.c_str());
    auto badge = Sprite::create(fileName);
    pBillBoard->addChild(badge);
    badge->setPosition(100, 100);
    badge->setGlobalZOrder(3);
    
    auto talkBg = Sprite::create("talk_bg.png");
    pBillBoard->addChild(talkBg, 1, 2);
    talkBg->setPosition(215, 22);
    talkBg->setGlobalZOrder(4);
    talkBg->setVisible(false);
    
    auto pTalk = Label::createWithTTF(arrTalk[iTag].c_str(), "fonts/Marker Felt.ttf", 60);
    pTalk->setSystemFontName("Hiragino Sans GB");
    pTalk->setColor(Color3B::BLACK);
    pTalk->setPosition(100, 100);
    pTalk->setScale(2);
    talkBg->addChild(pTalk);
    pTalk->setGlobalZOrder(5);
    
    return spMon;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    OcUtility::setDeviceMotionEnabled(true);
    
    auto listener = EventListenerCustom::create("DeviceMotion", [=](EventCustom* event){
        DeviceMotion *motion = static_cast<DeviceMotion*>(event->getUserData());
        if (lbLog) {
            lbLog->setString(StringUtils::format("motion:pintch = %0.f, yaw = %0.f, roll = %0.f",
                                                 CC_RADIANS_TO_DEGREES(motion->pintch),
                                                 CC_RADIANS_TO_DEGREES(motion->yaw),
                                                 CC_RADIANS_TO_DEGREES(motion->roll)));
        }
        if (perCamera) {
            perCamera->setRotation3D(Vec3(-CC_RADIANS_TO_DEGREES(motion->roll),
                                          CC_RADIANS_TO_DEGREES(motion->pintch),
                                          -CC_RADIANS_TO_DEGREES(motion->yaw)));
        }
        
    });
    
    auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithFixedPriority(listener, 1);
    
    showARMonster();
    showCameraMonster();

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
            spMonster = Sprite3D::create("model_1.c3t");
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
    
    updateCameraMonster();
}

void HelloWorld::showSomeMonster() {
    for (int i = 0; i < 20; i++) {
        auto sp = Sprite3D::create("model_1.c3t");
        this->addChild(sp);
        sp->setGlobalZOrder(1);
        sp->setScale(10);
        float x = random(100, 500);
        float y = random(100, 500);
        sp->setPosition(Vec2(x, y));
        sp->setTag(111);
        
        auto anim = Animation3D::create("motion_1.c3t", "Take 001");
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
    int iTag = item->getTag();
    if (iTag == 1) {
        showARMonster();

    } else if (iTag == 2) {
        show3DMonsters();
    
    } else {
        bool result = OcUtility::getInstance()->switchCamera();
        if (result) {
            isCameraBack = !isCameraBack;
            if (spBackgroud) {
                spBackgroud->setFlippedY(!isCameraBack);
            }
        }
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

