#include "../include/entrada.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregar_mundo_zambis_arquivo(MundoZambis *mundo, FILE *input_fp) {
  int qtd_povos = mundo->qtd_povos;

  // Lendo as habilidade e peso de cada povo
  for (int i = 1; i <= qtd_povos; i++) {
    int povo, habilidade, peso;

    fscanf(input_fp, "%d %d %d", &povo, &peso, &habilidade);

    mundo->pesos_por_povo[povo] = peso;
    mundo->habilidade_por_povo[povo] = habilidade;
  }

  int qtd_caminhos = mundo->qtd_caminhos;

  // Preenche os caminhos existentes para essa configuração de mundo de Zambis
  for (int i = 0; i < qtd_caminhos; i++) {
    int povo_u, povo_v, distancia;

    fscanf(input_fp, "%d %d %d", &povo_u, &povo_v, &distancia);

    mundo->grafo[povo_u][povo_v] = distancia;
  }
}

bool is_argumentos_validos(int argc, char *argv[]) {
  // A entrada do programa deve possuir 3 argumentos ou 5 argumentos
  if (argc != NUM_MIN_ARGS_ESPERADOS && argc != NUM_MAX_ARGS_ESPERADOS) {
    return false;
  }

  int estrategia = (int)atoi(argv[ESTRATEGIA_P]);

  if (estrategia != DP && estrategia != GULOSO) {
    return false;
  }

  // Se não possui a flag -i como argumento considera-se invalido
  if (strcmp(argv[FLAG_INPUT_P], FLAG_INPUT) != 0) {
    return false;
  }

  /*
    Se possui o número minimo de argumentos a validação já feita é suficiente
    portanto é válido os argumentos
  */
  if (argc == NUM_MIN_ARGS_ESPERADOS) {
    return true;
  }

  // Caso tenha 5 argumentos o 4 argumento obrigatoriamente deve ser o -o
  return strcmp(argv[FLAG_OUTPUT_P], FLAG_OUTPUT) == 0;
}

void *get_output_path(int argc, char *argv[], char *output_path) {
  strcpy(output_path, "output/");
  strcat(output_path, (NUM_MAX_ARGS_ESPERADOS == argc) ? argv[ARQUIVO_SAIDA_P]
                                                       : "output.txt");

  return output_path;
}

void printar_mundo_zambis(MundoZambis *mundo) {
  printf("=== MUNDO ZAMBIS ===\n");

  printf("Quantidade de povos: %d\n", mundo->qtd_povos);
  printf("Distância máxima da nave: %d\n", mundo->nave.distancia_max);
  printf("Peso máximo da nave: %d\n", mundo->nave.peso_max);
  printf("Quantidade de caminhos: %d\n\n", mundo->qtd_caminhos);

  printf("== Povos ==\n");
  for (int i = 1; i <= mundo->qtd_povos; i++) {
    printf("Povo %d: Habilidade = %d, Peso = %d\n", i,
           mundo->habilidade_por_povo[i], mundo->pesos_por_povo[i]);
  }

  printf("\n== Grafo de distâncias ==\n");
  for (int i = 1; i <= mundo->qtd_povos; i++) {
    for (int j = 1; j <= mundo->qtd_povos; j++) {
      int d = mundo->grafo[i][j];
      if (d == FLAG_SEM_CAMINHO)
        printf("  - ");
      else
        printf("%3d ", d);
    }
    printf(" <- povo %d\n", i);
  }

  printf("======================\n");
}