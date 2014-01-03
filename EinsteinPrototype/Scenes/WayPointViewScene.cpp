//
//  WayPointViewScene.cpp
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/8/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "WayPointViewScene.h"
#include "MinhasRotasScenes.h"
#include "CCLabelTTF.h"
using namespace std;

WayPointViewScene::WayPointViewScene(HomeMenuState category, int wayPoint,StateMachine _stateMachinePreviousPage)
{
    //Variavel que diz se exibe ou não facebook/4square
    enableCheckin= (category ==  SERVICOS);

    this->actualCategory = category;
    this->wayPoint = wayPoint;
    
    //Carrega o menu ( botoes inferiores (inicio,mapas,favoritos, procurar)
    InitView();
    
    //Carrega informacoes de waypoint ( titulo, nome, descrição e etc)
    LoadWayPoint();
    
    //Muda o titulo para o nome do Waypoint. Essa tela limita para 24 caracters mais o terminador /0
    ChangeTitle(wayPoint);
    this->stateMachinePreviousPage=_stateMachinePreviousPage;
    this->addButtonTop();
    
    isChangeToRoute =  false;
    is4SquareLoginCalled =  false;
}






WayPointViewScene::WayPointViewScene(int wayPoint,StateMachine _stateMachinePreviousPage)
{
    //Variavel que diz se exibe ou não facebook/4square
    //enableCheckin= (category ==  SERVICOS);

    //this->actualCategory = category;
    this->wayPoint = wayPoint;
    
    //Carrega o menu ( botoes inferiores (inicio,mapas,favoritos, procurar)
    InitView();
   
    
    //Carrega informacoes de waypoint ( titulo, nome, descrição e etc)
    LoadWayPoint();
    
    //Muda o titulo para o nome do Waypoint. Essa tela limita para 24 caracters mais o terminador /0
    ChangeTitle(wayPoint);
    this->stateMachinePreviousPage=_stateMachinePreviousPage;
    this->addButtonTop();
    
    isChangeToRoute =  false;
    is4SquareLoginCalled =  false;
}

/* Recarrega um waypoint sem recriar o fundo e titulos */
void WayPointViewScene::ReloadWayPointView()
{
    
    CCLayerColor *node = CCLayerColor::create(ccc4(0,0,0,0));
    this->addChild(node,0, 250);
    
    LoadMenu();
    
    //Carrega informacoes de waypoint ( titulo, nome, descrição e etc)
    LoadWayPoint();
    
    //Muda o titulo para o nome do Waypoint. Essa tela limita para 24 caracters mais o terminador /0
    ChangeTitle(wayPoint);
    
}

void WayPointViewScene::addFavoriteButton()
{

RotasFileIO* pRotas = new RotasFileIO();
// estava verificando o ponto de partida nos favoritos e por isso nunca vinha marcado.
// this->isFavoritoChecked = pRotas->Exists(wayPoint);
this->isFavoritoChecked = pRotas->Exists(this->wayPoint);
CCMenu* menu3 = CCMenu::create();

char* chFavoritos = "favoritos_Mapa_btn.png";

if( isFavoritoChecked)
chFavoritos = "favoritos_Mapa_press.png";

createMenuItem(menu3, 112233, chFavoritos, chFavoritos,0, 0, menu_selector(WayPointViewScene::favoriteButtonPressed), this);
 float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
        menu3->setPosition(ccp(250,480));
    }
    else
    {
        menu3->setPosition(ccp(250,400));
    }

menu3->setAnchorPoint(ccp(0,0));
this->addChild(menu3,3,1002);

}
void WayPointViewScene::favoriteButtonPressed(CCObject *sender)
{
    RotasFileIO* pRotas = new RotasFileIO();
    isFavoritoChecked =!isFavoritoChecked;
    if( isFavoritoChecked)
    {
        RotaRow row;
        row.wayPoint = this->wayPoint;
        row.favorite = 0;
        sprintf(row.description, " %s",LoadWayPointName(this->wayPoint));
        pRotas->SaveRota(row);
    }
    else
        pRotas->DeleteRota(this->wayPoint);
    reloadFavoriteButton();
}

void WayPointViewScene::reloadFavoriteButton()
{
    char* chFavoritos = "favoritos_Mapa_btn.png";
    
    if( isFavoritoChecked)
        chFavoritos = "favoritos_Mapa_press.png";
    
    CCMenu* menu3 = CCMenu::create();
    createMenuItem(menu3, 112233, chFavoritos, chFavoritos,0, 0, menu_selector(WayPointViewScene::favoriteButtonPressed), this);
    float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
        menu3->setPosition(ccp(250,480));
    }
    else
    {
        menu3->setPosition(ccp(250,400));
    }
    menu3->setAnchorPoint(ccp(0,0));
    
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer)->removeChildByTag(1002, true);
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer)->addChild(menu3,3,1002);
}

/* Cria os botoes de Rota e Horario*/
void WayPointViewScene::LoadMenu()
{
    addFavoriteButton();
    
    CCNode* node = this->getChildByTag(250);
    CCSprite *btnRotaSelected = CCSprite::create("btnShowRota_H.png"); // Botao pressionado
    CCSprite *btnRotaUnSelected = CCSprite::create("btnShowRota.png"); // Botao sem ser pressionado
    CCMenuItemSprite *itembtnRota = CCMenuItemSprite::create(btnRotaSelected, btnRotaUnSelected, btnRotaSelected, menu_selector(WayPointViewScene::btnRotaCallBack));
    itembtnRota->setPosition(ccp(0, 0));
    itembtnRota->setAnchorPoint(ccp(0, 0));
    
    CCSprite *btnHorarioSelected = CCSprite::create("btnShowHorario_H.png");  // Botao pressionado
    CCSprite *btnHorarioUnSelected = CCSprite::create("btnShowHorario.png");// Botao sem ser pressionado

    CCMenuItemSprite *itembtnHorario = CCMenuItemSprite::create(btnHorarioSelected, btnHorarioUnSelected, btnHorarioSelected, menu_selector(WayPointViewScene::btnHorarioCallBack));
	itembtnHorario->setAnchorPoint(ccp(0, 0));
    itembtnHorario->setPosition(ccp(40, 0));

    CCMenu* options = CCMenu::create();
    options->addChild(itembtnRota,1,wayPoint);
    options->addChild(itembtnHorario,1,wayPoint);
    float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
        options->setPosition(ccp(238, 253) );
    }
    else
    {
       options->setPosition(ccp(238, 173) );
    }

    
    node->addChild(options);
}

//Retorna para HowToGoScene ( Especialidades / Servicos ) ou para a tela de pesquisa
void WayPointViewScene::OnBack()
{
    //Fecha a janela de facebook ou 4square se estiver aberta
    if( is4SquareLoginCalled)
    {
        is4SquareLoginCalled = false;
        LayerWebView* l = (LayerWebView*)this->getChildByTag(99999999);
        l->forceRelease();
        this->removeChildByTag(99999999, true);
        
        ReloadWayPointView();
        
        return;
    }
    
    if( stateMachinePreviousPage.lastState == TARGET_FAVORITOS)
        changeScene( new MinhasRotasScene() );
    else
    {
        //Serve pra informar que o usuario esta voltando da pagina de waypoint
        this->stateMachinePreviousPage.lastState = TARGET_CATEGORY_WAYPOINT_PREVIOUS_PAGE;
        changeScene( new HowToGoScene( this->stateMachinePreviousPage) );
    }
}

void WayPointViewScene::draw()
{
    //Redireciona para a tela de rota
    if(isChangeToRoute == true)
        changeScene(new TraceRouteScene(this->wayPoint, this->stateMachinePreviousPage));
}

void WayPointViewScene::LoadWayPoint()
{
    CCNode* node = this->getChildByTag(250);
    //Monta o nome do arquivo com a imagem do waypoint formato {waypointID}.png na pasta IMGs
    char s[20] ;
    sprintf(s, "%d.png", wayPoint);

    //Carrega uma sobra enconstada no canto inferior da pagina
    CCSprite *sprtFundoEstabelecimento = CCSprite::create("SombraMagica.png");
    sprtFundoEstabelecimento->setAnchorPoint(ccp(0,0));
    float altura = CCDirector::sharedDirector()->getWinSize().height;
    if (altura>480) {
         sprtFundoEstabelecimento->setPosition(ccp(72, 280));
    }
    else
    {
        sprtFundoEstabelecimento->setPosition(ccp(72, 200));
    }

   
    node->addChild(sprtFundoEstabelecimento);

    CCSprite *sprtEstabelecimento = CCSprite::create(s);
    if( sprtEstabelecimento == 0)
        sprtEstabelecimento = CCSprite::create("ModeloWayPointFoto.png");

    sprtEstabelecimento->setAnchorPoint(ccp(0,0));
    if (altura>480) {
         sprtEstabelecimento->setPosition(ccp(0,297));
    }
    else
    {
        sprtEstabelecimento->setPosition(ccp(0,217));
    }

   
    node->addChild(sprtEstabelecimento);

    //Monta o nome do waypoint baseado na formatacao definida o arquivo de waypoint pList.
  // vector<std::string> vectNames = LoadWayPointNameToArray(this->wayPoint);
//    for(int i =0; i < vectNames.size();i++)
//        insertLabel(ccc3(72, 72, 72), ccp(30, 180- i*24), true, vectNames[i].c_str(), node, 16);//todo
//  
//    vector<std::string> vectDescription;//
//    vectDescription.push_back("Lorem ipsum dolor sit amet, consectetur adipis-");
//    vectDescription.push_back("cing elit. Aliquam in turpis eget velit interdum, ");
//    
//    vectDescription.push_back("vulputate  ac in diam In lacinia , magnadictum, ");
//    vectDescription.push_back("euismod gravida justo.");
////    for(int i =0; i < vectDescription.size();i++)
//    {            insertLabel(ccc3(72, 72, 72), ccp(30, 176*15 - 24 - i*20), false, vectDescription[i].c_str(), node, 12);//todo
//
//    }
    
    
   // CCSize tamanho_texto = CCSizeMake(0, 0);
   // CCLabelTTF::create("", tamanho_texto, 0, "Thonburi", 20);
    if (altura>480) {
       insertText(ccc3(72, 72, 72), ccp(30, 60),CCSizeMake(270, 180), true, "O texto apresentado nesta área representa as principais caracteristicas do local visitado. Este texto não pode ser muito grande visto que não existe rolagem nesta região e em telas de 3.5 polegadas o texto seria cortado.", node,"Thonburi", 16);
    }
    else
    {
        insertText(ccc3(72, 72, 72), ccp(30, 60),CCSizeMake(270, 100), true, "O texto apresentado nesta área representa as principais caracteristicas do local visitado. Este texto não pode ser muito grande visto que não existe rolagem nesta região e em telas de 3.5 polegadas o texto seria cortado.", node,"Thonburi", 16);
    }
    
//   CCLabelTTF *labelTitle;
//    labelTitle = CCLabelTTF::create(LoadWayPointName(this->wayPoint), "Lucida Grande", 16);
//    labelTitle->setPosition(ccp(30, 120));
//    labelTitle->setAnchorPoint(ccp(0,0));
//    labelTitle->setColor(ccc3(0, 0, 0));
//   node->addChild(labelTitle,5,651);
//    std::string title =LoadWayPointName(this->wayPoint);
//    insertLabel(ccc3(72, 72, 72), ccp(30, 180), true,title.c_str() , node, 16);


    
  if( enableCheckin  )
   {
//        /**************************************************************************************************/
       CCPoint point;
       if (altura>480) {
            point = ccp(30, 260);
       }
       else
       {
            point = ccp(30, 180);
       }
       
//        insertLabel(ccc3(72, 72, 72), ccp(30, 165-vectNames.size()*15), false, "Fazer Check in :", node, 10);
//        /**************************************************************************************************/
//        //Monta os botoes de facebook e 4square
        CCMenu* options = CCMenu::create();
        createMenuItem(options, 1, "FacebookButton.png", "FacebookButton_H.png", 40, 0, menu_selector(WayPointViewScene::btnCheckin), this);
        createMenuItem(options, 3, "FourSquareButton.png", "FourSquareButton_H.png", 0, 0, menu_selector(WayPointViewScene::btnCheckin), this);
        /**************************************************************************************************/
       // point = CCPointMake(30,point.y-45);
        options->setPosition(point);//todo
       node->addChild(options);
//        /**************************************************************************************************/
//        for(int i =0; i < vectDescription.size();i++)
//            insertLabel(ccc3(72, 72, 72), ccp(30, point.y - 24 - i*20), false, vectDescription[i].c_str(), node, 12);
//        /**************************************************************************************************/
    }
//    else
//    {
//        vectDescription.push_back("vulputate  ac in diam In lacinia , magnadictum, ");
//        vectDescription.push_back("euismod gravida justo.");
//        for(int i =0; i < vectDescription.size();i++)
//            insertLabel(ccc3(72, 72, 72), ccp(30, 176-vectNames.size()*15 - 24 - i*20), false, vectDescription[i].c_str(), node, 12);//todo
//    }
}

//Exibe a tela do facebook ou FourSquare
void WayPointViewScene::btnCheckin(CCObject *sender)
{
    if(  (AlertViewScene::isActive) ||  (LayerWebView::isSplashOn) )
        return;

    //CCNode* node = this->getChildByTag(250);
    removeChildByTag(250, true);
    is4SquareLoginCalled =  true;
    
    if(((CCNode*)sender)->getTag()  == 1)
         this->addChild(new LayerWebView(FACEBOOK,"4d584952304f88bf555a238a"),0, 99999999);
    else
        this->addChild(new LayerWebView(FOURSQUARE,"4d584952304f88bf555a238a"),0, 99999999);
}

//Ajusta a view fundo, topo e menus padroes
void WayPointViewScene::InitView()
{
    switch (actualCategory) {
        case LUGARES:
        setUpTopImage(Global::TOP_SRC_IMAGE_Especialidade);
            break;
        case BANHEIROS:
         setUpTopImage(Global::TOP_SRC_IMAGE_Banheiro); 
            break;
        case ESTACIOMENTOS:
       setUpTopImage(Global::TOP_SRC_IMAGE_Estacionamentos);
            break;
        case SERVICOS:
         setUpTopImage(Global::TOP_SRC_IMAGE_Servicos);
            break;
        default:
            return;
    }
    
    initFixedMenu();
    CCLayerColor *node = CCLayerColor::create(ccc4(0,0,0,0));
    this->addChild(node,0, 250);
 
    LoadMenu();
}

void WayPointViewScene::btnResponseRota(CCObject *sender)
{
    //Chamada para redirectionar usario para tela de rota
    //Um bug nao permite trocar a cena diretamente de um ccmenu, por isso fai criado a varia isChangeToRoute pra controlar isso
    WayPointViewScene* node = (WayPointViewScene*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer);
    node->isChangeToRoute = true;
     AlertViewScene::isActive = false;
}

void WayPointViewScene::btnHorarioCallBack(CCObject *sender)
{
    //Exibe uma alerta sobre horarios de funcionamento
    AlertViewScene::showMessageRota((CCNode*)sender,  menu_selector(WayPointViewScene::btnResponseRota), ((CCNode*)sender)->getTag());
}

void WayPointViewScene::btnRotaCallBack(CCObject *sender){
   
    //Chamada para redirectionar usario para tela de rota
    //Um bug nao permite trocar a cena diretamente de um ccmenu, por isso fai criado a varia isChangeToRoute pra controlar isso
    if(  AlertViewScene::isActive == true )
        return;
    
    WayPointViewScene* node = (WayPointViewScene*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer);
    node->isChangeToRoute = true;
}

WayPointViewScene::~WayPointViewScene()
{

}