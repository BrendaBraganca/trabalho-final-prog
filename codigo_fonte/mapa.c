#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// O mapa é uma matriz de caracteres
// O ponteiro para ponteiro ** permite que essa matriz seja alocada dinanmicamente
// O path (caminho para o arquivo de mapa que ainda vai ser renderizado) esta declarado como const porque ele vai ser usado somente para leitura e qualquer alteracao nele pode causar um erro


char **inicializarMapa(const char *path){
    FILE *arqMapa = fopen(path, "r");
    if(!arqMapa){
        perror("ERRO: o arquivo referente ao mapa nao pode ser aberto."); //funcao para tratar erro 
        return NULL;
    }

    char **mapa = malloc(LINHAS * sizeof(char *));
    if(!mapa){
        perror("ERRO: a memoria nao pode ser alocada para a criacao do mapa.");
        fclose(arqMapa);
        return NULL;
    }

    char linhaDoArquivo[COLUNAS +1 +1]; // soma o \n e o \0 de cada linha
    int linhaAtual = 0;
    
    while(linhaAtual < LINHAS && fgets(linhaDoArquivo, sizeof linhaDoArquivo, arqMapa)){
        size_t qtdCaracteres = strcspn(linhaDoArquivo, "\r\n"); //ve a quantidade de caracteres significativos da linha (sem contar o \n)
        if(qtdCaracteres < COLUNAS){
            fprintf(stderr, "A quantidade de colunas na linha %d é inferior a quantidade especificada nas instrucoes.", linhaAtual + 1); //usei stderr e nao stdout pq stderr e especifica para erros
            finalizarMapa(mapa);
            fclose(arqMapa);
            return NULL;
        }
        
        mapa[linhaAtual] = malloc(COLUNAS + 1); // +1 por causa do \0
        if(!mapa[linhaAtual]){
            perror("ERRO: nao foi possivel alocar memoria para a linha atual do mapa");
            finalizarMapa(mapa);
            fclose(arqMapa);
            return NULL;
        }

        memcpy(mapa[linhaAtual], linhaDoArquivo, COLUNAS); //copia a linha do arquivo do mapa para a estrutura do mapa
        mapa[linhaAtual][COLUNAS] ='\0';
        
        // FAZER UMA FUNCAO PARA VALIDAR CARACTERES??
        linhaAtual++;
    }

    if(linhaAtual != LINHAS){
        fprintf(stderr, "A quantidade de linhas do arquivo esta diferente do espeficicado");
        finalizarMapa(mapa);
        fclose(arqMapa);
        return NULL;
    }

    fclose(arqMapa);
    return mapa;

}

//precisa liberar a memoria do mapa linha por linha pq cada linha foi alocada dinamicamente

void finalizarMapa(char **mapa){
    if(!mapa) return;
    for(int i = 0; i < LINHAS; i++){
        free(mapa[i]);
    }
    free(mapa);
}


