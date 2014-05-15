//
//  UIDelegateBridge.h
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifdef __OBJC__

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>
#import "LayerWebView.h"
#import "BrowserLayer.h"
#include "cocos2d.h"




@interface UIWebViewBridge : NSObject<UIWebViewDelegate,UIAlertViewDelegate,UITextFieldDelegate>
{
    LayerWebView * mLayerWebView;
    UIView    *mView;
    UIWebView *mWebView;
    UIToolbar *mToolbar;
    UIBarButtonItem *mBackButton;
    
    bool isActive;
    int newKey;
    bool mustDimissKeyboard;
    
    const char* IdToCheckin;
}

- (void) SetCheckinID:(const char*)IdToCheckin;
- (bool) setUnitID : (int )id;
- (bool) isKeyBoardInDisplay;
-(void) CleanUp;
-(void) setStartLayerWebView : (LayerWebView*) iLayerWebView ;
-(void) setStartLayerWebView2 : (LayerWebView*) iLayerWebView;
-(void) setStartLayerWebViewWithURL : (BrowserLayer *)iLayerWebView andUrl:(char*) url;
-(void) backClicked:(id)sender;
- (void)removeBar;
@end
#endif

 

