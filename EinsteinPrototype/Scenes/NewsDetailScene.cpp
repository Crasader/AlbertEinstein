//
//  NewsDetailScene.cpp
//  EinsteinPrototype
//
//  Created by Camargo on 15/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "NewsDetailScene.h"
#include "HomeScene.h"

NewsDetailScene::NewsDetailScene()
{
 
}

void NewsDetailScene::btnBack(CCObject* node)
{
    baseLayer->setTouchEnabled(true);
    this->removeFromParentAndCleanup(true);
//    CCScene* newScene = CCScene::create();
//    newScene->addChild(new HomeScene(),0,Global::TAG_Parent_Layer);
//    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Global::TAG_Parent_Layer, true);
//    CCDirector::sharedDirector()->replaceScene(newScene);
}

void NewsDetailScene::btnMore(CCObject* node)
{
    CCApplication::sharedApplication().openURL(this->news->url_link.c_str());

}


void NewsDetailScene::initWithNews(News * varNews)
{
    this->news = varNews;
    //    CCSize size = CCSize(750, 400);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerColor * fundo =  CCLayerColor::create(ccc4(255, 255, 255, 255), size.width, size.height);
    fundo->setAnchorPoint(ccp(0,0));
    fundo->setPosition(ccp(0, 0));
   // fundo->setTouchEnabled(false);
    this->addChild(fundo,10);
    //    CCLayerColor * header =  CCLayerColor::create(ccc4(200, 200, 200, 255), size.width, 80);
    //    header->setAnchorPoint(ccp(0,0));
    //    header->setPosition(ccp(0, size.height -80));
    //    fundo->addChild(header);
    
    CCSprite *sprHeader;
    sprHeader = CCSprite::create("Header_Blue.png");
    
    sprHeader->setAnchorPoint(ccp(0,0));
    sprHeader->setScale(1);
    sprHeader->setPosition(ccp(0, size.height -90));
    this->addChild(sprHeader,100);
    CCSprite *sprTitle;
    sprTitle = CCSprite::create("tit_informacoes.png");
    
    sprTitle->setAnchorPoint(ccp(0,0));
    sprTitle->setScale(1);
    sprTitle->setPosition(ccp(110, size.height -50));
    this->addChild(sprTitle,101);
    
    CCMenu* menu = CCMenu::create();
    
    CCSprite *spriteDefault = CCSprite::create("Seta1.png");
    CCSprite *spriteSelected = CCSprite::create("Seta2.png");
    CCMenuItemSprite *item = CCMenuItemSprite::create(spriteDefault, spriteSelected, this,  menu_selector(NewsDetailScene::btnBack));
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(0, size.height -90));
    menu->addChild(item, 0, 100);
    menu->setPosition(ccp(10,30));
    menu->setAnchorPoint(ccp(0,0));
    this->addChild(menu,102);
    
    
    int margem = 10;
    int recuo = 15;
    
    int titleLines = 1;
    int textLines = 6;
    int titleFontSize =24;
    int textFontSize =18;

    CCSprite * img = getImageFromURL(varNews->imagem.c_str());
    
    if (img != NULL) {
        img->setAnchorPoint(ccp(0,0));
        img->setScale(0.95);
        img->setPosition(ccp(margem,size.height-100-img->getTextureRect().size.height));
        fundo->addChild(img);
        recuo = 130;
    }

    
    CCLabelTTF * labelTitle;
    
    //    if(bold)
    labelTitle = CCLabelTTF::criar(varNews->titulo.c_str(), CCSizeMake(size.width-margem, titleFontSize*titleLines), kCCTextAlignmentLeft, "LucidaGrandeBold", titleFontSize);
    //    else
    //         labelTitle = CCLabelTTF::create(datatext, "Lucida Grande", size);
    
    labelTitle->setPosition(ccp(10, size.height-100));
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(ccc3(80, 80, 80));
    fundo->addChild(labelTitle);
    
    CCLabelTTF *labelDesc;
    
    labelDesc = CCLabelTTF::criar(varNews->texto.c_str(), CCSizeMake(size.width-margem, textFontSize*textLines), kCCTextAlignmentLeft, "Lucida Grande", textFontSize);
    
    int areaTexto = size.height -img->getTextureRect().size.height - margem - 90;
    labelDesc->setPosition(ccp(margem, areaTexto - textFontSize*textLines));
    labelDesc->setAnchorPoint(ccp(0,0));
    labelDesc->setColor(ccc3(100, 100, 100));
    fundo->addChild(labelDesc);

    CCMenu* menu2 = CCMenu::create();
    
    CCSprite *spriteDefaultx = CCSprite::create("bt_saibamais.png");
    CCSprite *spriteDefaulty = CCSprite::create("bt_saibamais.png");
    CCMenuItemSprite *item2 = CCMenuItemSprite::create(spriteDefaultx, spriteDefaulty, this,  menu_selector(NewsDetailScene::btnMore));
    item2->setAnchorPoint(ccp(0, 0));
    menu2->setPosition(ccp(50, 50));
    item2->addChild(item2, 0, 100);
    item2->setPosition(ccp(10,30));
    item2->setAnchorPoint(ccp(0,0));
    fundo->addChild(menu2,1001);

    
  
}


struct BufferStruct
{
    char * buffer;
    size_t size;
};

size_t size = 0;

static size_t WriteMemoryCallback
(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    
    struct BufferStruct * mem = (struct BufferStruct *) data;
    
    mem->buffer = (char*)realloc(mem->buffer, mem->size + realsize + 1);
    
    if ( mem->buffer )
    {
        memcpy( &( mem->buffer[ mem->size ] ), ptr, realsize );
        mem->size += realsize;
        mem->buffer[ mem->size ] = 0;
    }
    return realsize;
}


size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
    ((string*)stream)->append((char*)ptr, 0, size*count);
    //    CCLog("%s", ((string*)stream)->c_str());
    return size*count;
}


CCSprite *  NewsDetailScene::getImageFromURL(const char* url)
{
    CURL *curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl)
    {
        string out = string(url);
        struct BufferStruct output;
        output.buffer = NULL;
        output.size = 0;
        curl_easy_setopt(curl, CURLOPT_URL, out.c_str());
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            return NULL;
        } else {
            //            FILE * fp;
            //            fp = fopen( "loucura.png","w");
            //            if( fp )
            //            fprintf(fp, output.buffer );
            //            fclose( fp );
            
            CCImage* img = new CCImage();
            img->initWithImageData((void*)output.buffer, (long)output.size, CCImage::kFmtPng);
            cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D(); //TODO:: leak
            texture->initWithImage(img);
            
            CCSprite* sprBackground = CCSprite::create(texture);
            return  sprBackground;

            
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

