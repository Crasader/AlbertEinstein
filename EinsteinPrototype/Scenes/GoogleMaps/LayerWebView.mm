//
//  LayerWebView.mm
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include "LayerWebView.h"
#include "UIWebViewBridge.h"
#include "HomeScene.h"
#include "curl.h"
static UIWebViewBridge *gUIWebViewBridge=0;
const char* LayerWebView::FacebookToken  = 0;
const char* LayerWebView::FourSquareToken  = 0;

bool LayerWebView::isSplashOn  = false;


LayerWebView::LayerWebView(SocialType _type,char* _IdCheckin)
{
    type = _type;
    setTouchEnabled(false);
    if( type == FACEBOOK)
            openBrowser(98,_IdCheckin);
        else
            openBrowser(99,_IdCheckin);
}

void LayerWebView::TargetLayer(cocos2d::CCLayer _layer)
{
    layer = _layer;
}



bool LayerWebView::checkInFacebook(const char* VenueID)
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
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/me/feed");
        char postData[500];

    
        sprintf(postData,"access_token=&message=", getSocialNetworkToken(FACEBOOK),"mensagem de info");
        
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData);
        
        
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
    return true;
}
 


bool LayerWebView::checkInFourSquare(const char* VenueID)
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
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.foursquare.com/v2/checkins/add");
        char postData[500];

        sprintf(postData,"v=20130417&venueId=%s&oauth_token=%s&client_id=%s&client_secret=%s", VenueID, getSocialNetworkToken(FOURSQUARE),"NVGS345UC4PRZAON3MB5EYKAC3Q0BURGQ35123Y1Y3K1CHQQ","5VAGKHZMO3LFJLXOUZZCSDHI3PBHQKR0KMN42ELC2GOZLKHC"
                );
        
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData);

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
    
    return true;
}


void LayerWebView::setSocialNetworkToken(SocialType type, const char* token)
{
    if( type == FACEBOOK)
       LayerWebView::FacebookToken = token;
    else
        LayerWebView::FourSquareToken = token;
}

LayerWebView::LayerWebView(int waypoint)
{
    setTouchEnabled(false);
    openBrowser(waypoint);
}

#include "ErrorViewScene.h"
void LayerWebView::showErrorImage()
{
    ErrorViewScene* newSplash = new ErrorViewScene();
    newSplash->setContentSize( CCDirector::sharedDirector()->getWinSize());
    CCDirector::sharedDirector()->getRunningScene()->addChild(newSplash,10,3333);
}

void LayerWebView::showLoadingPage()
{
    if(  LayerWebView::isSplashOn  == false )
    {
        LayerWebView::isSplashOn  = true;
        SplashHTMLoading* newSplash = new SplashHTMLoading();
        newSplash->setContentSize( CCDirector::sharedDirector()->getWinSize());
        CCDirector::sharedDirector()->getRunningScene()->addChild(newSplash,10,5555);
    }
}

void LayerWebView::hideLoadingPage()
{
   LayerWebView::isSplashOn  = false;
   CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(5555,true);
       CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(3333,true);
}

const char* LayerWebView::getSocialNetworkToken(SocialType type)
{
    if( type == FACEBOOK)
        return FacebookToken;
    else
       return FourSquareToken;
}

LayerWebView::LayerWebView()
{
    init();
}


void LayerWebView::openBrowser(int tag)
{
    cocos2d::CCSpriteFrameCache* sprite_cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    sprite_cache->addSpriteFramesWithFile("loading.plist");
    
    gUIWebViewBridge = [[UIWebViewBridge alloc] init];
    [gUIWebViewBridge setUnitID: tag];
    [gUIWebViewBridge setStartLayerWebView:this];
  
}

void LayerWebView::openBrowserWithUrl(char* turl)
{
    
    
}


void LayerWebView::openBrowser(int tag, const char* IdCheckin)
{
    gUIWebViewBridge = [[UIWebViewBridge alloc] init];
    [gUIWebViewBridge setUnitID: tag];
    [gUIWebViewBridge setStartLayerWebView : this];
    [gUIWebViewBridge SetCheckinID:IdCheckin];
}


void LayerWebView::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
     data->nNumberOfRows = this->plistMenuCurrent.size();
}

void LayerWebView::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    setDefaultListView(listView, data,0);
}


#include "PListFunctions.h"
void LayerWebView::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    if(plistMenuCurrent[data->nRow].key == 0)
    {
        PListFunctions::readServicesSection();
        return;
    }
    
    
    this->removeChildByTag(Global::TAG_Child_Layer,true);
    if ( !CCLayer::init() )
        return ;
    
    openBrowser(plistMenuCurrent[data->nRow].key);
}

void LayerWebView::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    
}

void LayerWebView::setUpMenu()
{

    int y =0;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    
    if (size.height > 480) {
       size.height= CCDirector::sharedDirector()->getWinSize().height-220-y;
    }
    else{
       size.height= CCDirector::sharedDirector()->getWinSize().height-130-y;
    }

    
    IFixedMenu::initListView(size, this);
    pList->setPosition(ccp(20,y));
}

//Libera a webview e esconde keyboards
void LayerWebView::forceRelease()
{
    if( gUIWebViewBridge != nil)
        [gUIWebViewBridge CleanUp];
}

//Verifica se alguem teclado esta visivel.
bool LayerWebView::isKeyBoardVisible()
{
    return [gUIWebViewBridge isKeyBoardInDisplay];
}

LayerWebView::~LayerWebView()
{
    [gUIWebViewBridge release];
}

void LayerWebView::OnBack()
{
  
    forceRelease();
    //  changeScene(new HomeScene());
    this->removeFromParentAndCleanup(true);
}

void LayerWebView::onBackbuttonClick()
{
    forceRelease();
  //  changeScene(new HomeScene());
    this->removeFromParentAndCleanup(true);
}

bool LayerWebView::init()
{
    insertItemListView( 5 ,"Alphaville");
    insertItemListView( 3 ,"Ibirapuera");
    insertItemListView( 4 ,"Jardins");
    insertItemListView( 1 ,"Morumbi");
    insertItemListView( 7 ,"Morato");
    insertItemListView( 8 ,"Paraisópolis");
    insertItemListView( 9 ,"Paulista");
    insertItemListView( 2 ,"Perdizes-Higienópolis");
    insertItemListView( 6 ,"Vila Mariana");
	insertItemListView(10 ,"Cidade Jardim");
    setUpMenu();
}