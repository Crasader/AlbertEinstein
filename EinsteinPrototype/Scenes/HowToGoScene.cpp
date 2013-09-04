// Essa classe é usada pela maioria das telas, serve pra exibir paginas HTML (Como Chegar)
// Servicos e as Especialidades medicas compartilham o mesmo codigo mas com codigos de pesquisa diferentes ver BuildCategoryWayPointState metodo
// 222 - Servicos
// 111 - Especialidades medicas
//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.

#include "HowToGoScene.h"
#include "MapaViewScene.h"
#include "HomeScene.h"
#include "LayerWebView.h"
#include "MinhasRotasScenes.h"
#include "GLES.h"

HowToGoScene::HowToGoScene(HomeMenuState category)
{
    machine.isCategoryChild = false;
    initListView(category);
}

HowToGoScene::HowToGoScene(StateMachine _machine)
{
    machine = _machine;
    initListView(machine.actual_category);
    this->state =  machine.lastState;
    this->stackCategory = machine.stackCategory;
    this->OnBack();
}

//Monta os waypoints de especialidades ou servicos
void HowToGoScene::BuildCategoryState()
{
    if( actualCategory == ESPECIALIDADES_MEDICAS)
    {
        lastIdInserted = 111;
        ChangeTitle(ESPECIALIDADES_MEDICAS);
        BuildCategoryWayPointState(111);
    }
    else if( actualCategory == SERVICOS)
    {
        lastIdInserted = 222;
        ChangeTitle(SERVICOS);
        BuildCategoryWayPointState(222);
    }
}

//Retorna para estado anterior ou pagina anterior se nao houver maias estados)
//Os itens navegados sao salvos em uma pilha
void HowToGoScene::OnBack()
{
    LayerWebView::forceRelease();
    if( state == TARGET_CATEGORY_WAYPOINT_PREVIOUS_PAGE)
         state = TARGET_CATEGORY;
    
    if( state == TARGET_CATEGORY_WAYPOINT)
        state = TARGET_CATEGORY;
    
    if(stackCategory.empty() == true)
        returnToHome();
    else
    {
        lastIdInserted = stackCategory.top();
		if(( lastIdInserted ==  222) || ( lastIdInserted ==  111))
            machine.isCategoryChild = false;
        
        if( lastIdInserted ==  222)
            ChangeTitle(SERVICOS);
        else if( lastIdInserted ==  111)
            ChangeTitle(ESPECIALIDADES_MEDICAS);
        else
            ChangeTitle(machine.title);
        BuildCategoryWayPointState(lastIdInserted);
        stackCategory.pop();
    }
}
//Inicializa o ListView ( Itens da lista) e configura pra exibir um titulo quando o item selecionado possuir
//Itens filhos
void HowToGoScene::setUpMenu()
{
   size.width = 0.90*CCDirector::sharedDirector()->getWinSize().width;
    this->removeChildByTag(555, true);
    this->removeChildByTag(556, true);
    if(  machine.isCategoryChild)
    {
        //Monta um titulo acima do listview com o nome do Pai selecionado
        size.height= 299;
        CCSprite* spriteBackButton = CCSprite::create("Background_Estacionamento_botoes.png");
        spriteBackButton->setAnchorPoint(ccp(0, 0));
        spriteBackButton->setOpacity(2000);
        spriteBackButton->setPosition(ccp(0, 350));
        
        CCLabelTTF *labelTitleDescription;
        labelTitleDescription = CCLabelTTF::create(LoadWayPointName(lastIdInserted), "Lucida Grande", 16);
        labelTitleDescription->setHorizontalAlignment(kCCTextAlignmentLeft);
        labelTitleDescription->setPosition(ccp(80, 370));
        labelTitleDescription->setAnchorPoint(ccp(0,0));
        labelTitleDescription->setColor(ccc3(72, 72, 72));
        
        this->addChild(spriteBackButton, 0, 555);
        this->addChild(labelTitleDescription,0, 556);
    }
    else
    {
        size.height= 364;
    }
    IFixedMenu::initListView(size, this);
    pList->BackgroundID = 0;
}

void HowToGoScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    //Numero de registro que o listview possui.
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void HowToGoScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    
    //Configura exibicao para padrao
    setDefaultListView(listView,data,0);
}

void HowToGoScene::btnResponse(CCObject *sender)
{
    
}

//Usuario clica na linha
void HowToGoScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    if(state ==  TARGET_CATEGORY)
    {
        if(plistMenuCurrent[data->nRow].hasChild == false)//Se nao possui filho é por que é ponto final ( waypoint)
            state = TARGET_CATEGORY_WAYPOINT;
        else
        {
            //Carrega os pontos que sao filhos desse item
            machine.isCategoryChild = true;
            stackCategory.push(lastIdInserted);
            lastIdInserted = plistMenuCurrent[data->nRow].key;
            sprintf( machine.title, "%s",plistMenuCurrent[data->nRow].text);
            ChangeTitle(machine.title);
            BuildCategoryWayPointState(lastIdInserted);
            machine.lastState = TARGET_NONE;
        }
   }
   if(state ==  TARGET_CATEGORY_WAYPOINT)
    {
        stackCategory.push(lastIdInserted);
        StateMachine stateMachinePreviousPage;
        stateMachinePreviousPage.stackCategory = this->stackCategory;
        stateMachinePreviousPage.lastState = TARGET_CATEGORY_WAYPOINT;
        stateMachinePreviousPage.actual_category = actualCategory;
        
        stateMachinePreviousPage.isCategoryChild = machine.isCategoryChild;
        

        sprintf(stateMachinePreviousPage.title, "%s",machine.title);
        targetWayPoint = plistMenuCurrent[data->nRow].key;
        changeScene(new WayPointViewScene(actualCategory,targetWayPoint,stateMachinePreviousPage));
    }
    else if(state ==  TARGET_BUILDING)
    {
        stackCategory.push(lastIdInserted);
        state = TARGET_FLOOR;
        BuildFloorState(plistTagMenuCurrent[data->nRow]);
    }
}

void HowToGoScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    
}

void HowToGoScene::setUpImageTop()
{
    this->size.height= 0.75*CCDirector::sharedDirector()->getWinSize().height; 
    this->size.width = 0.75*CCDirector::sharedDirector()->getWinSize().width;
}

//Inicializa a View, podendo carregar uma pagina html caso seja como_chegar ou carregar uma MAPA 3D.
//Tambem inicializa servicos ou especialidades
void HowToGoScene::initListView(HomeMenuState category)
{
    lastIdInserted = 0;
	
    actualCategory = category;
 
    switch (category)
    {
        case COMO_CHEGAR:
            size= setUpTopImage(Global::TOP_SRC_IMAGE_ComoChegar);
            setUpImageTop();
            ChangeTitle(COMO_CHEGAR);
            this->addButtonTop(COMO_CHEGAR);
            this->addChild(new LayerWebView());
            break;

        case ESPECIALIDADES_MEDICAS:
            state = TARGET_CATEGORY;
            this->size=setUpTopImage(Global::TOP_SRC_IMAGE_Especialidade);
            setUpImageTop();
            BuildCategoryState();
            addButtonTop();
            break;

        case MAPAS:
            this->size=setUpTopImage(Global::TOP_SRC_IMAGE_Mapas);
            setUpImageTop();
            this->addChild(Building3DLayer::create(),0,Global::TAG_Child_Layer);
            initFixedMenu(MAPAS);
            addButtonTop(MAPAS);
            return;
            break;

        case INFORMACOES:
            size= setUpTopImage(Global::TOP_SRC_IMAGE_ComoChegar);
            setUpImageTop();
            ChangeTitle(INFORMACOES);
            this->addButtonTop(INFORMACOES);
            this->addChild(new LayerWebView(100));
            break;
            
        case SERVICOS:
            state =  TARGET_CATEGORY;
            this->size=setUpTopImage(Global::TOP_SRC_IMAGE_Servicos);
            setUpImageTop();
            BuildCategoryState();
            addButtonTop();
            break;

        default:
            return;
    }
    this->initFixedMenu();
}

HowToGoScene::~HowToGoScene()
{

}
 