//
//  TraceRouteScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 18/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//
#include "TraceRouteScene.h"
#include "BathRoomScene.h"
#include "WayPointViewScene.h"
#include "HomeScene.h"
#include "MapaViewScene.h"
#include "AlertViewScene.h"
#include "ParkingViewScene.h"
#include "SearchScene.h"

void TraceRouteScene::OnBackPreviousPage()
{
    if(this->statePreviousPage.lastState == TARGET_SEARCH)
        changeScene( new SearchScene());
    else if((actualCategory == SERVICOS)||(actualCategory == ESPECIALIDADES_MEDICAS) || (this->statePreviousPage.lastState == TARGET_FAVORITOS))
        changeScene( new WayPointViewScene(statePreviousPage.actual_category, this->wayPoint, statePreviousPage));
    else  if(actualCategory == BANHEIROS)
        changeScene( new BathRoomScene());
    else  if((actualCategory == ESTACIOMENTOS_ENT)||(actualCategory == ESTACIOMENTOS_SAI))
        changeScene( new ParkingViewScene(this->stateEstacionamento,this->wayPoint) );
    return;
}

void TraceRouteScene::OnBack()
{
    
    //Retorna para um estado anterior
    // TARGET_WAYPOINT =>TARGET_FLOOR => TARGET_BUILDING => TELA_ANTERIOR quando ROTA_AZ
    // TARGET_FLOOR => TARGET_BUILDING=> TELA_ANTERIOR quando ROTA_POR_ANDAR
    // TARGET_WAYPOINT TELA_ANTERIOR quando ROTA_FAVORITOS

    if(state ==  TARGET_BUILDING)
    {
        OnBackPreviousPage();
        return;
    }
    else if(state ==  TARGET_FLOOR)
    {
        if(stateRota ==  ROTA_AZ)
        {
            state =  TARGET_BUILDING;
            BuildBuildingStateWithLastPoint(true);
        }
        else
        {
            OnBackPreviousPage();
            return;
        }
    }
    else if(state ==  TARGET_WAYPOINT)
    {
        state =  TARGET_FLOOR;
        if(stateRota ==  ROTA_POR_ANDAR)
            BuildFloorState(-1);
        else  if(stateRota ==  ROTA_FAVORITOS)
            OnBackPreviousPage();
        else
        {
            CCLOG(" floor point removed %d",insertedBuildingKey);
            BuildFloorState(insertedBuildingKey);
        }
    }
    UpdateHeader();
}


TraceRouteScene::TraceRouteScene(int _wayPointID, EstacionamentoState _stateEstacionamento ,StateMachine _statePreviousPage)
{
   	std::cout<<__PRETTY_FUNCTION__<<"\n";
    this->stateEstacionamento = _stateEstacionamento;
    this->statePreviousPage = _statePreviousPage;
    this->actualCategory = statePreviousPage.actual_category;
    this->wayPoint = _wayPointID;
    stateRota = ROTA_AZ;
    state = TARGET_BUILDING;
    initFixedMenu();
    Init();
    setTouchEnabled(true);
    //Registra funcionalidade voltar pra pagina anterior, ira adicionar um botao no topo na pagina e chamar o metodo OnBack()
    this->addButtonTop();
}

TraceRouteScene::TraceRouteScene(int wayPointID,StateMachine _statePreviousPage)
{
	std::cout<<__PRETTY_FUNCTION__<<"\n";
    stateRota = ROTA_AZ;
    this->statePreviousPage = _statePreviousPage;
    this->actualCategory = statePreviousPage.actual_category;
    this->wayPoint = wayPointID;
    state = TARGET_BUILDING;
    initFixedMenu();
    Init();
    setTouchEnabled(true);
    this->addButtonTop();
}

void TraceRouteScene::setUpMenu()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    size.height= 0.55*size.height - 20;
    size.width = 0.90*size.width;
    initListView(size, this);
}

//Carrega o fundo, muda o titulo e cria botoes
void TraceRouteScene::Init()
{
    setUpTopImage(Global::TOP_SRC_IMAGE_ComoChegar);
    ChangeTitle(wayPoint);
   
    

    CCSprite* spriteBackButton = CCSprite::create("Background_Estacionamento_botoes.png");
    spriteBackButton->setAnchorPoint(ccp(0, 0));
     float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
        spriteBackButton->setPosition(ccp(0, 420));
     
    }
    else
    {
       spriteBackButton->setPosition(ccp(0, 331));
       
    }
    insertLabel(ccc3(72, 72, 72), ccp(60, 60),12378999,0, true, "Selecione o ponto aonde você está:", spriteBackButton, 12);
    this->addChild(spriteBackButton, 0,99);
    
    CreateButtons(ROTA_AZ,this);
    
    
}


//Ajusta os botoes deixa o selecionado e o resto habilitado é recriado ao clicar nos itens
//Semelhante a TABVIEWS
void TraceRouteScene::CreateButtons(RotaState newState,CCNode *node)
{
    this->stateRota = newState;
    memset(currentText,' ',sizeof(currentText));
 
    if(node->getChildByTag(11)!=NULL)
        node->removeChildByTag(11, true);

    CCMenu* menuEntradaSaida = CCMenu::create();
    insertedBuildingKey = 0;
    switch (newState) {
		case ROTA_AZ:
			state = TARGET_BUILDING;
			BuildFloorState(-1);
            BuildBuildingStateWithLastPoint(true);
            createMenuItem(menuEntradaSaida, ROTA_AZ, "az_press.png", "az_press.png", 2,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            createMenuItem(menuEntradaSaida, ROTA_POR_ANDAR, "porandar_btn.png", "porandar_press.png", 85,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            createMenuItem(menuEntradaSaida, ROTA_FAVORITOS, "favoritos_btn.png", "favoritos_press.png", 177,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            break;
            
		case ROTA_POR_ANDAR:
            state = TARGET_FLOOR;
            BuildFloorState(-1);
            createMenuItem(menuEntradaSaida, ROTA_AZ, "az_btn.png", "az_press.png", 2,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            createMenuItem(menuEntradaSaida, ROTA_POR_ANDAR, "porandar_press.png", "porandar_press.png", 85,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            createMenuItem(menuEntradaSaida, ROTA_FAVORITOS, "favoritos_btn.png", "favoritos_press.png", 177,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            break;
            
		case ROTA_FAVORITOS:
            state = TARGET_WAYPOINT;
            BuildFavorites(this->wayPoint,true);
            createMenuItem(menuEntradaSaida, ROTA_AZ, "az_btn.png", "az_press.png", 2,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            createMenuItem(menuEntradaSaida, ROTA_POR_ANDAR, "porandar_btn.png", "porandar_press.png", 85,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            createMenuItem(menuEntradaSaida, ROTA_FAVORITOS, "favoritos_press.png", "favoritos_press.png", 177,0, menu_selector( TraceRouteScene::btnEntradaSaida),node);
            break;
            
		default:
            break;
    }
    clearHeader();
    float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
        menuEntradaSaida->setPosition(ccp(27,440));
        
    }
    else
    {
        menuEntradaSaida->setPosition(ccp(27,360));
        
    }

    
    node->addChild(menuEntradaSaida,0,11);
}



///Carrega predios junto com o ultimo  ponto visitado
void TraceRouteScene::LoadListView()
{
    BuildBuildingStateWithLastPoint(true);
}

/*
void TraceRouteScene::insertItemListView( int tagValue, const char* datatext)
{
    PairKey data ;
    data.key = tagValue;
    data.text = datatext;
    plistMenuCurrent.push_back(data);
}
*/
void TraceRouteScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void TraceRouteScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    setDefaultListView(listView,data,5);
}

//Remove o texto do cabecalho de navegacao da rota de deixa o padrao que e "Selecione..."
void TraceRouteScene::clearHeader()
{
    if(this->getChildByTag(298) != NULL)
        removeChildByTag(298, true);
    
    memset(currentText,' ',sizeof(currentText));
    
    if( plistMenuCurrent.size() > 0 )
        {
            if( plistMenuCurrent[0].key == -2)
                
                if( stateRota == ROTA_AZ)
                    sprintf(currentText,"%s", "Selecione o bloco ou o último ponto de chegada:" );
                else if (stateRota == ROTA_FAVORITOS)
                    sprintf(currentText,"%s", "Selecione o andar ou o último ponto de chegada:" );
                else
                    sprintf(currentText,"%s", "Selecione um ponto salvo:" );
            else
                if( stateRota == ROTA_AZ)
                    sprintf(currentText,"%s", "Selecione o bloco:" );
                else if (stateRota == ROTA_FAVORITOS)
                    sprintf(currentText,"%s", "Selecione um ponto salvo:" );

                else
                    sprintf(currentText,"%s", "Selecione o andar:" );
        }
        else
            if(stateRota != ROTA_FAVORITOS)
                sprintf(currentText,"%s", "Não há dados a serem carregados." );
            else
                sprintf(currentText,"%s", "Selecione um ponto salvo:" );

    CCLabelTTF *labelTitle;
    labelTitle = CCLabelTTF::create(currentText, "Lucida Grande", 12);
    float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
            labelTitle->setPosition(ccp(20, 425));
        
    }
    else
    {
            labelTitle->setPosition(ccp(20, 305));//todo
        
    }


    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(ccc3(126, 126, 126));
    this->addChild(labelTitle,0,298);
}

//adiciona um  texto de cabecalho com a navegacao da rota "
void TraceRouteScene::UpdateHeader()
{
    if(this->getChildByTag(298) != NULL)
        removeChildByTag(298, true);
    
    memset(currentText,' ',sizeof(currentText));
    if(state ==  TARGET_FLOOR)
    {
        switch(stateRota)
        {
            case ROTA_AZ:
                sprintf(currentText,"%s",buildingHeader );
                break;
            case ROTA_POR_ANDAR:
                sprintf(currentText,"%s",buildingHeaderFloor );
                break;
            case ROTA_FAVORITOS:
                sprintf(currentText," " );
                break;
            default:
                break;
        }
    }
    else if(state ==  TARGET_WAYPOINT)
    {
        switch(stateRota)
        {
            case ROTA_AZ:
                sprintf(currentText,"%s / %s",buildingHeader,buildingHeaderFloor );
                break;
            case ROTA_POR_ANDAR:
                sprintf(currentText,"%s",buildingHeaderFloor );
                break;
            case ROTA_FAVORITOS:
                 sprintf(currentText," " );
                break;
            default:
                break;
        }
    }
    float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
         insertLabel(ccc3(126, 126, 126), ccp(20, 425),298,0, false, currentText, this, 12);
        
    }
    else
    {
         insertLabel(ccc3(126, 126, 126), ccp(20, 305),298,0, false, currentText, this, 12);
        
    }

   
}


//Chamado quando o usuario clica em uma linha
void TraceRouteScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    //-2 Quando o usuario visita um ponto pela primeira vez um item e gravado na memoria o ListView atribui -2 para informar que e um ultimo
    // ponto salvo ( serve pra informar o LisView que ele tem icone diferente na linha, como alinhamento ).
    if( plistTagMenuCurrent[data->nRow] == -2)
    {
        changeScene(new MapViewScene(Global::ULTIMO_PONTO_CHEGADA_WAYPOINT,wayPoint));
        return ;
    }
    if(state ==  TARGET_BUILDING)
    {
        state = TARGET_FLOOR;
        insertedBuildingKey = plistTagMenuCurrent[data->nRow];
        strcpy (buildingHeader,plistMenuCurrent[data->nRow].text);
        UpdateHeader();
        
        stackCategory.push(insertedBuildingKey);
        BuildFloorState(insertedBuildingKey);
    }
    else if(state ==  TARGET_FLOOR)
    {
        insertedFloorKey = plistTagMenuCurrent[data->nRow];
        state = TARGET_WAYPOINT;

        strcpy (buildingHeaderFloor,plistMenuCurrent[data->nRow].text);
        if(currentText[0] == ' ')
            strcpy (currentText,plistMenuCurrent[data->nRow].text);
        else
        {
            if( this->stateRota == ROTA_AZ)
            {
                strcat(currentText," / ");
                strcat(currentText,plistMenuCurrent[data->nRow].text);
            }
        }
        UpdateHeader();
        BuildWayPointState(insertedFloorKey, this->wayPoint, false);
    }
    else if(state ==  TARGET_WAYPOINT)
    {
        int wayPointChoose = plistTagMenuCurrent[data->nRow];
        changeScene(new MapViewScene(wayPointChoose,this->wayPoint));
    }
}

void TraceRouteScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    
}

void TraceRouteScene::btnEntradaSaida(CCObject *sender)
{
    CCNode* node = (CCNode*)sender;
    CreateButtons((RotaState)node->getTag(),this);//todo
}

TraceRouteScene::~TraceRouteScene()
{

    plistMenuCurrent.clear();
    plistTagMenuCurrent.clear();
    pList->release();
}