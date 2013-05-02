//
//  EinsteinPrototypeAppController.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/15/12.
//  Copyright Farofa Studios 2012. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

