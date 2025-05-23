#include "../include/strategydp.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int calc_maximo_habilidade_dp(int povo_i, int peso_w, int dist_d,
                              DPData *dp_data, MundoZambis *mundo) {
  if (peso_w < 0 || dist_d < 0) {
    return INT_MIN;
  }

  if (dp_data->memo[povo_i][peso_w][dist_d] != -1) {
    return dp_data->memo[povo_i][peso_w][dist_d];
  }

  dp_data->memo[povo_i][peso_w][dist_d] = 0;

  int peso = get_peso_por_povo(mundo, povo_i);

  if (peso_w >= peso) {
    int habilidade = get_habilidade_por_povo(mundo, povo_i);

    int novo_valor =
        habilidade + calc_maximo_habilidade_dp(povo_i, peso_w - peso, dist_d,
                                               dp_data, mundo);

    if (novo_valor > dp_data->memo[povo_i][peso_w][dist_d]) {
      dp_data->memo[povo_i][peso_w][dist_d] = novo_valor;

      dp_data->estados[povo_i][peso_w][dist_d] =
          (Estado){povo_i, peso_w - peso, dist_d};
    }
  }

  int max_povos = dp_data->qtd_povos;

  for (int povo_v = 1; povo_v <= max_povos; povo_v++) {
    if (!sao_povos_vizinhos(mundo, povo_i, povo_v)) {
      continue;
    }

    int custo = get_distancia_entre_povos(mundo, povo_i, povo_v);

    if (dist_d >= custo) {
      int novo_valor = calc_maximo_habilidade_dp(povo_v, peso_w, dist_d - custo,
                                                 dp_data, mundo);

      if (novo_valor > dp_data->memo[povo_i][peso_w][dist_d]) {
        dp_data->memo[povo_i][peso_w][dist_d] = novo_valor;

        dp_data->estados[povo_i][peso_w][dist_d] =
            (Estado){povo_v, peso_w, dist_d - custo};
      }
    }
  }

  return dp_data->memo[povo_i][peso_w][dist_d];
}

CaminhoSolucao *reconstruir_caminho_dp(int povo_inicio, DPData *dp_data,
                                       MundoZambis *mundo) {
  int maximo = 0, curP = povo_inicio, curW = dp_data->peso_max,
      curD = dp_data->distancia_max;

  int soldadosPorPovo[dp_data->qtd_povos + 1];
  int ordemVisitados[dp_data->qtd_povos + 1];

  for (int i = 1; i <= dp_data->qtd_povos; i++) {
    soldadosPorPovo[i] = -1;
  }

  soldadosPorPovo[povo_inicio] = 0;
  ordemVisitados[0] = povo_inicio;

  int visitados = 1;

  while (dp_data->estados[curP][curW][curD].prev_povo != -1) {
    Estado prev = dp_data->estados[curP][curW][curD];

    if (soldadosPorPovo[prev.prev_povo] == -1) {
      soldadosPorPovo[prev.prev_povo] = 0;

      ordemVisitados[visitados] = prev.prev_povo;

      visitados++;
    }

    int diff =
        (dp_data->memo[curP][curW][curD] -
         dp_data->memo[prev.prev_povo][prev.prev_peso][prev.prev_distancia]);

    if (diff == get_habilidade_por_povo(mundo, prev.prev_povo)) {
      soldadosPorPovo[prev.prev_povo]++;
    }

    curP = prev.prev_povo;
    curW = prev.prev_peso;
    curD = prev.prev_distancia;
  }

  CaminhoSolucao *caminho_solucao = alocar_caminho_solucao(visitados + 1);

  int index = 0;
  for (int i = 0; i < visitados; i++) {
    int povo_visitado = ordemVisitados[i];

    caminho_solucao->recrutamentos[index] =
        (Recrutamento){povo_visitado, soldadosPorPovo[povo_visitado]};

    index++;
  }

  caminho_solucao->qtd_visitados = visitados;

  return caminho_solucao;
}

CaminhoSolucao *solve(MundoZambis *mundo, DPData *dp_data) {
  int qtd_povos = mundo->qtd_povos, peso_max = mundo->nave.peso_max,
      distancia_max = mundo->nave.distancia_max;

  int max_total = 0, povo_inicio;

  CaminhoSolucao **caminhos =
      (CaminhoSolucao **)calloc((qtd_povos + 1), sizeof(CaminhoSolucao *));

  if (caminhos == NULL) {
    exit(1);
  }

  for (int povo_i = 1; povo_i <= qtd_povos; povo_i++) {
    resetar_DPData(dp_data);

    int max_per_path = calc_maximo_habilidade_dp(povo_i, peso_max,
                                                 distancia_max, dp_data, mundo);

    CaminhoSolucao *caminho_solucao =
        reconstruir_caminho_dp(povo_i, dp_data, mundo);

    caminho_solucao->maxHabilidade = max_per_path;

    caminhos[povo_i] = caminho_solucao;

    if (max_per_path > max_total) {
      max_total = max_per_path;
      povo_inicio = povo_i;
    }
  }

  for (int i = 0; i <= qtd_povos; i++) {
    if (i != povo_inicio && caminhos[i] != NULL) {
      destruir_caminho_solucao(&caminhos[i]);
    }
  }

  CaminhoSolucao *melhor_caminho = caminhos[povo_inicio];

  free(caminhos);

  return melhor_caminho;
}

CaminhoSolucao *get_max_habilidade_path(MundoZambis *mundo) {
  int qtd_povos = mundo->qtd_povos, peso_max = mundo->nave.peso_max,
      distancia_max = mundo->nave.distancia_max;

  DPData dp_data;
  inicializar_DPDAta(&dp_data, qtd_povos, peso_max, distancia_max);

  CaminhoSolucao *caminho_solucao = solve(mundo, &dp_data);

  liberar_DPDAta(&dp_data, qtd_povos + 1, peso_max + 1);

  return caminho_solucao;
}