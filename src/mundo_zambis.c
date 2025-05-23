#include "../include/mundo_zambis.h"

#include <stdlib.h>

// Desaloca memória do TAD mundo de zambis
void destruir_mundo_zambis(MundoZambis *mundo) {
  free(mundo->habilidade_por_povo);
  mundo->habilidade_por_povo = NULL;
  free(mundo->pesos_por_povo);
  mundo->pesos_por_povo = NULL;

  if (mundo->grafo != NULL) {
    int tamanho = mundo->qtd_povos + 1;

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
MundoZambis *criar_mundo_zambis(int qtd_povos, int distancia_max, int peso_max,
                                int qtd_caminhos) {
  MundoZambis *mundo = (MundoZambis *)malloc(sizeof(MundoZambis));

  mundo->qtd_caminhos = qtd_caminhos;
  mundo->qtd_povos = qtd_povos;
  mundo->nave.distancia_max = distancia_max;
  mundo->nave.peso_max = peso_max;

  int qtd_povosMax = qtd_povos + 1;

  mundo->habilidade_por_povo = (int *)malloc(qtd_povosMax * sizeof(int));

  if (mundo->habilidade_por_povo == NULL) {
    destruir_mundo_zambis(mundo);
    return NULL;
  }

  mundo->pesos_por_povo = (int *)malloc(qtd_povosMax * sizeof(int));

  if (mundo->pesos_por_povo == NULL) {
    destruir_mundo_zambis(mundo);
    return NULL;
  }

  mundo->grafo = (int **)malloc(qtd_povosMax * sizeof(int *));

  if (mundo->grafo == NULL) {
    destruir_mundo_zambis(mundo);
    return NULL;
  }

  for (int i = 0; i < qtd_povosMax; i++) {
    mundo->grafo[i] = (int *)malloc(qtd_povosMax * sizeof(int));

    if (mundo->grafo[i] == NULL) {
      destruir_mundo_zambis(mundo);
      return NULL;
    }
  }

  // Inicializa o grafo como não tendo caminho em nenhum ponto
  for (int i = 0; i <= qtd_povos; i++) {
    for (int j = 0; j <= qtd_povos; j++) {
      mundo->grafo[i][j] = FLAG_SEM_CAMINHO;
    }
  }

  return mundo;
}

int get_peso_por_povo(MundoZambis *mundo, int povo) {
  return mundo->pesos_por_povo[povo];
}

int get_habilidade_por_povo(MundoZambis *mundo, int povo) {
  return mundo->habilidade_por_povo[povo];
}

bool sao_povos_vizinhos(MundoZambis *mundo, int povo_u, int povo_v) {
  if (povo_u == povo_v) {
    return false;
  }

  return mundo->grafo[povo_u][povo_v] != FLAG_SEM_CAMINHO;
}

int get_distancia_entre_povos(MundoZambis *mundo, int povo_u, int povo_v) {
  return mundo->grafo[povo_u][povo_v];
}
