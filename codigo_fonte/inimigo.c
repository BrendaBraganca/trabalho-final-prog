#include "inimigo.h"
#include "raylib.h"
#include "mapa.h"

void inicializaInimigos(tipoInimigo inimigos[], char **mapa){
    int acc = 0;
    for(int i = 0; i < LINHAS; i++){
        for(int j =0; j < COLUNAS; j++){
            if(mapa[i][j] == 'E' && acc < MAX_INIMIGOS){
                inimigos[acc].posicao.linha = i;
                inimigos[acc].posicao.coluna = j;
                inimigos[acc].direcao = GetRandomValue(0,3); //da uma direcao aleatoria
                acc++;
            }
        }
    }
}

void desenhaInimigos(tipoInimigo inimigos[]){
    for(int i = 0; i < MAX_INIMIGOS; i++){
        int x = inimigos[i].posicao.coluna *20; // largura 
        int y = inimigos[i].posicao.linha *20;  // altura 
        DrawRectangle(x, y, 20,20, RED);
    }
}