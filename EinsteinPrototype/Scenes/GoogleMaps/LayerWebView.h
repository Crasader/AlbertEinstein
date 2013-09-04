//
//  LayerWebView.h
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __CC_UIWEBVIEW_H__
#define __CC_UIWEBVIEW_H__


#include "CCCommon.h"
#include "cocos2d.h"
 
#include "Enumerations.h"
#include "SplashHTMLoading.h"

class LayerWebView : public IFixedMenu, public cocos2d::extension::CCListViewDelegate
{
private:

	int mWebViewLoadCounter;
    cocos2d::CCSize size;
    
    SocialType type;
    
    
    static const char* FacebookToken ;
    static const char* FourSquareToken ;
    
 
public:
 
    
    
 
    
    static bool isSplashOn;
    
    static void setSocialNetworkToken(SocialType type, const char* token);
    
    
    static const char* getSocialNetworkToken(SocialType type);
    
    LayerWebView();
    LayerWebView(int waypoint);
    
    LayerWebView(SocialType _type,char* _IdCheckin);
    ~LayerWebView();
    bool init();
    void openBrowser(int tag);
    
    void openBrowser(int tag, const char* IdCheckin);

    void onBackbuttonClick();
    void setUpMenu();
    static void forceRelease();
    static bool isKeyBoardVisible();
    
    
    static bool checkInFourSquare(const char* VenueID);
    static bool checkInFacebook(const char* VenueID);
    
    
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    
    void showLoadingPage();
    void hideLoadingPage();
    void showErrorImage();

    
};
    

#endif