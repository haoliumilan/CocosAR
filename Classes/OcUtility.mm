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

OcUtility* OcUtility::s_pcIf = NULL;

OcUtility* OcUtility::getInstance()
{
    if(s_pcIf == NULL){
        s_pcIf = new OcUtility();
    }
    
    return s_pcIf;
}

void OcUtility::showAR()
{
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtrl.viewController showAR];
    
}

void OcUtility::showOne()
{
    AppController *appCtrl = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtrl.viewController showOne];
}