#ifndef MUNDO_ZAMBIS_H
#define MUNDO_ZAMBIS_H

#include <stdbool.h>

#define FLAG_SEM_CAMINHO -1

typedef struct NaveZorc {
  int distancia_max, peso_max;
} NaveZorc;

typedef struct MundoZambis {
  int **grafo;
  int *povo, qtd_povos, qtd_caminhos;
  int *habilidade_por_povo, *pesos_por_povo;
  NaveZorc nave;
} MundoZambis;

MundoZambis *criar_mundo_zambis(int qtd_povos, int distancia_max, int peso_max,
                                int qtd_caminhos);

int get_peso_por_povo(MundoZambis *mundo, int povo);

int get_habilidade_por_povo(MundoZambis *mundo, int povo);

bool sao_povos_vizinhos(MundoZambis *mundo, int povo_u, int povo_v);

int get_distancia_entre_povos(MundoZambis *mundo, int povo_u, int povo_v);

// Desaloca memória do TAD mundo de zambis
void destruir_mundo_zambis(MundoZambis *mundo);

#include "mundo_zambis.h"
#include <stdio.h>

void printar_mundo_zambis(MundoZambis *mundo);

#endif