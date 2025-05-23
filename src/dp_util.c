#include "../include/dp_util.h"

#include <stdio.h>
#include <stdlib.h>
void alocar_matriz_tridimensional(int ****matrix, int x, int y, int z) {
  *matrix = malloc(x * sizeof(int **));

  for (int i = 0; i < x; i++) {
    (*matrix)[i] = malloc(y * sizeof(int *));
    for (int j = 0; j < y; j++) {
      (*matrix)[i][j] = malloc(z * sizeof(int));
    }
  }
}

void alocar_matriz_tridimensional_estado(Estado ****matrix, int x, int y,
                                         int z) {
  *matrix = malloc(x * sizeof(Estado **));

  for (int i = 0; i < x; i++) {
    (*matrix)[i] = malloc(y * sizeof(Estado *));
    for (int j = 0; j < y; j++) {
      (*matrix)[i][j] = malloc(z * sizeof(Estado));
    }
  }
}

void liberar_matriz_tridimensional(int ***matrix, int x, int y) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      free(matrix[i][j]);
      matrix[i][j] = NULL;
    }
    free(matrix[i]);
    matrix[i] = NULL;
  }
  free(matrix);
  matrix = NULL;
}

void liberar_matriz_tridimensional_estado(Estado ***matrix, int x, int y) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      free(matrix[i][j]);
      matrix[i][j] = NULL;
    }
    free(matrix[i]);
    matrix[i] = NULL;
  }
  free(matrix);
  matrix = NULL;
}

void liberar_DPDAta(DPData *dp_data, int qtd_povos, int peso_max) {
  liberar_matriz_tridimensional_estado(dp_data->estados, qtd_povos, peso_max);
  liberar_matriz_tridimensional(dp_data->memo, qtd_povos, peso_max);
}

void inicializar_DPDAta(DPData *dp_data, int qtd_povos, int peso_max,
                        int distancia_max) {
  dp_data->peso_max = peso_max;
  dp_data->distancia_max = distancia_max;
  dp_data->qtd_povos = qtd_povos;

  alocar_matriz_tridimensional_estado(&dp_data->estados, qtd_povos + 1,
                                      peso_max + 1, distancia_max + 1);
  alocar_matriz_tridimensional(&dp_data->memo, qtd_povos + 1, peso_max + 1,
                               distancia_max + 1);
}

void resetar_DPData(DPData *dp_data) {
  for (int i = 0; i <= dp_data->qtd_povos; i++) {
    for (int j = 0; j <= dp_data->peso_max; j++) {
      for (int k = 0; k <= dp_data->distancia_max; k++) {
        dp_data->memo[i][j][k] = -1;
        dp_data->estados[i][j][k] = (Estado){-1, -1, -1};
      }
    }
  }
}