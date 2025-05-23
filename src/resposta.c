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
