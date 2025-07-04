#pragma once
#include "posicao.h"
#define JOGADOR_H

typedef struct tipoJogador{
    tipoPosicao posicao;
    int vidas;
    int bombas;
    int pontuacao;
    int chaves;
} tipoJogador;

void inicializarJogador(tipoJogador *j);
void desenharJogador(tipoJogador j);




