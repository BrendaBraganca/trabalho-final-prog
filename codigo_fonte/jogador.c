#include "jogador.h"
#include "raylib.h"

void inicializarJogador(tipoJogador *j){
    j->posicao.linha =1;
    j->posicao.coluna=1;
    j->vidas= 3;
    j->bombas=3;
    j->pontuacao = 0;
}

void desenharJogador(tipoJogador j) {
    int x = j.posicao.coluna * 20;
    int y = j.posicao.linha * 20;
    DrawRectangle(x, y, 20, 20, BLUE);
}



