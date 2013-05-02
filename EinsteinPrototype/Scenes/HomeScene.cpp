//
//  HomeScene.cpp
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "HomeScene.h"
 
HomeScene::HomeScene()
{
 
    InitHome();
}

void HomeScene::InitHome()
{
    IFixedMenu::initFixedMenu(INICIO);
    CCMenu* options = CCMenu::create();
    
	CCSprite* logo = CCSprite::create("homeLogo.png");
	logo->setAnchorPoint(ccp(0, 0));
	logo->setPosition(ccp(0, 315));
    
    
    //Cria os menus que ficam no meio da tela ( como chegar, banheiros, especialidades e etc) 
    createMenuItem(options, COMO_CHEGAR, "ComoChegar.png", "ComoChegar_H.png",0, 209, menu_selector(HomeScene::FixedMenuCallBack), this);
    createMenuItem(options, BANHEIROS, "Banheiros.png", "Banheiros_H.png",0, 102, menu_selector(HomeScene::FixedMenuCallBack), this);
    createMenuItem(options, ESPECIALIDADES_MEDICAS, "EspecialidadesMedicas.png", "EspecialidadesMedicas_H.png",107, 209, menu_selector(HomeScene::FixedMenuCallBack), this);
    createMenuItem(options, ESTACIOMENTOS, "Estacionamento.png", "Estacionamento_H.png",107, 102, menu_selector(HomeScene::FixedMenuCallBack), this);
    createMenuItem(options, SERVICOS, "Servicos.png", "Servicos_H.png",215, 209, menu_selector(HomeScene::FixedMenuCallBack), this);
    createMenuItem(options, INFORMACOES, "Informacoes.png", "Informacoes_H.png",215, 102, menu_selector(HomeScene::FixedMenuCallBack), this);

    options->setAnchorPoint(ccp(0, 0));
	options->setPosition(ccp(0, 0)); 
    
    CCSprite* welcome = CCSprite::create("BemVindo.png");
	welcome->setAnchorPoint(ccp(0, 0));
	welcome->setPosition(ccp(0, welcome->boundingBox().size.height -10));

    CCLayerColor* container = CCLayerColor::create(ccc4(255,255,255,255));
    container->addChild(welcome);
    container->addChild(options);
    container->addChild(logo);
    this->addChild(container);
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
void IFixedMenu::initFixedMenu(HomeMenuState newState)
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
    this->addChild(oFixedMenu,2,0);
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