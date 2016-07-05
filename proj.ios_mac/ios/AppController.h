#import <UIKit/UIKit.h>
#import "SampleGLResourceHandler.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, assign) id<SampleGLResourceHandler> glResourceHandler;

@end

