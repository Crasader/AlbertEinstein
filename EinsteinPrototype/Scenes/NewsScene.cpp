//
//  NewsScene.cpp
//  EinsteinPrototype
//
//  Created by Camargo on 02/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "NewsScene.h"
#include "HomeScene.h"

struct SimpleStructure
{
    NewsScene * owner;
};
int listPos =0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool alreadyLoaded = false;

void* ReloadFunction(void* arg)
{
    pthread_mutex_lock(&mutex);
    SimpleStructure* args = (SimpleStructure*)arg;
    
    args->owner->loading->removeFromParentAndCleanup(true);
    args->owner->list->reload();
    pthread_mutex_unlock(&mutex);
    return NULL;
}

NewsScene::NewsScene()
{
    CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();
    
    
    char tmpbuf[128];
    
    time_t ltime;
    
    struct tm *today;
    
    
    
    time( &ltime );
    
    today = localtime( &ltime );
    
    
    
    /* Use strftime to build a customized time string. */
    
    strftime( tmpbuf, 128,"Today is %A, day %d of %B in the year %Y.\n", today );
 
    
    std::stringstream ss;
    ss << today->tm_mday<<"-" << today->tm_mon<<"-" << today->tm_year+1900;
    
    // Create a new dictionary
    CCDictionary * myDictionary = new CCDictionary();
    CCDictionary * myData = new CCDictionary();
    
    // Add the data you would like to store
    
    
    string url;
    
    url = url +"http://clientes.farofastudios.com.br/Einstein/admin/v2/json.php?data=" + ss.str();
    string postData = "key=val";
    
    requestor->addGetTask(url, this, callfuncND_selector(NewsScene::onHttpRequestCompleted));
    
    createListView();
}

void NewsScene::btnBack(CCObject* node)
{
    this->removeFromParentAndCleanup(true);
    listNews.clear();
    CCScene* newScene = CCScene::create();
    newScene->addChild(new HomeScene(),0,Global::TAG_Parent_Layer);
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Global::TAG_Parent_Layer, true);
    CCDirector::sharedDirector()->replaceScene(newScene);
}

void NewsScene::createListView()
{
//    CCSize size = CCSize(750, 400);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
  
    CCLayerColor * fundo =  CCLayerColor::create(ccc4(255, 255, 255, 255), size.width, size.height);
    fundo->setAnchorPoint(ccp(0,0));
    fundo->setPosition(ccp(0, 0));
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
    CCMenuItemSprite *item = CCMenuItemSprite::create(spriteDefault, spriteSelected, this,  menu_selector(NewsScene::btnBack));
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(0, size.height -90));
    menu->addChild(item, 0, 100);
    menu->setPosition(ccp(10,30));
    menu->setAnchorPoint(ccp(0,0));
    this->addChild(menu,102);
    
    list = CCListView::create(CCListViewModeVertical);
    list = cocos2d::extension::CCListView::create(CCListViewModeVertical);
    list->setDelegate(this);
    list->setContentSize(CCSize(size.width, size.height-70));
    list->setAnchorPoint(ccp(0,0));
    list->setPosition(ccp(0, 0));
    list->setColor(ccc3(50, 50, 50));
    list->setSeparatorStyle(CCListViewCellSeparatorStyleNone);
    
    fundo->addChild(list);
    this->loading = new AnimatedSprite("loader_einstein1.png");
	this->loading->addAnimation(AnimatedSprite::animationWithFile("loader_einstein", 24, 0.030f), "anim");//frame_count
    
    this->addChild(this->loading,999);
    this->loading->runAnimation("anim",true, true);
	this->loading->setAnchorPoint(ccp(0.5f, 0.5f));
	this->loading->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 - 30));
    this->loading->setVisible(true);
}

void NewsScene::parseResult(char *retorno)
{
    char *source = retorno;
    char *endptr;
    JsonValue value;
    JsonAllocator allocator;
    JsonParseStatus status = json_parse(source, &endptr, &value, allocator);
    
    if (status != JSON_PARSE_OK) {
        fprintf(stderr, "error at %zd, status: %d\n", endptr - source, (int)status);
//        exit(EXIT_FAILURE);
        
    } else {
        fprintf(stderr, "success, status: %d\n", (int)status);
        listPos =0;
        printReturn(value);
        alreadyLoaded = true;
        this->list->reload();
        this->loading->removeFromParentAndCleanup(true);
//        pthread_t thread;
//        SimpleStructure* args = new SimpleStructure();
//        args->owner = this;
//        pthread_create(&thread, NULL, &ReloadFunction, args);
    }
}


char *lastKey;

void NewsScene::printReturn(JsonValue o)
{
    switch (o.getTag())
    {
        case JSON_TAG_NUMBER:
            printf("%g\n", o.toNumber());
            //sum += o.toNumber();
            break;
        case JSON_TAG_BOOL:
            printf("%s\n", o.toBool() ? "true" : "false");
            break;
        case JSON_TAG_STRING:
        {
            string theValue =o.toString();
            setValue(theValue);
        }
            break;
        case JSON_TAG_ARRAY:
            for (auto i : o)
            {
               // tmpNews = new News();
                listNews.push_back(new News());
                printReturn(i->value);
            }
            
            break;
        case JSON_TAG_OBJECT:
            for (auto i : o)
            {
                printf("%s = ", i->key);
                lastKey = i->key;
                printReturn(i->value);
            }
            break;
        case JSON_TAG_NULL:
            printf("null\n");
            break;
    }
}

void NewsScene::setValue(string value)
{
    News *tmpNews;
   
    tmpNews = listNews.at(listPos);
    if(strcmp (lastKey,"cod_notificacao") == 0)
    {
      //  string tmpstring = value;
        tmpNews->cod_notificacao =value;
    }
    if(strcmp (lastKey,"codUsuario") == 0)
    {
        tmpNews->codUsuario = value;
    }
    if(strcmp (lastKey,"notiAtivo") == 0)
    {
        tmpNews->notiAtivo = value;
    }
    if(strcmp (lastKey,"usuario") == 0)
    {
        tmpNews->usuario = value;
    }
    if(strcmp (lastKey,"titulo") == 0)
    {
        tmpNews->titulo = value;
    }
    if(strcmp (lastKey,"texto") == 0)
    {
        tmpNews->texto = value;
    }
    if(strcmp (lastKey,"imagem") == 0)
    {
        tmpNews->imagem = value;
    }
    if(strcmp (lastKey,"url_link") == 0)
    {
        tmpNews->url_link = value;
        
        if (strncmp(tmpNews->url_link.c_str(), "http", strlen("http")) != 0) {
            tmpNews->url_link = "http://" + tmpNews->url_link;
        }
    }
    if(strcmp (lastKey,"nome") == 0)
    {
        tmpNews->nome = value;
    }
    if(strcmp (lastKey,"email") == 0)
    {
        tmpNews->email = value;
    }
    if(strcmp (lastKey,"notificacao_validade") == 0)
    {
        tmpNews->notificacao_validade = value;
    }
    if(strcmp (lastKey,"notificacao_cadastro") == 0)
    {
        tmpNews->notificacao_cadastro = value;
        listPos++;
      //  this->listNews.push_back(tmpNews);
    }
}

void NewsScene::CCListView_numberOfCells(CCListView *listView, CCListViewProtrolData *data)
{
    int qtd = 0;
    if (alreadyLoaded)
    {
        qtd =this->listNews.size();
    }
    data->nNumberOfRows = qtd;
}

void NewsScene::CCListView_cellForRow(CCListView *listView, CCListViewProtrolData *data)
{
    CCSize listItemSize = CCSize(list->getContentSize().width, 119);
    
    News *item = (News*) this->listNews.at(data->nRow);
    
    CCListViewCell *cell = CCListViewCell::node();
    cell->setOpacity(255);
    cell->setColor(ccc3(255, 255, 255));
    cell->setContentSize(listItemSize);
    data->cell = cell;
    
    CCSprite *sprBackground;
    sprBackground = CCSprite::create("background_informacoes.png");
    
    if(item->url_link.empty())
    {
        sprBackground = CCSprite::create("background_informacoes2.png");
    }
 
    sprBackground->setAnchorPoint(ccp(0,0));
    sprBackground->setScale(1);
    sprBackground->setPosition(ccp(0,0));
    cell->addChild(sprBackground);
    
    int margem = 25;
    int recuo = 15;
    int titleLines = 1;
    int textLines = 6;
    int titleFontSize =18;
    int textFontSize =12;
    CCSprite * img = getImageFromURL(item->imagem.c_str());
    
    if (img != NULL) {
        img->setAnchorPoint(ccp(0,0));
        img->setScale(0.35);
        img->setPosition(ccp(10,margem));
        cell->addChild(img);
        recuo = 130;
    }
    
    
    CCLabelTTF * labelTitle;
    
//    if(bold)
    labelTitle = CCLabelTTF::criar(item->titulo.c_str(), CCSizeMake(cell->getContentSize().width-margem, titleFontSize*titleLines), kCCTextAlignmentLeft, "LucidaGrandeBold", titleFontSize);
//    else
//         labelTitle = CCLabelTTF::create(datatext, "Lucida Grande", size);
    
    labelTitle->setPosition(ccp(10, cell->getContentSize().height-titleFontSize*titleLines-3));
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(ccc3(80, 80, 80));
    cell->addChild(labelTitle);
    
    CCLabelTTF *labelDesc;

    labelDesc = CCLabelTTF::criar(item->texto.c_str(), CCSizeMake(cell->getContentSize().width-recuo-margem, textFontSize*textLines), kCCTextAlignmentLeft, "Lucida Grande", textFontSize);
    
    labelDesc->setPosition(ccp(recuo, margem));
    labelDesc->setAnchorPoint(ccp(0,0));
    labelDesc->setColor(ccc3(100, 100, 100));
    cell->addChild(labelDesc);
    
//    CCSprite *img = getImageFromURL("http://clientes.farofastudios.com.br/Einstein/admin/v2/imagens/1396547632556.png");
//    labelDesc->setPosition(ccp(0, cell->getContentSize().height  - 20 ));
//    labelDesc->setAnchorPoint(ccp(0,0));
//    labelDesc->setColor(ccc3(255, 255, 255));
//    cell->addChild(labelDesc);
}

void NewsScene::CCListView_didClickCellAtRow(CCListView *listView, CCListViewProtrolData *data)
{
//    BrowserScene * tmpBrowserScene = new BrowserScene();
//    
//    this->addChild((CCLayer*)tmpBrowserScene,500);
    int pos = data->nRow;
    News * item  = (News*)this->listNews.at(pos);
    //CCApplication::sharedApplication().openURL(item->url_link.c_str());
    
    NewsDetailScene * tmpDetail = new  NewsDetailScene;
    tmpDetail->initWithNews(item);
    this->list->setTouchEnabled(false);
    tmpDetail->baseLayer = this->list;
    this->getParent()->addChild((CCLayer*)tmpDetail);
    
}

void NewsScene::CCListView_didScrollToRow(CCListView *listView, CCListViewProtrolData *data)
{
}

void NewsScene::onHttpRequestCompleted(cocos2d::CCObject *pSender, void *data)
{
    HttpResponsePacket *response = (HttpResponsePacket *)data;
    
    if (response->request->reqType == cocos2d::extension::kHttpRequestGet) {
        if (response->succeed) {
            CCLog("Get Request Completed!");
//            CCLog("Content: %s", response->responseData.c_str());
            parseResult((char*)response->responseData.c_str());
        } else {
            CCLog("Get Error: %s", response->responseData.c_str());
        }
    } else if (response->request->reqType == cocos2d::extension::kHttpRequestPost) {
        if (response->succeed) {
            CCLog("Post Request Completed!");
            CCLog("Content: %s", response->responseData.c_str());
        } else {
            CCLog("Post Error: %s", response->responseData.c_str());
        }
    } else if (response->request->reqType == cocos2d::extension::kHttpRequestDownloadFile) {
        if (response->succeed) {
            CCLog("Download Request Completed! Downloaded:");
            
           // std::vector<char> *buffer = response->responseData.c_str();
            std::vector<std::string>::iterator iter;
            for (iter = response->request->files.begin(); iter != response->request->files.end(); ++iter) {
                std::string url = *iter;
                CCLog("%s", url.c_str());
            }
        } else {
            CCLog("Download Error: %s", response->responseData.c_str());
        }
    }
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

CCSprite *  NewsScene::getImageFromURL(const char* url)
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
//            sprBackground->setAnchorPoint(ccp(0,0));
//            sprBackground->setScale(1);
//            sprBackground->setPosition(ccp(0,0));
//            
//            this->addChild(sprBackground);
//
//            
//           cout << "Variable response : " << &output.buffer;

        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

//void NewsScene::setUpMenu()
//{
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
//    
//    
//    if (size.height > 480) {
//        size.height= 280;
//    }
//    else{
//        size.height= 260;
//    }
//    
//    
//    
//    size.width = 0.90*size.width;
//    
//    initListView(size, this);
//    pList->setPosition(ccp(20,141));
//}
//
//Retorna para a tela inicial Home
//void BathRoomScene::OnBack()
//{
//    returnToHome();
//}
//
//void BathRoomScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
//{
//    setDefaultListView(listView, data, 0);
//}
//
//void BathRoomScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
//{
//    //Redireciona para a rota
//    StateMachine machine;
//    machine.actual_category = actualCategory;
//    machine.lastState = state;
//    changeScene(new TraceRouteScene(0,machine) );
//}
//
//void BathRoomScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
//{
//    
//}
