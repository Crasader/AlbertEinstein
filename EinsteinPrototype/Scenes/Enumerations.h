//
//  Enumerations.h
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Enumerations_h
#define EinsteinPrototype_Enumerations_h
#include "cocos2d.h"
#include "extensions/CCScrollView/CCScrollView.h"
#include "extensions/CCListView/CCListView.h"
#include "extensions/CCScrollView/CCScrollView.h"
#include "extensions/CCListView/CCListView.h"
#include "AlertViewScene.h"

 
#include <locale>
#include <iostream>
#include <algorithm>
#include <stack>

//using namespace cocos2d;
//using namespace std;



typedef enum
{
    FACEBOOK,
    FOURSQUARE
} SocialType;


typedef enum{
	ACTUAL_BUILDING,
	ACTUAL_FLOOR,
	ACTUAL_WAYPOINT,
	TARGET_BUILDING,
	TARGET_FLOOR,
	TARGET_WAYPOINT,
    TARGET_CATEGORY,
    TARGET_SERVICE,
    TARGET_CATEGORY_WAYPOINT,
    TARGET_CATEGORY_WAYPOINT_PREVIOUS_PAGE,
    TARGET_PATH_FINDER_PAGE,
    TARGET_NONE,
    TARGET_FAVORITOS,
    TARGET_SEARCH,
    PATH_FINDER
} MenuListState;
struct PairKey
{
    public:
    bool hasChild;
    int key;
    int keyParent;
    const char* text;
};

/*
class LabelTTF : public CCLabelTTF
{
public:
    LabelTTF();
    
};*/

typedef enum{
	INICIO,
	MAPAS,
	MINHAS_ROTAS,
    ESPECIALIDADES_MEDICAS,
    BANHEIROS,
    COMO_CHEGAR,
    SERVICOS,
    ESTACIOMENTOS,
    INFORMACOES,
	PROCURAR,
    MAPA_ESTACIONAMENTO,
    
    ESTACIOMENTOS_ENT,
    ESTACIOMENTOS_SAI,
    NONE
} HomeMenuState;

struct StateMachine
{
    char title[200];
    std::stack<int> stackCategory;
    
 bool   isCategoryChild;
    MenuListState lastState;
    HomeMenuState actual_category;
    StateMachine()
    {
        actual_category  = NONE;
        lastState = TARGET_NONE;
    }
};

class IFixedMenu :  public cocos2d::CCLayer
{
protected:
    
    std::stack<int> stackCategory;
    cocos2d::extension::CCListView *pList;
    std::vector<PairKey>  plistMenuCurrent;
    std::vector<int> plistTagMenuCurrent;
    HomeMenuState actualCategory;
    int wayPoint;
    int targetWayPoint;
 
    virtual void releaseData();
    
public:
    IFixedMenu();
    ~IFixedMenu();

    
    void cleanUp();
    void returnToHome();
    void changeScene(CCLayer *Node);
    void initListView(cocos2d::CCSize size, cocos2d::extension::CCListViewDelegate* parent );
    void BuildCategoryWayPointState(int bKey);
	void BuildWayPointByBuilding(int bKey);
    void insertLabel(cocos2d::ccColor3B color, cocos2d::CCPoint position,  bool bold,  const char* datatext, CCNode* parent, int size);
    void insertLabel(cocos2d::ccColor3B color, cocos2d::CCPoint position, int tagValue,int zOrder, bool bold,  const char* datatext, CCNode* parent, int size);
    
    
    static std::vector<std::string>  LoadWayPointNameToArray(int wayPointID);

    
    static const  char*  LoadWayPointName(int wayPointID);
   // const  char*  LoadBuildNameFromFloorPoint(int floorID);
    
    
    HomeMenuState getCategory(int _waypointID);
    
    void SearchItem( const char* keyword);
    void insertItemListView( int tagValue, const char* datatext);
    void insertItemListView( int tagValue, const char* isChild, const char* datatext);
    void insertItemListView( int tagValue, const char* isChild, const char* datatext, int keyParent);
    virtual void OnBack();
    virtual void BuildBuildingState();
    virtual void BuildFavorites(bool removeWayPoint, int waypointToRemove);
    virtual void BuildBuildingStateWithLastPoint(bool include);
    virtual void BuildFloorState(int buildingKey);
    virtual void BuildWayPointState(int floorKey, bool removeWayPoint, int waypointToRemove);
    virtual void setUpMenu(){};
    void initFixedMenu();
    void initFixedMenu(HomeMenuState newState);
    void FixedMenuCallBack(CCObject *sender);
    void createMenuItem(cocos2d::CCMenu* menuParent, int tag, char* spriteFileName,char* spriteFileNameSelected, float posX, float posY,cocos2d::SEL_MenuHandler menuCallback, CCNode* parent);
    void createMenuItemLabel(cocos2d::extension::CCListViewCell* cell, int tag, const  char* Text,const cocos2d::ccColor3B& color3, float posX, float posY,cocos2d::SEL_MenuHandler menuCallback, CCNode* parent);
    void ChangeTitle(const char* title);
    void ChangeTitle(int waypointID);
 
    void ChangeTitle(HomeMenuState menu);
    void addButtonTop();
    void addButtonTop(HomeMenuState menu);
    void btnTop(CCObject *sender);
    cocos2d::CCSize setUpTopImage(const char* spritename);
    void setDefaultListView(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data, float space);
};

template<typename Container>
bool in_quote(const Container& cont, const std::string& s)
{
    return std::search(cont.begin(), cont.end(), s.begin(), s.end()) != cont.end();
}

class Global
{
    public :
    static const char*  TOP_SRC_IMAGE_Mapas ;
    static const char*TOP_SRC_IMAGE_Rotas   ;
    static const char* TOP_SRC_IMAGE_Procurar   ;
    static const char* TOP_SRC_IMAGE_Especialidade ;
    static const char* TOP_SRC_IMAGE_Banheiro  ;
    static const char* TOP_SRC_IMAGE_Estacionamentos   ;
    static const char* TOP_SRC_IMAGE_Servicos;
    static const char* TOP_SRC_IMAGE_Informacoes;
    static const char* TOP_SRC_IMAGE_ComoChegar;
    static int ULTIMO_PONTO_CHEGADA_WAYPOINT;
    static int TAG_Child_WayPoint;
    static int TAG_Parent_Layer;
    static int TAG_Child_Layer;
    static int TAG_Alert_Layer;
    static bool contains(  std::string str ,std::string str2);
};

class KeyboardNotificationLayer : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate
{
public:
    bool IsCleanUp = false;
    KeyboardNotificationLayer();
    
	void onClickTrackNode(bool bClicked);
    
	void registerWithTouchDispatcher();
	void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
    
    void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& info) ;
	// CCLayer
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void setTextField(cocos2d::CCTextFieldTTF* pTextField);
    void btnCleanTextBox(CCObject *sender);
    	cocos2d::CCNode*  __pTrackNode;
protected:

	cocos2d::CCPoint  __beginPos;
};


typedef enum
{
    EST_ENTRADA,
	EST_SAIDA
} EstacionamentoState;










 

#endif
