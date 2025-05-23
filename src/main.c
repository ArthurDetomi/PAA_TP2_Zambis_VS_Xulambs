#include <limits.h>
#include <stdio.h>

#include "../include/entrada.h"
#include "../include/mundo_zambis.h"
#include "../include/resposta.h"
#include "../include/strategy_guloso.h"
#include "../include/strategydp.h"
#include "../include/tempo.h"

#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Verifica se os argumentos de linha de comando são válidos
  if (!is_argumentos_validos(argc, argv)) {
    printf("Erro: Parâmetros inválidos.\n");
    printf("Uso correto: ./tp1 {estrateǵia} -i input/in.txt -o saida.txt\n");
    printf("Onde '-i' indica o arquivo de entrada.\n");
    printf("Onde '-o' indica o arquivo de saída (Opcional)\n");
    printf("Estratégia aceita somente 1(Programação Dinâmica) e 2(Guloso)\n");

    return 1;
  }

  // Obtém o caminho do arquivo de entrada a partir dos argumentos
  char *input_path = argv[ARQUIVO_ENTRADA_P];

  // Tenta abrir o arquivo de entrada no modo leitura
  FILE *input_fp = fopen(input_path, "r");

  // Verifica se o arquivo foi aberto com sucesso
  if (input_fp == NULL) {
    perror("Erro ao abrir arquivo de entrada\n");
    return 1;
  }

  // Inicia o temporizador para medir o tempo total de execução
  Temporizador tempo_total;
  iniciarTemporizador(&tempo_total);

  // Obtém o caminho de arquivo de saída através dos argumentos
  char output_path[80];
  get_output_path(argc, argv, output_path);

  // Tenta criar um arquivo para salvar a saída principal
  FILE *output_fp = fopen(output_path, "w");

  if (output_fp == NULL) {
    perror("Erro ao abrir arquivo de saida\n");
    return 1;
  }

  int estrategia_escolhida = atoi(argv[ESTRATEGIA_P]);

  int qtdTestes;

  // Lê a quantidade de testes que serão realizados
  fscanf(input_fp, "%d", &qtdTestes);

  int num_teste = 1;

  while (qtdTestes--) {
    int num_povos, distancia_max, peso_max, qtd_caminhos;

    fscanf(input_fp, "%d %d %d %d", &num_povos, &distancia_max, &peso_max,
           &qtd_caminhos);

    // Cria um mundo de zambis com as espeficições fornecidas
    MundoZambis *mundo_zambis =
        criar_mundo_zambis(num_povos, distancia_max, peso_max, qtd_caminhos);

    if (mundo_zambis == NULL) {
      perror("Falha ao alocar memória\n");
      fclose(input_fp);
      fclose(output_fp);
      exit(1);
    }

    // Carrega o mundo de zambis com as espeficações do arquivo de input
    carregar_mundo_zambis_arquivo(mundo_zambis, input_fp);

    // Inicia o temporizador para medir o tempo deste teste específico
    Temporizador tempo_teste;
    iniciarTemporizador(&tempo_teste);

    CaminhoSolucao *caminho_solucao = NULL;

    // Executa a estratégia escolhida
    switch (estrategia_escolhida) {
    case DP:
      caminho_solucao = get_max_habilidade_path(mundo_zambis);
      break;
    case GULOSO:
      caminho_solucao = get_caminho_max_habilidade(mundo_zambis);
      break;
    }

    // Finaliza a medição de tempo para este teste
    finalizarTemporizador(&tempo_teste);

    if (caminho_solucao == NULL) {
      perror("Falha ao alocar memória\n");
      destruir_mundo_zambis(mundo_zambis);
      fclose(input_fp);
      fclose(output_fp);
      exit(1);
    }

    // Escreve no terminal o caminho solução
    printf("\tTeste %d\n", num_teste);
    imprimir_caminho_solucao_terminal(caminho_solucao);
    imprimirTempos(&tempo_teste);

    // Escreve no arquivo de saída a solução no formato esperado
    escrever_caminho_solucao_arquivo(caminho_solucao, output_fp);

    // Liberando memória da heap utilizada
    destruir_caminho_solucao(&caminho_solucao);
    destruir_mundo_zambis(mundo_zambis);

    num_teste++;
    printf("\n");
  }

  // Finaliza a medição do tempo total de execução
  finalizarTemporizador(&tempo_total);

  // Exibe o tempo total de execução do programa
  imprimirTempos(&tempo_total);

  // Fecha todos os arquivos
  fclose(input_fp);
  fclose(output_fp);

  return 0;
}