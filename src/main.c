#include <limits.h>
#include <stdio.h>

#define MAX 101

#define max(x, y) x > y ? x : y

int p, d, w, c, k;

int grafo[MAX][MAX];

int memo[MAX][MAX][MAX];

int habilidades[MAX], pesos[MAX];

int dp(int i, int w, int d) {
  if (w < 0 || d < 0) {
    return INT_MIN;
  }

  if (memo[i][w][d] != -1) {
    return memo[i][w][d];
  }

  memo[i][w][d] = 0;

  if (w >= pesos[i]) {
    memo[i][w][d] = max(memo[i][w][d], habilidades[i] + dp(i, w - pesos[i], d));
  }

  for (int j = 1; j <= p; j++) {
    if (grafo[i][j] == -1) {
      continue;
    }

    int custo = grafo[i][j];

    if (d >= custo) {
      memo[i][w][d] = max(memo[i][w][d], dp(j, w, d - custo));
    }
  }

  return memo[i][w][d];
}

int dp_bup(int start, int maxW, int maxD, int maxP) {
  memo[start][0][0] = 0;

  for (int w = 0; w <= maxW; w++) {
    for (int d = 0; d <= maxD; d++) {
      for (int p = start; p <= maxP; p++) {
        if (memo[p][w][d] < 0)
          continue;

        if (w + pesos[p] <= maxW) {
          memo[p][w + pesos[p]][d] =
              max(memo[p][w + pesos[p]][d], memo[p][w][d] + habilidades[p]);
        }

        for (int v = 1; v <= maxP; v++) {
          if (grafo[p][v] == -1)
            continue;

          int custo = grafo[p][v];

          if (d + custo <= maxD) {
            memo[v][w][d + custo] = max(memo[v][w][d + custo], memo[p][w][d]);
          }
        }
      }
    }
  }

  int maximo = 0;

  for (int i = 1; i <= p; i++) {
    for (int j = 0; j <= w; j++) {
      for (int k = 0; k <= d; k++) {

        maximo = max(maximo, memo[i][j][k]);
      }
    }
  }

  return maximo;
}

int calculate_max_habilidade() {
  int max_total = 0;

  for (int i = 1; i <= p; i++) {

    for (int i = 0; i <= p; i++) {
      for (int j = 0; j <= w; j++) {
        for (int k = 0; k <= d; k++) {
          memo[i][j][k] = INT_MIN;
        }
      }
    }
    // int max_per_path = dp(i, w, d);
    int max_per_path = dp_bup(i, w, d, p);

    for (int i = 1; i <= p; i++) {
      for (int j = 0; j <= w; j++) {
        for (int k = 0; k <= d; k++) {
          memo[i][j][k] = -1;
        }
      }
    }

    int aux = dp(i, w, d);

    printf("rec = %d, int = %d\n", aux, max_per_path);

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

    /*
    for (int i = 0; i <= p; i++) {
      for (int j = 0; j <= w; j++) {
        for (int k = 0; k <= d; k++) {
          memo[i][j][k] = -1;
        }
      }
    }
    */

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