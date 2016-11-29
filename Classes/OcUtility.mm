//
//  OcUtility.cpp
//  CocoaAR
//
//  Created by admin on 7/4/16.
//
//

#include "OcUtility.hpp"

#include "AppController.h"
#include "RootViewController.h"
#include <Vuforia/Vuforia.h>
#include <Vuforia/Renderer.h>
#include <Vuforia/State.h>
#include <Vuforia/Trackable.h>
#include <Vuforia/Frame.h>
#include <Vuforia/Image.h>
#include <Vuforia/TrackableResult.h>
#include <Vuforia/Tool.h>
#include "SampleApplicationUtils.h"

#import<CoreMotion/CoreMotion.h>


USING_NS_CC;

@interface CCDeviceMotionDispatcher : NSObject
{
    CMMotionManager *_motionManager;
    DeviceMotion *_deviceMotion;
}

+ (id) sharedDeviceMotionDispatcher;
- (id) init;
- (void) setDeviceMotionEnabled: (bool) isEnabled;

@end

@implementation CCDeviceMotionDispatcher

static CCDeviceMotionDispatcher* s_pDeviceMotionDispatcher;

+ (id) sharedDeviceMotionDispatcher
{
    if (s_pDeviceMotionDispatcher == nil) {
        s_pDeviceMotionDispatcher = [[self alloc] init];
    }
    
    return s_pDeviceMotionDispatcher;
}

- (id) init
{
    if( (self = [super init]) ) {
        _deviceMotion = new DeviceMotion();
        _motionManager = [[CMMotionManager alloc] init];
    }
    return self;
}

- (void) dealloc
{
    s_pDeviceMotionDispatcher = nullptr;
    delete _deviceMotion;
    [_motionManager release];
    [super dealloc];
}

- (void) setDeviceMotionEnabled: (bool) isEnabled
{
    if (isEnabled)
    {
        [_motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue currentQueue] withHandler:^(CMDeviceMotion *motion, NSError *error) {
            [self deviceMotion:motion];
        }];
        [_motionManager setDeviceMotionUpdateInterval:0.1];
    }
    else
    {
        [_motionManager stopDeviceMotionUpdates];
    }
}

- (void)deviceMotion:(CMDeviceMotion *)motion
{
    CMRotationRate rate = motion.rotationRate;
    CMAttitude *attitude = motion.attitude;
    _deviceMotion->rotationX = rate.x;
    _deviceMotion->rotationY = rate.y;
    _deviceMotion->rotationZ = rate.z;
    _deviceMotion->pintch = attitude.pitch;
    _deviceMotion->yaw = attitude.yaw;
    _deviceMotion->roll = attitude.roll;
    _deviceMotion->quater.x = attitude.quaternion.x;
    _deviceMotion->quater.y = attitude.quaternion.y;
    _deviceMotion->quater.z = attitude.quaternion.z;
    _deviceMotion->quater.w = attitude.quaternion.w;

    EventCustom event("DeviceMotion");
    event.setUserData(_deviceMotion);
    auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);

}

@end


OcUtility* OcUtility::s_pcIf = NULL;

OcUtility* OcUtility::getInstance()
{
    if(s_pcIf == NULL){
        s_pcIf = new OcUtility();
    }
    
    return s_pcIf;
}

void OcUtility::showARViewController()
{
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtrl.viewController showARViewController];
}

void OcUtility::showARControl()
{
//    experimental::Viewport vp(0, -106, 1136, 852);
//    Camera::setDefaultViewport(vp);
    
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtrl.viewController showARControl];
}

bool OcUtility::switchCamera()
{
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    return [appCtrl.viewController switchCamera];
}

void OcUtility::showOne()
{
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtrl.viewController showOne];
}

cocos2d::Texture2D *OcUtility::getARTexture2D()
{
    cocos2d::Texture2D* texture2d = new cocos2d::Texture2D();
    texture2d->autorelease();
    // Render video background and retrieve tracking state
    Vuforia::State state = Vuforia::Renderer::getInstance().begin();
    Vuforia::setFrameFormat(Vuforia::RGB565, YES);

    Vuforia::Frame frame = state.getFrame();
    int num = frame.getNumImages();
    if (num == 0) {
        return NULL;
    }
    const Vuforia::Image *oneImage;
    for (int i = 0; i < num; i++)
    {
        oneImage = frame.getImage(i);
        if (oneImage->getFormat() == 1) {
            texture2d->initWithData(oneImage->getPixels(),
                            oneImage->getBufferWidth() * oneImage->getBufferHeight() * 2,
                            Texture2D::PixelFormat::RGB565,
                            oneImage->getBufferWidth(),
                            oneImage->getBufferHeight(),
                            cocos2d::Size(oneImage->getBufferWidth(), oneImage->getBufferHeight()));
            break;
        }
    }
    
    isTarget = false;
    for (int i = 0; i < state.getNumTrackableResults(); ++i) {
        // Get the trackable
        const Vuforia::TrackableResult* result = state.getTrackableResult(i);
        const Vuforia::Trackable& trackable = result->getTrackable();
        NSLog(@"trackable.getName() = %s", trackable.getName());
        
        Vuforia::Matrix44F modelViewProjection;
        Vuforia::Matrix34F matrix34 = result->getPose();
        Vuforia::Matrix44F modelViewMatrix = Vuforia::Tool::convertPose2GLMatrix(matrix34);
        
//        float kObjectScaleNormal = 3.0f;
//        SampleApplicationUtils::translatePoseMatrix(0.0f, 0.0f, kObjectScaleNormal, &modelViewMatrix.data[0]);
//        SampleApplicationUtils::scalePoseMatrix(kObjectScaleNormal, kObjectScaleNormal, kObjectScaleNormal, &modelViewMatrix.data[0]);

        AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
        ARControl *arCtrl = [appCtrl.viewController getARControl];
        SampleApplicationUtils::multiplyMatrix(&arCtrl.vapp.projectionMatrix.data[0], &modelViewMatrix.data[0], &modelViewProjection.data[0]);
        targetMat = Mat4(modelViewProjection.data);
        isTarget = true;
    }
    
    
    Vuforia::Renderer::getInstance().end();
    
    return texture2d;
}

cocos2d::Mat4 OcUtility::getProjectionMatrix()
{
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    ARControl *arCtrl = [appCtrl.viewController getARControl];
    return Mat4(arCtrl.vapp.projectionMatrix.data);
}

void OcUtility::printMatrix(const float* mat)
{
    log("printMatrix ----");
    for (int r = 0; r < 4; r++) {
        log("%7.3f %7.3f %7.3f %7.3f ", mat[r], mat[r+4], mat[r+8], mat[r+12]);
    }
}

Mat4 OcUtility::getTargetMat()
{
    return targetMat;
}

bool OcUtility::getIsTarget()
{
    return isTarget;
}

//convert quaternion to Euler angle
void OcUtility::getRotat3DFromQuat(Vec3 *rotat3D, Quaternion *rotatQuat)
{
    float x = rotatQuat->x, y = rotatQuat->y, z = rotatQuat->z, w = rotatQuat->w;
    rotat3D->x = atan2f(2.f * (w * x + y * z), 1.f - 2.f * (x * x + y * y));
    float sy = 2.f * (w * y - z * x);
    if ((sy > 1) || (sy < -1)) {
        log("sy = %f", sy);
    }
    sy = clampf(sy, -1, 1);
    rotat3D->y = asinf(sy);
    rotat3D->z = atan2f(2.f * (w * z + x * y), 1.f - 2.f * (y * y + z * z));
    
    rotat3D->x = CC_RADIANS_TO_DEGREES(rotat3D->x);
    rotat3D->y = CC_RADIANS_TO_DEGREES(rotat3D->y);
    rotat3D->z = -CC_RADIANS_TO_DEGREES(rotat3D->z);
    
}

//convert Euler angle to quaternion
void OcUtility::getRotatQuatFrom3D(cocos2d::Quaternion *rotatQuat, cocos2d::Vec3 *rotat3D)
{
    float halfRadx = CC_DEGREES_TO_RADIANS(rotat3D->x / 2.f), halfRady = CC_DEGREES_TO_RADIANS(rotat3D->y / 2.f), halfRadz = -CC_DEGREES_TO_RADIANS(rotat3D->z / 2.f);
    float coshalfRadx = cosf(halfRadx), sinhalfRadx = sinf(halfRadx), coshalfRady = cosf(halfRady), sinhalfRady = sinf(halfRady), coshalfRadz = cosf(halfRadz), sinhalfRadz = sinf(halfRadz);
    rotatQuat->x = sinhalfRadx * coshalfRady * coshalfRadz - coshalfRadx * sinhalfRady * sinhalfRadz;
    rotatQuat->y = coshalfRadx * sinhalfRady * coshalfRadz + sinhalfRadx * coshalfRady * sinhalfRadz;
    rotatQuat->z = coshalfRadx * coshalfRady * sinhalfRadz - sinhalfRadx * sinhalfRady * coshalfRadz;
    rotatQuat->w = coshalfRadx * coshalfRady * coshalfRadz + sinhalfRadx * sinhalfRady * sinhalfRadz;

}

void OcUtility::setDeviceMotionEnabled(bool isEnabled)
{
    [[CCDeviceMotionDispatcher sharedDeviceMotionDispatcher] setDeviceMotionEnabled:isEnabled];
}




