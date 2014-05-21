//
//  News.h
//  EinsteinPrototype
//
//  Created by Camargo on 04/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_News_h
#define EinsteinPrototype_News_h

#include <iostream>
#include "PFObject.h"

using namespace cocos2d;
using namespace std;

class News {
    
public:
    News();
    ~News();
    string cod_notificacao;
    string codUsuario;
    string notiAtivo;
    string usuario;
    string titulo;
    string texto;
    string imagem;
    string url_link;
    string nome;
    string email;
    string notificacao_validade;
    string notificacao_cadastro;
};

#endif
