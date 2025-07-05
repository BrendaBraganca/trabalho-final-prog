#pragma once
#include "jogador.h"
#include "bomba.h"
#include "inimigo.h"

int salvarJogo(const char *arq, tipoJogador jogador, tipoBomba bombas[], tipoInimigo inimigos[], char **mapa);
int carregarJogo(const char *arq, tipoJogador *jogador, tipoBomba bombas[], tipoInimigo inimigos[], char ***mapa);













