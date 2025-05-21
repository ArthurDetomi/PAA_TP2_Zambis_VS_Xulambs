#include <limits.h>
#include <stdio.h>

#define MAX 101

#define max(x, y) x > y ? x : y

int p, d, w, c, k;

int grafo[MAX][MAX];

int memo[MAX][MAX][MAX];

int habilidades[MAX], pesos[MAX];

typedef struct Estado {
  int prevP, prevW, prevD;
} Estado;

Estado estados[MAX][MAX][MAX];

int dp(int i, int w, int d) {
  if (w < 0 || d < 0) {
    return INT_MIN;
  }

  if (memo[i][w][d] != -1) {
    return memo[i][w][d];
  }

  memo[i][w][d] = 0;

  if (w >= pesos[i]) {
    int novo_valor = habilidades[i] + dp(i, w - pesos[i], d);

    if (novo_valor > memo[i][w][d]) {
      estados[i][w][d] = (Estado){i, w - pesos[i], d};
      memo[i][w][d] = novo_valor;
    }
  }

  for (int j = 1; j <= p; j++) {
    if (grafo[i][j] == -1) {
      continue;
    }

    int custo = grafo[i][j];

    if (d >= custo) {
      int novo_valor = dp(j, w, d - custo);

      if (novo_valor > memo[i][w][d]) {
        memo[i][w][d] = novo_valor;
        estados[i][w][d] = (Estado){j, w, d - custo};
      }
    }
  }

  return memo[i][w][d];
}

int calculate_max_habilidade() {
  int max_total = 0;

  for (int i = 1; i <= p; i++) {

    for (int i = 0; i <= p; i++) {
      for (int j = 0; j <= w; j++) {
        for (int k = 0; k <= d; k++) {
          memo[i][j][k] = INT_MIN;
          estados[i][j][k] = (Estado){-1, -1, -1};
        }
      }
    }
    // int max_per_path = dp(i, w, d);
    // int max_per_path = dp_bup_final(w, d, p);
    int max_per_path = 99;

    for (int i = 1; i <= p; i++) {
      for (int j = 0; j <= w; j++) {
        for (int k = 0; k <= d; k++) {
          memo[i][j][k] = -1;
          estados[i][j][k] = (Estado){-1, -1, -1};
        }
      }
    }

    int aux = dp(i, w, d);
    int maximo = 0, curP, curW, curD;

    for (int i = 1; i <= p; i++) {
      for (int j = 0; j <= w; j++) {
        for (int k = 0; k <= d; k++) {
          if (memo[i][j][k] >= maximo) {
            maximo = memo[i][j][k];
            curP = i;
            curW = j;
            curD = k;
          }
        }
      }
    }

    int soldadosPorPovo[p + 1];

    for (int l = 1; l <= p; l++) {
      soldadosPorPovo[l] = -1;
    }

    soldadosPorPovo[i] = 0;

    while (estados[curP][curW][curD].prevP != -1) {
      Estado prev = estados[curP][curW][curD];

      int diff =
          (memo[curP][curW][curD] - memo[prev.prevP][prev.prevW][prev.prevD]);
      /*
      printf(
          "curP=%d, curW=%d, curD=%d diff=%d, memo=%d memoAnterior=%d, calcu "
          "= %d\n",
          curP, curW, curW, curD, memo[curP][curW][curD],
          memo[prev.prevP][prev.prevW][prev.prevD],
          memo[curP][curW][curD] - memo[prev.prevP][prev.prevW][prev.prevD]);
      */

      if (soldadosPorPovo[prev.prevP] == -1) {
        soldadosPorPovo[prev.prevP] = 0;
      }

      if (memo[curP][curW][curD] - memo[prev.prevP][prev.prevW][prev.prevD] ==
          habilidades[prev.prevP]) {
        soldadosPorPovo[prev.prevP]++;
      }

      curP = prev.prevP;
      curW = prev.prevW;
      curD = prev.prevD;
    }

    for (int i = 1; i <= p; i++) {
      if (soldadosPorPovo[i] != -1) {
        printf("Povo %d: soldados %d recrutados\n", i, soldadosPorPovo[i]);
      }
    }

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

      printf("Povo %d: Habilidade %d\n", i, habilidades[i]);
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