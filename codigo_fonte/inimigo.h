#pragma once
#define INIMIGO_H
#include "posicao.h"
#define MAX_INIMIGOS 5

typedef struct {
    tipoPosicao posicao;
    int direcao; // 0=cima 1=baixo 2=esq 3=dir
} tipoInimigo;

void inicializaInimigos(tipoInimigo inimigos[], char **mapa);
void desenhaInimigos(tipoInimigo inimigos[]);
void movimentarInimigos(tipoInimigo inimigos[], char **mapa);


