#ifndef RESPOSTA_H
#define RESPOSTA_H

#include <stdio.h>

typedef struct Recrutamento {
  int povo, qtdSoldados;
} Recrutamento;

typedef struct CaminhoSolucao {
  Recrutamento *recrutamentos;
  int maxHabilidade;
  int qtd_visitados;
} CaminhoSolucao;

CaminhoSolucao *alocar_caminho_solucao(int tamanho);

void destruir_caminho_solucao(CaminhoSolucao **caminho);

void imprimir_caminho_solucao_terminal(CaminhoSolucao *caminho_solucao);

void escrever_caminho_solucao_arquivo(CaminhoSolucao *caminho_solucao,
                                      FILE *output_fp);

#endif