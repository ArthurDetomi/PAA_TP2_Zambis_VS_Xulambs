#ifndef DP_UTIL_H
#define DP_UTIL_H

typedef struct Estado {
  int prev_povo, prev_peso, prev_distancia;
} Estado;

typedef struct {
  int qtd_povos;
  int peso_max;
  int distancia_max;

  int ***memo;
  Estado ***estados;
} DPData;

void alocar_matriz_tridimensional(int ****matrix, int x, int y, int z);

void alocar_matriz_tridimensional_estado(Estado ****matrix, int x, int y,
                                         int z);

void liberar_matriz_tridimensional(int ***matrix, int x, int y);

void liberar_matriz_tridimensional_estado(Estado ***matrix, int x, int y);

void liberar_DPDAta(DPData *dp_data, int qtd_povos, int peso_max);

void inicializar_DPDAta(DPData *dp_data, int qtd_povos, int peso_max,
                        int distancia_max);

void resetar_DPData(DPData *dp_data);

#endif