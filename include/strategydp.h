#ifndef STRATEGYDP_H
#define STRATEGYDP_H

#include "../include/mundo_zambis.h"

typedef struct Recrutamento {
  int povo, qtdSoldados;
} Recrutamento;

typedef struct CaminhoSolucao {
  Recrutamento *recrutamentos;
  int maxHabilidade;
} CaminhoSolucao;

CaminhoSolucao get_max_habilidade_path(MundoZambis *mundo);

#endif