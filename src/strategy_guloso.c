#include "../include/strategy_guloso.h"

#include <stdlib.h>

#define NAO_VISITADO -1

/*
  Calcula a eficiência de visitar um povo vizinho a partir do povo atual.
  Eficiência = (habilidade / peso) / distância
  A ideia é priorizar visitas com maior retorno de habilidade por unidade de
  peso e distância.
*/
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

/*
  Ordena os povos em ordem decrescente de eficiência de visita.
  Estratégia gulosa: prioriza os povos mais "valiosos" para recrutar soldados.
  Complexidade: O(n log n)
*/
void quickSort(int *V, int inicio, int fim, MundoZambis *mundo, int povo_u) {
  int pivo;
  if (fim > inicio) {
    pivo = particiona(V, inicio, fim, mundo, povo_u);
    quickSort(V, inicio, pivo - 1, mundo, povo_u);
    quickSort(V, pivo + 1, fim, mundo, povo_u);
  }
}

/*
  Estratégia gulosa:
  A cada passo, recruta o máximo possível de soldados do povo atual,tenta
  visitar vizinhos mais eficientes enquanto houver peso e distância disponíveis.
*/
CaminhoSolucao *max_habilidade_guloso_por_povo(int povo_inicio,
                                               MundoZambis *mundo) {
  int povo_atual = povo_inicio, habilidade_atual = 0,
      qtd_povos = mundo->qtd_povos;
  int distancia_atual = mundo->nave.distancia_max;
  int peso_restante = mundo->nave.peso_max;

  int soldadosPorPovo[qtd_povos + 1];
  int ordemVisitados[qtd_povos + 1];

  for (int i = 1; i <= qtd_povos; i++) {
    soldadosPorPovo[i] = NAO_VISITADO;
  }

  int visitados = 0;

  while (peso_restante > 0 && distancia_atual > 0) {
    if (soldadosPorPovo[povo_atual] == NAO_VISITADO) {
      soldadosPorPovo[povo_atual] = 0;
      ordemVisitados[visitados] = povo_atual;
      visitados++;
    }

    int peso_povo_atual = get_peso_por_povo(mundo, povo_atual);

    // Pegar o máximo de soldados possível do povo atual
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

    // Ordena todos os povos vizinhos do povo_atual por odem de eficiência
    quickSort(vizinhos, 0, qtd_vizinhos - 1, mundo, povo_atual);

    int povo_anterior = povo_atual;

    if (distancia_atual == 0) {
      break;
    }

    for (int i = 0; i < qtd_vizinhos; i++) {
      int vizinho = vizinhos[i];

      int distancia_vizinho =
          get_distancia_entre_povos(mundo, povo_atual, vizinho);

      int peso_por_povo_viz = get_peso_por_povo(mundo, vizinho);

      if (peso_por_povo_viz <= peso_restante &&
          distancia_atual >= distancia_vizinho) {
        povo_atual = vizinho;
        distancia_atual -= distancia_vizinho;
        break;
      }
    }

    // Caso não tenha sido possível viajar para nenhum outro local
    if (povo_anterior == povo_atual) {
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

/*
  Executa a estratégia gulosa a partir de cada povo como ponto inicial.
  Retorna o caminho com maior habilidade total recrutada.
*/
CaminhoSolucao *get_caminho_max_habilidade(MundoZambis *mundo) {
  int qtd_povos = mundo->qtd_povos, peso_max = mundo->nave.peso_max,
      distancia_max = mundo->nave.distancia_max;

  CaminhoSolucao **caminhos =
      (CaminhoSolucao **)calloc((qtd_povos + 1), sizeof(CaminhoSolucao *));

  if (caminhos == NULL) {
    perror("Erro ao alocar memória\n");
    exit(1);
  }

  // Para marcar o caminho realizado mais eficiente e de qual povo se partiu
  int max_total = 0, povo_inicio;

  // Para cada povo do mundo de zambis executa a estratégia gulosa
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