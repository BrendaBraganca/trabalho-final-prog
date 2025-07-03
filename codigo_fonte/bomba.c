#include "bomba.h"
#include "raylib.h"

void inicializarBombas(tipoBomba bombas[]){
    for(int i =0; i < MAX_BOMBAS; i++){
        bombas[i].ativa = 0;
        bombas[i].explodindo = 0;
        bombas[i].tempoRestante = 00;
    }
}

int plantarBombas(tipoBomba bombas[], tipoPosicao pos, int bombasRestantes){
    if (bombasRestantes <= 0){
        return 0;
    }
    for(int i =0; i <MAX_BOMBAS; i++){
        if(!bombas[i].ativa && !bombas[i].explodindo){
            bombas[i].posicao = pos;
            bombas[i].tempoRestante = TEMPO_EXPLOSAO;
            bombas[i].ativa = 1;
            return 1;
        }
    }
    return 0;
}

void atualizarBombas(tipoBomba bombas[], float intervalo){ //atualiza cada bomba do jogador
    for(int i =0; i < MAX_BOMBAS; i++){
        if(bombas[i].ativa){
            bombas[i].tempoRestante = bombas[i].tempoRestante - intervalo;
            if(bombas[i].tempoRestante<= 0){ //bomba volta para o estoque depois de explodir
                bombas[i].ativa = 0;
                bombas[i].explodindo =1;
                bombas[i].tempoExplosao =0.5f; //templo que a explosão ficará visível na tela
            }
        }
        else if(bombas[i].explodindo){
            bombas[i].tempoExplosao = bombas[i].tempoExplosao - intervalo;
            if (bombas[i].tempoExplosao <=0){
                bombas[i].explodindo = 0;
            }
            
        }
    }
}

void desenharBombas(tipoBomba bombas []){
    for(int i = 0;  i < MAX_BOMBAS; i++){
        int x = bombas[i].posicao.coluna *20;
        int y = bombas[i].posicao.linha *20;

        if(bombas[i].ativa){
            DrawRectangle(x, y, 20, 20, DARKBLUE); //desenha a bomba plantada
        }
        else if(bombas[i].explodindo){
            //desenhar uma cruz
            DrawRectangle(x, y, 20, 20, ORANGE); // centro da explosao
            DrawRectangle(x-20, y, 20, 20, ORANGE);  // uma casa para a esquerda
            DrawRectangle(x+20, y, 20, 20, ORANGE); // uma casa para direita
            DrawRectangle(x, y-20, 20, 20, ORANGE); // uma casa para cima
            DrawRectangle(x, y +20, 20, 20, ORANGE); //uma casa para baixo
        }
    }
}















