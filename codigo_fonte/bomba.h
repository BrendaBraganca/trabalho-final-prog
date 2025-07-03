#pragma once
#include "posicao.h"
#define BOMBA_H
#define MAX_BOMBAS 3
#define TEMPO_EXPLOSAO 3.0f
#define RAIO_EXPLOSAO 100

typedef struct {
    tipoPosicao posicao;
    float tempoRestante; //quanto falta para explodir
    int ativa;
    int explodindo; 
    float tempoExplosao; // cronometro para explosao
} tipoBomba;

void inicializarBombas(tipoBomba bombas[]);
int plantarBombas(tipoBomba bombas[], tipoPosicao posicao, int bombasRestantes); // retorna se a bomba conseguiu ser plantada ou não
void atualizarBombas(tipoBomba bombas[], float intervalo); 
void desenharBombas(tipoBomba bombas[]);




