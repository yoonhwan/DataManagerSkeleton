//
//  DataManagerSkeletonAppController.h
//  DataManagerSkeleton
//
//  Created by yoonhwan on 13. 6. 3..
//  Copyright __MyCompanyName__ 2013년. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

