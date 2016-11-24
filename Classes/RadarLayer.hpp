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
    
    void showRadarDot(float posX, float posY);
    void setRadarRotate(float degrees);
};

#endif /* RadarLayer_hpp */
