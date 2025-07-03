#include "raylib.h"
#include "mapa.h"
#include "jogador.h"
#include "inimigo.h"
#include "bomba.h"
#include <stdio.h>

int main(void) {
    InitWindow(1200, 600, "Começando o jogo");
    SetTargetFPS(60);  

    //Inicializa o mapa
    char **mapa = inicializarMapa("mapas/mapaB.txt");
    if (!mapa) {
        fprintf(stderr, "Falha ao carregar mapa!\n");
        CloseWindow();
        return 1;
    }

    //Inicializa os Inimigos
    tipoInimigo inimigos[MAX_INIMIGOS];
    inicializaInimigos(inimigos, mapa);

    //Inicializa o Jogador
    tipoJogador jogador;
    inicializarJogador(&jogador);

    //Inicializa as Bombas
    tipoBomba bombas[MAX_BOMBAS];
    inicializarBombas(bombas);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
    

        // Desenha o mapa
        for (int linha = 0; linha < LINHAS; linha++) {
            for (int col = 0; col < COLUNAS; col++) {
                char bloco = mapa[linha][col];
                int x = col * 20;
                int y = linha * 20;

                switch (bloco) {
                    case 'W': //parede indestrutivel
                        DrawRectangle(x, y, 20, 20, DARKGRAY);
                        break;
                    case 'D': // parede destruivel
                        DrawRectangle(x, y, 20, 20, GRAY);
                        break;
                    case 'K': // caixa com chave
                    case 'B': //caixa sem chave
                        DrawRectangle(x, y, 20, 20, BROWN);
                        break;
                    case 'E': // inimigo
                        DrawRectangle(x, y, 20, 20, RED);
                        break;
                    case 'J': //jogador
                        DrawRectangle(x, y, 20, 20, LIGHTGRAY);
                        break;
                    case ' ':
                        DrawRectangle(x, y, 20, 20, LIGHTGRAY);
                        break;
                }
            }
        }

        //desenha o jogador a partir da struct
        desenharJogador(jogador);

        //desenha os inimigos a partir da struct tambem
        desenhaInimigos(inimigos);

        //movitacao do jogador
        if (IsKeyPressed(KEY_UP)) {
            int linhaDeCima = jogador.posicao.linha -1;
            int colunaAtual = jogador.posicao.coluna;
            char proxPosicao = mapa[linhaDeCima][colunaAtual];
            if(proxPosicao == ' ' || proxPosicao == 'J' ){
                jogador.posicao.linha--;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            int linhaDeBaixo = jogador.posicao.linha +1;
            int colunaAtual = jogador.posicao.coluna;
            char proxPosicao = mapa[linhaDeBaixo][colunaAtual];
            if(proxPosicao == ' '){
                jogador.posicao.linha++;
            }
        }          
        if (IsKeyPressed(KEY_LEFT)) {
            int colunaDaEsquerda = jogador.posicao.coluna -1;
            int linhaAtual = jogador.posicao.linha;
            char proxPosicao = mapa[linhaAtual][colunaDaEsquerda];
            if(proxPosicao == ' ' || proxPosicao == 'J'){
                jogador.posicao.coluna--;
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            int colunaDaDireita = jogador.posicao.coluna +1;
            int linhaAtual = jogador.posicao.linha;
            char proxPosicao =  mapa[linhaAtual][colunaDaDireita];
            if(proxPosicao == ' '){
                jogador.posicao.coluna++;
            }
        }

        // logica das bombas
        float intervalo = GetFrameTime();
        atualizarBombas(bombas, intervalo);
        if (IsKeyPressed(KEY_B)) {
            if (plantarBombas(bombas, jogador.posicao, jogador.bombas)) {
                jogador.bombas--; // desconta do arsenal
            }
        }

        desenharBombas(bombas);

        ////////////Desenho do painel na parte inferior
        DrawRectangle(0, LINHAS * 20, COLUNAS * 20, 100, BLACK);

        char texto[128];
        int vidas = 3;
        int pontuacao = 150;
        int bombas = 5;

        sprintf(texto, "Vidas: %d   Pontos: %d   Bombas: %d", vidas, pontuacao, bombas);
        DrawText(texto, 10, LINHAS * 20 + 30, 20, WHITE);
        ///////////

        EndDrawing();
    }

    
    finalizarMapa(mapa);
    CloseWindow();

    return 0;
}




