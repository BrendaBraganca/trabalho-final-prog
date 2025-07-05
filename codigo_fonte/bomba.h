#pragma once
#include "posicao.h"
#include "jogador.h"
#include "inimigo.h"
#define BOMBA_H
#define MAX_BOMBAS 3
#define TEMPO_EXPLOSAO 3.0f
#define RAIO_EXPLOSAO 100
#define MIN(a, b) ((a) < (b) ? (a) : (b)) // pra pontuacao nao ficar negativa

typedef struct {
    tipoPosicao posicao;
    float tempoRestante; //quanto falta para explodir
    int ativa;
    int explodindo; 
    float tempoExplosao; // cronometro para explosao
} tipoBomba;

void inicializarBombas(tipoBomba bombas[]);
int plantarBombas(tipoBomba bombas[], tipoPosicao posicao, int bombasRestantes); // retorna se a bomba conseguiu ser plantada ou não
void explodirBombas(tipoBomba *bomba, char **mapa, tipoInimigo inimigos[], tipoJogador *jogador);
void atualizarBombas(tipoBomba bombas[], float intervalo, char **mapa, tipoInimigo in