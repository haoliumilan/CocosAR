/*===============================================================================
Copyright (c) 2016 PTC Inc. All Rights Reserved.

Copyright (c) 2012-2015 Qualcomm Connected Experiences, Inc. All Rights Reserved.

Vuforia is a trademark of PTC Inc., registered in the United States and other 
countries.
===============================================================================*/

#import "ARControl.h"
#import "AppController.h"
#import "RootViewController.h"
#import <Vuforia/Vuforia.h>
#import <Vuforia/TrackerManager.h>
#import <Vuforia/ObjectTracker.h>
#import <Vuforia/Trackable.h>
#import <Vuforia/DataSet.h>
#import <Vuforia/CameraDevice.h>
#import <Vuforia/Renderer.h>
#import <Vuforia/TrackableResult.h>
#import "SampleApplicationUtils.h"
#import <Vuforia/Frame.h>
#import <Vuforia/Image.h>

@interface ARControl ()

@end

@implementation ARControl

@synthesize vapp;
@synthesize isBackCamera;

- (id)initWithParentViewController:(UIViewController *)viewCtrl
{
    self = [super init];
    
    isBackCamera = NO;
    // Custom initialization
    vapp = [[SampleApplicationSession alloc] initWithDelegate:self];
    
    CGRect viewFrame = [self getCurrentARViewFrame];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(dismissARViewController)
                                                 name:@"kDismissARViewController"
                                               object:nil];
    
    // we use the iOS notification to pause/resume the AR when the application goes (or come back from) background
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(pauseAR)
     name:UIApplicationWillResignActiveNotification
     object:nil];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(resumeAR)
     name:UIApplicationDidBecomeActiveNotification
     object:nil];
    
    // initialize AR
    [vapp initAR:Vuforia::GL_20 orientation:viewCtrl.interfaceOrientation];
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (BOOL)switchCamera
{
    NSError * error = nil;
    if ([vapp stopCamera:&error]) {
        bool result = NO;
        if (isBackCamera) {
            result = [vapp startAR:Vuforia::CameraDevice::CAMERA_DIRECTION_FRONT error:&error];
            isBackCamera = NO;
        } else {
            result = [vapp startAR:Vuforia::CameraDevice::CAMERA_DIRECTION_BACK error:&error];
            isBackCamera = YES;
        }

        return result;
    } else {
        return NO;
    }
   
}

#pragma mark - SampleApplicationControl
// callback called when the initailization of the AR is done
- (void) onInitARDone:(NSError *)initError {
    NSLog(@"onInitARDone");
    if (initError == nil) {
        NSError * error = nil;
        [vapp startAR:Vuforia::CameraDevice::CAMERA_DIRECTION_BACK error:&error];
        isBackCamera = YES;
        
    } else {
        NSLog(@"Error initializing AR:%@", [initError description]);
        dispatch_async( dispatch_get_main_queue(), ^{
            
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                            message:[initError localizedDescription]
                                                           delegate:self
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
            [alert show];
        });
    }
}

// Initialize the application trackers
- (bool) doInitTrackers {
    // Initialize the object tracker
    Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
    Vuforia::Tracker* trackerBase = trackerManager.initTracker(Vuforia::ObjectTracker::getClassType());
    if (trackerBase == NULL)
    {
        NSLog(@"Failed to initialize ObjectTracker.");
        return false;
    }
    return true;
}

// load the data associated to the trackers
- (bool) doLoadTrackersData {
    return [self loadObjectTrackerDataSet:@"res/StonesAndChips.xml"];
}

// start the application trackers
- (bool) doStartTrackers {
    Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
    Vuforia::Tracker* tracker = trackerManager.getTracker(Vuforia::ObjectTracker::getClassType());
    if(tracker == 0) {
        return false;
    }
    tracker->start();
    return true;
}

- (bool) doStopTrackers {
    // Stop the tracker
    Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
    Vuforia::Tracker* tracker = trackerManager.getTracker(Vuforia::ObjectTracker::getClassType());
    
    if (NULL != tracker) {
        tracker->stop();
        NSLog(@"INFO: successfully stopped tracker");
        return YES;
    }
    else {
        NSLog(@"ERROR: failed to get the tracker from the tracker manager");
        return NO;
    }
}

- (bool) doUnloadTrackersData {
    if (dataSetCurrent != NULL) {
        // Get the object tracker:
        Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
        Vuforia::ObjectTracker* objectTracker = static_cast<Vuforia::ObjectTracker*>(trackerManager.getTracker(Vuforia::ObjectTracker::getClassType()));
        
        if (objectTracker == NULL)
        {
            NSLog(@"Failed to unload tracking data set because the ObjectTracker has not been initialized.");
            return false;
        }
        // Activate the data set:
        if (!objectTracker->deactivateDataSet(dataSetCurrent))
        {
            NSLog(@"Failed to deactivate data set.");
            return false;
        }
        // Activate the data set:
        if (!objectTracker->destroyDataSet(dataSetCurrent))
        {
            NSLog(@"Failed to destroy data set.");
            return false;
        }
        dataSetCurrent = NULL;
    }
    return true;
}

- (bool) doDeinitTrackers {
    Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
    trackerManager.deinitTracker(Vuforia::ObjectTracker::getClassType());
    return YES;
}

#pragma mark - UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [[NSNotificationCenter defaultCenter] postNotificationName:@"kDismissARViewController" object:nil];
}

#pragma mark -
- (void) pauseAR {
    NSError * error = nil;
    if (![vapp pauseAR:&error]) {
        NSLog(@"Error pausing AR:%@", [error description]);
    }
}

- (void) resumeAR {
    NSError * error = nil;
    if(! [vapp resumeAR:&error]) {
        NSLog(@"Error resuming AR:%@", [error description]);
    }
    // on resume, we reset the flash
    Vuforia::CameraDevice::getInstance().setFlashTorchMode(false);
}

// Load the image tracker data set
- (BOOL)loadObjectTrackerDataSet:(NSString*)dataFile
{
    NSLog(@"loadObjectTrackerDataSet (%@)", dataFile);
    BOOL ret = YES;
    dataSetCurrent = NULL;
    
    // Get the Vuforia tracker manager image tracker
    Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
    Vuforia::ObjectTracker* objectTracker = static_cast<Vuforia::ObjectTracker*>(trackerManager.getTracker(Vuforia::ObjectTracker::getClassType()));
    
    if (NULL == objectTracker) {
        NSLog(@"ERROR: failed to get the ObjectTracker from the tracker manager");
        return NULL;
    } else {
        dataSetCurrent = objectTracker->createDataSet();
        
        if (NULL != dataSetCurrent) {
            NSLog(@"INFO: successfully loaded data set");
            
            // Load the data set from the app's resources location
            if (!dataSetCurrent->load([dataFile cStringUsingEncoding:NSASCIIStringEncoding], Vuforia::STORAGE_APPRESOURCE)) {
                NSLog(@"ERROR: failed to load data set");
                objectTracker->destroyDataSet(dataSetCurrent);
                dataSetCurrent = NULL;
                ret = NO;
            }
            else {
                // Activate the data set
                if (objectTracker->activateDataSet(dataSetCurrent)) {
                    NSLog(@"INFO: successfully activated data set");
                }
                else {
                    NSLog(@"ERROR: failed to activate data set");
                    ret = NO;
                }
            }
        }
        else {
            NSLog(@"ERROR: failed to create data set");
            ret = NO;
        }
    }
    
    return ret;
}

- (CGRect)getCurrentARViewFrame
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGRect viewFrame = screenBounds;
    
    // If this device has a retina display, scale the view bounds
    // for the AR (OpenGL) view
    if (YES == vapp.isRetinaDisplay) {
        viewFrame.size.width *= [UIScreen mainScreen].nativeScale;
        viewFrame.size.height *= [UIScreen mainScreen].nativeScale;
    }
    return viewFrame;
}

- (void)getARResult
{
    NSLog(@"getARResult");
    // Render video background and retrieve tracking state
    Vuforia::State state = Vuforia::Renderer::getInstance().begin();
    Vuforia::Renderer::getInstance().drawVideoBackground();
    NSLog(@"state.getNumTrackables() = %d", state.getNumTrackables());
    Vuforia::Frame frame = state.getFrame();
    NSLog(@"frame.getNumImages() = %d", frame.getNumImages());
    const Vuforia::Image *oneImage = frame.getImage(0);
    NSLog(@"oneImage.getWidth() = %d, oneImage.getBufferWidth() = %d", oneImage->getWidth(), oneImage->getBufferWidth());
    NSLog(@"oneImage->getHeight() = %d, oneImage->getBufferHeight() = %d", oneImage->getHeight(), oneImage->getBufferHeight());

    for (int i = 0; i < state.getNumTrackableResults(); ++i) {
        // Get the trackable
        const Vuforia::TrackableResult* result = state.getTrackableResult(i);
        const Vuforia::Trackable& trackable = result->getTrackable();
        NSLog(@"trackable.getName() = %s", trackable.getName());
        
        SampleApplicationUtils::checkGlError("EAGLView renderFrameVuforia");
    }
    
    Vuforia::Renderer::getInstance().end();
    
}


@end
