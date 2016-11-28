//
//  RadarLayer.hpp
//  CocosAR
//
//  Created by liuhao on 23/11/2016.
//
//

#ifndef RadarLayer_hpp
#define RadarLayer_hpp

#include <stdio.h>

#include "cocos2d.h"

class Radar : public cocos2d::Node
{
    cocos2d::DrawNode* pDrawNode;
public:

    virtual bool init() override;
    
    CREATE_FUNC(Radar);

    void setRadarRotate(float degrees);
    void resetRadar();
    void showRadarDot(float posX, float posY, cocos2d::Color4F color);
    void showRadarDot(float posX, float posY);
    void showRadarTriangle(float posX, float posY);
};

#endif /* RadarLayer_hpp */
