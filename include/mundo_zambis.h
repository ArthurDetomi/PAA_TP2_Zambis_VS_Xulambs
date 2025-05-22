#ifndef MUNDO_ZAMBIS_H
#define MUNDO_ZAMBIS_H

#include <stdbool.h>

typedef struct NaveZorc {
  int maxDist, maxPeso;
} NaveZorc;

typedef struct MundoZambis {
  int **grafo;
  int *povo, qtdPovos;
  int *habilidadesPorPovo, *pesosPorPovo;
  NaveZorc nave;
} MundoZambis;

MundoZambis *criar_mundo_zambis(int qtdPovos, int maxDist, int maxPeso);

int get_peso_por_povo(MundoZambis *mundo, int povo);

int get_habilidade_por_povo(MundoZambis *mundo, int povo);

bool sao_povos_vizinhos(MundoZambis *mundo, int povoU, int povoV);

void destruir_mundo_zambis(MundoZambis *mundo);

#endif