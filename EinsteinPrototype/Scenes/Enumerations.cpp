//
//  Enumerations.cpp
//  EinsteinPrototype
//  Essa classe contem inicializacoes basicas comuns a todas as classes. A maioria das telas herdam desta classe.
//  Created by Farofa Studios on 9/26/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.



#include "Enumerations.h"
#include "RotasFileIO.h"
#include <vector>
#include <algorithm>
#include <string>

#include "PListFunctions.h"
#include "GLES.h"

using namespace std;
using namespace cocos2d;

const char* Global::TOP_SRC_IMAGE_ComoChegar = "Header_Blue.png";
const char* Global::TOP_SRC_IMAGE_Mapas = "HeaderMapas.png";
const char* Global::TOP_SRC_IMAGE_Rotas   ="HeaderMinhasrotas.png";
const char* Global::TOP_SRC_IMAGE_Procurar  ="HeaderProcurar.png";
const char* Global::TOP_SRC_IMAGE_Especialidade  ="Header_Blue.png";
const char* Global::TOP_SRC_IMAGE_Banheiro  ="Header_Blue.png";
const char* Global::TOP_SRC_IMAGE_Estacionamentos  ="Header_Blue.png";
const char* Global::TOP_SRC_IMAGE_Servicos  ="Header_Blue.png";
const char* Global::TOP_SRC_IMAGE_Informacoes  ="Header_Blue.png";

int Global::ULTIMO_PONTO_CHEGADA_WAYPOINT = 0;
int Global:: TAG_Parent_Layer = 300;
int Global::TAG_Child_Layer = 200;
int Global::TAG_Child_WayPoint =400;
int Global::TAG_Alert_Layer = 220;

/* NATURAL SORTING (http://jan.varho.org/?p=263) */

static int digitcnt(const char **p, int decimal)
{
	const char *s;
	while (!decimal && **p == '0')
		(*p)++;
	s = *p;
	while (isdigit(*s))
		s++;
	return s - *p;
}

static int digitcmp(const char **p1, const char **p2, int decimal)
{
	int c, c1 = digitcnt(p1, decimal), c2 = digitcnt(p2, decimal);
	if (decimal || c1 == c2)
		if ((c = strncmp(*p1, *p2, c1 < c2? c1: c2)))
			return c;
	if (c1 != c2)
		return c1 - c2;
	*p1 += c1, *p2 += c2;
	return 0;
}

int strnatcmp(const char *s1, const char *s2)
{
	int c, decimal = 0;
	do {
		while (isdigit(*s1) && isdigit(*s2))
			if ((c = digitcmp(&s1, &s2, decimal)))
				return c;
		if (*s1 != *s2)
			return (unsigned char)*s1 - (unsigned char)*s2;
		decimal = *s1++ == '.';
	} while (*s2++);
	return 0;
}

struct compareByNum
{
	bool operator()(const std::string& a, const std::string& b)
	{
		return strnatcmp(a.c_str(), b.c_str()) < 0;
	}
};
 

//adiciona um item a um menu
void IFixedMenu::createMenuItem(CCMenu* menuParent, int tag,char* spriteFileName,char* spriteFileNameSelected, float posX, float posY,SEL_MenuHandler menuCallback, CCNode* parent)
{
    CCSprite *spriteDefault = CCSprite::create(spriteFileName);
    CCSprite *spriteSelected = CCSprite::create(spriteFileNameSelected);

    CCMenuItemSprite *item = CCMenuItemSprite::create(spriteDefault, spriteSelected, parent, menuCallback);
    item->setAnchorPoint(ccp(0, 0));
    item->setPosition(ccp(posX, posY));
    menuParent->addChild(item, 0, tag);
}


//Inicializa/re-cria o listview
void IFixedMenu::initListView(cocos2d::CCSize size, cocos2d::extension::CCListViewDelegate* parent)
{
	std::cout<<__PRETTY_FUNCTION__<<"\n";
     pList = cocos2d::extension::CCListView::create(cocos2d::extension::CCListViewModeVertical);
     pList->setOpacity(0);
     pList->setContentSize(size);
     pList->setDelegate(parent);
     pList->setSeparatorStyle(cocos2d::extension::CCListViewCellSeparatorStyleSingleLine);
     
     if(this->getChildByTag(Global::TAG_Child_Layer)!=NULL)
     this->removeChildByTag(Global::TAG_Child_Layer, true);
     
     pList->setAnchorPoint(ccp(0,0));
     pList->setPosition(ccp(20,50));
     this->addChild(pList,0,Global::TAG_Child_Layer);
}

//Esconde a barra de statu de bateria
IFixedMenu::IFixedMenu()
{
    
    plistTagMenuCurrent.clear();
    plistMenuCurrent.clear();

    CCApplication::sharedApplication().hideBars(false);
}

//Faz a transicao da cena atual para uma nova
void IFixedMenu::changeScene(CCLayer *Node)
{
    cleanUp();
    CCScene* newScene = CCScene::create();
    newScene->addChild(Node,0,Global::TAG_Parent_Layer);
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Global::TAG_Parent_Layer, true);
    CCDirector::sharedDirector()->replaceScene(newScene);
}

//Esse metodo monta uma configuracao padrao para cada linha do listview.
void IFixedMenu::setDefaultListView(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data, float space)
{
    PairKey currentData = plistMenuCurrent[data->nRow];
    CCSize listItemSize = CCSize(100, 60);
    
    cocos2d::extension::CCListViewCell *cell = cocos2d::extension::CCListViewCell::node();
    cell->setOpacity(0);
    cell->setContentSize(CCSizeMake(594, 60));
    cell->setSelectionColor(ccc4(82,204, 204, 255));
    
    CCLabelTTF *labelCount;
    labelCount = CCLabelTTF::create(currentData.text, "Lucida Grande", 15);
    labelCount->setPosition(ccp(10+space,(cell->getContentSize().height * 0)+ cell->getContentSize().height / 2));
    labelCount->setAnchorPoint(ccp(0,0.5));
    labelCount->setColor(ccc3(72, 72, 72));
    
    CCSprite* sprLine=    CCSprite::create("Line.png");
    sprLine->setPosition(ccp(0, 60));
    sprLine->setScaleX(2);
    cell->addChild(sprLine);
    
    if( currentData.key == -2)
    {
        //Icone de ultimo ponto visitado
        CCSprite* i=    CCSprite::create("local_preto.png");
        i->setAnchorPoint(ccp(0,0));
        i->setPosition(ccp(10,20));
        cell->addChild(i, 1,101022);
        labelCount->setPosition(ccp(30,(cell->getContentSize().height * 0)+ cell->getContentSize().height / 2));
    }

    if(( currentData.key != 330) && ( currentData.key != 331))
    {
        if( plistMenuCurrent[data->nRow].hasChild)
        {
            //Seta indicando que há filhos para esse item
            CCSprite* i=    CCSprite::create(">_preta.png");
            i->setAnchorPoint(ccp(0,0));
            i->setPosition(ccp(270,20));
            cell->addChild(i, 0,404020);
        }
    }
    //Sprite da linha separador dos itens
    if( data->nRow == this->plistMenuCurrent.size()-1)
    {
        CCSprite* sprLine2=    CCSprite::create("Line.png");
        sprLine2->setPosition(ccp(0, 0));
        sprLine2->setScaleX(2);
        cell->addChild(sprLine2);
    }

    CCSize lblSize = labelCount->getContentSize();
    data->cell = cell;
    data->cell->addChild(labelCount,2,379);

    //Se for passado waypoints de banheiros, esse trecho ira adicionar imagens de banheiro ( F/M)
    CCSprite *sprtInfoSaida = NULL;
    switch (currentData.key) {
        case 330: //Se waypointid e feminino
            sprtInfoSaida = CCSprite::create("femea_preto.png");
            data->cell->addChild(sprtInfoSaida,1,380);
            labelCount->setPosition(ccp(40, (cell->getContentSize().height * 0)
                                        + cell->getContentSize().height / 2));
            break;
        case 331://Se waypointid e masculo
            sprtInfoSaida = CCSprite::create("macho_preto.png");
            data->cell->addChild(sprtInfoSaida,1,381);
            labelCount->setPosition(ccp(40, (cell->getContentSize().height * 0)
                                        + cell->getContentSize().height / 2));
            break;
        default:
            break;
    }
    
    if(sprtInfoSaida != NULL)
    {
        sprtInfoSaida->setAnchorPoint(ccp(0,0));
        sprtInfoSaida->setPosition(ccp(10, (cell->getContentSize().height * 0) + cell->getContentSize().height / 2 - 10));
        sprtInfoSaida->setOpacity(200);
    }

    this->plistTagMenuCurrent.push_back(currentData.key);
}

//CallBack Padrao do botao Top Voltar
//Para alterar fluxo dele de voltar a classe deve implementar OnBack
void IFixedMenu::btnTop(CCObject* node)
{
    if(!AlertViewScene::isActive )
        OnBack();
}

//Virtual metodo a ser executado ao clicar no botao no topo  voltar
void IFixedMenu::OnBack()
{
    //Implementar
}

IFixedMenu::~IFixedMenu()
{
    cleanUp();

}


//Cria uma imagem no topo
CCSize IFixedMenu::setUpTopImage(const char *spritename)
{
    CCLayerColor* c = CCLayerColor::create(ccc4(255,255,255,255));
    CCSprite* TitleImg = CCSprite::create(spritename);
    TitleImg->setAnchorPoint(ccp(0, 0));
    TitleImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height - TitleImg->boundingBox().size.height-20));

    this->addChild(c,0,3);
    this->addChild(TitleImg,1,2);
    CCSize size =  CCDirector::sharedDirector()->getWinSize();
    size.height =size.height - TitleImg->boundingBox().size.height;
 
    return TitleImg->boundingBox().size;
}


//Adiciona um botao no topo do lado esquerdo
void IFixedMenu::addButtonTop()
{
    CCMenu* menu = CCMenu::create();
    createMenuItem(menu, 123, "Seta1.png", "Seta2.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
    menu->setPosition(ccp(10,30));
    menu->setAnchorPoint(ccp(0,0));
    this->getChildByTag(2)->addChild(menu,2,1000);
}


//Adiciona um botao no topo do lado esquerdo de acordo com a tela que foi selecionada pelo usuario
void IFixedMenu::addButtonTop(HomeMenuState homeMenuState)
{
    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(10,30));
    menu->setAnchorPoint(ccp(0,0));

    switch (homeMenuState) {
        case MINHAS_ROTAS:
            createMenuItem(menu, 123, "voltarAmarelo_btn.png", "voltarAmarelo_press.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
            break;
        case PROCURAR:
            createMenuItem(menu, 123, "voltarVermelho_btn.png", "voltarVermelho_press.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
            break;
        case MAPAS:
            createMenuItem(menu, 123, "voltarVerde_btn.png", "voltarVerde_press.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
            break;
        default:
            menu->setPosition(ccp(10,30));
            createMenuItem(menu, 123, "Seta1.png", "Seta2.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
            break;
    }
    this->getChildByTag(2)->addChild(menu,1,1000);
}


//Cria um item de menu com hover e over
void IFixedMenu::createMenuItemLabel(cocos2d::extension::CCListViewCell* cell, int tag, const char* Text,const ccColor3B& color3, float posX, float posY,SEL_MenuHandler menuCallback, CCNode* parent)
{
    CCLabelTTF *labelNormalState;
    labelNormalState = CCLabelTTF::create(Text, "Lucida Grande", 15);
    labelNormalState->setAnchorPoint(ccp(0,0));
    labelNormalState->setColor(ccc3(72, 72, 72));

    CCLabelTTF *labelTouch;
    labelTouch = CCLabelTTF::create(Text, "Lucida Grande",15);
    labelTouch->setAnchorPoint(ccp(0,0));
    labelTouch->setColor(ccc3(255, 255,255));
    	
    CCMenu* menu = CCMenu::create();
    CCMenuItemSprite *item = CCMenuItemSprite::create(labelNormalState, labelTouch, NULL, NULL);
    item->setAnchorPoint(ccp(0, 0));
    item->setContentSize(cell->getContentSize());
    menu->setPosition(ccp(posX, posY));
    menu->addChild(item, 0, tag);
    
    menu->setContentSize(cell->getContentSize());
    cell->addChild(menu);
}

 

//Altera o titulo no topo até 24 caracteres
void IFixedMenu::ChangeTitle(int _waypointID)
{
    std::string name =  LoadWayPointName(_waypointID);

    if( name.length() > 24)
        ChangeTitle((name.substr(0,20)+ "...").c_str());
    else
        ChangeTitle(name.c_str());
}


//Altera o titulo no topo
void IFixedMenu::ChangeTitle(const char *title)
{
     CCLabelTTF *labelTitle;
     labelTitle = CCLabelTTF::create(title, "LucidaGrandeBold", 15);
     labelTitle->setPosition(ccp(100,45));
     labelTitle->setAnchorPoint(ccp(0,0.5));
     labelTitle->setColor(ccc3(255, 255, 255));
    
    if(this->getChildByTag(2)->getChildByTag(345) != NULL)
        this->getChildByTag(2)->removeChildByTag(345, true);
    
    if( strcmp(title,"" ) !=0)
        this->getChildByTag(2)->addChild(labelTitle,3,345);
}


//Carrega uma imagem com o titulo da secao
void IFixedMenu::ChangeTitle(HomeMenuState menu)
{

    CCSprite *sprite = NULL;
    switch (menu) {
        case BANHEIROS:
            sprite = CCSprite::create("tit_banheiros.png");
            break;
        case COMO_CHEGAR:
            sprite = CCSprite::create("tit_comochegar.png");
            break;
        case ESTACIOMENTOS:
            sprite = CCSprite::create("tit_Estacionamento.png");
            break;
        case SERVICOS:
            sprite = CCSprite::create("tit_servicos.png");
            break;
        case ESPECIALIDADES_MEDICAS:
            sprite = CCSprite::create("tit_especialidadesmedicas.png");
            break;
        case MINHAS_ROTAS:
            sprite = CCSprite::create("tit_tracarrota.png");
            break;
        default:
            break;
    }

    if( sprite != NULL)
    {
        
        sprite->setPosition(ccp(100,45));
        sprite->setAnchorPoint(ccp(0,0.5));
         if(this->getChildByTag(2)->getChildByTag(345) != NULL)
            this->getChildByTag(2)->removeChildByTag(345, true);
        this->getChildByTag(2)->addChild(sprite,3,345);
    }
}

 

//Carrega o nome do waypoint e retorna na forma array representando as quebras de linhas
std::vector<std::string>  IFixedMenu::LoadWayPointNameToArray(int _wayPointID)
{
    return PListFunctions::readWayPointNameFormatado(_wayPointID);
}

 void IFixedMenu::releaseData()
{
    
}

//Carrega o nome de waypoint pelo id do waypoint
const char* IFixedMenu::LoadWayPointName(int wayPointID)
{
    //Sao pontos que nao existem, no waypoint plist
    switch (wayPointID) {
        case 0:
            return "Banheiro";
             break;
            
        case 991:
            return "Restaurantes/Café";
            break;
            
            
        case 992:
            return "Cabeleireiros";
            break;
            
        case 993:
            return "Box Cônvenios";
            break;
            
        case 994:
            return "Banco/Caixa Eletrônicos";
            break;
            
        case 995:
            return "Oncologia";
            break;
            
        case 996:
            return "Consultórios";
            break;
        case 332:
            return "Maternidade";
            break;
        default:
            break;
    }
    
    vector<WayPoint> lstVector = PListFunctions::readWayPointSection();
    for(int i =0;i < lstVector.size(); i ++)
        if(wayPointID == lstVector[i].key)
            return lstVector[i].name;
    return "";
}


//Carregas predios com o ultimo ponto salvo ou nao
void IFixedMenu::BuildBuildingStateWithLastPoint(bool include)
{
    cleanUp();
    vector<BuildingSection>  lstBuildings = PListFunctions::readBuildingSection();
    
    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<int> wID;
    
    for(int i = 0; i < lstBuildings.size();i++)
    {
        waypointStr.push_back(lstBuildings[i].name);
		waypointNormalOrder.push_back(lstBuildings[i].name);
		wID.push_back(lstBuildings[i].key);
    }
    
    std::sort(waypointStr.begin(), waypointStr.end(),compareByNum());
	for(int i = 0; i < waypointStr.size(); i++){
		std::string itemName = waypointStr.at(i);
		int itemID;
		
		for(int j = 0; j < waypointNormalOrder.size(); j++){
			std::string bNameNormalOrder = waypointNormalOrder.at(j);
			if(!itemName.compare(bNameNormalOrder)){
				itemID = wID.at(j);
				
				insertItemListView(itemID,"s",itemName.c_str());
				break;
			}
		}
    }

	setUpMenu();
}


//Carregas predios com o ultimo ponto salvo
void IFixedMenu::BuildBuildingState()
{
    BuildBuildingStateWithLastPoint(false);
}

//Carregas os waypoints favoritos que foram  salvos pelo usuario
void IFixedMenu::BuildFavorites(bool removeWayPoint, int waypointToRemove)
{
    cleanUp();

    
    RotasFileIO mRotas;// = new RotasFileIO();
    std::vector<RotaRow>  lstRows = mRotas.LoadMinhasRotas();
    for(int i = 0; i < lstRows.size(); i++)
    {
        
        if(removeWayPoint)
            if(waypointToRemove != lstRows[i].wayPoint)
                insertItemListView( lstRows[i].wayPoint,lstRows[i].description);
        else
            insertItemListView( lstRows[i].wayPoint,lstRows[i].description);
    }
    setUpMenu();
}

//Carregas os andares baseado em buildingKey pai
void IFixedMenu::BuildFloorState(int buildingKey)
{
    cleanUp();
    
    if((buildingKey == -1) && (Global::ULTIMO_PONTO_CHEGADA_WAYPOINT != 0 ))
        insertItemListView(-2,"Ultimo ponto de Chegada");
    
    vector<FloorSection> lstFloors = PListFunctions::readFloorSection();
    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<int> wID;
    
    for(int i = 0; i < lstFloors.size(); i++)
    {
        if((buildingKey == lstFloors[i].buildingID)|| (buildingKey == -1))
        {
			waypointStr.push_back(lstFloors[i].name);
			waypointNormalOrder.push_back(lstFloors[i].name);
			wID.push_back(lstFloors[i].key);
		}
    }
    
    std::sort(waypointStr.begin(), waypointStr.end(),compareByNum());
	for(int i = 0; i < waypointStr.size(); i++){
		std::string itemName = waypointStr.at(i);
		int itemID;
		
		for(int j = 0; j < waypointNormalOrder.size(); j++){
			std::string bNameNormalOrder = waypointNormalOrder.at(j);
			if(!itemName.compare(bNameNormalOrder)){
				itemID = wID.at(j);
				
				insertItemListView(itemID,"s",itemName.c_str());
				break;
			}
		}
	}
    setUpMenu();
}

//Retorna a Categoria do waypoint podendo ser NONE, ESPECIALIDADE_MEDICA OU SERVICO
HomeMenuState IFixedMenu::getCategory(int _waypointID)
{
    vector<ServiceSection> lstSection = PListFunctions::readServicesSection();
    for(int i = 0; i < lstSection.size();i++)
    {
          if(lstSection[i].key  == _waypointID)
          {
              if( lstSection[i].categoryID == 222)
                  return SERVICOS;
              else  if( lstSection[i].categoryID  == 111)
                  return ESPECIALIDADES_MEDICAS;
              else
                  return getCategory(lstSection[i].categoryID);
          }
    }
    
    return NONE;
}

//Carrega waypoints filhos de uma parentKeyID
void IFixedMenu::BuildCategoryWayPointState(int bKey)
{
    cleanUp();
  
    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<std::string> parentStr;
	std::vector<int> wID;
	std::vector<int> category;
    vector<ServiceSection> lstSection = PListFunctions::readServicesSection();
    for(int i = 0; i < lstSection.size();i++)
    {
        if(lstSection[i].categoryID == bKey)
        {
            
			wID.push_back(lstSection[i].key);
			waypointStr.push_back(lstSection[i].name);
			waypointNormalOrder.push_back(lstSection[i].name);
			
			parentStr.push_back(lstSection[i].hasChild);
			category.push_back(lstSection[i].categoryID);
            /*
            insertItemListView(lstSection[i].key,lstSection[i].hasChild,(lstSection[i].name),lstSection[i].categoryID);
             */
        }
    }
 
    
 	std::sort(waypointStr.begin(), waypointStr.end(),compareByNum());
	for(int i = 0; i < waypointStr.size(); i++){
		std::string itemName = waypointStr.at(i);
		int itemID;
		int itemCat;
		std::string itemParentName;
		
		for(int j = 0; j < waypointNormalOrder.size(); j++){
			std::string bNameNormalOrder = waypointNormalOrder.at(j);
			if(!itemName.compare(bNameNormalOrder)){
				itemID = wID.at(j);
				itemCat = category.at(j);
				itemParentName = parentStr.at(j);
				
				insertItemListView(itemID,itemParentName.c_str(),itemName.c_str(),itemCat);
				break;
			}
		}
	}
  
    setUpMenu();
 
  
}

void IFixedMenu::BuildWayPointByBuilding(int bKey)
{
    cleanUp();
  
    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<std::string> parentStr;
	std::vector<int> wID;
	std::vector<int> category;
    vector<FloorSection> lstSection = PListFunctions::readFloorSection();
    for(int i = 0; i < lstSection.size();i++)
    {
        if(lstSection[i].buildingID == bKey)
        {
            
			wID.push_back(lstSection[i].key);
			waypointStr.push_back(lstSection[i].name);
			waypointNormalOrder.push_back(lstSection[i].name);
			
			parentStr.push_back(lstSection[i].name);
			category.push_back(lstSection[i].key);
            /*
            insertItemListView(lstSection[i].key,lstSection[i].hasChild,(lstSection[i].name),lstSection[i].categoryID);
             */
        }
    }
 
    
 	std::sort(waypointStr.begin(), waypointStr.end(),compareByNum());
	for(int i = 0; i < waypointStr.size(); i++){
		std::string itemName = waypointStr.at(i);
		int itemID;
		int itemCat;
		std::string itemParentName;
		
		for(int j = 0; j < waypointNormalOrder.size(); j++){
			std::string bNameNormalOrder = waypointNormalOrder.at(j);
			if(!itemName.compare(bNameNormalOrder)){
				itemID = wID.at(j);
				itemCat = category.at(j);
				itemParentName = parentStr.at(j);
				
				insertItemListView(itemID,itemParentName.c_str(),itemName.c_str(),itemCat);
				break;
			}
		}
	}
  
    setUpMenu();
 
  
}


//Inserre um item que sera usado em uma ListView
void IFixedMenu::insertItemListView( int tagValue, const char* datatext)
{
    PairKey data ;
    data.key = tagValue;
    data.text = strdup(datatext);
    data.hasChild =  false;
    data.keyParent = 0;
    plistMenuCurrent.push_back(data);
}


//Inserre um item que sera usado em uma ListView
void IFixedMenu::insertItemListView( int tagValue, const char* isChild, const char* datatext)
{
    PairKey data ;
    data.key = tagValue;
    data.text = strdup(datatext);
    data.hasChild =(strcmp(isChild, "s") == 0);
    plistMenuCurrent.push_back(data);
}


//Inserre um item que sera usado em uma ListView
void IFixedMenu::insertItemListView( int tagValue, const char* isChild, const char* datatext, int keyParent)
{
    PairKey data ;
    data.key = tagValue;
    data.text = strdup(datatext);
    
    
    data.hasChild =(strcmp(isChild, "s") == 0);
    data.keyParent =keyParent;
    plistMenuCurrent.push_back(data);
}

//Cria uma label
void IFixedMenu::insertLabel(ccColor3B color, CCPoint position, int tagValue,int zOrder, bool bold,  const char* datatext, CCNode* parent, int size)
{
    CCLabelTTF *labelTitle;
    
    if( bold)
        labelTitle = CCLabelTTF::create(datatext, "LucidaGrandeBold", size);
    else
       labelTitle = CCLabelTTF::create(datatext, "Lucida Grande", size);
    
    labelTitle->setPosition(position);
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(color);
    parent->addChild(labelTitle,zOrder,tagValue);
}

//Cria uma label
void IFixedMenu::insertLabel(ccColor3B color, CCPoint position,  bool bold,  const char* datatext, CCNode* parent, int size)
{
    CCLabelTTF *labelTitle;
    
    if( bold)
        labelTitle = CCLabelTTF::create(datatext, "LucidaGrandeBold", size);
    else
        labelTitle = CCLabelTTF::create(datatext, "Lucida Grande", size);
    
    labelTitle->setPosition(position);
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(color);
    parent->addChild(labelTitle);
}


//Procura um item baseado em uma keyword ou nao. keyword em branco ira trazer todos os itens
void IFixedMenu::SearchItem( const char* keyword)
{
    cleanUp();

    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<int> wID;

    vector<KeyWordSection> lstVector = PListFunctions::readKeyWordSection();
    for(int i = 0; i < lstVector.size(); i++)
    {
        if( lstVector[i].keywords.size() > 0)
        {
            for(int ii = 0 ; ii < lstVector[i].keywords.size();ii++)
            {
                if( Global::contains(lstVector[i].keywords[ii].c_str(),keyword) == true)
                {
                    waypointStr.push_back(lstVector[i].keywords[ii].c_str());
                    waypointNormalOrder.push_back(lstVector[i].keywords[ii].c_str());
                    wID.push_back(lstVector[i].key);
                }
            }
        }
    }
    
    /*
    CCDictionary *plist = CCDictionary::createWithContentsOfFileThreadSafe("waypoints.plist");
    CCDictionary *waypoints = (CCDictionary *)plist->valueForKey("keywords");
   
    
    
    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<int> wID;
    
    CCDictElement *element = NULL;
    CCDICT_FOREACH(waypoints, element)
    {
        int key = CCString::create(element->getStrKey())->intValue();
        CCDictionary *waypoint = (CCDictionary *)element->getObject();
        CCArray* arr = (CCArray*)waypoint->valueForKey("name");
        
        if( arr->count() > 0 )
            for(int i = 0 ; i < arr->count();i++)
                if( Global::contains(((CCString*)arr->objectAtIndex(i))->m_sString.c_str(),keyword) == true)
                {
                              std::string name  = ((CCString*)arr->objectAtIndex(0))->m_sString;
                    
                    waypointStr.push_back(name);
                    waypointNormalOrder.push_back(name);
                    wID.push_back(key);
                    break;
                }
    }*/
    
    

    
    
    std::sort(waypointStr.begin(), waypointStr.end(),compareByNum());
	for(int i = 0; i < waypointStr.size(); i++){
		std::string itemName = waypointStr.at(i);
		int itemID;
		
		for(int j = 0; j < waypointNormalOrder.size(); j++){
			std::string bNameNormalOrder = waypointNormalOrder.at(j);
			if(!itemName.compare(bNameNormalOrder)){
				itemID = wID.at(j);
				
				insertItemListView(itemID,itemName.c_str());
				break;
			}
		}
	}
}
 
/******Carrega waypoints baseado em floorid PAI */
void IFixedMenu::BuildWayPointState( int floorKey, bool removeWayPoint, int waypointToRemove)
{
    cleanUp();
    std::vector<std::string> waypointStr;
	std::vector<std::string> waypointNormalOrder;
	std::vector<int> wID;
    vector<WayPoint> lstvector =   PListFunctions::readWayPointSection();
 
    for(int i = 0; i < lstvector.size();i++)
    {
          if(floorKey == lstvector[i].floorID && !lstvector[i].isWC && !lstvector[i].isElevator && !lstvector[i].isEscalator && !lstvector[i].isTransferRight && !lstvector[i].isTransferLeft){
              {
                  
                  if( removeWayPoint == true)
                  {
                      if(waypointToRemove !=  lstvector[i].key)
                      {
                          waypointStr.push_back( lstvector[i].name);
                          waypointNormalOrder.push_back( lstvector[i].name);
                          wID.push_back( lstvector[i].key);
                      }
                  }
                  else
                  {
                      waypointStr.push_back( lstvector[i].name);
                      waypointNormalOrder.push_back( lstvector[i].name);
                      wID.push_back( lstvector[i].key);
                      
                  }
              }
          }
    }

	std::sort(waypointStr.begin(), waypointStr.end(),compareByNum());
	for(int i = 0; i < waypointStr.size(); i++){
		std::string itemName = waypointStr.at(i);
		int itemID;
		
		for(int j = 0; j < waypointNormalOrder.size(); j++){
			std::string bNameNormalOrder = waypointNormalOrder.at(j);
			if(!itemName.compare(bNameNormalOrder)){
				itemID = wID.at(j);
                insertItemListView(itemID,itemName.c_str());
                
				break;
			}
		}
	}
    setUpMenu();
}

/*******************************************************************************************************************************/
/* KeyboardNotificationLayer                                                                                                   */
/*******************************************************************************************************************************/
USING_NS_CC;
static CCRect getRect(CCNode * pNode)
{
    CCRect rc;
    rc.origin = pNode->getPosition();
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

KeyboardNotificationLayer::KeyboardNotificationLayer()
: __pTrackNode(0)
{
    setTouchEnabled(true);
}

void KeyboardNotificationLayer::onClickTrackNode(bool bClicked)
{
    CCTextFieldTTF * pTextField = (CCTextFieldTTF*)__pTrackNode;
    if (bClicked)
    {
        pTextField->attachWithIME();
        
        CCMenu* mnClose = CCMenu::create();
        CCSprite *spriteDefault = CCSprite::create("Close_TextBox.png");
        CCMenuItemSprite *item = CCMenuItemSprite::create(spriteDefault, spriteDefault, this, menu_selector(KeyboardNotificationLayer::btnCleanTextBox));
        item->setAnchorPoint(ccp(0, 0));
        item->setPosition(-1,2);
        mnClose->addChild(item);
        mnClose->setPosition(ccp(pTextField->getParent()->getContentSize().width-25, 6));
        __pTrackNode->getParent()->addChild(mnClose,0,346);
    }
    else
    {
 
        if(__pTrackNode->getParent()->getChildByTag(346)!=NULL)
            __pTrackNode->getParent()->removeChildByTag(346, true);
        pTextField->detachWithIME();
    }
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void KeyboardNotificationLayer::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    if(__pTrackNode->getParent()->getChildByTag(346)!=NULL)
        __pTrackNode->getParent()->removeChildByTag(346, true);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    if (! __pTrackNode)
        return;
    
    CCRect rectTracked = getRect(__pTrackNode);
    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! CCRect::CCRectIntersectsRect(rectTracked, info.end))
        return;
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = CCRect::CCRectGetMaxY(info.end) - CCRect::CCRectGetMinY(rectTracked);

    
    // move all the children node of KeyboardNotificationLayer
    CCArray * children = getChildren();
    CCNode * node = 0;
    
    if(children != NULL)
    {
      int count = children->count();
      CCPoint pos;
      for (int i = 0; i < count; ++i)
       {
        node = (CCNode*)children->objectAtIndex(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
       }
    }
}

// CCLayer function
bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    __beginPos = pTouch->locationInView();
    __beginPos = CCDirector::sharedDirector()->convertToGL(__beginPos);
    return true;
}

#include "HomeScene.h"
void IFixedMenu::returnToHome()
{
    releaseData();
    cleanUp();
    changeScene(new HomeScene());
}

/*Limpa variaveis de Menu e libera memoria de chars[] criados via strdup */
void IFixedMenu::cleanUp()
{
    for(int i = 0; i < plistMenuCurrent.size();i++)
        delete plistMenuCurrent[i].text;
    plistTagMenuCurrent.clear();
    plistMenuCurrent.clear();
}

/*Compara duas String e retorna se sao iguais. Se str2 for em branco retorna true*/
bool Global::contains(  std::string str ,std::string str2)
{
    if(str.length() > 0)
    {
        str[0] = toupper(str[0]);
        for(std::string::iterator it = str.begin(); it != str.end(); ++it)
            *it = toupper(*it);
    }

    if(str2.length() > 0)
    {
    for(std::string::iterator it2 = str2.begin(); it2 != str2.end(); ++it2)
        *it2 = toupper(*it2);
    }
    
    if(strcmp(str2.c_str()," ")== 0)
        return  true;
    
    if(str.find(str2)!=std::string::npos)
        return true;
    
    return false;
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (! __pTrackNode)
        return;

    CCPoint endPos = pTouch->locationInView();
    endPos = CCDirector::sharedDirector()->convertToGL(endPos);
    
    float delta = 6.0f;
    if (::abs(endPos.x - __beginPos.x) > delta || ::abs(endPos.y - __beginPos.y) > delta)
    {
        // not click
        __beginPos.x = __beginPos.y = -1;
        return;
    }
    
    // decide the trackNode is clicked.
    CCRect rect;
    CCPoint point = convertTouchToNodeSpaceAR(pTouch);
    rect = getRect(__pTrackNode);
    this->onClickTrackNode(CCRect::CCRectContainsPoint(rect, point));
    CCLOG("----------------------------------");
}

void KeyboardNotificationLayer::setTextField(cocos2d::CCTextFieldTTF* pTextField)
{
	__pTrackNode = pTextField;
}

void KeyboardNotificationLayer::btnCleanTextBox(CCObject *sender)
{
    if(__pTrackNode->getParent()->getChildByTag(346)!=NULL)
        __pTrackNode->getParent()->removeChildByTag(346, true);
    
    IsCleanUp = true;
    ((CCTextFieldTTF*)__pTrackNode)->setString("");
}
