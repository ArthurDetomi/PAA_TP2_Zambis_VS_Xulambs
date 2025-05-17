#include <stdio.h>

#define MAX 10001

#define max(x, y) x > y ? x : y

int p, d, w, c, k;

int grafo[MAX][MAX];

int habilidades[MAX], pesos[MAX];

int dp(int i, int w, int d) { return 1; }

int calculate_max_habilidade() {
  int max_total = 0;

  for (int i = 1; i <= p; i++) {
    int max_per_path = dp(i, w, d);
    max_total = max(max_total, max_per_path);
  }

  return max_total;
}

int main() {
  scanf("%d", &k);

  while (k--) {
    scanf("%d %d %d %d", &p, &d, &w, &c);

    for (int i = 0; i <= p; i++) {
      for (int j = 0; j <= p; j++) {
        grafo[i][j] = -1;
      }
    }

    for (int i = 1; i <= p; i++) {
      int aux;

      scanf("%d %d %d", &aux, &pesos[i], &habilidades[i]);
    }

    for (int i = 0; i < c; i++) {
      int pi, pj, di;
      scanf("%d %d %d", &pi, &pj, &di);

      grafo[pi][pj] = di;
    }

    int maximo_habilidade = calculate_max_habilidade();

    printf("%d\n", maximo_habilidade);
  }

  return 0;
}