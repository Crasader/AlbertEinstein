//
//  HomeScene.cpp
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "HomeScene.h"
#import "CCEGLView.h"
#include "CCAction.h"

 
HomeScene::HomeScene()
{
 
    InitHome();
}

bool showNotification = false;

void HomeScene::CreateMenu()
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCMenu* options = CCMenu::create();
    
    createMenuItem(options, LUGARES, "especialidades_new.png","especialidade_new_hover.png", 40, winsize.height - 186, menu_selector(HomeScene::FixedMenuCallBack), this);
    
	createMenuItem(options, SERVICOS, "servicos_new.png", "servicos_new_hover.png",winsize.width - 20 - 90, winsize.height - 186, menu_selector(HomeScene::FixedMenuCallBack), this);

    createMenuItem(options, BANHEIROS, "sanitarios_new.png", "sanitarios_new_hover.png", 20, 170, menu_selector(HomeScene::FixedMenuCallBack), this);
    
	createMenuItem(options, ESTACIOMENTOS, "estacionamento_new.png","estacionamento_new_hover.png",winsize.width/2 - 35, 90, menu_selector(HomeScene::FixedMenuCallBack), this);
    
    if (showNotification) {
        createMenuItem(options, INFORMACOES, "informacoes_new_badge.png", "informacoes_new_hover_badge.png",winsize.width - 20 - 70, 170, menu_selector(HomeScene::FixedMenuCallBack), this);
    }
    else
    {
        createMenuItem(options, INFORMACOES, "informacoes_new.png", "informacoes_new_hover.png",winsize.width - 20 - 70, 170, menu_selector(HomeScene::FixedMenuCallBack), this);
        
    }
    
    options->setAnchorPoint(ccp(0, 0));
	options->setPosition(ccp(0, 0));
    CCNode * homeConteiner = this->getChildByTag(1009);
    CCNode * optionsOld = homeConteiner->getChildByTag(1010);
    if (optionsOld != NULL) {
        optionsOld->removeFromParentAndCleanup(true);
    }
    homeConteiner->addChild(options,101,1010);
    
    CCDelayTime *delayAction = CCDelayTime::actionWithDuration(30.0f);
    // perform the selector call
    CCCallFunc *callSelectorAction = CCCallFunc::actionWithTarget(this,
                                                                  callfunc_selector(HomeScene::loadInfo));
    // run the action
    this->runAction(CCSequence::actions(delayAction,
                                        callSelectorAction,
                                        NULL));
    
    
    
}

void HomeScene::loadInfo()
{
    CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();
    
    FileFunctions * tmpFile = new FileFunctions;
    string sufix = tmpFile->loadDay();
    
    string url;
    std::stringstream ss;
    ss << sufix;
    
    url = url +"http://clientes.farofastudios.com.br/Einstein/admin/v2/json.php?data=" + ss.str();
    string postData = "key=val";
    
    requestor->addGetTask(url, this, callfuncND_selector(HomeScene::onHttpRequestCompleted));

}

void HomeScene::onHttpRequestCompleted(cocos2d::CCObject *pSender, void *data)
{
    HttpResponsePacket *response = (HttpResponsePacket *)data;
    
    if (response->request->reqType == cocos2d::extension::kHttpRequestGet) {
        if (response->succeed) {
            CCLog("Get Request Completed!");
            CCLog("Content: %s", response->responseData.c_str());
            if (response->responseData.length() >100)
            {
                showNotification = true;
                CreateMenu();
            }
            else
                showNotification = false;
           // parseResult((char*)response->responseData.c_str());
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
            
  
//            std::vector<std::string>::iterator iter;
//            for (iter = response->request->files.begin(); iter != response->request->files.end(); ++iter) {
//                std::string url = *iter;
//                CCLog("%s", url.c_str());
//            }
        } else {
            CCLog("Download Error: %s", response->responseData.c_str());
        }
    }
}

void HomeScene::InitHome()
{
    IFixedMenu::initFixedMenu(INICIO);
	CCSprite* logo ;
	   
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    char * imgName = "bg_ios7.png";
    if (winsize.height > 480) {
        imgName = "bg_ios7_4inch.png";
    }
    logo = CCSprite::create(imgName);
    logo->setAnchorPoint(ccp(0, 0));
	logo->setPosition(ccp(0, 0));
    
    
    //Cria os menus que ficam no meio da tela ( como chegar, banheiros, especialidades e etc) 
   // createMenuItem(options, COMO_CHEGAR, "comochegar_new.png", "comochegar_new_hover.png", 20, winsize.height - 186, menu_selector(HomeScene::FixedMenuCallBack), this);
    
  
//   createMenuItem(options, NOTICIAS, "icon_news.png", "icon_news.png",winsize.width - 20 - 70, winsize.height - 186, menu_selector(HomeScene::FixedMenuCallBack), this);
	
	   
    CCSprite* welcome = CCSprite::create("BemVindo.png");
	welcome->setAnchorPoint(ccp(0, 0));
	welcome->setPosition(ccp(0, welcome->boundingBox().size.height -10));

    CCLayerColor* container = CCLayerColor::create(ccc4(255,255,255,255));
    container->addChild(welcome);
	container->addChild(logo);
    this->addChild(container, 100, 1009);
    CreateMenu();
    loadInfo();
}

/*******************************************************************************************************************************/
/* IFixedMenu                                     
   Menu com nenhum item selecionado ( padrao) 
 */
/*******************************************************************************************************************************/
void IFixedMenu::initFixedMenu(){
    initFixedMenu(NONE);
}



//Ajusta os botoes deixa um selecionado desabilitado e o resto habilitado é recriado ao clicar nos itens
//´E o painel com os botões mapas, inicio, procurar na parte inferior da tela
void IFixedMenu::initFixedMenu(HomeMenuState newState, CCLayer * target)
{
    CCMenu* oFixedMenu = CCMenu::create();
    oFixedMenu->setAnchorPoint(ccp(0, 0));
	oFixedMenu->setPosition(ccp(0, 0));
    
    switch(newState)
    {
        case INICIO:
            createMenuItem(oFixedMenu, INICIO, "FooterInicio_press.png", "FooterInicio_press.png",0, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MINHAS_ROTAS, "FooterMinhasRotas_btn.png", "FooterMinhasRotas_press.png",162, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MAPAS, "FooterMapas_btn.png", "FooterMapas_press.png",81, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, PROCURAR, "FooterProcurar_btn.png", "FooterProcurar_press.png",243, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            break;
        case MINHAS_ROTAS:
            createMenuItem(oFixedMenu, INICIO, "FooterInicio_btn.png", "FooterInicio_press.png",0, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MINHAS_ROTAS, "FooterMinhasRotas_press.png", "FooterMinhasRotas_press.png",162, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MAPAS, "FooterMapas_btn.png", "FooterMapas_press.png",81, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, PROCURAR, "FooterProcurar_btn.png", "FooterProcurar_press.png",243, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            break;
        case MAPAS:
            createMenuItem(oFixedMenu, INICIO, "FooterInicio_btn.png", "FooterInicio_press.png",0, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MINHAS_ROTAS, "FooterMinhasRotas_btn.png", "FooterMinhasRotas_press.png",162, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MAPAS, "FooterMapas_press.png", "FooterMapas_press.png",81, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, PROCURAR, "FooterProcurar_btn.png", "FooterProcurar_press.png",243, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            break;
        case PROCURAR:
            createMenuItem(oFixedMenu, INICIO, "FooterInicio_btn.png", "FooterInicio_press.png",0, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MINHAS_ROTAS, "FooterMinhasRotas_btn.png", "FooterMinhasRotas_press.png",162, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MAPAS, "FooterMapas_btn.png", "FooterMapas_press.png",81, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, PROCURAR, "FooterProcurar_press.png", "FooterProcurar_press.png",243, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            break;
        case NONE:
            createMenuItem(oFixedMenu, INICIO, "FooterInicio_btn.png", "FooterInicio_press.png",0, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MAPAS, "FooterMapas_btn.png", "FooterMapas_press.png",81, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, MINHAS_ROTAS, "FooterMinhasRotas_btn.png", "FooterMinhasRotas_press.png",162, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            createMenuItem(oFixedMenu, PROCURAR, "FooterProcurar_btn.png", "FooterProcurar_press.png",243, 0, menu_selector(HomeScene::FixedMenuCallBack), this);
            break;
        default:
            throw "";
            break;
    }
    target->addChild(oFixedMenu,999,0);
}
void IFixedMenu::initFixedMenu(HomeMenuState newState)
{
    initFixedMenu(newState, this);
}

//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
void IFixedMenu::FixedMenuCallBack(CCObject *sender)
{
    if( LayerWebView::isKeyBoardVisible())
        return;
    
   CCApplication::sharedApplication().hideBars(false);
   if(AlertViewScene::isActive )
        return;
    CCLayer* newNode =  NULL;
    LayerWebView::forceRelease();
    switch (((CCNode*)sender)->getTag())
    {
        case MINHAS_ROTAS:
            newNode =  new MinhasRotasScene();
            break;
        case ESTACIOMENTOS:
            newNode = new ParkingScene();
            break;
        case PROCURAR:
            newNode = new SearchScene();
            break;
        case BANHEIROS:
            newNode = new BathRoomScene();
            break;
        case INICIO:
            CCApplication::sharedApplication().hideBars(false);
            newNode =  new HomeScene();
            break;
        case INFORMACOES://noticias
        {
            showNotification = false;
            newNode = (CCLayer*)new NewsScene();
        }
            break;
        default:
            newNode = new HowToGoScene((HomeMenuState)((CCNode*)sender)->getTag());
            break;
    }
    if( newNode != NULL)
        changeScene(newNode);
}

HomeScene::~HomeScene()
{
    
}