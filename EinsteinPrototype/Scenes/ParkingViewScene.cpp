//
//  ParkingViewScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 19/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "ParkingViewScene.h"
#include "LayerWebView.h"
#include "AlertViewScene.h"

using namespace cocos2d;

void ParkingViewScene::OnBack()
{
    LayerWebView::forceRelease();
    changeScene(new ParkingScene(this->state));
}

ParkingViewScene::ParkingViewScene(EstacionamentoState _state, int _wayPointID)
{
    
    isChangeToRoute = false;
    plistTagMenuCurrent.clear();
    plistMenuCurrent.clear();

    setUpTopImage(Global::TOP_SRC_IMAGE_Estacionamentos);
    ChangeTitle(ESTACIOMENTOS);
    this->addButtonTop();
    //initFixedMenu();
    this->wayPointID = _wayPointID;
    this->state=_state;
    if( state == EST_SAIDA)
    {
        actualCategory = ESTACIOMENTOS;
        CCLayerColor *node = CCLayerColor::create(ccc4(0,0,0,0));
        node->setAnchorPoint(ccp(0,0));
        node->setPosition(ccp(0,0));
        this->addChild(node,0, 250);
 
        //Carrega informacoes do Estacionamento ( semelhante waypointID
        LoadParkingId();
    }
    else
    {
        //Isso e so exibido quando o usuario esta na entrada
       if( wayPointID == 51)
         //Exibe uma mensagem de alerta configurada para estacionamentos
         AlertViewScene::showMessageParking( this,menu_selector(ParkingViewScene::btnResponse),250);
       else
         //Exibe o mapa do Google Maps
         this->addChild(new LayerWebView(wayPointID));
    }
}


void ParkingViewScene::btnResponse(CCObject *sender)
{
    //Esse metodo é chamado quando exibe um alerta ( waypoint 51). Carrega o mapa do Google Maps para esse  waypoint
    AlertViewScene::isActive = false;
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer)->removeChildByTag(Global::TAG_Alert_Layer, true);
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer)->addChild(new LayerWebView(51));
}

void ParkingViewScene::draw()
{
    //Redireciona para a tela de rota
    if(isChangeToRoute == true)
    {
        StateMachine machine;
        machine.actual_category = ESTACIOMENTOS_ENT;
        if(state == EST_SAIDA )
            machine.actual_category = ESTACIOMENTOS_SAI;
        
        ParkingViewScene* node = (ParkingViewScene*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer);

        EstacionamentoState _state = node->state;
        changeScene( new TraceRouteScene(this->wayPointID, _state, machine));
    }
}


void ParkingViewScene::btnRotaCallBack(CCObject *sender)
{
    //Manda o usuario para tela de rota e salva informacoes de qual estado o usuario esta ( saida/entrada) e etc
        isChangeToRoute = false;

    ParkingViewScene* node = (ParkingViewScene*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer);
    node->isChangeToRoute = true;
}

 

void ParkingViewScene::LoadParkingId()
{
    CCNode* node = this->getChildByTag(250);
    char s[20] ;

    sprintf(s, "%d.png", this->wayPointID);
    
    CCSprite *sprtEstabelecimento = CCSprite::create(s);
    if( sprtEstabelecimento == 0)
        sprtEstabelecimento = CCSprite::create("ModeloWayPointFoto.png");
    
    sprtEstabelecimento->setAnchorPoint(ccp(0,0));
    sprtEstabelecimento->setPosition(ccp(0,217));
    node->addChild(sprtEstabelecimento);

    
    //Waypoints do estacionamentos sao referidos como retirada de veiculos, de acordo
    //com o id, aqui o texto é alterado.
    switch(wayPointID)
    {
        case 19:
            insertLabel(ccc3(72, 72, 72), ccp(20, 180), true, "Bloco A1", node, 16);
            break;
        case 51:
            insertLabel(ccc3(72, 72, 72), ccp(20, 180), true, "Bloco B", node, 16);
            break;
        case 89:
            insertLabel(ccc3(72, 72, 72), ccp(20, 180), true, "Bloco D/E", node, 16);
            break;
    }
 

    //Cria um label ( aqui vira descricao do estacionamento)
    insertLabel(ccc3(72, 72, 72), ccp(20, 160), false, "Auditorio Moise Safra", node, 12);
    
    CCSprite *btnHorarioSelected = CCSprite::create("btnShowRotaEstacionmanto.png"); // botao pressionado
    CCSprite *btnHorarioUnSelected = CCSprite::create("btnShowRotaEstacionmanto.png");//botao nao pressionado
    CCMenuItemSprite *itembtnHorario = CCMenuItemSprite::create(btnHorarioSelected, btnHorarioUnSelected, btnHorarioSelected, menu_selector(ParkingViewScene::btnRotaCallBack));
	itembtnHorario->setAnchorPoint(ccp(0, 0));
    itembtnHorario->setPosition(ccp(40, 0));
    
    CCMenu* options = CCMenu::create();
    options->addChild(itembtnHorario,0,wayPointID);
    options->setPosition(ccp(230, 171) );
    node->addChild(options);
    
    CCSprite *sprtFundoEstabelecimento = CCSprite::create("SombraMagica.png");
    sprtFundoEstabelecimento->setAnchorPoint(ccp(0,0));
    sprtFundoEstabelecimento->setPosition(ccp(72, 200));
    node->addChild(sprtFundoEstabelecimento);
}
