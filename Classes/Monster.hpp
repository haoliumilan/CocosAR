//
//  Monster.hpp
//  CocosAR
//
//  Created by liuhao on 25/11/2016.
//
//

#ifndef Monster_hpp
#define Monster_hpp

#include <stdio.h>

#include "cocos2d.h"

class Monster : public cocos2d::Node
{
    cocos2d::Sprite3D *pMonster;
    bool isGenerate;
    float timeInScreen;
    std::string monId;
public:
    
    virtual bool init() override;
    
    CREATE_FUNC(Monster);

    void initMonster(std::string mId, int index, float heightRate);
    
    cocos2d::Vec3 getCenter();

    void generateMonster();
    
    bool getIsGenerate();
    
    void setTimeInScreen(float time);
    float getTimeInScreen();
    
    cocos2d::Rect getTouchRect();
    cocos2d::AABB getAABB();
    std::string getMonsterId();
    
};

#endif /* Monster_hpp */
