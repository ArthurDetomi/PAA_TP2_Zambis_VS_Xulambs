#include "../include/strategydp.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/*
  Calcula o máximo de habilidade que pode ser acumulada partindo de um povo_i
  com uma capacidade restante de peso(peso_w)  e distancia(dist_d)

  Utiliza a abordagem de programação dinâmica top-down(De baixo para cima)
  aplicando memoização para evitar recomputações de estados já calculados.
*/
int calc_maximo_habilidade_dp(int povo_i, int peso_w, int dist_d,
                              DPData *dp_data, MundoZambis *mundo) {
  if (peso_w < 0 || dist_d < 0) {
    return INT_MIN;
  }

  /*
    Verifica se o estado já foi computado, caso já tenha sido evita as chamadas
    recursivas
  */
  if (dp_data->memo[povo_i][peso_w][dist_d] != -1) {
    return dp_data->memo[povo_i][peso_w][dist_d];
  }

  dp_data->memo[povo_i][peso_w][dist_d] = 0;

  int peso = get_peso_por_povo(mundo, povo_i);

  // Tenta recrutar mais soldados no povo atual (sem se mover)
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

  // Tenta se mover para todos os povos vizinhos sem recrutar
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

/*
  Reconstrói o caminho ótimo percorrido na DP a partir do estado final da
  solução.
  A matriz 'estados' armazena, para cada subproblema (povo, peso, distância),
  o estado anterior que levou àquele valor ótimo. Com isso, é possível refazer
  o caminho de decisões tomadas.
  - 'soldadosPorPovo' acumula quantos soldados foram recrutados em cada povo.
  - 'ordemVisitados' armazena a ordem em que os povos foram visitados no
  trajeto.
*/
CaminhoSolucao *reconstruir_caminho_dp(int povo_inicio, DPData *dp_data,
                                       MundoZambis *mundo) {
  int soldadosPorPovo[dp_data->qtd_povos + 1];
  int ordemVisitados[dp_data->qtd_povos + 1];

  // Inicializa com -1 para marcar não foi visitado
  for (int i = 1; i <= dp_data->qtd_povos; i++) {
    soldadosPorPovo[i] = -1;
  }

  soldadosPorPovo[povo_inicio] = 0;

  // A primeira posição sempre vai ser pela qual se partiu
  ordemVisitados[0] = povo_inicio;

  // Visitados pós povo de inicio começara pelo indice 1, 0(É o povo por qual se
  // iniciou)
  int visitados = 1;

  // Estados atual para começar a retroceder nos estados
  int maximo = 0, povo_atual = povo_inicio, peso_atual = dp_data->peso_max,
      distancia_atual = dp_data->distancia_max;

  // Retrocede pelos estados anteriores
  while (dp_data->estados[povo_atual][peso_atual][distancia_atual].prev_povo !=
         -1) {
    Estado prev = dp_data->estados[povo_atual][peso_atual][distancia_atual];

    // Povo foi visitado pela primeira vez
    if (soldadosPorPovo[prev.prev_povo] == -1) {
      soldadosPorPovo[prev.prev_povo] = 0;

      ordemVisitados[visitados] = prev.prev_povo;

      visitados++;
    }

    int diff =
        (dp_data->memo[povo_atual][peso_atual][distancia_atual] -
         dp_data->memo[prev.prev_povo][prev.prev_peso][prev.prev_distancia]);

    // Se a diferença entre o estado posterior e anterior é igual a habilidade
    // do respectivo povo visitado significa que foi recrutado um soldado
    if (diff == get_habilidade_por_povo(mundo, prev.prev_povo)) {
      soldadosPorPovo[prev.prev_povo]++;
    }

    povo_atual = prev.prev_povo;
    peso_atual = prev.prev_peso;
    distancia_atual = prev.prev_distancia;
  }

  CaminhoSolucao *caminho_solucao = alocar_caminho_solucao(visitados + 1);

  // Preenche a ordem de recrutamento para o caminho solução final
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

/*
  Busca pelo melhor caminho de recrutamento, considerando todos os povos como
  ponto de partida

  - Executa a DP partindo de cada povo (povo_i) como início armazenando qual
  atingiu a maior habilidade
  - Após o loop entre cada povo libera a memória dos caminhos que não chegaram
  na solução ótima e retorna apenas o caminho com melhor solução.
 */
CaminhoSolucao *buscar_caminho_max_habilidade(MundoZambis *mundo,
                                              DPData *dp_data) {
  int qtd_povos = mundo->qtd_povos, peso_max = mundo->nave.peso_max,
      distancia_max = mundo->nave.distancia_max;

  int max_total = 0, povo_inicio;

  CaminhoSolucao **caminhos =
      (CaminhoSolucao **)calloc((qtd_povos + 1), sizeof(CaminhoSolucao *));

  if (caminhos == NULL) {
    exit(1);
  }

  for (int povo_i = 1; povo_i <= qtd_povos; povo_i++) {
    // Pre-Processamento: Reseta as matrizes utilizadas pela DP para o caso
    // base.
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

  // Liberando memórias de todos os caminhos, exceto o que atingiu a habilidade
  // máxima
  for (int i = 0; i <= qtd_povos; i++) {
    if (i != povo_inicio && caminhos[i] != NULL) {
      destruir_caminho_solucao(&caminhos[i]);
    }
  }

  CaminhoSolucao *melhor_caminho = caminhos[povo_inicio];

  free(caminhos);

  return melhor_caminho;
}

/*
  Encontra e retorna o caminho ótimo de recrutamento que maximiza a habilidade
  total disponível para a batalha, respeitando os limites de peso e distância da
  nave.

  Inicializa os dados necessários para DP e após encontrar o melhor caminho
  libera a memória utilizada
*/
CaminhoSolucao *get_max_habilidade_path(MundoZambis *mundo) {
  int qtd_povos = mundo->qtd_povos, peso_max = mundo->nave.peso_max,
      distancia_max = mundo->nave.distancia_max;

  DPData dp_data;
  inicializar_DPDAta(&dp_data, qtd_povos, peso_max, distancia_max);

  CaminhoSolucao *caminho_solucao =
      buscar_caminho_max_habilidade(mundo, &dp_data);

  liberar_DPDAta(&dp_data, qtd_povos + 1, peso_max + 1);

  return caminho_solucao;
}