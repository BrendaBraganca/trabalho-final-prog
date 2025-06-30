#define MAPA_H

//tamanhos especificados no roteiro do trabalho
#define LINHAS 25
#define COLUNAS 60

char **inicializarMapa(const char *path);
void finalizarMapa(char **mapa);

