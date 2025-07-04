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
    float tempoPosicaoInimigo = 0;

    //Inicializa o Jogador
    tipoJogador jogador;
    inicializarJogador(&jogador);

    //Inicializa as Bombas
    tipoBomba bombas[MAX_BOMBAS];
    inicializarBombas(bombas);

    int nivelCompleto = 0;
    double tempoAlertaNivel = 0;

    while (!WindowShouldClose()) {

        float intervaloInimigo = GetFrameTime();
        tempoPosicaoInimigo = tempoPosicaoInimigo + intervaloInimigo;
        if(tempoPosicaoInimigo >= 0.5f){
            movimentarInimigos(inimigos, mapa);
            tempoPosicaoInimigo = 0;
        }

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
                        DrawRectangle(x, y, 20, 20, PURPLE); //depois colocar em marrom dnv
                        break;
                    case 'B': //caixa sem chave
                        DrawRectangle(x, y, 20, 20, BROWN);
                        break;
                    case ' ':
                        DrawRectangle(x, y, 20, 20, LIGHTGRAY);
                        break;
                    case 'J':
                    case 'E':
                        break;
                    case 'C':  // caso a caixa com chave seja explodida
                         DrawRectangle(x, y, 20, 20, YELLOW);
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
            if(proxPosicao == ' ' || proxPosicao == 'J' || proxPosicao == 'C'){
                jogador.posicao.linha--;
                if(proxPosicao == 'C'){
                    jogador.chaves++;
                    mapa[linhaDeCima][colunaAtual] = ' ';
                }
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            int linhaDeBaixo = jogador.posicao.linha +1;
            int colunaAtual = jogador.posicao.coluna;
            char proxPosicao = mapa[linhaDeBaixo][colunaAtual];
            if(proxPosicao == ' ' || proxPosicao == 'C'){
                jogador.posicao.linha++;
                if(proxPosicao == 'C'){
                    jogador.chaves++;
                    mapa[linhaDeBaixo][colunaAtual] = ' ';
                }
            }
        }          
        if (IsKeyPressed(KEY_LEFT)) {
            int colunaDaEsquerda = jogador.posicao.coluna -1;
            int linhaAtual = jogador.posicao.linha;
            char proxPosicao = mapa[linhaAtual][colunaDaEsquerda];
            if(proxPosicao == ' ' || proxPosicao == 'J' || proxPosicao == 'C'){
                jogador.posicao.coluna--;
                if(proxPosicao == 'C'){
                    jogador.chaves++;
                    mapa[linhaAtual][colunaDaEsquerda] = ' ';
                }
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            int colunaDaDireita = jogador.posicao.coluna +1;
            int linhaAtual = jogador.posicao.linha;
            char proxPosicao =  mapa[linhaAtual][colunaDaDireita];
            if(proxPosicao == ' ' || proxPosicao == 'C'){
                jogador.posicao.coluna++;
                if(proxPosicao == 'C'){
                    jogador.chaves++;
                    mapa[linhaAtual][colunaDaDireita] = ' ';
                }
            }
        }

        // logica das bombas
        float intervalo = GetFrameTime();
        atualizarBombas(bombas, intervalo, mapa, inimigos, &jogador);
        if (IsKeyPressed(KEY_B)) {
            if (plantarBombas(bombas, jogador.posicao, jogador.bombas)) {
                jogador.bombas--; // desconta do arsenal
            }
        }

        desenharBombas(bombas);

        // Passando para o proximo nivel
        if(jogador.chaves == 5 && !nivelCompleto){
            nivelCompleto = 1;
            tempoAlertaNivel = GetTime();
        }

        if(nivelCompleto){
            const char *msgNivel = "NOVO NIVEL!!!";
            int tamFonte = 30;
            int larguraTexto = MeasureText(msgNivel, tamFonte);
            int x = (1200 - larguraTexto)/2; //deixar texto no meio
            int y = 250;

            DrawRectangle(x-10, y-10, larguraTexto + 20, tamFonte+20, PURPLE);
            DrawText(msgNivel, x, y, tamFonte, WHITE);
            if(GetTime() - tempoAlertaNivel >= 2.0){
                nivelCompleto = 0;
                jogador.chaves = 0;
                jogador.posicao.linha = 1;
                jogador.posicao.coluna = 1;

                finalizarMapa(mapa);
                mapa = inicializarMapa("mapas/mapaA.txt");
            }
        }

        ////////////Desenho do painel na parte inferior
        DrawRectangle(0, LINHAS * 20, COLUNAS * 20, 100, BLACK);

        char texto[128];

        sprintf(texto, "Vidas: %d   Pontos: %d   Bombas: %d  Chaves: %d", jogador.vidas, jogador.pontuacao, jogador.bombas, jogador.chaves); // tirar a parte das chaves depois
        DrawText(texto, 10, LINHAS * 20 + 30, 20, WHITE);
        ///////////

        EndDrawing();
    }

    
    finalizarMapa(mapa);
    CloseWindow();

    return 0;
}




