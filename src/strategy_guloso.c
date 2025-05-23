#include "../include/strategy_guloso.h"

#include <stdlib.h>

double eficiencia_visita(int povo_origem, int povo_vizinho,
                         MundoZambis *mundo) {
  int habilidade = get_habilidade_por_povo(mundo, povo_vizinho);
  int peso = get_peso_por_povo(mundo, povo_vizinho);
  int distancia = get_distancia_entre_povos(mundo, povo_origem, povo_vizinho);

  return ((double)habilidade / (double)peso) / (double)distancia;
}

int particiona(int *V, int inicio, int fim, MundoZambis *mundo,
               int povo_origem) {
  int esq = inicio, dir = fim;
  int pivo_id = V[inicio];
  double eficiencia_pivo = eficiencia_visita(povo_origem, pivo_id, mundo);

  while (esq < dir) {
    while (esq <= fim &&
           eficiencia_visita(povo_origem, V[esq], mundo) >= eficiencia_pivo)
      esq++;
    while (dir >= inicio &&
           eficiencia_visita(povo_origem, V[dir], mundo) < eficiencia_pivo)
      dir--;

    if (esq < dir) {
      int aux = V[esq];
      V[esq] = V[dir];
      V[dir] = aux;
    }
  }

  V[inicio] = V[dir];
  V[dir] = pivo_id;
  return dir;
}

void quickSort(int *V, int inicio, int fim, MundoZambis *mundo, int povo_u) {
  int pivo;
  if (fim > inicio) {
    pivo = particiona(V, inicio, fim, mundo, povo_u);
    quickSort(V, inicio, pivo - 1, mundo, povo_u);
    quickSort(V, pivo + 1, fim, mundo, povo_u);
  }
}

CaminhoSolucao *max_habilidade_guloso_por_povo(int povo_inicio,
                                               MundoZambis *mundo) {
  int povo_atual = povo_inicio, habilidade_atual = 0,
      qtd_povos = mundo->qtd_povos;
  int distancia_atual = mundo->nave.distancia_max;
  int peso_restante = mundo->nave.peso_max;

  int soldadosPorPovo[qtd_povos + 1];
  int ordemVisitados[qtd_povos + 1];

  // Inicializa com -1 para marcar não foi visitado
  for (int i = 1; i <= qtd_povos; i++) {
    soldadosPorPovo[i] = -1;
  }

  int visitados = 0;

  while (true) {
    if (soldadosPorPovo[povo_atual] == -1) {
      soldadosPorPovo[povo_atual] = 0;
      ordemVisitados[visitados] = povo_atual;
      visitados++;
    }

    int peso_povo_atual = get_peso_por_povo(mundo, povo_atual);

    if (peso_restante >= peso_povo_atual) {
      int qtd_soldados_recrutados = peso_restante / peso_povo_atual;

      soldadosPorPovo[povo_atual] += qtd_soldados_recrutados;

      peso_restante = peso_restante % peso_povo_atual;

      int habilidade_povo_atual = get_habilidade_por_povo(mundo, povo_atual);

      habilidade_atual += (qtd_soldados_recrutados * habilidade_povo_atual);
    }

    if (peso_restante == 0) {
      break;
    }

    int vizinhos[qtd_povos + 1];

    int qtd_vizinhos = 0;

    for (int povo_v = 1; povo_v <= qtd_povos; povo_v++) {
      if (!sao_povos_vizinhos(mundo, povo_atual, povo_v)) {
        continue;
      }

      vizinhos[qtd_vizinhos] = povo_v;

      qtd_vizinhos++;
    }

    quickSort(vizinhos, 0, qtd_vizinhos - 1, mundo, povo_atual);

    int povo_antes_visita = povo_atual;

    if (distancia_atual == 0) {
      break;
    }

    for (int i = 0; i < qtd_vizinhos; i++) {
      int vizinho = vizinhos[i];

      int custo = get_distancia_entre_povos(mundo, povo_atual, vizinho);

      if (vizinho != povo_atual &&
          get_peso_por_povo(mundo, vizinho) <= peso_restante &&
          distancia_atual >= custo) {
        povo_atual = vizinho;
        distancia_atual -= custo;
        break;
      }
    }

    if (povo_antes_visita == povo_atual) {
      break;
    }
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
  caminho_solucao->maxHabilidade = habilidade_atual;

  return caminho_solucao;
}

CaminhoSolucao *get_caminho_max_habilidade(MundoZambis *mundo) {
  int qtd_povos = mundo->qtd_povos, peso_max = mundo->nave.peso_max,
      distancia_max = mundo->nave.distancia_max;

  CaminhoSolucao **caminhos =
      (CaminhoSolucao **)calloc((qtd_povos + 1), sizeof(CaminhoSolucao *));

  if (caminhos == NULL) {
    exit(1);
  }

  int max_total = 0, povo_inicio;

  for (int povo_i = 1; povo_i <= qtd_povos; povo_i++) {
    CaminhoSolucao *caminho_solucao =
        max_habilidade_guloso_por_povo(povo_i, mundo);

    caminhos[povo_i] = caminho_solucao;

    if (caminho_solucao != NULL && caminho_solucao->maxHabilidade > max_total) {
      max_total = caminho_solucao->maxHabilidade;
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