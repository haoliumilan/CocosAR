//
//  OcUtility.hpp
//  CocoaAR
//
//  Created by admin on 7/4/16.
//
//

#ifndef OcUtility_hpp
#define OcUtility_hpp

#include "cocos2d.h"

class OcUtility
{
    static OcUtility* s_pcIf;

public:
    static OcUtility* getInstance();
    
    void showAR();
    
    void showOne();
};

#endif /* OcUtility_hpp */
