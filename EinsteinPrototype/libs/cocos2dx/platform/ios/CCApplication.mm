/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "CCApplication.h"

#import <UIKit/UIKit.h>

#import "CCGeometry.h"
#import "CCDirectorCaller.h"

NS_CC_BEGIN

CCApplication* CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication()
{
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = 0;
}

int CCApplication::run()
{
    if (applicationDidFinishLaunching()) 
    {
        [[CCDirectorCaller sharedDirectorCaller] startMainLoop];
    }
    return 0;
}

void CCApplication::setAnimationInterval(double interval)
{
    [[CCDirectorCaller sharedDirectorCaller] setAnimationInterval: interval ];
}

#include "curl.h"

/*
@implementation HideBar
+ (void) removeBar :(bool)enable
{
    if( enable)
        [[UIApplication sharedApplication] setStatusBarHidden: YES];
    else
        [[UIApplication sharedApplication] setStatusBarHidden: NO];
}

@end*/


void CCApplication::hideBars(bool enable)
{
    if( enable)
        [[UIApplication sharedApplication] setStatusBarHidden: YES];
    else
        [[UIApplication sharedApplication] setStatusBarHidden: NO];
}
void CCApplication::openURL(const char* pszUrl, const char* param,int tag)
{
    //https://api.foursquare.com/v2/checkins/add checkin foursquare
    //https://foursquare.com/oauth2/authenticate?client_id=YOUR_CLIENT_ID getting token
    if( tag == 2)
    {
    CURL *curl;
    CURLcode res;
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
         just as well be a https:// URL if that is what should receive the
         data. */
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://foursquare.com/oauth2/authenticate?client_id=YOUR_CLIENT_ID");
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "access_token=AAACEdEose0cBAIOPRdhbBjtf8hFDtI7bQ0Wvr6tUZCeY087dqr5aZALxJ7g4WS9HYtkTO79pYNXDrT5eXXmxfAujvqZCj7EfFcoSfVn8To6ENDr4XsF&expires=1362517200");
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            CCLOG( "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    
    }
    else
    {
        char*t = "https://www.facebook.com/dialog/feed?app_id=458358780877790&link=https://developers.facebook.com/docs/reference/dialogs/&picture=http://fbrell.com/f8.jpg&name=Facebook%20Dialogs&caption=Reference%20Documentation&description=Using%20Dialogs%20to%20interact%20with%20users.&redirect_uri=https://mighty-lowlands-6381.herokuapp.com/";
    
      NSString *msg = [NSString stringWithCString:t encoding:NSASCIIStringEncoding];
      NSURL *nsUrl = [NSURL URLWithString:msg];
      [[UIApplication sharedApplication] openURL:nsUrl];
    }
}

CCApplication::Orientation CCApplication::setOrientation(Orientation eOritation)
{
    UIApplication * app = [UIApplication sharedApplication];
    UIInterfaceOrientation newOrientation;
    switch (eOritation)
    {
    case kOrientationPortrait:
        newOrientation = UIInterfaceOrientationPortrait;
        break;
    case kOrientationPortraitUpsideDown:
        newOrientation = UIInterfaceOrientationPortraitUpsideDown;
        break;
    case kOrientationLandscapeLeft:
        newOrientation = UIInterfaceOrientationLandscapeRight;
        break;
    case kOrientationLandscapeRight:
        newOrientation = UIInterfaceOrientationLandscapeLeft;
        break;
    default:
        newOrientation = UIInterfaceOrientationPortrait;
        break;
    }
    if (newOrientation != [app statusBarOrientation])
    {
        [app setStatusBarOrientation: newOrientation];
    }
    return eOritation;
}

//void CCApplication::statusBarFrame(cocos2d::CCRect * rect)
//{
//    rect->origin.x = [[UIApplication sharedApplication] statusBarFrame].origin.x;
//    rect->origin.y = [[UIApplication sharedApplication] statusBarFrame].origin.y;
//    rect->size.width = [[UIApplication sharedApplication] statusBarFrame].size.width;
//    rect->size.height = [[UIApplication sharedApplication] statusBarFrame].size.height;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////////////////////////////

CCApplication& CCApplication::sharedApplication()
{
    CC_ASSERT(sm_pSharedApplication);
    return *sm_pSharedApplication;
}

ccLanguageType CCApplication::getCurrentLanguage()
{
    // get the current language and country config
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];

    // get the current language code.(such as English is "en", Chinese is "zh" and so on)
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
    NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];

    ccLanguageType ret = kLanguageEnglish;
    if ([languageCode isEqualToString:@"zh"])
    {
        ret = kLanguageChinese;
    }
    else if ([languageCode isEqualToString:@"en"])
    {
        ret = kLanguageEnglish;
    }
    else if ([languageCode isEqualToString:@"fr"]){
        ret = kLanguageFrench;
    }
    else if ([languageCode isEqualToString:@"it"]){
        ret = kLanguageItalian;
    }
    else if ([languageCode isEqualToString:@"de"]){
        ret = kLanguageGerman;
    }
    else if ([languageCode isEqualToString:@"es"]){
        ret = kLanguageSpanish;
    }
    else if ([languageCode isEqualToString:@"ru"]){
        ret = kLanguageRussian;
    }

    return ret;
}

bool CCApplication::isIpad()
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        return true;
    }
    
    return false;
}

NS_CC_END
