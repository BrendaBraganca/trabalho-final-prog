#include "bomba.h"
#include "mapa.h"
#include "inimigo.h"
#include "jogador.h"
#include "raylib.h"
#include <stdlib.h>


void inicializarBombas(tipoBomba bombas[]){
    for(int i =0; i < MAX_BOMBAS; i++){
        bombas[i].ativa = 0;
        bombas[i].explodindo = 0;
        bombas[i].tempoRestante = 0;
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

void explodirBombas(tipoBomba *bomba, char **mapa, tipoInimigo inimigos[], tipoJogador *jogador){ //bomba e jogador tem que ser passados por referencia por que as alteracoes tem que realmente fazer uma mudanca na estrutuea
    int linhaDaBomba = bomba->posicao.linha;
    int colunaDaBomba = bomba->posicao.coluna;

    int coordenadasBomba[5][2] = {
        {0, 0}, {-1,0}, {1,0}, {0,-1}, {0, 1}
    };

    for(int i =0; i < 5; i++){
        int novaLinhaBomba = linhaDaBomba + coordenadasBomba[i][0];
        int novaColunaBomba = colunaDaBomba + coordenadasBomba[i][1];

        //se tentar acessar uma area invalida, pula para uma nova coordenada 
        if(novaLinhaBomba < 0 || novaLinhaBomba >= LINHAS || novaColunaBomba < 0 || novaColunaBomba >= COLUNAS) continue;

        char localBomba = mapa[novaLinhaBomba][novaColunaBomba];

        if (localBomba == 'K') {
            mapa[novaLinhaBomba][novaColunaBomba] = 'C';  //caso seja uam caixa com chave ->  chave fica visível
            jogador->pontuacao += 100;
        } else if (localBomba == 'B' || localBomba == 'D') {
            mapa[novaLinhaBomba][novaColunaBomba] = ' ';
            jogador->pontuacao += 100;
        }

        for(int j =0; j < MAX_INIMIGOS; j++){
            if(inimigos[j].posicao.linha == novaLinhaBomba && inimigos[j].posicao.coluna == novaColunaBomba){
                inimigos[j].posicao.linha = -1; //o inimigo fica numa posicao invalida e nao e desnhado
                inimigos[j].posicao.coluna = -1;
                jogador->pontuacao = jogador->pontuacao +20;
            }
        }

        if(jogador->posicao.linha == novaLinhaBomba && jogador->posicao.coluna == novaColunaBomba){
            jogador->vidas--;
            jogador->pontuacao = MIN(abs(jogador->pontuacao - 100),  0);
        }

    }
    bomba->ativa =0;

}

void atualizarBombas(tipoBomba bombas[], float intervalo, char **mapa, tipoInimigo inimigos[], tipoJogador *jogador){ //atualiza cada bomba do jogador
    for(int i =0; i < MAX_BOMBAS; i++){
        if(bombas[i].ativa){
            bombas[i].tempoRestante = bombas[i].tempoRestante - intervalo;
            if(bombas[i].tempoRestante<= 0){ //bomba volta para o estoque depois de explodir
                explodirBombas(&bombas[i], mapa, inimigos, jogador);
                bombas[i].explodindo =1;
                bombas[i].tempoExplosao =0.5f; //templo que a explosão ficará visível na tela
                jogador->bombas++;
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

