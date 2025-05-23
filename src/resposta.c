#include "../include/resposta.h"

#include <stdlib.h>

CaminhoSolucao *alocar_caminho_solucao(int tamanho) {
  CaminhoSolucao *caminho = (CaminhoSolucao *)malloc(sizeof(CaminhoSolucao));

  if (caminho == NULL) {
    return NULL;
  }

  caminho->recrutamentos =
      (Recrutamento *)malloc((tamanho + 1) * sizeof(Recrutamento));

  if (caminho->recrutamentos == NULL) {
    destruir_caminho_solucao(&caminho);
    return NULL;
  }

  return caminho;
}

void destruir_caminho_solucao(CaminhoSolucao **caminho) {
  if (caminho == NULL || *caminho == NULL)
    return;

  free((*caminho)->recrutamentos);
  (*caminho)->recrutamentos = NULL;

  free(*caminho);
  *caminho = NULL;
}

void imprimir_caminho_solucao_terminal(CaminhoSolucao *caminho_solucao) {
  printf("Máximo de habilidade = %d\n", caminho_solucao->maxHabilidade);
  printf("Caminho percorrido:\n");
  for (int i = 0; i < caminho_solucao->qtd_visitados; i++) {
    printf("Povo %d: Quantidade de soldados recrutados = %d\n",
           caminho_solucao->recrutamentos[i].povo,
           caminho_solucao->recrutamentos[i].qtdSoldados);
  }
}

void escrever_caminho_solucao_arquivo(CaminhoSolucao *caminho_solucao,
                                      FILE *output_fp) {
  fprintf(output_fp, "%d ", caminho_solucao->maxHabilidade);
  for (int i = 0; i < caminho_solucao->qtd_visitados; i++) {
    fprintf(output_fp, "%d %d", caminho_solucao->recrutamentos[i].povo,
            caminho_solucao->recrutamentos[i].qtdSoldados);

    if (i != caminho_solucao->qtd_visitados - 1) {
      fprintf(output_fp, " ");
    }
  }
  fprintf(output_fp, "\n");
}
