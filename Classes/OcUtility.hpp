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

class CC_DLL DeviceMotion
: public cocos2d::Ref
{
public:
    double rotationX;
    double rotationY;
    double rotationZ;
    double pintch;
    double yaw;
    double roll;
    
    DeviceMotion() {
        rotationX = 0.0;
        rotationY = 0.0;
        rotationZ = 0.0;
        pintch = 0.0;
        yaw = 0.0;
        roll = 0.0;
    }
};

class OcUtility
{
    static OcUtility* s_pcIf;
    cocos2d::Mat4 targetMat = cocos2d::Mat4::IDENTITY;
    bool isTarget = false;
public:
    static OcUtility* getInstance();
    
    void showARViewController();
    
    void showARControl();
    
    void showOne();
    
    cocos2d::Texture2D *getARTexture2D();
    
    static void printMatrix(const float* mat);

    cocos2d::Mat4 getTargetMat();
    bool getIsTarget();
    
    void getRotat3DFromQuat(cocos2d::Vec3 *rotat3D, cocos2d::Quaternion *rotatQuat);
    
    void getRotatQuatFrom3D(cocos2d::Quaternion *rotatQuat, cocos2d::Vec3 *rotat3D);
    
    cocos2d::Mat4 getProjectionMatrix();
    
    
    static void setDeviceMotionEnabled(bool isEnabled);
};

#endif /* OcUtility_hpp */
