//学籍番号: 4619023 氏名:　加藤　零
#define maxN 50
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  int N, M;            /* 頂点数，辺数 */
  int Adj[maxN][maxN]; /* 隣接行列を表す変数 */
  char fname[128];     /* 読み込むファイルの名前 */
  FILE *fp;            /* 入力ファイル */
  int u, v, i;

  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d %d", &N, &M);
  if (N > maxN) {
    printf("N is too large, setting N = %d\n", maxN);
    N = maxN; /* N が 50 を超えるときは警告をした上で */
  }
  for (u = 0; u < N; u++) { /* 隣接行列の初期化 */
    for (v = 0; v < N; v++) {
      Adj[u][v] = 0;
    }
  }
  for (i = 0; i < M; i++) {
    fscanf(fp, " %d %d", &u, &v); /* 順番に辺の両端 u, v を読み込む */
    Adj[u][v] += 1;               /* 隣接行列に対する処理 */
    Adj[v][u] += 1;
  }
  fclose(fp);
  printf("Adjacency matrix\n");
  for (u = 0; u < N; u++) {
    printf("  |");
    for (v = 0; v < N; v++) {
      printf(" %d", Adj[u][v]);
    }
    printf(" |\n");
  }
  return 0;
}