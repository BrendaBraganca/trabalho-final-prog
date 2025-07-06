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

int sa