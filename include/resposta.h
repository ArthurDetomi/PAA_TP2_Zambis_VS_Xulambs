#ifndef RESPOSTA_H
#define RESPOSTA_H

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

#endif