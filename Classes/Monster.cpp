//
//  Monster.cpp
//  CocosAR
//
//  Created by liuhao on 25/11/2016.
//
//

#include "Monster.hpp"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;
using namespace std;

bool Monster::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    pMonster = NULL;
    isGenerate = false;
    timeInScreen = 0;
    monId = "050";
    
    return true;
}

void Monster::initMonster(std::string mId, int index, float heightRate)
{
    monId = mId;
    auto fileName = StringUtils::format("monster/%s/%s_model.c3t", mId.c_str(), mId.c_str());
    pMonster = Sprite3D::create(fileName);
    addChild(pMonster);
    pMonster->setOpacity(0);
    pMonster->setGlobalZOrder(1);
    pMonster->setCascadeOpacityEnabled(true);
    
    fileName = StringUtils::format("monster/%s/%s_anim_%d.c3t",
                                   mId.c_str(), mId.c_str(), random(1, 3));
    auto anim = Animation3D::create(fileName, "Take 001");
    if (anim) {
        auto animate = Animate3D::create(anim);
        animate->setQuality(Animate3DQuality::QUALITY_HIGH);
        auto repeate = RepeatForever::create(animate);
        pMonster->runAction(repeate);
    }
    
    auto rect = pMonster->getBoundingBox();
    auto posY = rect.size.height*heightRate;
    log("rect size = %f, %f", rect.size.width, rect.size.height);
    fileName = StringUtils::format("icon_2/head_%d.png", index+1);
    auto pHead = Sprite::create(fileName);
    pMonster->addChild(pHead, 1);
    pHead->setGlobalZOrder(2);
    pHead->setPosition3D(Vec3(0, posY, 0));
    pHead->setScale(0.03);
    
    fileName = StringUtils::format("icon_2/badge_%d.png", index%4+1);
    auto badge = Sprite::create(fileName);
    pMonster->addChild(badge);
    badge->setGlobalZOrder(3);
    badge->setPosition3D(Vec3(0, posY, 0));
    badge->setScale(0.03);

}

Vec3 Monster::getCenter()
{
    auto aabb = pMonster->getAABB();
    auto center = aabb.getCenter();
    return center;
}

void Monster::generateMonster()
{
    isGenerate = true;
    
    auto rootps = PUParticleSystem3D::create("timeShift.pu", "pu_mediapack_01.material");
    rootps->setScale(2.0f);
    rootps->setPosition3D(Vec3(0, 10, 0));
    rootps->startParticleSystem();
    addChild(rootps, 1);
    rootps->setGlobalZOrder(2);
    rootps->setCameraMask(4);

    pMonster->runAction(FadeIn::create(5.0));

}

bool Monster::getIsGenerate()
{
    return isGenerate;
}

void Monster::setTimeInScreen(float time)
{
    timeInScreen = time;
}

float Monster::getTimeInScreen()
{
    return timeInScreen;
}

Rect Monster::getTouchRect()
{
    return pMonster->getBoundingBox();
}

AABB Monster::getAABB()
{
    return pMonster->getAABB();
}

std::string Monster::getMonsterId()
{
    return monId;
}




