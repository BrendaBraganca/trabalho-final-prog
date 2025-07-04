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
                inimigos[acc].direcao = GetRandomValue(0,3); //da uma direcao aleatoria e armazena para usar depois
                acc++;
            }
        }
    }
}

void desenhaInimigos(tipoInimigo inimigos[]){
    for(int i = 0; i < MAX_INIMIGOS; i++){
        if(inimigos[i].posicao.linha >= 0 && inimigos[i].posicao.coluna >=0){
            DrawRectangle(inimigos[i].posicao.coluna * 20, inimigos[i].posicao.linha * 20, 20, 20, PINK);
        }
    }
}

void movimentarInimigos(tipoInimigo inimigos[], char **mapa){
    for(int i= 0; i < MAX_INIMIGOS; i++){
        if (inimigos[i].posicao.linha < 0 || inimigos[i].posicao.coluna < 0) continue;
        int novaDirecao = GetRandomValue(0,3); // sorteia uma nova direcao aleatoria
        int novaLinha = inimigos[i].posicao.linha;
        int novaColuna = inimigos[i].posicao.coluna;

        switch (novaDirecao){
        case 0:
            novaLinha--;
            break;
        case 1:
            novaLinha++; 
            break;
        case 2:
            novaColuna--;
            break;
        case 3:
            novaColuna++;
        }

         // ve se tem obstaculos na frente
        if(mapa[novaLinha][novaColuna] == ' '){
            inimigos[i].posicao.linha = novaLinha;
            inimigos[i].posicao.coluna = novaColuna;
        }

    }

}















