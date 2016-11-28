//
//  RadarLayer.cpp
//  CocosAR
//
//  Created by liuhao on 23/11/2016.
//
//

#include "RadarLayer.hpp"

USING_NS_CC;

bool Radar::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = Sprite::create("radar_1.png");
    this->addChild(bg);

    auto zeye = Director::getInstance()->getZEye();
    auto eyeDegrees = CC_RADIANS_TO_DEGREES(atan2f(visibleSize.width*0.5, zeye)*2);
    log("eyeAngle = %f", eyeDegrees);

    auto eyeBg = ProgressTimer::create(Sprite::create("radar_2.png"));
    this->addChild(eyeBg);
    eyeBg->setType(ProgressTimer::Type::RADIAL);
    eyeBg->setMidpoint(Vec2(0.5f, 0.5f));
    eyeBg->setPercentage(eyeDegrees/3.6);
    eyeBg->setRotation(-eyeDegrees*0.5);
    
    auto scanSp = Sprite::create("radar_3.png");
    this->addChild(scanSp);
    scanSp->runAction(RepeatForever::create(RotateBy::create(2.0, 360)));
    
    pDrawNode = DrawNode::create();
    this->addChild(pDrawNode);
//    dNode->drawSolidCircle(Vec2(0, 0), 100, CC_DEGREES_TO_RADIANS(90), 8, Color4F(1, 0, 0, 1));
    
    return true;
}

void Radar::resetRadar()
{
    pDrawNode->clear();
}

void Radar::showRadarDot(float posX, float posY, cocos2d::Color4F color)
{
    pDrawNode->drawDot(Vec2(posX, posY), 10, color);
}

void Radar::showRadarDot(float posX, float posY)
{
    showRadarDot(posX, posY, Color4F::RED);
}

void Radar::showRadarTriangle(float posX, float posY)
{
    auto vec1 = Vec2(posX, posY+10*tanf(M_PI/3));
    auto vec2 = Vec2(posX-10*tanf(M_PI/3), posY-10);
    auto vec3 = Vec2(posX+10*tanf(M_PI/3), posY-10);
    pDrawNode->drawTriangle(vec1, vec2, vec3, Color4F::BLUE);
}

void Radar::setRadarRotate(float degrees)
{
    pDrawNode->setRotation(degrees);
}

