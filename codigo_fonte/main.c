#include "raylib.h"
#include "mapa.h"
#include "jogador.h"
#include "inimigo.h"
#include "bomba.h"
#include "save.h"
#include <stdio.h>
#include <stdlib.h>

void reiniciaJogo(char ***mapa, tipoJogador *jogador, tipoBomba bombas[], tipoInimigo inimigos[], int *gameOver){
    finalizarMapa(*mapa);
    *mapa = inicializarMapa("mapas/mapaB.txt");
    jogador->vidas = 3;
    jogador->pontuacao = 0;
    jogador->bombas = 3;
    jogador->chaves = 0;
    jogador->posicao.linha = 1;
    jogador->posicao.coluna = 1;
    inicializarBombas(bombas);
    inicializaInimigos(inimigos, *mapa);
    *gameOver = 0;
}

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

    //Logica de nivel
    int nivelCompleto = 0;
    double tempoAlertaNivel = 0;

    //Logica game over
    int gameOver = 0;
    double tempoGameOver = 0;

    //Logica do menu
    int menuTAB = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if(!gameOver){
            //inimigo comeca a se movimentar
            float intervaloInimigo = GetFrameTime();
            tempoPosicaoInimigo = tempoPosicaoInimigo + intervaloInimigo;
            if(tempoPosicaoInimigo >= 0.5f){
                movimentarInimigos(inimigos, mapa);
                tempoPosicaoInimigo = 0;
            }

            // verifica se inimigo tocou no jogador
            for(int i = 0; i < MAX_INIMIGOS; i++){
                if(inimigos[i].posicao.linha == jogador.posicao.linha && inimigos[i].posicao.coluna == jogador.posicao.coluna){

                    jogador.vidas--;
                    jogador.pontuacao = MIN(abs(jogador.pontuacao - 100), 0);
                    jogador.posicao.linha = 1;
                    jogador.posicao.coluna = 1;
                    break; // se dois inimigos tiverem juntos e matarem o jogador, ele so perde uma vida
                }
            }    

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
                            DrawRectangle(x, y, 20, 20, BROWN);
                            break;
                        case 'K': // caixa com chave
                            DrawRectangle(x, y, 20, 20, PURPLE); 
                            break;
                        case 'B': //caixa sem chave
                            DrawRectangle(x, y, 20, 20, PURPLE);
                            break;
                        case ' ':
                            DrawRectangle(x, y, 20, 20, LIGHTGRAY); //LIGHTGRAY
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

            //implementa menu
            if(IsKeyPressed(KEY_TAB)){
                menuTAB = !menuTAB; //ativa e desativa menu
            }

            if(menuTAB){
                const char *acoes[] = {
                    "N -> Novo Jogo",
                    "C -> Carregar Jogo",
                    "S -> Salvar Jogo",
                    "Q -> Sair do Jogo",
                    "V -> Voltar ao Jogo"
                };

                for(int i = 0; i < 5; i++){
                    DrawText(acoes[i], 400, 100 + i * 30, 20, BLACK);
                }
                if(IsKeyPressed(KEY_N)){
                    reiniciaJogo(&mapa, &jogador, bombas, inimigos, &gameOver);
                    menuTAB = 0;
                }
                if(IsKeyPressed(KEY_C)){
                    if(carregarJogo("bin/preJogado.dat", &jogador, bombas, inimigos, &mapa) == 0){
                        menuTAB = 0;
                    }; // escrever essa funcao
                }
                if(IsKeyPressed(KEY_S)){
                    salvarJogo("bin/jogoSalvo.dat", jogador, bombas, inimigos, mapa);
                    menuTAB = 0;
                }
                if(IsKeyPressed(KEY_Q)){
                    CloseWindow();
                    return 0;
                }
                if(IsKeyPressed(KEY_V)){
                    menuTAB = 0;
                }
                EndDrawing();
                continue; // "pausa" o jogo até sair do menu
            }


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

            //Game Over
            if(jogador.vidas == 0){
                gameOver = 1;
                tempoGameOver = GetTime();
            }


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
        } else {
            const char *msgGameOver = "GAME OVER";
            int tamFonte = 50;
            int larguraTexto = MeasureText(msgGameOver, tamFonte);
            int x = (GetScreenWidth() - larguraTexto)/2;
            int y = (GetScreenHeight() - tamFonte)/2;

            DrawRectangle(x-20, y-20, larguraTexto + 40, tamFonte +40, PURPLE);
            DrawText(msgGameOver, x, y, tamFonte, WHITE);

            if(GetTime() - tempoGameOver >= 2.0){
                reiniciaJogo(&mapa, &jogador, bombas, inimigos, &gameOver);
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




