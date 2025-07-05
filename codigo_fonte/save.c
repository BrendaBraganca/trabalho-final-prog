#include "save.h"
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char **alocarMapa(void){
    char **mapaVazio = malloc(LINHAS * sizeof(char *));
    for(int i =0; i < LINHAS; i++){
        mapaVazio[i] = malloc(COLUNAS * sizeof(char));
    }
    return mapaVazio;
}

static void liberarMapa(char **mapaAlocado){
    for(int i = 0; i < LINHAS; i++){
        free(mapaAlocado[i]);
    }
    free(mapaAlocado);
}

int salvarJogo(const char *arquivo, tipoJogador jogador, tipoBomba bombas[], tipoInimigo inimigos[], char **mapa){
    FILE *arq = fopen(arquivo, "wb");
    if(!arq){
        return -1;
    }
    fwrite(&jogador, sizeof(tipoJogador), 1, arq);
    fwrite(bombas, sizeof(tipoBomba), MAX_BOMBAS, arq);
    fwrite(inimigos, sizeof(tipoInimigo), MAX_INIMIGOS, arq);
    for(int i =0; i< LINHAS; i++){
        fwrite(mapa[i], sizeof(char), COLUNAS, arq);
    }
    fclose(arq);
    return 0;
}

int carregarJogo(const char *arquivo, tipoJogador *jogador, tipoBomba bombas[], tipoInimigo inimigos[], char ***mapa){
    FILE *arq = fopen(arquivo, "rb");
    if(!arq){
        return -1;
    }
    if(fread(jogador, sizeof(tipoJogador), 1, arq) != 1){
        fclose(arq);
        return -1;
    }
    if(fread(bombas, sizeof(tipoBomba), MAX_BOMBAS, arq) != MAX_BOMBAS){
        fclose(arq);
        return -1;
    }
    if(fread(inimigos, sizeof(tipoInimigo), MAX_INIMIGOS, arq) != MAX_INIMIGOS){
        fclose(arq);
        return -1;
    }

    char **mapaNovo = alocarMapa();
    for(int i =0; i < LINHAS; i++){
        if(fread(mapaNovo[i], sizeof(char), COLUNAS, arq) != COLUNAS){
            fclose(arq);
            liberarMapa(mapaNovo);
        }
    }

    fclose(arq);

    // esta substituindo o mapa antigo pelo novo
    if(*mapa){
        liberarMapa(*mapa); // evita memory leak
    }
    *mapa = mapaNovo;
    return 0;

}





