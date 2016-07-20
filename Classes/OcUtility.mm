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

using namespace cocos2d;

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
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtrl.viewController showARControl];
    
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
        if (i == 0) {
            texture2d->initWithData(oneImage->getPixels(),
                                    oneImage->getBufferWidth() * oneImage->getBufferHeight() * 2,
                                    Texture2D::PixelFormat::RGB565,
                                    oneImage->getBufferWidth(),
                                    oneImage->getBufferHeight(),
                                    cocos2d::Size(oneImage->getBufferWidth(), oneImage->getBufferHeight()));
        }
    }
    
    isTarget = false;
    for (int i = 0; i < state.getNumTrackableResults(); ++i) {
        // Get the trackable
        const Vuforia::TrackableResult* result = state.getTrackableResult(i);
        const Vuforia::Trackable& trackable = result->getTrackable();
        NSLog(@"trackable.getName() = %s", trackable.getName());
        Vuforia::Matrix44F modelViewProjection;
        Vuforia::Matrix44F modelViewMatrix = Vuforia::Tool::convertPose2GLMatrix(result->getPose());
        AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
        ARControl *arCtrl = [appCtrl.viewController getARControl];
        SampleApplicationUtils::translatePoseMatrix(0.0f, 0.0f, 10, &modelViewMatrix.data[0]);
        SampleApplicationUtils::scalePoseMatrix(10, 10, 10, &modelViewMatrix.data[0]);
        SampleApplicationUtils::multiplyMatrix(&arCtrl.vapp.projectionMatrix.data[0], &modelViewMatrix.data[0], &modelViewProjection.data[0]);
        SampleApplicationUtils::printMatrix(modelViewProjection.data);
        targetMat = Mat4(modelViewProjection.data);
        isTarget = true;
    }
    
    
    Vuforia::Renderer::getInstance().end();
    
    return texture2d;
}

void OcUtility::printMatrix(const float* mat)
{
    for (int r = 0; r < 4; r++, mat += 4) {
        printf("%7.3f %7.3f %7.3f %7.3f ", mat[0], mat[1], mat[2], mat[3]);
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





