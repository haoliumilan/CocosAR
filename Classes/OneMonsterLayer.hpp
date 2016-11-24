//
//  OneMonsterLayer.hpp
//  CocosAR
//
//  Created by liuhao on 24/11/2016.
//
//

#ifndef OneMonsterLayer_hpp
#define OneMonsterLayer_hpp

#include <stdio.h>
#include "cocos2d.h"

class OneMonster : public cocos2d::Layer
{
    cocos2d::Sprite3D *pMonster;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(OneMonster);
    
    void showMonster();
    
    void menuCallback(Ref* sender);

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;

};


#endif /* OneMonsterLayer_hpp */
