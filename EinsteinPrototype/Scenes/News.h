//
//  News.h
//  EinsteinPrototype
//
//  Created by Camargo on 04/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_News_h
#define EinsteinPrototype_News_h

#include "PFObject.h"

using namespace cocos2d;

class News : public CCObject{
    
public:
    News();
    ~News();
    char *                    cod_notificacao;
    char *                    codUsuario;
    char *                    notiAtivo;
    char *                    usuario;
    char *                    titulo;
    char *                    texto;
    char *                    imagem;
    char *                    url_link;
    char *                    nome;
    char *                    email;
    char *                    notificacao_validade;
    char *                    notificacao_cadastro;
    
};


#endif
