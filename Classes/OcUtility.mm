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
    NSLog(@"getARResult");
    cocos2d::Texture2D* texture2d = new cocos2d::Texture2D();
    // Render video background and retrieve tracking state
    Vuforia::State state = Vuforia::Renderer::getInstance().begin();
    NSLog(@"state.getNumTrackables() = %d", state.getNumTrackables());
    Vuforia::Frame frame = state.getFrame();
    int num = frame.getNumImages();
    if (num == 0) {
        return NULL;
    }
    const Vuforia::Image *oneImage;
    for (int i = 0; i < num; i++)
    {
        oneImage = frame.getImage(i);
        NSLog(@"oneImage.getWidth() = %d, oneImage.getBufferWidth() = %d", oneImage->getWidth(), oneImage->getBufferWidth());
        NSLog(@"oneImage->getHeight() = %d, oneImage->getBufferHeight() = %d", oneImage->getHeight(), oneImage->getBufferHeight());
        NSLog(@"oneImage->getStride() = %d, oneImage->getFormat() = %d", oneImage->getStride(), oneImage->getFormat());
    }
    texture2d->initWithData(oneImage->getPixels(), 614400, Texture2D::PixelFormat::A8, 640, 480, cocos2d::Size(640, 480));
    
    for (int i = 0; i < state.getNumTrackableResults(); ++i) {
        // Get the trackable
        const Vuforia::TrackableResult* result = state.getTrackableResult(i);
        const Vuforia::Trackable& trackable = result->getTrackable();
        NSLog(@"trackable.getName() = %s", trackable.getName());
    }
    
    
    Vuforia::Renderer::getInstance().end();
    
    return texture2d;
}
