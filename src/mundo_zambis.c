#include "../include/mundo_zambis.h"

#include <stdlib.h>

// Desaloca memória do TAD mundo de zambis
void destruir_mundo_zambis(MundoZambis *mundo, ) {
  free(mundo->habilidadesPorPovo);
  mundo->habilidadesPorPovo = NULL;
  free(mundo->pesosPorPovo);
  mundo->pesosPorPovo = NULL;

  if (mundo->grafo != NULL) {
    int tamanho = mundo->qtdPovos + 1;

    for (int i = 0; i < tamanho; i++) {
      free(mundo->grafo[i]);
      mundo->grafo[i] = NULL;
    }

    free(mundo->grafo);
    mundo->grafo = NULL;
  }

  free(mundo);
  mundo = NULL;
}

// Cria um mundo de zambis em caso de falha de alocação de memória retorna NULL
MundoZambis *criar_mundo_zambis(int qtdPovos, int maxDist, int maxPeso) {
  MundoZambis *mundo = (MundoZambis *)malloc(sizeof(MundoZambis));

  mundo->qtdPovos = qtdPovos;
  mundo->nave.maxDist = maxDist;
  mundo->nave.maxPeso = maxPeso;

  int qtdPovosMax = qtdPovos + 1;

  mundo->habilidadesPorPovo = (int *)malloc(qtdPovosMax * sizeof(int));

  if (mundo->habilidadesPorPovo == NULL) {
    destruir_mundo_zambis(mundo);
    return NULL;
  }

  mundo->pesosPorPovo = (int *)malloc(qtdPovosMax * sizeof(int));

  if (mundo->pesosPorPovo == NULL) {
    destruir_mundo_zambis(mundo);
    return NULL;
  }

  mundo->grafo = (int **)malloc(qtdPovosMax * sizeof(int *));

  if (mundo->grafo == NULL) {
    destruir_mundo_zambis(mundo);
    return NULL;
  }

  for (int i = 0; i < qtdPovosMax; i++) {
    mundo->grafo[i] = (int *)malloc(qtdPovosMax * sizeof(int));

    if (mundo->grafo[i] == NULL) {
      destruir_mundo_zambis(mundo);
      return NULL;
    }
  }

  return mundo;
}
